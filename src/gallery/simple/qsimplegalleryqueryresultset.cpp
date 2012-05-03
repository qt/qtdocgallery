/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

//Backend
#include "qgallerysimpleutility.h"
#include "qsimplegalleryqueryresultset.h"
//API
#include "qdocumentgallery.h"
#include "qgalleryqueryrequest.h"

#include <QDir>
#include <QStringList>
#include <QtDebug>
#include <QDateTime>
#include "qmdlitemusic.h"

QT_ADDON_GALLERY_BEGIN_NAMESPACE

QSimpleGalleryQueryResultSet::QSimpleGalleryQueryResultSet(QObject *parent)
    : QSimpleGalleryResultSet(parent)
{
    m_request = static_cast<QGalleryQueryRequest *>(parent);
    //m_live = m_request->autoUpdate();

    createQuery();
}

QSimpleGalleryQueryResultSet::~QSimpleGalleryQueryResultSet()
{
}

QString QSimpleGalleryQueryResultSet::itemType() const
{
    if (isValid()) {
        if (m_itemType.isEmpty())
            return m_request->rootType();
        else
            return m_itemType;
    }
    return QString();
}

void QSimpleGalleryQueryResultSet::createQuery()
{
    m_itemType = QString();
    //qDebug() << "createQuery: " << m_request->rootType() << ", filter: " << m_request->filter();
    if (m_request->rootType() == QDocumentGallery::Album) {
        QMdLiteMusic musicDb;
        bool isJsonDb = musicDb.dbType();
        //m_isValid = true;
        QStringList propertyList;
        propertyList << QString::fromLatin1("albumArt")
                     << QString::fromLatin1("title")
                     << QString::fromLatin1("artist")
                     << QString::fromLatin1("albumArtist");
        //m_result.clear();
        QString filterStr = filterToString(QString::fromLatin1("albums"), m_request->filter(), isJsonDb);
        // Add the root item to the query
        if (!m_request->rootItem().toString().isEmpty()) {

            QStringList parts = m_request->rootItem().toString().split(QString::fromLatin1(":/"));
            if (QString::compare(QString::fromLatin1("artist"), parts.at(0), Qt::CaseInsensitive) == 0) {
                QString artistCleaned(parts.at(1));
                // TODO: Jsondb stuff here

                if (isJsonDb) {
                    artistCleaned.replace(QString::fromLatin1("\""), QString::fromLatin1("\\\""));
                    artistCleaned.replace(QString::fromLatin1("\\"), QString::fromLatin1("\\\\"));
                    QString artistFilter(QLatin1String("[?artist=\""));
                    artistFilter.append(artistCleaned).append(QString::fromLatin1("\"]"));
                    filterStr.prepend(artistFilter);
                    //qDebug() << filterStr;
                }
                else {
                    artistCleaned.replace(QString::fromLatin1("'"), QString::fromLatin1("''"));
                    if (!filterStr.isEmpty())
                        filterStr.append(QString::fromLatin1(" AND artist = '")).append(artistCleaned).append(QString::fromLatin1("'"));
                    else
                        filterStr.append(QString::fromLatin1("artist = '")).append(artistCleaned).append(QString::fromLatin1("'"));
                }
            }
        }
        m_resultQuery = musicDb.createAlbumQuery(propertyList, filterStr, m_request->sortPropertyNames());

        connect(m_resultQuery, SIGNAL(rowChanged(quint32, int)), this, SLOT(itemChanged(quint32, int)), Qt::QueuedConnection);
        connect(m_resultQuery, SIGNAL(itemsInserted(int, int)), this, SIGNAL(itemsInserted(int, int)), Qt::QueuedConnection);
        connect(m_resultQuery, SIGNAL(itemsRemoved(int, int)), this, SIGNAL(itemsRemoved(int, int)), Qt::QueuedConnection);

        /*if (query->isActive()) {
            m_count = 0;*/
            emit progressChanged(1, 3);
            /*QStringList propertyList;
            QDocumentGallerySimpleUtility::GetDataFieldsForItemType( propertyList, itemType() );
            int count = propertyList.count();
            qDebug () << "Property list count: " << count;
            while (query->next()) {
                QVariantList row;
                row.append(query->value(0)); // Add id as the first item
                for (int ii=0; ii<count; ii++) {
                    int propertyKey = QDocumentGallerySimpleUtility::GetPropertyKey( propertyList[ii] );
                    row.append(query->value(QDocumentGallerySimpleUtility::GetPropertyIndex(propertyKey, itemType())));
                    //qDebug () << "Appending: " << query.value(QDocumentGallerySimpleUtility::GetPropertyIndex(propertyKey, itemType())) << " to row";
                }
                //qDebug () << "Appending row to m_result";
                m_result.append(row);
                m_count++;
            }*/
            emit progressChanged(2, 3);
            emit finished();
            //m_cursorPosition = -1;
        //}
        //else {
        //    m_isValid = false;
        //    emit finished();
        //}
    }
    else if (m_request->rootType() == QDocumentGallery::Artist) {
        QMdLiteMusic musicDb;
        bool isJsonDb = musicDb.dbType();
        //m_isValid = true;
        //m_result.clear();
        QString filterStr = filterToString(QString::fromLatin1("songs"), m_request->filter(), isJsonDb);
        bool artistRootItem = false;
        if (!m_request->rootItem().toString().isEmpty()) {
            QStringList parts = m_request->rootItem().toString().split(QString::fromLatin1(":/"));
            if (QString::compare(QString::fromLatin1("artist"), parts.at(0), Qt::CaseInsensitive) == 0) {
                artistRootItem = true;
                m_itemType = QDocumentGallery::AlbumArtist;
                m_resultQuery = musicDb.createArtistAlbumsQuery(parts.at(1), filterStr);
            }
        }
        else m_resultQuery = musicDb.createArtistQuery(filterStr);

        connect(m_resultQuery, SIGNAL(rowChanged(quint32, int)), this, SLOT(itemChanged(quint32, int)), Qt::QueuedConnection);
        connect(m_resultQuery, SIGNAL(itemsInserted(int, int)), this, SIGNAL(itemsInserted(int, int)), Qt::QueuedConnection);
        connect(m_resultQuery, SIGNAL(itemsRemoved(int, int)), this, SIGNAL(itemsRemoved(int, int)), Qt::QueuedConnection);
        //QMdLiteQueryResult query = musicDb.createAlbumArtistQuery();
        //if (query->isActive()) {
            //m_count = 0;
            emit progressChanged(1, 3);
            //QStringList propertyList;
            //QDocumentGallerySimpleUtility::GetDataFieldsForItemType( propertyList, itemType() );
            /*int count = propertyList.count();
            //qDebug () << "Property list count: " << count;
            while (query->next()) {
                QVariantList row;
                row.append(query->value(0)); // Add id as the first item
                for (int ii=0; ii<count; ii++) {
                    int propertyKey = QDocumentGallerySimpleUtility::GetPropertyKey( propertyList[ii] );
                    if (!artistRootItem && (propertyKey == QDocumentGallerySimpleUtility::EAlbumTitle ||
                            propertyKey == QDocumentGallerySimpleUtility::EAlbumArt)) {
                        QString value = query->value (QDocumentGallerySimpleUtility::GetPropertyIndex(propertyKey, itemType())).toString();
                        QStringList values = value.split (",,");
                        values.removeDuplicates();
                        row.append (values);
                        //qDebug () << "Appending: " << values << " to row";
                    }
                    else {
                        row.append(query->value(QDocumentGallerySimpleUtility::GetPropertyIndex(propertyKey, itemType())));
                        //qDebug () << "Appending: " << query.value(QDocumentGallerySimpleUtility::GetPropertyIndex(propertyKey, itemType())) << " to row";
                    }
                }
                //qDebug () << "Appending row to m_result";
                m_result.append(row);
                m_count++;
            }*/
            emit progressChanged(2, 3);
            emit finished();
            //m_cursorPosition = -1;
        /*}
        else {
            //m_isValid = false;
            emit finished();
        }*/
    }
    else if (m_request->rootType() == QDocumentGallery::Audio) {
        QMdLiteMusic musicDb;
        bool isJsonDb = musicDb.dbType();
        //m_isValid = true;
        //m_result.clear();
        QStringList propertyList;
        propertyList << QString::fromLatin1("url")
                     << QString::fromLatin1("title")
                     << QString::fromLatin1("artist")
                     << QString::fromLatin1("albumTitle")
                     << QString::fromLatin1("albumArtist")
                     << QString::fromLatin1("albumArt")
                     << QString::fromLatin1("trackNumber")
                     << QString::fromLatin1("genre");
        QString filterStr = filterToString(QString::fromLatin1("songs"), m_request->filter(), isJsonDb);
        // Add the root item to the query
        // TODO: add jsondb support to below
        if (!m_request->rootItem().toString().isEmpty()) {
            QStringList parts = m_request->rootItem().toString().split(QString::fromLatin1(":/"));
            /*if (QString::compare("album", parts.at(0), Qt::CaseInsensitive) == 0) {
                if (isJsonDb) {
                    QString albumFilter(QLatin1String("[?_uuid in \""));
                    albumFilter.append(parts.at(1)).append("->songs\"]");
                    filterStr.prepend(albumFilter);
                    qDebug() << filterStr;
                }
                else {
                    if (!filterStr.isEmpty())
                        filterStr.append(" AND songs.albumId = ").append(parts.at(1));
                    else
                        filterStr.append("songs.albumId = ").append(parts.at(1));
                }
            }
            else*/  if (QString::compare(QString::fromLatin1("artist"), parts.at(0), Qt::CaseInsensitive) == 0) {
                QString artistCleaned(parts.at(1));
                if (isJsonDb) {
                    artistCleaned.replace(QString::fromLatin1("\""), QString::fromLatin1("\\\""));
                    artistCleaned.replace(QString::fromLatin1("\\"), QString::fromLatin1("\\\\"));
                    QString artistFilter(QLatin1String("[?artist=\""));
                    artistFilter.append(artistCleaned).append(QString::fromLatin1("\"]"));
                    filterStr.prepend(artistFilter);
                    //qDebug() << filterStr;
                }
                else {
                    artistCleaned.replace(QString::fromLatin1("'"), QString::fromLatin1("''"));
                    if (!filterStr.isEmpty())
                        filterStr.append(QString::fromLatin1(" AND songs.artist = ")).append(artistCleaned);
                    else
                        filterStr.append(QString::fromLatin1("songs.artist = ")).append(artistCleaned);
                }
            }
        }
        m_resultQuery = musicDb.createMusicQuery(propertyList, filterStr, m_request->sortPropertyNames());

        connect(m_resultQuery, SIGNAL(rowChanged(quint32, int)), this, SLOT(itemChanged(quint32, int)), Qt::QueuedConnection);
        connect(m_resultQuery, SIGNAL(itemsInserted(int, int)), this, SIGNAL(itemsInserted(int, int)), Qt::QueuedConnection);
        connect(m_resultQuery, SIGNAL(itemsRemoved(int, int)), this, SIGNAL(itemsRemoved(int, int)), Qt::QueuedConnection);
        /*if (query->isActive()) {
            m_count = 0;*/
            emit progressChanged(1, 3);
            /*while (query->next()) {
                QVariantList row;
                QStringList propertyList;
                row.append(query->value(0)); // Add id as the first item
                QDocumentGallerySimpleUtility::GetDataFieldsForItemType( propertyList, itemType() );
                const int count = propertyList.count();
                for (int ii=0; ii<count; ii++) {
                    int propertyKey = QDocumentGallerySimpleUtility::GetPropertyKey( propertyList[ii] );
                    row.append(query->value(QDocumentGallerySimpleUtility::GetPropertyIndex(propertyKey, itemType())));
                    //qDebug () << "Appending: " << query.value(QDocumentGallerySimpleUtility::GetPropertyIndex(propertyKey, itemType())) << " to row";
                }
                m_result.append(row);
                m_count++;
            }*/
            emit progressChanged(2, 3);
            emit finished();
        /*}
        else {
            m_isValid = false;
            emit finished();
        }*/
    }
    else if (m_request->rootType() == QDocumentGallery::Image) {
        //QMdLiteImages imagesDb;
    }
    //qDebug() << "createQuery done: " << QDateTime::currentMSecsSinceEpoch() - startTime;
}

#include "moc_qsimplegalleryqueryresultset.cpp"
QT_ADDON_GALLERY_END_NAMESPACE
