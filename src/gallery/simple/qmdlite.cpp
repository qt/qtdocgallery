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

#include <QDesktopServices>
#include <QStringList>
#include <QtDebug>
#include <QDir>
#include <QVector>
#include <QCryptographicHash>
#include <QImage>
#include <QDateTime>

#include "qmdlite.h"
#include "qmdlite_p.h"

#ifdef _SQLITE_QMDBACKEND
#include <QSqlQuery>
#include "qmdlitequeryresultsqlite_p.h"
#else
#include <private/jsondb-connection_p.h>
#include <jsondb-client.h>
#include <private/jsondb-strings_p.h>
#include "qmdlitequeryresultjsondb_p.h"
QT_ADDON_JSONDB_USE_NAMESPACE
#endif

QMdLitePrivate::QMdLitePrivate ()
{
}

QMdLitePrivate::~QMdLitePrivate ()
{
#ifdef _SQLITE_QMDBACKEND
    m_db.close();
#endif
}

QString QMdLitePrivate::sortProperties2String (const QStringList& sortProperties, bool ascending)
{
    QString ret;

    if (sortProperties.count() > 0) {
#ifdef _SQLITE_QMDBACKEND
        ret.append(QLatin1String(" ORDER BY "));
        if (!ascending) ret.append(sortProperties.join(QLatin1String(" DESC, ")));
        else ret.append(sortProperties.join(QLatin1String(", ")));
#else
        foreach (const QString &property, sortProperties) {
            ret.append (QLatin1String("["));
            if (ascending) ret.append(QLatin1String("/")).append(property);
            else ret.append(QLatin1String("\\")).append(property);
            ret.append (QLatin1String("]"));
        }
#endif
    }
    return ret;
}

// Return id always as the first item in row. Properties in index+1.
QString QMdLitePrivate::createQueryString
(const QString& table, const QStringList& properties, const QString& filterStr, const QStringList& sortProperties)
{
#ifdef _SQLITE_QMDBACKEND
    // Always read id
    QString sqlQueryStr("SELECT id");
    if (properties.length() > 0) {
        sqlQueryStr.append(", ");
        sqlQueryStr.append(properties.join(", "));
    }
    sqlQueryStr.append(" FROM ").append(table);

    if (filterStr.length() > 0) {
        sqlQueryStr.append(" WHERE ");
        sqlQueryStr.append(filterStr);
    }
    sqlQueryStr.append(sortProperties2String (sortProperties));
    //qDebug () << "Select: " << sqlQueryStr;
    return sqlQueryStr;
#else
    QString jsonQuery = QLatin1String("[?_type=\"");
    jsonQuery.append(table).append(QLatin1String("\"]"));

    if (filterStr.length() > 0) {
        jsonQuery.append(QLatin1String("["));
        jsonQuery.append(filterStr);
        jsonQuery.append(QLatin1String("]"));
    }
    if (properties.length() > 0) {
        jsonQuery.append(QLatin1String("[= [ _uuid"));
        foreach (QString property, properties) {
            jsonQuery.append(QLatin1String(" , "));
            jsonQuery.append(property);
        }

        //jsonQuery.append(properties.join(QLatin1String(",")));
        jsonQuery.append(QLatin1String(" ] ]"));
    }
    jsonQuery.append(sortProperties2String (sortProperties));
    //qDebug () << "query: " << jsonQuery;
    return jsonQuery;
#endif
}

int QMdLitePrivate::query
(const QString& table, QList<QVariantList>& rows, const QStringList& properties, const QString& filterStr, const QStringList& sortProperties)
{
    QString queryStr = createQueryString (table, properties, filterStr, sortProperties);
    int ret = 0;

#ifdef _SQLITE_QMDBACKEND
    QSqlQuery query(queryStr, m_db);

    while (query.next()) {
        QVariantList row;
        row.append(query.value(0)); // Id
        for (int ii=1; ii<=properties.count(); ii++) {
            row.append (query.value(ii));
        }
        rows.append(row); ret++;
    }
#else
    // TODO: change into using int request (QVariantMap&) + a connections to void response (int, QVariant&) -signal
    const QVariant data = m_db->sync(JsonDbConnection::makeQueryRequest(queryStr));
    const QVariantList list = data.toMap().value(QLatin1String("data")).toList();
    foreach (const QVariant &item, list) {
        QVariantList row = item.toList();
        rows.append(row); ret++;
    }
    //qDebug () << "query count returned: " << ret;
#endif
    return ret;
}

#ifndef _SQLITE_QMDBACKEND
QVariant QMdLitePrivate::jsonDbCreate (JsonDbConnection *db, const QStringList& properties, const QVariantList& values)
{
    QVariantMap map;
    for (int ii=0; ii<values.length(); ii++) {
        map.insert (properties[ii], values[ii]);
    }
    const QVariant data = db->sync(JsonDbConnection::makeCreateRequest(map));
    const QVariantMap result = data.toMap();
    return result.value(QLatin1String("_uuid"));
}
#endif

QVariant QMdLitePrivate::insert (const QString &table, QStringList& properties, QVariantList& values)
{
    Q_ASSERT (properties.length() == values.length());

#ifdef _SQLITE_QMDBACKEND
    QString queryStr(QString::fromLatin1("INSERT INTO ").append(table).append(" ("));
    queryStr.append(properties.join(", ")).append(") VALUES (");
    for (int ii=0; ii<values.length(); ii++) {
        if (values.at(ii).isNull())
            queryStr.append("NULL");
        else if (values.at(ii).type() == QVariant::String) {
            QString insStr = values.at(ii).toString();
            insStr.replace(QString("'"), QString("''"));
            queryStr.append("'").append(insStr).append("'");
        }
        else
            queryStr.append(values.at(ii).toString());
        if (ii < values.length()-1) queryStr.append(", ");
    }
    queryStr.append(")");

    //qDebug() << "Insert query: " << queryStr;

    QSqlQuery query(m_db);
    bool ret = query.exec(queryStr);
    quint32 insertId = -1;

    if (!ret) {
        qDebug () << "Error: " << query.lastError().text() << " on insert query: " << queryStr;
        return -1;
    }
    else {
        insertId = query.lastInsertId().toInt();
    }

    return insertId;
#else
    properties.prepend (QLatin1String("_type"));
    values.prepend (table);
    return QMdLitePrivate::jsonDbCreate (m_db, properties, values);
#endif
}

bool QMdLitePrivate::update (const QString &table, QVariant id, const QStringList& properties, const QVariantList& values)
{
    Q_ASSERT (properties.length() == values.length());
#ifdef _SQLITE_QMDBACKEND
    QString queryStr(QString::fromLatin1("UPDATE ").append(table).append(" SET "));
    for (int ii=0; ii<properties.length(); ii++) {
        if (values.at(ii).isNull())
            queryStr.append("=NULL");
        if (values.at(ii).type() == QVariant::String) {
            QString updStr = values[ii].toString();
            updStr.replace(QString("'"), QString("''"));
            queryStr.append(properties[ii]).append ("=").append("'").append(updStr).append("'");
        }
        else
            queryStr.append(properties[ii]).append ("=").append(values[ii].toString());
        if (ii < properties.length()-1) queryStr.append(",");
    }
    queryStr.append(" WHERE id = ").append(QString::number(id.toInt()));
    //qDebug() << "Update query: " << queryStr;

    QSqlQuery query(m_db);
    bool ret = query.exec(queryStr);

    if (!ret)
        qDebug () << "Error: " << query.lastError().text() << " on update query: " << queryStr;

    return ret;
#else
    //qDebug () << "Updating object: " << id << " of type " << table;
    QString _uuid = id.toString();
    QString jsonQuery = QLatin1String("[?_type=\"");
    jsonQuery.append(table).append(QLatin1String("\"][?_uuid=\"")).append(_uuid).append(QLatin1String("\"]"));
    const QVariant data = m_db->sync(JsonDbConnection::makeQueryRequest(jsonQuery));
    const QVariantList list = data.toMap().value(QLatin1String("data")).toList();
    if (list.count() != 1) {
        qDebug () << "Error on update: no object to be updated found, _uuid: " << id;
        return false;
    }
    QVariantMap object;
    object = list.at(0).toMap();
    qDebug () << "Updating object(2): " << object[QLatin1String("_uuid")] << " of type " << object[QLatin1String("_type")];
    for (int ii=0; ii<properties.length(); ii++) {
        /*if (object.contains(properties[ii]))
            qDebug () << "  setting " << properties[ii] << " from " << object[properties[ii]] << " to " << values[ii];
        else
            qDebug () << "  adding " << properties[ii] << " with value " << values[ii];*/
        object[properties[ii]] = values[ii];
    }
    const QVariant data2 = m_db->sync(JsonDbConnection::makeUpdateRequest(object));
    //qDebug () << "Update returned: " << data2;
    return true;
#endif
}

QString QMdLitePrivate::cleanStr (const QString &str) {
    if (str.isNull() || str.length() < 1) return QString::fromLatin1(" ");
    QString ret = str;
    //QRegExp re("\\(.*\\)|\\{.*\\}|\\[.*\\]|<.*>|[\\(\\)_\\{\\}\\[\\]\\!@#$\\^&\\*\\+\\=\\|\\\\/\"'\\?~`]");
    QRegExp re(QLatin1String("[\\(\\)_\\{\\}\\[\\]\\!@#$\\^&\\*\\+\\=\\|\\\\/\"'\\?~`]"));
    //QRegExp re2("\\  *");
    while (ret.length() != ret.replace(re, QLatin1String("")).length()) {
        //while (ret.length() != ret.replace(re2, " ").length());
    }
    return ret.toLower().simplified();
}

bool QMdLitePrivate::remove(const QString &table, QList<QVariant> &ids)
{
    Q_UNUSED(table)

    if (ids.length() < 1)
        return true;

#ifdef _SQLITE_QMDBACKEND
    QStringList idStrs;
    foreach (QVariant id, ids) {
        QString numStr; numStr.setNum(id.toUInt());
        idStrs.append(numStr);
    }

    QString queryStr(QString::fromLatin1("DELETE FROM ").append(table).append(" WHERE id IN ("));
    queryStr.append(idStrs.join(", ")).append(")");

    qDebug() << "Delete query: " << queryStr;

    QSqlQuery deleteQuery(queryStr, m_db);
    bool ret = deleteQuery.exec();

    if (!ret) qDebug () << "Error: " << deleteQuery.lastError().text() << " on delete.";

    return ret;
#else
    foreach (QVariant id, ids) {
        QVariantMap object;
        object[QLatin1String("_uuid")] = id;
        m_db->sync(JsonDbConnection::makeRemoveRequest(object));
        //m_jsonDbclient->remove(object);
    }
#endif
    return true;
}

QFileInfoList QMdLitePrivate::listAllFiles (const QString& dirPath, const QStringList& filters)
{
    QDir files(dirPath);
    //qDebug() << "listAllFiles: " << files.canonicalPath();
    files.setFilter(QDir::Files);
    files.setNameFilters(filters);
    QFileInfoList ret = files.entryInfoList();
    QDir directories(dirPath);
    directories.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList dirs = directories.entryInfoList();
    foreach (QFileInfo fi, dirs) {
        ret.append(listAllFiles(fi.canonicalFilePath(), filters));
    }
    return ret;
}

QString QMdLitePrivate::dbDirName ()
{
    QString ret = QDir::homePath();
    if (ret.isEmpty())
        ret = QString::fromLatin1("/krakatau");
    ret.append (QDir::separator()).append(QLatin1String(".qmdlite"));

    if (!QFile::exists (ret)) {
        QDir dirO;
        dirO.mkpath(ret);
    }
    //qDebug () << "dbDirName: " << ret;
    //qDebug () << "current path: " << QDir::currentPath();
    return ret;
}

bool QMdLitePrivate::openDb(const QString &dbPath, const QString &dbName)
{
#ifdef _SQLITE_QMDBACKEND
    // Find QSLite driver
    m_db = QSqlDatabase::addDatabase("QSQLITE", dbName);
    m_db.setDatabaseName(dbPath);
    // Open database
    return m_db.open();
#else
    Q_UNUSED(dbPath)
    Q_UNUSED(dbName)
#endif
    return true;
}

QMdLite::QMdLite(QObject *parent)
    : QObject(parent)
{
    //qDebug() << "QMdLite::QMdLite";
}

QMdLite::~QMdLite()
{
    //qDebug() << "QMdLite::~QMdLite";
}
