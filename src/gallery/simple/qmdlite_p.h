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

#ifndef QMDLITEPRIVATE_H
#define QMDLITEPRIVATE_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QImage>
#include <QVariantList>
#include "qmdlite.h"

#ifdef _SQLITE_QMDBACKEND
class QDqlDatabase;
#else
#include <private/jsondb-connection_p.h>
QT_ADDON_JSONDB_BEGIN_NAMESPACE
class JsonDbConnection;
QT_ADDON_JSONDB_END_NAMESPACE
QT_ADDON_JSONDB_USE_NAMESPACE
#endif

class QMdLitePrivate : public QObject
{
    Q_OBJECT

public:
    QMdLitePrivate();
    ~QMdLitePrivate();

    bool openDb(const QString &dbPath, const QString &dbName);
    static QString sortProperties2String(const QStringList &sortProperties, bool ascending = true);
    QString createQueryString(const QString &table, const QStringList& properties,
                              const QString &filterStr = QString(),
                              const QStringList&sortProperties = QStringList());
    int query(const QString &table, QList<QVariantList> &rows, const QStringList &properties = QStringList(),
              const QString &filterStr = QString(), const QStringList &sortProperties = QStringList());
    bool remove(const QString &table, QList<QVariant> &ids);
    QVariant insert(const QString &table, QStringList &properties, QVariantList &values);
    bool update(const QString &table, QVariant id, const QStringList &properties, const QVariantList &values);

    static QString filterToString(const QString &tableName, const QString &filterStr = QString());
    static QString cleanStr(const QString &str);
    static QFileInfoList listAllFiles(const QString &, const QStringList &);
    static QString dbDirName();
    QMdLite::DBType dbType()
    {
#ifdef _SQLITE_QMDBACKEND
        return QMdLite::SQLite;
#else
        return QMdLite::JsonDB;
#endif
    }

#ifdef _SQLITE_QMDBACKEND
    QSqlDatabase m_db;
#else
    JsonDbConnection *m_db;
    static QVariant jsonDbCreate(JsonDbConnection *, const QStringList &, const QVariantList &);
#endif
};

#endif // QMDLITEPRIVATE_H
