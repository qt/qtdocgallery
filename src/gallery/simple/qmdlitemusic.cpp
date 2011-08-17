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

#include "qmdlitemusic.h"
#include "qmdlitemusic_p.h"
#include "qmdlite_p.h"
#include <QtDebug>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDesktopServices>
#include <QBuffer>
#include <QImage>

#ifdef _SQLITE_QMDBACKEND

#include "qmdlitequeryresultsqlite_p.h"
#include <QSqlQuery>

#else

#include "qmdlitequeryresultjsondb_p.h"
#include <private/jsondb-connection_p.h>
#include <jsondb-client.h>

#endif


QString QMdLiteMusicPrivate::m_albumArtDir;

QString QMdLiteMusicPrivate::musicAlbumArtDir ()
{
    if (m_albumArtDir.isNull()) {
        QString ret = dbDirName();
        ret.append(QDir::separator()).append(QLatin1String("album-art")).append(QDir::separator());
        ret = QDir::toNativeSeparators(ret);
        if (!QFile::exists (ret)) {
            QDir dirO;
            dirO.mkpath(ret);
        }
        m_albumArtDir = ret;
    }
    return m_albumArtDir;
}

    //bool updateMusic = false;
#ifdef _SQLITE_QMDBACKEND
QMdLiteMusicPrivate::QMdLiteMusicPrivate (QMdLiteMusic *q) : m_q(q)
{
    QString mDbName = QMdLiteMusic::musicDbName();

    //if (!QFile::exists(mDbName))
        //updateMusic = true;
    if (!openDb (mDbName, "music"))
        qWarning () << "Can not open the music database!" + m_db.lastError().text();
    else {
        // Create the database, if not existing
        QSqlQuery query(m_db);
        // Note that due to clean_str the album url can not be unique at the moment (some albums might end up having the same cover)
        ret = query.exec("CREATE TABLE IF NOT EXISTS albums "
                         "(id INTEGER PRIMARY KEY, "
                         "url TEXT, "
                         "title TEXT UNIQUE, "
                         "artist TEXT)");
        if (!ret)
            qWarning() << query.lastError().text() << " on create table albums.";
        ret = query.exec("CREATE TABLE IF NOT EXISTS songs "
                         "(id INTEGER PRIMARY KEY, "
                         "url TEXT UNIQUE, "
                         "albumId INTEGER, "
                         "trackNumber INTEGER, "
                         "title TEXT, "
                         "artist TEXT, "
                         "genre TEXT, "
                         "FOREIGN KEY(albumId) REFERENCES albums(id) ON DELETE CASCADE)"
                         );
        if (!ret)
            qWarning() << query.lastError().text() << " on create table songs.";

        createMusicIndexes();

        //if (updateMusic)
          //  _updateMusicDb();
    }
}
#else
QMdLiteMusicPrivate::QMdLiteMusicPrivate (QMdLiteMusic *q, JsonDbConnection *jsonDbConn) : m_q(q)
{
    if (jsonDbConn) {
        //qDebug() << "QMdLiteMusicPrivate::QMdLiteMusicPrivate using: " << jsonDbConn;
        m_db = jsonDbConn;
    }
    else {
        //qDebug() << "QMdLiteMusicPrivate::QMdLiteMusicPrivate creating a JsonDbConnection";
        m_db = JsonDbConnection::instance();
        /*m_db = new JsonDbConnection;
        qDebug() << "QMdLiteMusicPrivate::QMdLiteMusicPrivate connecting to the server";
        m_db->connectToServer();*/
    }
}
#endif

QMdLiteMusicPrivate::~QMdLiteMusicPrivate ()
{
#ifdef _SQLITE_QMDBACKEND
    m_db.close();
    QSqlDatabase::removeDatabase("music");
#endif
}

QString QMdLiteMusicPrivate::albumArtImageBaseName (const QString& albumArtist, const QString& title)
{
    QString helper = cleanStr (title);
    QString ret = QString::fromAscii(QCryptographicHash::hash ( helper.toLocal8Bit(), QCryptographicHash::Md5).toHex());
    helper = cleanStr(albumArtist);
    ret.append(QLatin1String("-")).append(QString::fromAscii(QCryptographicHash::hash ( helper.toLocal8Bit(), QCryptographicHash::Md5).toHex()));
    return ret;
}

QString QMdLiteMusicPrivate::albumArtImagePath (const QString& baseName)
{
    QString path = baseName;
    path.append(QLatin1String(".jpeg"));
    return path;
}

QString QMdLiteMusicPrivate::albumArtImageCompressedPath (const QString& baseName)
{
    QString path = baseName;
    path.append(QLatin1String(".pkm"));
    return path;
}

QString QMdLiteMusicPrivate::albumArtThumbnailImagePath (const QString& baseName)
{
    QString path = baseName;
    path.append(QLatin1String("-thumbnail.ppm"));
    return path;
}

QString QMdLiteMusicPrivate::albumArtCompressedTexturePath (const QString& baseName)
{
    QString path = baseName;
    path.append(QLatin1String("-thumbnail.pkm"));
    return path;
}

QMdLiteMusic::QMdLiteMusic(QObject *parent, JsonDbConnection *jsonDbConn) :
    QMdLite(parent),  d(new QMdLiteMusicPrivate (this, jsonDbConn))
{
    //qDebug() << "QMdLiteMusic::QMdLiteMusic";
}

QMdLiteMusic::~QMdLiteMusic()
{
    //qDebug() << "QMdLiteMusic::~QMdLiteMusic";
}

QMdLite::DBType QMdLiteMusic::dbType()
{
    return d->dbType();
}

#ifndef _SQLITE_QMDBACKEND
QMdLiteQueryResultIf *QMdLiteMusic::createAlbumQuery (const QStringList& properties, const QString& filterStr, const QStringList& sortProperties)
{
    //QString jsonQuery = d->createQueryString("albums", properties, filterStr, sortProperties);
    //return d->m_db.exec(sqlQueryStr);
    QString type(cJsondbNamespace + QLatin1String("AlbumsView"));
    QString baseQuery(QLatin1String("[?_type=\"") + cJsondbNamespace + QLatin1String("AlbumsView\"]"));
    QStringList _properties;
    _properties.append (QLatin1String("_uuid"));
    foreach (QString property, properties) {
        if (property.compare(QLatin1String("title"), Qt::CaseInsensitive) == 0)
            _properties.append(QLatin1String("album.title"));
        else if (property.compare(QLatin1String("artist"), Qt::CaseInsensitive) == 0)
            _properties.append(QLatin1String("artistToDisplay"));
        else if (property.compare(QLatin1String("albumArtist"), Qt::CaseInsensitive) == 0)
            _properties.append(QLatin1String("album.artist"));
        else
            _properties.append (property);
    }
    QStringList _sortProperties;
    foreach (QString sortProperty, sortProperties) {
        if (sortProperty.compare(QLatin1String("title"), Qt::CaseInsensitive) == 0)
            _sortProperties.append(QLatin1String("album.title"));
        else if (sortProperty.compare(QLatin1String("artist"), Qt::CaseInsensitive) == 0)
            _sortProperties.append(QLatin1String("artistToDisplay"));
        else if (sortProperty.compare(QLatin1String("albumArtist"), Qt::CaseInsensitive) == 0)
            _sortProperties.append(QLatin1String("album.artist"));
        else
            _sortProperties.append (sortProperty);
    }
    QMdLiteQueryResult *query =
            new QMdLiteQueryResult(d->m_db, type, baseQuery, filterStr, _properties, _sortProperties);
    return query;
}

//
QMdLiteQueryResultIf *QMdLiteMusic::createArtistAlbumsQuery (const QString& artist, const QString& filterStr)
{
    /*QString artistCleaned(artist);
    artistCleaned.replace(QString("'"), QString("''"));
    // Always read id
    QString sqlQueryStr("SELECT songs.albumId, songs.artist, COUNT(*), COUNT(*), albums.title, albums.url "
                        "FROM songs, albums WHERE "
                        "songs.albumId = albums.id AND songs.artist = '");
    sqlQueryStr.append(artistCleaned).append("'");
    if (!filterStr.isEmpty()) {
        sqlQueryStr.append(" ").append(filterStr);
    }
    sqlQueryStr.append(" GROUP BY songs.albumId ORDER BY albums.title");
    return d->m_db.exec(sqlQueryStr);*/
    QString artistCleaned = artist;
    artistCleaned.replace(QLatin1String("\""), QLatin1String("\\\""));
    artistCleaned.replace(QLatin1String("\\"), QLatin1String("\\\\"));
    QString type(cJsondbNamespace + QLatin1String("AlbumsOfArtistsView"));
    QString baseQuery(QLatin1String("[?_type=\"") + cJsondbNamespace + QLatin1String("AlbumsOfArtistsView\"][?artist=\""));
    baseQuery.append(artistCleaned).append (QLatin1String("\"]"));
    QStringList _properties;
    _properties.append(QLatin1String("_uuid"));
    _properties.append(QLatin1String("artist"));
    _properties.append(QLatin1String("songCount"));
    _properties.append(QLatin1String("album.artist"));
    _properties.append(QLatin1String("album.title"));
    _properties.append(QLatin1String("albumArt"));
    //_properties.append(QLatin1String("albumId"));
    QStringList _sortProperties;
    _sortProperties.append(QLatin1String("album.title"));

    QMdLiteQueryResult *query =
            new QMdLiteQueryResult(d->m_db, type, baseQuery, filterStr, _properties, _sortProperties);
    //qDebug () << "createArtistAlbumsQuery: " << baseQuery << ", " << filterStr << ", " << _properties << ", " << _sortProperties;
    return query;
}

//
QMdLiteQueryResultIf *QMdLiteMusic::createArtistQuery (const QString& filterStr)
{
    /*
    // SELECT id, GROUP_CONCAT(url, ",,"), GROUP_CONCAT(title, ",,"), artist, COUNT(*) FROM albums GROUP BY artist ORDER BY artist
    // Always read id
    QString sqlQueryStr("SELECT songs.id, songs.artist, COUNT(*), COUNT(DISTINCT albums.title), GROUP_CONCAT(albums.title, ',,'), GROUP_CONCAT(albums.url, ',,') "
                        "FROM songs, albums  WHERE songs.albumId = albums.id ");
    if (!filterStr.isEmpty()) {
        sqlQueryStr.append(" AND ").append(filterStr);
    }
    sqlQueryStr.append(" GROUP BY songs.artist ORDER BY songs.artist");
    return d->m_db.exec(sqlQueryStr);

    QString jsonQuery(QLatin1String());
    */
    QString type(cJsondbNamespace + QLatin1String("ArtistsView"));
    QString baseQuery(QLatin1String("[?_type=\"") + cJsondbNamespace + QLatin1String("ArtistsView\"]"));
    QStringList _properties;
    _properties.append (QLatin1String("_uuid"));
    _properties.append(QLatin1String("artist"));
    _properties.append(QLatin1String("songCount"));
    _properties.append(QLatin1String("albums"));
    _properties.append(QLatin1String("covers"));
    QStringList _sortProperties;
    _sortProperties.append (QLatin1String("artist"));
    QMdLiteQueryResult *query =
            new QMdLiteQueryResult(d->m_db, type, baseQuery, filterStr, _properties, _sortProperties);
    //qDebug () << "createArtistQuery: " << baseQuery << ", " << filterStr << ", " << _properties << ", " << _sortProperties;
    return query;
}

QMdLiteQueryResultIf *QMdLiteMusic::createMusicQuery (const QStringList& properties, const QString& filterStr, const QStringList& sortProperties)
{
    // [?_type="songs"][?title=~"/*searchstr*/wi"][= albumId->title, albumId->artist, albumId->url][/title]
    // [?_type="songs"][?albumId->title=~"/*searchstr*/wi"][= albumId->title, albumId->artist, albumId->url][/title]

    QStringList _properties;
    _properties.append(QLatin1String("_uuid"));
    foreach (QString property, properties) {
        if (property.compare(QLatin1String("albumTitle"), Qt::CaseInsensitive) == 0)
            _properties.append(QLatin1String("album.title"));
        else if (property.compare(QLatin1String("albumArtist"), Qt::CaseInsensitive) == 0)
            _properties.append(QLatin1String("album.artist"));
        else
            _properties.append(property);
    }
    _properties.append(QLatin1String("_uuid"));
    QStringList _sortProperties;
    foreach (QString sortProperty, sortProperties) {
        if (sortProperty.compare(QLatin1String("albumTitle"), Qt::CaseInsensitive) == 0)
            _sortProperties.append(QLatin1String("album.title"));
        else if (sortProperty.compare(QLatin1String("albumArtist"), Qt::CaseInsensitive) == 0)
            _sortProperties.append(QLatin1String("album.artist"));
        else
            _sortProperties.append(sortProperty);
    }

    QString type(cJsondbNamespace + QLatin1String("SongsView"));
    QString baseQuery(QLatin1String("[?_type=\"") + cJsondbNamespace + QLatin1String("SongsView\"]"));
    //qDebug () << "createMusicQuery: " << baseQuery << ", " << filterStr << ", " << _properties;
    QMdLiteQueryResult *query =
            new QMdLiteQueryResult(d->m_db, type, baseQuery, filterStr, _properties, _sortProperties);
    return query;
}

#else

QMdLiteQueryResult QMdLiteMusic::createAlbumQuery (const QStringList& properties, const QString& filterStr, const QStringList& sortProperties)
{
    QString sqlQueryStr = d->createQueryString("albums", properties, filterStr, sortProperties);
    QSqlQuery q = d->m_db.exec(sqlQueryStr);
    if (!q.isActive())
        qWarning () << "Error: " << q.lastError().text() << " on album query.";
    return QMdLiteQueryResult(new QMdLiteSqlQueryResult (q));
}

//
/*QSqlQuery QMdLiteMusic::createAlbumArtistQuery (const QString& artist, const QString& filterStr)
{
    // Always read id
    QString sqlQueryStr("SELECT id, artist, COUNT(*), GROUP_CONCAT(albums.title, ',,'), GROUP_CONCAT(albums.url, ',,') "
                        "FROM songs, albums GROUP BY songs.albumId ORDER BY artist WHERE "
                        "songs.albuId = albums.id AND songs.artist = ");
    sqlQueryStr.append(artist);
    if (!filterStr.isEmpty()) {
        sqlQueryStr.append(" ").append(filterStr);
    }
    QSqlQuery q = d->m_db.exec(sqlQueryStr);
    if (!q.isActive())
        qWarning () << "Error: " << q.lastError().text() << " on album query.";
    return QMdLiteSqlQueryResult (q);
}*/

//
QMdLiteQueryResult QMdLiteMusic::createArtistAlbumsQuery (const QString& artist, const QString& filterStr)
{
    QString artistCleaned(artist);
    artistCleaned.replace(QString("'"), QString("''"));
    // Always read id
    QString sqlQueryStr("SELECT songs.albumId, songs.artist, COUNT(*), COUNT(*), albums.title, albums.url "
                        "FROM songs, albums WHERE "
                        "songs.albumId = albums.id AND songs.artist = '");
    sqlQueryStr.append(artistCleaned).append("'");
    if (!filterStr.isEmpty()) {
        sqlQueryStr.append(" ").append(filterStr);
    }
    sqlQueryStr.append(" GROUP BY songs.albumId ORDER BY albums.title");
    QSqlQuery q = d->m_db.exec(sqlQueryStr);
    if (!q.isActive())
        qWarning () << "Error: " << q.lastError().text() << " on artists albums query.";
    return QMdLiteQueryResult(new QMdLiteSqlQueryResult (q));
}


//
QMdLiteQueryResult QMdLiteMusic::createArtistQuery (const QString& filterStr)
{
    // SELECT id, GROUP_CONCAT(url, ",,"), GROUP_CONCAT(title, ",,"), artist, COUNT(*) FROM albums GROUP BY artist ORDER BY artist
    // Always read id
    QString sqlQueryStr("SELECT songs.id, songs.artist, COUNT(*), COUNT(DISTINCT albums.title), GROUP_CONCAT(albums.title, ',,'), GROUP_CONCAT(albums.url, ',,') "
                        "FROM songs, albums  WHERE songs.albumId = albums.id ");
    if (!filterStr.isEmpty()) {
        sqlQueryStr.append(" AND ").append(filterStr);
    }
    sqlQueryStr.append(" GROUP BY songs.artist ORDER BY songs.artist");
    QSqlQuery q = d->m_db.exec(sqlQueryStr);
    if (!q.isActive())
        qWarning () << "Error: " << q.lastError().text() << " on artist query.";
    return QMdLiteQueryResult(new QMdLiteSqlQueryResult (q));
}

//
QMdLiteQueryResult QMdLiteMusic::createMusicQuery (const QStringList& properties, const QString& filterStr, const QStringList& sortProperties)
{
    // Always read id
    QString sqlQueryStr("SELECT songs.id");
    bool joinAlbumTable = false;
    foreach (QString property, properties) {
        sqlQueryStr.append(", ");
        if (property.compare("albumTitle", Qt::CaseInsensitive) == 0) {
            joinAlbumTable = true;
            sqlQueryStr.append("albums.title");
        }
        else if (property.compare("albumArtist", Qt::CaseInsensitive) == 0) {
            joinAlbumTable = true;
            sqlQueryStr.append("albums.artist");
        }
        else if (property.compare("albumCover", Qt::CaseInsensitive) == 0) {
            joinAlbumTable = true;
            sqlQueryStr.append("albums.url");
        }
        else
            sqlQueryStr.append("songs.").append(property);
    }

    sqlQueryStr.append(" FROM songs");
    if (joinAlbumTable) {
        sqlQueryStr.append(", albums");
    }


    if (filterStr.length() > 0) {
        sqlQueryStr.append(" WHERE ");
        sqlQueryStr.append(filterStr);
        if (joinAlbumTable) {
            sqlQueryStr.append(" AND songs.albumId=albums.id");
        }
    }
    else if (joinAlbumTable) {
        sqlQueryStr.append(" WHERE songs.albumId=albums.id");
    }
    if (sortProperties.count() > 0) {
        sqlQueryStr.append(" ORDER BY ");
        if (joinAlbumTable) {
            bool beg = true;
            foreach (QString property, sortProperties) {
                if (!beg)
                    sqlQueryStr.append(", ");
                else beg = false;
                if (property.compare("albumTitle", Qt::CaseInsensitive) == 0) {
                    joinAlbumTable = true;
                    sqlQueryStr.append("albums.title");
                }
                else if (property.compare("albumArtist", Qt::CaseInsensitive) == 0) {
                    joinAlbumTable = true;
                    sqlQueryStr.append("albums.artist");
                }
                else if (property.compare("albumCover", Qt::CaseInsensitive) == 0) {
                    joinAlbumTable = true;
                    sqlQueryStr.append("albums.url");
                }
                else
                    sqlQueryStr.append("songs.").append(property);
            }

        }
        else
            sqlQueryStr.append(sortProperties.join(", "));
    }
    //qDebug () << "Music query: " << sqlQueryStr;
    QSqlQuery q = d->m_db.exec(sqlQueryStr);
    if (!q.isActive())
        qWarning () << "Error: " << q.lastError().text() << " on music query.";
    return QMdLiteQueryResult(new QMdLiteSqlQueryResult (q));
}

#endif

/*QString QMdLiteMusic::albumArtImagePath (const QString& title)
{
    QString imageBaseName = QMdLiteMusicPrivate::albumArtImageBaseName(title);
    QString imageName = QMdLiteMusicPrivate::albumArtImagePath(QMdLiteMusicPrivate::musicAlbumArtDir()+imageBaseName);
    return imageName;
}

QString QMdLiteMusic::albumArtThumbnailImagePath (const QString& title)
{
    QString imageBaseName = QMdLiteMusicPrivate::albumArtImageBaseName(title);
    QString thumbnailName;

#ifdef QT_OPENGL_ES
    thumbnailName = QMdLiteMusicPrivate::albumArtCompressedTexturePath(QMdLiteMusicPrivate::musicAlbumArtDir()+imageBaseName);
    if (!QFile::exists(thumbnailName)) {
        thumbnailName = QMdLiteMusicPrivate::albumArtCompressedTexturePath(QString::fromAscii(":/image/default"));
    }
#else
    thumbnailName = QMdLiteMusicPrivate::albumArtThumbnailImagePath(QMdLiteMusicPrivate::musicAlbumArtDir()+imageBaseName);
    if (!QFile::exists(thumbnailName)) {
        thumbnailName = QMdLiteMusicPrivate::albumArtThumbnailImagePath(QString::fromAscii(":/image/default"));
    }
#endif
    //qDebug() << "albumArtThumbnailImagePath returning: " << thumbnailName;

    return thumbnailName;
}*/
