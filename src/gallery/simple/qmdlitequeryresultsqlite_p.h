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

#ifndef QMDLITEQUERYRESULTSQLITE_P_H
#define QMDLITEQUERYRESULTSQLITE_P_H

class QSqlQuery;
class QSqlDatabase;

class QMdLiteQueryResultPrivate
{
public:
    QMdLiteQueryResultPrivate(QSqlQuery &query) : m_query(query) {}
    QSqlQuery m_query;
};

class QMdLiteQueryResult : public QMdLiteQueryResultIf
{
public:
    QMdLiteSqlQueryResult(QSqlQuery &query)
        : d(new QMdLiteQueryResultPrivate(query)) {}
    virtual ~QMdLiteSqlQueryResult() {}

    virtual int at() const;
    virtual bool first();
    virtual bool isActive() const;
    virtual bool last();
    virtual bool next();
    virtual bool previous();
    virtual bool seek(int index, bool relative = false, bool forceRefresh = false);
    virtual int size() const;
    virtual QVariant value(int index) const;
    void setSqlQuery(QSqlQuery &query);

private:
    friend class QMdLiteQueryResultPrivate;
    QScopedPointer<QMdLiteQueryResultPrivate> d;

    Q_DISABLE_COPY(QMdLiteSqlQueryResult);
};

#endif // QMDLITEQUERYRESULTSQLITE_P_H
