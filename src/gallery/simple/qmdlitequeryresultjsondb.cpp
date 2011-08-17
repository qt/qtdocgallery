/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDocGallery module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qmdlite_p.h"
#include "qmdlitequeryresultjsondb_p.h"

#include <private/jsondb-connection_p.h>
#include <jsondb-client.h>
#include <private/jsondb-strings_p.h>

#include <QStringList>
#include <QTime>

QMdLiteQueryResultPrivate::QMdLiteQueryResultPrivate
(QMdLiteQueryResult *q, JsonDbConnection *jdbCon, const QString& type, const QString& baseQuery,
 const QString& filterStr, const QStringList& properties, const QStringList& sortProperties, bool chunk) :
    m_type(type), m_baseQuery(baseQuery),
    m_filterString(filterStr), m_properties(properties),
    m_sortProperties(sortProperties), m_count(0), m_index(-1),
    m_chunk(chunk), m_valid(false), m_itemsInsertedSent(false), m_cache_start(-1), m_requestPending(false),
    m_jsonDbConnection(jdbCon), m_q(q)
{
    m_jsonDbClient = new JsonDbClient(jdbCon);

    //qDebug () << "JsonDb client " << m_jsonDbClient << " created.";
    connect(m_jsonDbClient, SIGNAL(response(int, const QVariant&)), this,
        SLOT(jsonDbResponse(int, const QVariant&)));
    connect(m_jsonDbClient, SIGNAL(error( int, int, const QString&)),
            this, SLOT(jsonDbError(int, int, const QString&)));
    connect(m_jsonDbClient, SIGNAL(notified(const QString&,const QVariant&,const QString&)),
            this, SLOT(jsonDbNotified(const QString&,const QVariant&,const QString&)));
    //qDebug () << "Slots connected.";

    QString query = m_baseQuery;
    if (!m_filterString.isEmpty()) query.append(m_filterString);
    if (m_chunk) {
        query.append(QLatin1String("[count]"));
    }
    else {
        query.append(QMdLitePrivate::sortProperties2String (m_sortProperties));
        query.append(QLatin1String("[=")).append(JsonDbString::kUuidStr).append(QLatin1String("]"));
    }
    //qDebug () << "Sending query:" << query;
    m_countRequestId.append(m_jsonDbClient->query(query));
    //const QVariant data = m_jsonDbConnection->sync(JsonDbConnection::makeQueryRequest(query));

    // Create the notification object
    QStringList notificationProperties;
    notificationProperties << QLatin1String ("_type") << QLatin1String ("query") << QLatin1String ("actions");
    QVariantList notificationValues;
    notificationValues << QLatin1String ("notification") << m_baseQuery.append(m_filterString);
    QVariantList actions;
    actions << QLatin1String ("create") << QLatin1String ("update") << QLatin1String ("remove");
    notificationValues << QVariant(actions);
    QVariantMap map;
    for (int ii=0; ii<notificationValues.length(); ii++) {
        map.insert (notificationProperties[ii], notificationValues[ii]);
    }
    //qDebug () << "Creating:" << map;
    m_notifRequestId = m_jsonDbClient->create(map);
}

void QMdLiteQueryResultPrivate::handleCountAndNotifRequest (int id, const QVariant &v)
{
    //QTime t;
    //t.start();

    int idx = -1;
    if ((idx = m_countRequestId.indexOf(id)) >= 0) {
        for (int ii=0; ii<=idx; ii++)
            m_countRequestId.removeAt(ii);
        const QVariantList list = v.toMap().value(QLatin1String("data")).toList();
        if (m_chunk) {
            m_count = list.at(0).toMap().value(QLatin1String("count")).toInt();
        }
        else {
            QStringList newUuids;
            foreach (const QVariant &item, list) {
                QString id = item.toString();
                newUuids.append(id);
            }
            bool bruteForce = false;
            if (!m_count) bruteForce = true;
            else if ((quint32)newUuids.count() > m_count) {
                // Objects added
                // Sorting should be the same
                //qDebug () << "(optimized) newCount is " << newUuids.count() << ", m_count: " << m_count;

                QList<quint32> insertedItems;
                for (quint32 ii=0; ii<(quint32)newUuids.count(); ii++) {
                    if ((quint32)m_uuids.count() > ii && newUuids[ii] == m_uuids[ii]) continue;
                    else {//if (newUuids[ii] == m_insertedObject[QLatin1String("_uuid")]){
                        m_uuids.insert(ii, newUuids[ii]);
                        insertedItems.append (ii);
                        if (m_index >= (int)ii) m_index++;
                        QMap<quint32, quint64>::iterator voidIs;
                        for (voidIs = m_voidIndexes.begin(); voidIs != m_voidIndexes.end(); ++voidIs) {
                            if (voidIs.key() >= ii) {
                                quint32 key = voidIs.key();
                                quint64 stamp = voidIs.value();
                                m_voidIndexes.remove (key);
                                key++;
                                m_voidIndexes[key] = stamp;
                            }
                        }
                        QVariantMap newObject = m_insertedObjects.take(newUuids[ii]);
                        if (in_cache(ii)) {
                            //qDebug () << "index: " << ii << "_uuid: " << newUuids[ii] << " is in cache";
                            QVariantList newRow;
                            if (!newObject.isEmpty()) {
                                //qDebug () << "Found object from insertedObjects";
                                for (int jj=0; jj<m_properties.count(); jj++) {
                                    newRow.append(newObject[m_properties.at(jj)]);
                                }
                                m_row_cache.insert(ii-m_cache_start, newRow);
                                if ((int)ii>m_index)
                                    m_row_cache.removeLast();
                                else {
                                    m_row_cache.removeFirst();
                                    m_cache_start++;
                                }
                            }
                            else {
                                qDebug () << "Warning: New object not found from inserted objects.";
                                m_q->seek(m_index, false, true);
                            }
                        }
                        else if (m_cache_start >= (qint32)ii) {
                            m_cache_start++;
                        }
                    }
                }
                m_count = m_uuids.count();
                //qDebug () << "(optimized) Setting count to " << m_count;

                if (insertedItems.count()) {
                    quint32 firstIndex = insertedItems[0], count = 1;
                    for (int ii=1; ii<insertedItems.count(); ii++) {
                        if (insertedItems[ii] == firstIndex+count) {
                            count++;
                            continue;
                        }
                        else {
                            //qDebug () << "itemsInserted: " << firstIndex << ", count: " << count;
                            emit m_q->itemsInserted(firstIndex, count);
                            firstIndex = insertedItems[ii]; count = 1;
                        }
                    }
                    if (count) {
                        //qDebug () << "itemsInserted: " << firstIndex << ", count: " << count;
                        emit m_q->itemsInserted(firstIndex, count);
                    }
                }
            }
            else if ((quint32)newUuids.count() < m_count) {
                // Objects removed
                // Sorting should be the same
                //qDebug () << "(optimized) newCount is " << newUuids.count() << ", m_count: " << m_count;

                QList<quint32> removedItems;
                for (quint32 ii=0; ii<(quint32)m_uuids.count(); ii++) {
                    if ((quint32)newUuids.count() > ii && newUuids[ii] == m_uuids[ii]) continue;
                    else {
                        m_uuids.removeAt(ii);
                        m_count--;
                        removedItems.append (ii);
                        if (in_cache(ii)) {
                            m_row_cache.removeAt (ii-m_cache_start);
                            // TODO: Should maybe add an item to the end of the cache?
                        }
                        else if (m_cache_start > (qint32)ii) m_cache_start--;
                    }
                }
                //qDebug () << "(optimized) Setting count to " << m_count;

                if (removedItems.count()) {
                    quint32 firstIndex = removedItems[0], count = 1;
                    for (int ii=1; ii<removedItems.count(); ii++) {
                        if (removedItems[ii] == firstIndex+count) {
                            count++;
                            continue;
                        }
                        else {
                            //qDebug () << "itemsRemoved: " << firstIndex << ", count: " << count;
                            emit m_q->itemsRemoved(firstIndex, count);
                            firstIndex = removedItems[ii]; count = 1;
                        }
                    }
                    if (count) {
                        //qDebug () << "itemsRemoved: " << firstIndex << ", count: " << count;
                        emit m_q->itemsRemoved(firstIndex, count);
                    }
                }
            }
            else {
                //qDebug () << "Same count, do nothing.";
                bruteForce = false;
            }

            if (bruteForce) {
                // Brute-Force method...
                m_row_cache.clear();
                m_cache_start = -1;
                m_uuids.clear();
                bool firstCountrequest = m_count == 0;
                if (m_count) {
                    quint32 count = m_count;
                    m_count = 0;
                    emit m_q->itemsRemoved(0, count);
                }
                m_uuids = newUuids;
                m_count = m_uuids.count();
                //qDebug () << "(Brute-Force) Setting count to " << m_count;
                if (!firstCountrequest && m_count) {
                    // For first request send itemsInserted only when real rows received
                    emit m_q->itemsInserted (0, m_count);
                    m_itemsInsertedSent = true;
                }
                else
                    m_q->seek(0);

                //m_q->seek (m_index);
            }
        }
        m_valid = true;
    }
    else if (id == m_notifRequestId) {
        const QVariantMap result = v.toMap();
        m_notifUuid = result.value(QLatin1String("_uuid")).toString();
        //qDebug () << "Notification object " << m_notifUuid << " created.";
        m_notifRequestId = 0;
    }
    else {
        // Most probably an 'old' count request
        // and we want to use always the latest one
        qDebug () << "Count/notif request skipped.";
    }
    //qDebug () << "Handling the count/notif request took: " << t.elapsed() << " milliseconds.";
}

QMdLiteQueryResultPrivate::~QMdLiteQueryResultPrivate ()
{
    //qDebug () << "JsonDb client " << m_jsonDbClient << " to be deleted.";

    //Delete the notification object
    if (!m_notifUuid.isEmpty()) {
        QVariantMap object;
        object[QLatin1String("_uuid")] = m_notifUuid;
        //qDebug () << "Notification object " << m_notifUuid << " to be deleted.";

        m_jsonDbClient->remove(object);
    }
    delete m_jsonDbClient;
}

void QMdLiteQueryResultPrivate::jsonDbError(int id, int code, const QString& message)
{
    qDebug () << "jsonDbError, id: " << id << ", code: " << code << ", message: " << message;
}

void QMdLiteQueryResultPrivate::jsonDbNotified(const QString& _uuid, const QVariant& object, const QString& action)
{
    //QTime t;
    //t.start();

    QVariantMap obj = object.toMap();
    // Hack. TODO: think how the subobjects would be handled better
    // Now hardcoded the album subobject handling here
    if (obj.value(QLatin1String("album")).isValid()) {
        QVariantMap album = obj.take(QLatin1String("album")).toMap();
        obj[QLatin1String("album.title")] = album[QLatin1String("title")];
        obj[QLatin1String("album.artist")] = album[QLatin1String("artist")];
    }

    // TODO: cancel the old count request, if new is needed.
    //if (m_countRequestId > 0) return;

    if (action == QLatin1String("create")) {
        if (obj[QLatin1String("_type")].toString() != m_type) {
            //qDebug() << "(create) Non-relevant notification sent by jsondb. Ignored. " << m_type << ", " << obj[QLatin1String("_type")].toString();
            return;
        }
        if (m_chunk) {
            // Brute-Force method...
            m_row_cache.clear();
            emit m_q->itemsRemoved (0, m_count);
            m_count++;
            emit m_q->itemsInserted (0, m_count);
        }
        else {
            QString query = m_baseQuery;
            if (!m_filterString.isEmpty()) query.append(m_filterString);
            query.append(QMdLitePrivate::sortProperties2String (m_sortProperties));
            query.append(QLatin1String("[=")).append(JsonDbString::kUuidStr).append(QLatin1String("]"));

            //qDebug () << "Insert _uuid: " << obj[QLatin1String("_uuid")] << " into insertedObjects. Type: " << obj[QLatin1String("_type")];
            m_insertedObjects.insert(obj[QLatin1String("_uuid")].toString(), obj);
            //if (!m_countRequestId)
            //qDebug() << "Create: " << _uuid << ", " << m_baseQuery << ", inserted objects count: " << m_insertedObjects.count();
            m_countRequestId.append(m_jsonDbClient->query(query));
            //qDebug () << "Handling the notified (create) took: " << t.elapsed() << " milliseconds.";
        }
    }
    else if (action == QLatin1String("update")) {
        if (obj[QLatin1String("_type")].toString() != m_type) {
            //qDebug() << "(update) Non-relevant notification sent by jsondb. Ignored. " << m_type << ", " << obj[QLatin1String("_type")].toString();
            return;
        }
        // Check if the changed item is in the cache
        if (m_cache_start > -1 && m_row_cache.count() > 0) {
            if (m_chunk) {
                quint32 idx = m_cache_start;
                foreach (QVariantList row, m_row_cache) {
                    if (!row.isEmpty()) {
                        // NOTE: _uuid must be the first item for this to work
                        if (row.first().toString() == obj[QLatin1String("_uuid")]) {
                            //qDebug () << "_uuid: " << obj[QLatin1String("_uuid")] << " updated.";
                            if (in_cache(idx)) {
                                for (int ii=0; ii<m_properties.count(); ii++) {
                                    if (m_row_cache[idx-m_cache_start].count() < ii+1)
                                        m_row_cache[idx-m_cache_start].append (obj[m_properties.at(ii)]);
                                    else
                                        m_row_cache[idx-m_cache_start][ii] = obj[m_properties.at(ii)];
                                }
                                //qDebug () << "rowChanged: " << idx;
                                emit m_q->rowChanged(idx, 1);
                            }
                            //if (m_requestPending)
                                // Not sure, if this works...
                                //m_voidIndexes.insert(idx, QDateTime::currentMSecsSinceEpoch ());
                            return;
                        }
                        idx++;
                    }
                }
            }
            else {
                quint32 idx = 0;
                foreach (QString uuid, m_uuids) {
                    if (uuid == obj[QLatin1String("_uuid")]) {
                        if (in_cache(idx)) {
                            for (int ii=0; ii<m_properties.count(); ii++) {
                                if (m_row_cache[idx-m_cache_start].count() < ii+1)
                                    m_row_cache[idx-m_cache_start].append (obj[m_properties.at(ii)]);
                                else
                                    m_row_cache[idx-m_cache_start][ii] = obj[m_properties.at(ii)];
                            }
                            emit m_q->rowChanged(idx, 1);
                            //if (m_requestPending)
                            // Not sure, if this works
                            //m_voidIndexes.insert(idx, QDateTime::currentMSecsSinceEpoch ());
                        }
                        //qDebug () << "Handling the notified (update) took: " << t.elapsed() << " milliseconds.";
                        return;
                    }
                    idx++;
                }
            }
        }
        //qDebug () << "jsonDbNotification, _uuid: " << _uuid << ", object id: " << obj[QLatin1String("_uuid")] << ", action: " << action;
    }
    else if (action == QLatin1String("remove")) {
        if (m_chunk) {
            // Brute-Force method...
            emit m_q->itemsRemoved (0, m_count);
            m_count--;
            emit m_q->itemsInserted (0, m_count);
        }
        else {
            QString query = m_baseQuery;
            if (!m_filterString.isEmpty()) query.append(m_filterString);
            query.append(QMdLitePrivate::sortProperties2String (m_sortProperties));
            query.append(QLatin1String("[=")).append(JsonDbString::kUuidStr).append(QLatin1String("]"));
            m_removedObjects.insert(obj[QLatin1String("_uuid")].toString(), obj);
            //if (!m_countRequestId)
            m_countRequestId.append(m_jsonDbClient->query(query));
            //qDebug () << "(Remove) Setting count request id to " << m_countRequestId;
        }
    }
    else
       qDebug () << "qtm-gallery: jsonDbNotification, _uuid: " << _uuid << ", object id: " << obj[QLatin1String("_uuid")] << ", action: " << action;
}

void QMdLiteQueryResultPrivate::jsonDbResponse(int id, const QVariant &v)
{

    if (m_countRequestId.contains(id) || id == m_notifRequestId) {
        handleCountAndNotifRequest(id, v);
        return;
    }
    if (!m_requestPending) {
        // Most probably an 'old' count request
        // and we want to use always the latest one
        qDebug () << "Request skipped.";
        return;
    }
    //QTime t;
    //t.start();

    QList<QVariantList> newRows;
    QVariantMap m = v.toMap();
    //qDebug () << "Response: " << v;
    //qDebug () << "toMap: " << m;
    if (m.contains(QLatin1String("data"))) {
        QVariantList data = m.value(QLatin1String("data")).toList();
        //qDebug () << "data: " << data;
        foreach (QVariant item, data) {
            QVariantList row = item.toList();
            newRows.append(row);
        }
    }
    else {
        qDebug() << "Illegal response from JsonDB. No data found.";
        return;
    }

    // Add new rows to cache
    if (m_requestOffset < m_cache_start) {
        // remove the unneeded entries
        int removed = 0;
        while (!m_row_cache.isEmpty() && m_row_cache.count()+newRows.count() > CACHE_SIZE) {
            m_row_cache.removeLast(); removed++;
        }
        //qDebug() << "Removed " << removed  <<" rows from the end of the cache.";
        //qDebug() << "Adding rows from: " << m_requestOffset <<", to: " << m_requestOffset+m_requestLimit << " to the beginning of cache.";
        while (!newRows.isEmpty())
            m_row_cache.prepend(newRows.takeLast());
        m_cache_start = m_requestOffset;
        //emit m_q->rowChanged(m_requestOffset, m_requestLimit);
    }
    else {
        if (m_cache_start > -1 && m_row_cache.count() > 0) {
            // remove the unneeded entries
            int removed = 0;
            while (!m_row_cache.isEmpty() && m_row_cache.count()+newRows.count() > CACHE_SIZE) {
                m_row_cache.removeFirst();
                m_cache_start++; removed++;
            }
            //qDebug() << "Removed " << removed  <<" rows from the beginning of the cache.";
        }
        else m_cache_start = m_requestOffset;
        //qDebug() << "Adding rows from: " << m_requestOffset <<", to: " << m_requestOffset+m_requestLimit << " to the end of the cache.";
        m_row_cache.append(newRows);
        //emit m_q->rowChanged(m_requestOffset, m_requestLimit);
        //m_cache_start += m_requestLimit;
    }
    //qDebug () << "m_cacheStart: " << m_cache_start << ", count: " << m_row_cache.count();
    // Do something with void indexes
    quint64 latestSeek = m_requestTimestamp;
    qint32 idxToSeek = -1;
    if (!m_voidIndexes.isEmpty()) {
        //qDebug() << "m_voidIndexes.count(): " << m_voidIndexes.count();
        qint32 firstIdx = -1;
        qint32 lastIdx = -1;
        QMutableMapIterator<quint32, quint64> ii(m_voidIndexes);
        while (ii.hasNext()) {
            ii.next();
            quint32 idx = ii.key();
            quint64 tstamp = ii.value();
            //qDebug() << "idx: " << idx;
            if ((qint32)idx >= m_cache_start && (qint32)idx < m_cache_start + m_row_cache.count()) {
                // Try to avoid emitting row changed multiple times.
                if (firstIdx < 0) {
                    firstIdx = lastIdx = idx;
                }
                else if ((qint32)idx == lastIdx + 1) {
                    lastIdx++;
                }
                else {
                    //qDebug() << "Emitting row changed for: " << firstIdx << ", " << lastIdx - firstIdx + 1;
                    emit m_q->rowChanged (firstIdx, lastIdx - firstIdx + 1);
                    firstIdx = lastIdx = idx;
                }
                ii.remove();
            }
            else  {
                if (tstamp > latestSeek) {
                    latestSeek = tstamp;
                    idxToSeek = idx;
                }
            }
        }
        if (firstIdx > -1) {
            //qDebug() << "Emitting row changed for: " << firstIdx << ", " << lastIdx - firstIdx + 1;
            emit m_q->rowChanged (firstIdx, lastIdx - firstIdx + 1);
        }
    }
    if (!m_itemsInsertedSent) {
        emit m_q->itemsInserted (0, m_count);
        m_itemsInsertedSent = true;
    }
    m_requestPending = false;
    if (idxToSeek > -1) {
        qDebug() << "Needing a new request immediately. Consider tuning your system.";
        m_q->seek(idxToSeek, false, true);
    }
    //qDebug () << "Handling the response took: " << t.elapsed() << " milliseconds.";
}


int QMdLiteQueryResult::at () const
{
    return d->m_index;
}

bool QMdLiteQueryResult::first ()
{
    if (isActive())
        return seek (0);
    return false;
}

bool QMdLiteQueryResult::isActive () const
{
    return d->m_valid && d->m_count > 0;
}

bool QMdLiteQueryResult::last ()
{
    if (isActive())
        return seek (d->m_count-1);
    return false;
}

bool QMdLiteQueryResult::next ()
{
    if (isActive())
        return seek (d->m_index+1);
    return false;
}

bool QMdLiteQueryResult::previous ()
{
    if (isActive() && d->m_index > -1)
        return seek (d->m_index-1);
    return false;
}

bool QMdLiteQueryResult::seek ( int index, bool relative, bool forceRefresh )
{
    /* If we would go for the _uuid based queries
QStringList uuidList;
for (int ii=newRowsStart; ii<newRowsEnd; ii++) uuidList.append(d->m_uuids.at(ii));
    query.append ("[?_uuid in %1]").arg(uuidList.join(QLatin1String(",")));*/

    //QTime t;
    //t.start();

    if (relative) index += d->m_index < 0 ? 0 : d->m_index;
    d->m_index = index;
    if (index < 0 || (quint32)index >= d->m_count) return false;

    //qDebug () << "seek: " << d->m_index;

    if (!d->m_requestPending) {
        if (forceRefresh) {
            // Refresh the whole cache
            d->m_requestOffset = qMax (index-CACHE_SIZE/2, 0);
            int newRowsEnd = qMin ((quint32)index+CACHE_SIZE/2, d->m_count);
            d->m_requestLimit = newRowsEnd - d->m_requestOffset;
            d->m_cache_start = -1;
            d->m_row_cache.clear();
        }
        // Check if we need to read more stuff in front of the cache
        else if (index >= 0 && index < d->m_cache_start + SAFETY_MARGIN && d->m_cache_start > 0) {
            // create & send the request
            d->m_requestOffset = qMax (index-CACHE_SIZE/2, 0);
            int newRowsEnd = qMin (index+CACHE_SIZE/2, d->m_cache_start);
            newRowsEnd = qMin ((quint32)newRowsEnd, d->m_count);
            d->m_requestLimit = newRowsEnd - d->m_requestOffset;
        }
        // Check if we need to read more stuff to the end of the cache
        else if (d->m_cache_start < 0 ||
                ((quint32)d->m_cache_start + d->m_row_cache.count() < d->m_count &&
                 index > d->m_cache_start + d->m_row_cache.count() - SAFETY_MARGIN)) {
            d->m_requestOffset = qMax(index, (int)(d->m_cache_start + d->m_row_cache.count()));
            int newRowsEnd = qMin ((quint32)index+CACHE_SIZE/2, d->m_count);
            d->m_requestLimit = newRowsEnd - d->m_requestOffset;
        }
        else return true;
        //qDebug () << "seek ( " << index << ", " << relative << ", " << forceRefresh << " )";
        //qDebug () << "d->m_requestOffset = " << d->m_requestOffset << ", d->m_requestLimit" << d->m_requestLimit;
        QString query = d->m_baseQuery;
        if (d->m_chunk) {
            if (!d->m_filterString.isEmpty()) query.append(d->m_filterString);
            query.append(QMdLitePrivate::sortProperties2String (d->m_sortProperties));
        }
        else {
            query.append(QLatin1String("[? _uuid in ["));
            for (int ii=0; ii<d->m_requestLimit && ii<d->m_uuids.count()-d->m_requestOffset; ii++) {
                if (ii) query.append(QLatin1String(", "));
                query.append(QLatin1String("\"")).append(d->m_uuids[ii+d->m_requestOffset]).append(QLatin1String("\""));
            }
            query.append(QLatin1String("]]"));
            query.append(QMdLitePrivate::sortProperties2String (d->m_sortProperties));
            //qDebug () << "Sending a request: " << query;
        }
        if (d->m_properties.length() > 0) {
            query.append(QLatin1String("[= [ "));
            query.append(d->m_properties.join(QLatin1String(",")));
            query.append(QLatin1String(" ] ]"));
        }
        //qDebug () << "Sending a request: " << query << " with offset: " << d->m_requestOffset
        //             << " and limit: " << d->m_requestLimit;
        d->m_requestPending = true;
        d->m_requestTimestamp = QDateTime::currentMSecsSinceEpoch ();
        //qDebug () << "Request timestamp is " << d->m_requestTimestamp;
        if (d->m_chunk)
            d->m_requestId = d->m_jsonDbClient->query(query, d->m_requestOffset, d->m_requestLimit);
        else {
            d->m_requestId = d->m_jsonDbClient->query(query);
        }
        //qDebug () << "Request " << d->m_requestId << " sent.";
    }
    else {
        //qDebug () << "Seek to " << d->m_index << " missed.";
    }
    //qDebug () << "Handling the seek took: " << t.elapsed() << " milliseconds.";
    return true;
}

int QMdLiteQueryResult::size () const
{
    return d->m_count;
}

QVariant QMdLiteQueryResult::value ( int index ) const
{
    if (d->m_index < 0) {
        qDebug () << "Asking value before seek!";
        return QVariant();
    }
    //qDebug() << "QMdLiteQueryResult::value (" << index << ") " << "m_index = " << d->m_index;
    //qDebug() << "QMdLiteQueryResult::value: m_index = " << d->m_index << ", m_cache_start = " << d->m_cache_start;
    if (d->in_cache(d->m_index)) {
        //qDebug() << "Returing value " << d->m_row_cache.at(d->m_index-d->m_cache_start).at(index) <<
        //            "for row: " << d->m_index << "and index: " << index;
        if (!d->m_row_cache.at(d->m_index-d->m_cache_start).isEmpty()) {
            // Remove the index from void indexes as we are now returning the real value
            d->m_voidIndexes.remove(d->m_index);
            return d->m_row_cache.at(d->m_index-d->m_cache_start).at(index);
        }
    }
    //qDebug() << "Returing an empty value for " << d->m_index;
    d->m_voidIndexes.insert(d->m_index, QDateTime::currentMSecsSinceEpoch ());

    return QVariant(QString(QLatin1String("")));
}

bool QMdLiteQueryResult::isValid () const
{
    if (d->m_index < 0 || d->m_index >= (qint32)d->m_count) return false;
    return true;
}

