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

#ifndef QSIMPLEGALLERYRESULTSET_H
#define QSIMPLEGALLERYRESULTSET_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QObject>
#include <QUrl>
#include <QStringList>
#include <QVector>
#include <QHash>
#include <QVariantList>
#include <qgalleryresultset.h>

#include "qmdlitemusic.h"

QT_ADDON_GALLERY_BEGIN_NAMESPACE

class QSimpleGalleryResultSet : public QGalleryResultSet
{
    Q_OBJECT

public:
    QSimpleGalleryResultSet(QObject *parent = 0);
    ~QSimpleGalleryResultSet();

    QStringList propertyNames() const;
    int propertyKey(const QString &property) const;
    QGalleryProperty::Attributes propertyAttributes(int key) const;
    QVariant::Type propertyType(int key) const;

    int itemCount() const;

    int currentIndex() const;
    bool fetch(int index);
    bool fetchNext();
    bool fetchPrevious();
    bool fetchFirst();
    bool fetchLast();

    QVariant itemId() const;
    QUrl itemUrl() const;
    QList<QGalleryResource> resources() const;

    QVariant metaData(int key) const;
    bool setMetaData(int key, const QVariant &value);

    bool isValid() const;

    static QString filterToString(const QString &tableName, const QGalleryFilter& filter, bool jsonDb);

    //void cancel();

    //bool waitForFinished(int msecs);

    //bool event(QEvent *event);

public Q_SLOTS:
    //void refresh(int serviceId = -1);
    void itemChanged (quint32, int);

Q_SIGNALS:
    void itemEdited(const QString &service);

protected:
    //QSimpleGalleryResultSet(QSimpleGalleryResultSetPrivate &dd, QObject *parent);

    //void timerEvent(QTimerEvent *event);

    /*QMdLiteMusic *m_musicDb;
    QMdLiteImages *m_imagesDb;
    QSqlQuery m_query;*/
    //int m_count;
    //bool m_live;

    //QList<QVariantList> m_result;

    QMdLiteQueryResultIf *m_resultQuery;
    QList<int> m_keys;

    //int m_cursorPosition;
    //bool m_isValid;

private:
    //Q_DECLARE_PRIVATE(QSimpleGalleryResultSet)

    static QString _jsonDbFilterToString(const QString &tableName, const QGalleryFilter& filter, bool inOr = false);
};

QT_ADDON_GALLERY_END_NAMESPACE

#endif
