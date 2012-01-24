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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#ifndef QGALLERYTRACKERRESULTSET_P_H
#define QGALLERYTRACKERRESULTSET_P_H

#include <qgalleryresultset.h>

#include "qgallerydbusinterface_p.h"
#include "qgallerytrackerlistcolumn_p.h"

QT_ADDON_GALLERY_BEGIN_NAMESPACE

class QDBusPendingCallWatcher;
class QGalleryDBusInterfaceFactory;
class QGalleryTrackerImageColumn;
class QGalleryTrackerSchema;

class QGalleryTrackerResultSetPrivate;

struct QGalleryTrackerResultSetArguments
{
    QGalleryTrackerResultSetArguments()
        : idColumn(0)
        , urlColumn(0)
        , typeColumn(0)
        , updateMask(0)
        , identityWidth(0)
        , tableWidth(0)
        , valueOffset(0)
        , compositeOffset(0)
    {
    }

    ~QGalleryTrackerResultSetArguments()
    {
        qDeleteAll(valueColumns);
        qDeleteAll(compositeColumns);
    }

    void clear()
    {
        valueColumns.clear();
        compositeColumns.clear();
    }

    QScopedPointer<QGalleryTrackerCompositeColumn> idColumn;
    QScopedPointer<QGalleryTrackerCompositeColumn> urlColumn;
    QScopedPointer<QGalleryTrackerCompositeColumn> typeColumn;
    int updateMask;
    int identityWidth;
    int tableWidth;
    int valueOffset;
    int compositeOffset;
    QGalleryDBusInterfacePointer queryInterface;
    QString sparql;
    QStringList propertyNames;
    QStringList fieldNames;
    QVector<QGalleryProperty::Attributes> propertyAttributes;
    QVector<QVariant::Type> propertyTypes;
    QVector<QGalleryTrackerValueColumn *> valueColumns;
    QVector<QGalleryTrackerCompositeColumn *> compositeColumns;
    QVector<int> aliasColumns;
    QVector<int> resourceKeys;
    QString service;
};

class QGalleryTrackerResultSet : public QGalleryResultSet
{
    Q_OBJECT
public:
    QGalleryTrackerResultSet(
            QGalleryTrackerResultSetArguments *arguments, bool autoUpdate, QObject *parent = 0);
    ~QGalleryTrackerResultSet();

    QStringList propertyNames() const;
    int propertyKey(const QString &property) const;
    QGalleryProperty::Attributes propertyAttributes(int key) const;
    QVariant::Type propertyType(int key) const;

    int itemCount() const;

    int currentIndex() const;
    bool fetch(int index);

    QVariant itemId() const;
    QUrl itemUrl() const;
    QString itemType() const;
    QList<QGalleryResource> resources() const;

    QVariant metaData(int key) const;
    bool setMetaData(int key, const QVariant &value);

    void cancel();

    bool waitForFinished(int msecs);

    bool event(QEvent *event);

public Q_SLOTS:
    void refresh(int serviceId = -1);

Q_SIGNALS:
    void itemEdited(const QString &service);

protected:
    QGalleryTrackerResultSet(QGalleryTrackerResultSetPrivate &dd, QObject *parent);

    void timerEvent(QTimerEvent *event);

private:
    Q_DECLARE_PRIVATE(QGalleryTrackerResultSet)
    Q_PRIVATE_SLOT(d_func(), void _q_queryFinished(QDBusPendingCallWatcher *))
    Q_PRIVATE_SLOT(d_func(), void _q_parseFinished())
};

QT_ADDON_GALLERY_END_NAMESPACE

#endif
