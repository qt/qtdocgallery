/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef QMDLITEQUERYRESULTJSONDB_P_H
#define QMDLITEQUERYRESULTJSONDB_P_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QObject>
#include <private/jsondb-connection_p.h>
#include <jsondb-client.h>
#include "qmdlite.h"

// Number of rows in cache
// TODO: Change this to KBytes instead of number of rows?
#define CACHE_SIZE 50
// When to update the cache
#define SAFETY_MARGIN 15

QT_ADDON_JSONDB_BEGIN_NAMESPACE
class JsonDbConnection;
class JsonDbClient;
QT_ADDON_JSONDB_END_NAMESPACE

QT_ADDON_JSONDB_USE_NAMESPACE
class QMdLiteQueryResult;

class QMdLiteQueryResultPrivate : public QObject
{
    Q_OBJECT
public:
    QMdLiteQueryResultPrivate(QMdLiteQueryResult *q, JsonDbConnection *jdbCon, const QString &type,
                              const QString &baseQuery, const QString &filterStr,
                              const QStringList &properties, const QStringList &sortProperties,
                              bool chunk = false);
    ~QMdLiteQueryResultPrivate();
    QString m_type;
    QString m_baseQuery;
    QString m_filterString;
    QStringList m_properties;
    QStringList m_sortProperties;
    QStringList m_uuids;
    quint32 m_count;
    qint32 m_index;
    bool m_chunk; // Do we use chunk queries or uuid based
    bool m_valid; // True when both count/uuid query is complete as well as notification object creation
    bool m_itemsInsertedSent;
    QString m_notifUuid;
    qint32 m_cache_start;
    QList<QVariantList> m_row_cache;
    bool m_requestPending;
    QList<int> m_countRequestId;
    int m_notifRequestId;
    quint32 m_requestId;
    quint64 m_requestTimestamp; // Time when request was sent
    qint32 m_requestOffset; //
    qint32 m_requestLimit; //
    QMap<quint32, quint64> m_voidIndexes; // Indexes returned with default values + timestamp
    QMap<QString, QVariantMap> m_insertedObjects;
    QMap<QString, QVariantMap> m_removedObjects;
    JsonDbConnection *m_jsonDbConnection;
    JsonDbClient *m_jsonDbClient;

    QMdLiteQueryResult *m_q;

    inline bool in_cache (quint32 idx) const
    {
        return m_cache_start >= 0
               && idx >= (quint32)m_cache_start
               && idx < (quint32)m_cache_start + m_row_cache.count();
    }

    void handleCountAndNotifRequest(int id, const QVariant &v);

public Q_SLOTS:
    void jsonDbResponse(int id, const QVariant &v);
    void jsonDbError(int id, int code, const QString &message);
    void jsonDbNotified(const QString &, const QVariant &, const QString &);
};

class QMdLiteQueryResult : public QMdLiteQueryResultIf
{
    Q_OBJECT

public:
    QMdLiteQueryResult(JsonDbConnection *jdbCon, const QString &type, const QString &baseQuery,
                       const QString &filterStr, const QStringList &properties, const QStringList &sortProperties)
        : d(new QMdLiteQueryResultPrivate(this, jdbCon, type, baseQuery, filterStr,
                                          properties, sortProperties, false))
    {
    }

    virtual ~QMdLiteQueryResult() {}

    virtual int at() const;
    virtual bool first();
    virtual bool isActive() const;
    virtual bool last();
    virtual bool next();
    virtual bool previous();
    virtual bool seek(int index, bool relative = false, bool forceRefresh = false);
    virtual int size() const;
    virtual bool isValid() const;
    virtual QVariant value(int index) const;

private:
    friend class QMdLiteQueryResultPrivate;
    QScopedPointer<QMdLiteQueryResultPrivate> d;

    Q_DISABLE_COPY(QMdLiteQueryResult)
};
#endif // QMDLITEQUERYRESULTJSONDB_P_H
