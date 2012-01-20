/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
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

#include "qgalleryresource.h"
#include "qdocumentgallery.h"
//Backend
#include "qsimplegalleryresultset.h"
#include "qgallerysimpleutility.h"

#include <QtDebug>


QT_ADDON_GALLERY_BEGIN_NAMESPACE

QSimpleGalleryResultSet::QSimpleGalleryResultSet(QObject *parent)
    : QGalleryResultSet(parent)
    , m_resultQuery(0)
//    , m_musicDb(0)
//    , m_imagesDb(0)
//    , m_count(0)
//    , m_cursorPosition(-1)
//    , m_isValid(false)
{
}

QSimpleGalleryResultSet::~QSimpleGalleryResultSet()
{
    /*if (m_musicDb) delete m_musicDb;
    if (m_imagesDb) delete m_imagesDb;*/
    if (m_resultQuery)
        delete m_resultQuery;
}

QString QSimpleGalleryResultSet::_jsonDbFilterToString(const QString &tableName,
                                                       const QGalleryFilter& filter, bool inOr)
{
    QString ret;
    switch (filter.type()) {
    case QGalleryFilter::Invalid:
        break;
    case QGalleryFilter::Intersection: {
        if (inOr) {
            qDebug () << "And inside or not supported.";
            return QString();
        }
        QGalleryIntersectionFilter interFilter = filter.toIntersectionFilter();
        for (int ii=0; ii<interFilter.filters().length(); ii++)
            ret.append(_jsonDbFilterToString(tableName, interFilter.filters()[ii]));
        break;
    }
    case QGalleryFilter::Union: {
        if (!inOr)
            ret.append(QLatin1String("[?"));
        QGalleryUnionFilter unionFilter = filter.toUnionFilter();
        for (int ii=0; ii<unionFilter.filters().length(); ii++) {
            if (ii)
                ret.append(QString::fromAscii(" | "));
            ret.append(_jsonDbFilterToString(tableName, unionFilter.filters()[ii], !inOr));
        }
        if (!inOr)
            ret.append(QLatin1String("]"));
        break;
    }
    case QGalleryFilter::MetaData:
        QGalleryMetaDataFilter metadataFilter = filter.toMetaDataFilter();
        if (!inOr)
            ret.append(QLatin1String("[?"));
        if (metadataFilter.propertyName().compare(QString::fromAscii("albumTitle"), Qt::CaseInsensitive) == 0)
            ret.append(QString::fromAscii("album.title"));
        else if (metadataFilter.propertyName().compare(QString::fromAscii("albumArtist"), Qt::CaseInsensitive) == 0)
            ret.append(QString::fromAscii("album.artist"));
        /*else if (metadataFilter.propertyName().compare("albumCover", Qt::CaseInsensitive) == 0)
            ret.append ("albumId->url");*/
        else
            ret.append(metadataFilter.propertyName());
        switch (metadataFilter.comparator()) {
        case QGalleryFilter::Equals:
            if (metadataFilter.isNegated())
                ret.append(QString::fromAscii("!"));
            ret.append(QString::fromAscii("="));
            ret.append(QString::fromAscii("\"")).append(metadataFilter.value().toString()).append(QString::fromAscii("\""));
            break;
        case QGalleryFilter::LessThan:
            if (metadataFilter.isNegated())
                ret.append(QString::fromAscii(">="));
            else
                ret.append(QString::fromAscii("<"));
            ret.append(metadataFilter.value().toString());
            break;
        case QGalleryFilter::GreaterThan:
            if (metadataFilter.isNegated())
                ret.append(QString::fromAscii("<="));
            else
                ret.append(QString::fromAscii(">"));
            ret.append(metadataFilter.value().toString());
            break;
        case QGalleryFilter::LessThanEquals:
            if (metadataFilter.isNegated())
                ret.append(QString::fromAscii(">"));
            else
                ret.append(QString::fromAscii("<="));
            ret.append(metadataFilter.value().toString());
            break;
        case QGalleryFilter::GreaterThanEquals:
            if (metadataFilter.isNegated())
                ret.append(QString::fromAscii("<"));
            else
                ret.append(QString::fromAscii(">="));
            ret.append(metadataFilter.value().toString());
            break;
        case QGalleryFilter::Contains:
            ret.append(QString::fromAscii("=~"));
            ret.append(QString::fromAscii("\"/*")).append(metadataFilter.value().toString().
                                      replace(QString::fromAscii("\""), QString::fromAscii("\\\""))).append(QString::fromAscii("*/iw\""));
            break;
        case QGalleryFilter::StartsWith:
            ret.append(QString::fromAscii("=~"));
            ret.append(QString::fromAscii("\"/")).append(metadataFilter.value().toString().
                                     replace(QString::fromAscii("\""), QString::fromAscii("\\\""))).append(QString::fromAscii("*/iw\""));
            break;
        case QGalleryFilter::EndsWith:
            ret.append(QString::fromAscii("=~"));
            ret.append(QString::fromAscii("\"/*")).append(metadataFilter.value().toString().
                                     replace(QString::fromAscii("\""), QString::fromAscii("\\\""))).append(QString::fromAscii("/iw\""));
            break;
        case QGalleryFilter::Wildcard:
            ret.append(QString::fromAscii("=~"));
            ret.append(QString::fromAscii("\"/*")).append(metadataFilter.value().toString().
                                      replace(QString::fromAscii("\""), QString::fromAscii("\\\""))).append(QString::fromAscii("*/iw\""));
            break;
        case QGalleryFilter::RegExp:
            ret.append(QString::fromAscii("=~"));
            ret.append(QString::fromAscii("\"/")).append(metadataFilter.value().toString().
                                      replace(QString::fromAscii("\""), QString::fromAscii("\\\""))).append(QString::fromAscii("/i\""));
            break;
        }
        if (!inOr)
            ret.append(QLatin1String("]"));
        //qDebug () << ret;
        break;
    }
    return ret;
}

QString QSimpleGalleryResultSet::filterToString(const QString &tableName, const QGalleryFilter& filter, bool jsonDb)
{
    QString ret;
    if (!jsonDb) { // SQL
        switch (filter.type()) {
        case QGalleryFilter::Invalid:
            break;
        case QGalleryFilter::Intersection:
        {
            QGalleryIntersectionFilter interFilter = filter.toIntersectionFilter();
            ret.append(QLatin1String(" ("));
            ret.append(filterToString(tableName, interFilter.filters()[0], false));
            for (int ii=1; ii<interFilter.filters().length(); ii++) {
                ret.append(QLatin1String(" AND "));
                ret.append(filterToString(tableName, interFilter.filters()[ii], false));
            }
            ret.append(QLatin1String(" )"));
        }
            break;
        case QGalleryFilter::Union:
        {
            QGalleryUnionFilter unionFilter = filter.toUnionFilter();
            ret.append(QLatin1String(" ("));
            ret.append(filterToString(tableName, unionFilter.filters()[0], false));
            for (int ii=1; ii<unionFilter.filters().length(); ii++) {
                ret.append(QLatin1String(" OR "));
                ret.append(filterToString(tableName, unionFilter.filters()[ii], false));
            }
            ret.append(QLatin1String(") "));
        }
            break;
        case QGalleryFilter::MetaData:
            QGalleryMetaDataFilter metadataFilter = filter.toMetaDataFilter();
            if (metadataFilter.isNegated()) ret.append(QLatin1String(" NOT "));
            if (metadataFilter.propertyName().compare(QLatin1String("albumTitle"), Qt::CaseInsensitive) == 0) {
                ret.append(QLatin1String("albums.title"));
            }
            else if (metadataFilter.propertyName().compare(QLatin1String("albumArtist"), Qt::CaseInsensitive) == 0) {
                ret.append(QLatin1String("albums.artist"));
            }
            else if (metadataFilter.propertyName().compare(QLatin1String("albumCover"), Qt::CaseInsensitive) == 0) {
                ret.append(QLatin1String("albums.url"));
            }
            else ret.append(tableName).append(QLatin1String(".")).append(metadataFilter.propertyName());
            switch (metadataFilter.comparator()) {
            case QGalleryFilter::Equals:
                ret.append(QLatin1String(" = "));
                ret.append(metadataFilter.value().toString());
                break;
            case QGalleryFilter::LessThan:
                ret.append(QLatin1String(" < "));
                ret.append(metadataFilter.value().toString());
                break;
            case QGalleryFilter::GreaterThan:
                ret.append(QLatin1String(" > "));
                ret.append(metadataFilter.value().toString());
                break;
            case QGalleryFilter::LessThanEquals:
                ret.append(QLatin1String(" <= "));
                ret.append(metadataFilter.value().toString());
                break;
            case QGalleryFilter::GreaterThanEquals:
                ret.append(QLatin1String(" >= "));
                ret.append(metadataFilter.value().toString());
                break;
            case QGalleryFilter::Contains:
                ret.append(QLatin1String(" LIKE "));
                ret.append(QLatin1String("'%")).append(metadataFilter.value().toString().replace(QLatin1String("'"), QLatin1String("''"))).append(QLatin1String("%'"));
                break;
            case QGalleryFilter::StartsWith:
                ret.append(QLatin1String(" LIKE "));
                ret.append(QLatin1String("'")).append(metadataFilter.value().toString().replace(QLatin1String("'"), QLatin1String("''"))).append(QLatin1String("%'"));
                break;
            case QGalleryFilter::EndsWith:
                ret.append(QLatin1String(" LIKE "));
                ret.append(QLatin1String("'%")).append(metadataFilter.value().toString().replace(QLatin1String("'"), QLatin1String("''"))).append(QLatin1String("'"));
                break;
            case QGalleryFilter::Wildcard:
                ret.append(QLatin1String(" GLOB "));
                ret.append(QLatin1String("'")).append(metadataFilter.value().toString().replace(QLatin1String("'"), QLatin1String("''"))).append(QLatin1String("'"));
                break;
            case QGalleryFilter::RegExp:
                qWarning() << "RegExp not supported.";
                break;
            }
            break;
        }
    }
    else {
        // JsonDb
        ret = _jsonDbFilterToString(tableName, filter);
        //qDebug() << ret;
    }
    return ret;
}

int QSimpleGalleryResultSet::propertyKey(const QString &property) const
{
    return QDocumentGallerySimpleUtility::GetPropertyKey( property );
}

QGalleryProperty::Attributes QSimpleGalleryResultSet::propertyAttributes(int key) const
{
    if (key == QDocumentGallerySimpleUtility::EUri) {
        return (QGalleryProperty::CanRead | QGalleryProperty::CanSort | QGalleryProperty::CanFilter );
    } else {
        return (QGalleryProperty::CanRead | QGalleryProperty::CanWrite | QGalleryProperty::CanSort | QGalleryProperty::CanFilter );
    }
}

QVariant::Type QSimpleGalleryResultSet::propertyType(int key) const
{
    return QDocumentGallerySimpleUtility::GetPropertyType( key );
}

int QSimpleGalleryResultSet::itemCount() const
{
    //qDebug () << "Count asked. Returning " << m_resultQuery->size();
    return m_resultQuery->size();
}

bool QSimpleGalleryResultSet::isValid() const
{
    //qDebug () << "isValid asked. Returning " << m_resultQuery->isValid();
    return m_resultQuery->isValid();
}

// Item type + :/ + unique id = itemId
QVariant QSimpleGalleryResultSet::itemId() const
{
    QString idVar;

    if (isValid()) {
        // Id is always the first value in the query
        QString id = m_resultQuery->value(0).toString();
        idVar = itemType() + QLatin1String(":/") + id;
    }
    //qDebug () << "itemId asked. Returning " << idVar;

    return QVariant(idVar);
}

QUrl QSimpleGalleryResultSet::itemUrl() const
{
    if ( isValid()) {
        // Url is always the second value
        const QUrl url =
                QUrl::fromLocalFile(m_resultQuery->value(1).toString());
        //qDebug () << "itemUrl asked. Returning " << url.toString();
        return url;
    }
    return QUrl();
}

QList<QGalleryResource> QSimpleGalleryResultSet::resources() const
{
    QList<QGalleryResource> resources;
    //qDebug () << "resources asked.";

    if (isValid()) {
        const QUrl url = itemUrl();

        QStringList propertyList;
        QDocumentGallerySimpleUtility::GetDataFieldsForItemType( propertyList, itemType() );

        const int count = propertyList.count();
        QMap<int, QVariant> attributes;

        for (int i = 0; i < count; i++) {
            int propertyKey = QDocumentGallerySimpleUtility::GetPropertyKey( propertyList[i] );
            QVariant value = metaData( propertyKey );
            if (!value.isNull())
                attributes.insert(propertyKey, value);
        }
        resources.append(QGalleryResource(url, attributes));
    }
    return resources;
}

QVariant QSimpleGalleryResultSet::metaData(int key) const
{
    if (!isValid())
        return QVariant();

    //qDebug () << "Returning: " <<
    //             m_resultQuery->value(QDocumentGallerySimpleUtility::GetPropertyIndex(key, itemType())) <<
    //             " for " << key << " idx: " << currentIndex();
    return m_resultQuery->value(QDocumentGallerySimpleUtility::GetPropertyIndex(key, itemType()));
}

bool QSimpleGalleryResultSet::setMetaData(int key, const QVariant &value)
{
    //Q_UNUSED(key)
    //Q_UNUSED(value)
    qDebug () << "setMetaData for key: " << key << " to value: " << value << " called.";
    // TODO: implement editing with qexif

    /*bool ret = false;

    TRAPD( err, QDocumentGalleryMDSUtility::SetMetaDataFieldL(
            m_itemArray[m_cursorPosition], value, key) );
    if (err) {
        return false;
    } else {
        if (ret) {
            TRAP( err, m_session->CommitObjectL( *m_itemArray[m_cursorPosition] ) );
            if (err) {
                return false;
            } else {
                return true;
            }
        }
    }
    return false;*/
    return false;
}

int QSimpleGalleryResultSet::currentIndex() const
{

    if (isValid()) {
        //qDebug () << "currentIndex asked. Returning: " << m_cursorPosition;
        return m_resultQuery->at();
    }
    return -1;
}

bool QSimpleGalleryResultSet::fetch(int index)
{
    //qDebug () << "fetch with index: " << index << " called.";
    if (m_resultQuery->seek(index)) {
        emit currentIndexChanged(currentIndex());
        //emit currentItemChanged();
        return true;
    }
    return false;
}

bool QSimpleGalleryResultSet::fetchNext()
{
    //qDebug () << "fetchNext called.";
    if (m_resultQuery->next()) {
        emit currentIndexChanged(currentIndex());
        //emit currentItemChanged();
        return true;
    }
    return false;
}

bool QSimpleGalleryResultSet::fetchPrevious()
{
    //qDebug () << "fetchPrevious called.";
    if (m_resultQuery->previous()) {
        emit currentIndexChanged(currentIndex());
        //emit currentItemChanged();
        return true;
    }
    return false;
}

bool QSimpleGalleryResultSet::fetchFirst()
{
    //qDebug () << "fetchFirst called.";
    if (m_resultQuery->first()) {
        emit currentIndexChanged(currentIndex());
        //emit currentItemChanged();
        return true;
    }
    return false;
}

bool QSimpleGalleryResultSet::fetchLast()
{
    //qDebug () << "fetchLast called.";
    if (m_resultQuery->last()) {
        emit currentIndexChanged(currentIndex());
        //emit currentItemChanged();
        return true;
    }
    return false;
}

void QSimpleGalleryResultSet::itemChanged(quint32 index, int count)
{
    QList<int> keys;
    foreach (const QString& property, propertyNames()) {
        keys.append(propertyKey(property));
    }
    emit metaDataChanged(index, count, keys);
}

QStringList QSimpleGalleryResultSet::propertyNames() const
{
    QStringList propertyNames;
    QDocumentGallerySimpleUtility::GetDataFieldsForItemType (propertyNames, itemType());
    return propertyNames;
}

#include "moc_qsimplegalleryresultset.cpp"

QT_ADDON_GALLERY_END_NAMESPACE
