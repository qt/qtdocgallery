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

#ifndef QMDLITE_H
#define QMDLITE_H

#include <QObject>
#include <QVariantList>
#include <QStringList>

class QMdLiteQueryResultIf : public QObject
{
    Q_OBJECT

public:
    virtual ~QMdLiteQueryResultIf() {}

protected:
    // This is an abstract base class
    QMdLiteQueryResultIf() {}

public:
    virtual int at() const = 0;
    virtual bool first() = 0;
    virtual bool isActive() const = 0;
    virtual bool last() = 0;
    virtual bool next() = 0;
    virtual bool previous() = 0;
    virtual bool seek(int index, bool relative = false, bool forceRefresh = false) = 0;
    virtual int size() const = 0;
    virtual bool isValid() const = 0;
    virtual QVariant value(int index) const = 0;

signals:
    void rowChanged(quint32 index, int count);
    void itemsInserted(int index, int count);
    void itemsRemoved(int index, int count);
};

class QMdLite : public QObject
{
public:
    ~QMdLite();

protected:
    // This is an abstract base class
    QMdLite(QObject *parent = 0);

public:
    enum DBType {
        SQLite,
        JsonDB
    };

    virtual DBType dbType () = 0;
    //virtual void *DBConnection() = 0;

    QString createQueryString(const QString &table, const QStringList &properties,
                              const QString &filterStr = QString(),
                              const QStringList &sortProperties = QStringList());
    int query(const QString &table, QList<QVariantList> &rows, const QStringList &properties = QStringList(),
              const QString &filterStr = QString(), const QStringList &sortProperties = QStringList());
    bool remove(const QString &table, QList<quint32> &ids);
    quint32 insert(const QString &table, const QStringList &properties, const QVariantList &values);
    bool update(const QString &table, quint32 id, const QStringList &properties, const QVariantList &values);
};

#endif // QMDLITE_H
