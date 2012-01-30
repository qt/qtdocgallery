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

#ifndef QGALLERYDBUSINTERFACE_P_H
#define QGALLERYDBUSINTERFACE_P_H

#include "qgallery_p.h"

#include <QtCore/qshareddata.h>
#include <QtDBus/qdbusabstractinterface.h>

struct QGalleryTrackerGraphUpdate
{
    int graph;
    int subject;
    int predicate;
    int object;
};

QT_ADDON_GALLERY_BEGIN_NAMESPACE

class QGalleryDBusInterface : public QDBusAbstractInterface, public QSharedData
{
    Q_OBJECT
public:
    QGalleryDBusInterface(
            const QString &service,
            const QString &path,
            const char *interface,
            const QDBusConnection &connection = QDBusConnection::sessionBus(),
            QObject *parent = 0)
        : QDBusAbstractInterface(service, path, interface, connection, parent) {}

Q_SIGNALS:
    void GraphUpdated(
            const QString &className,
            const QVector<QGalleryTrackerGraphUpdate> &deletes,
            const QVector<QGalleryTrackerGraphUpdate> &inserts);
};

typedef QExplicitlySharedDataPointer<QGalleryDBusInterface> QGalleryDBusInterfacePointer;

class QGalleryTrackerDaemonDBusInterface : public QGalleryDBusInterface
{
    Q_OBJECT
public:
    QGalleryTrackerDaemonDBusInterface(
            const QString &service,
            const QString &path,
            const char *interface,
            const QDBusConnection &connection = QDBusConnection::sessionBus(),
            QObject *parent = 0)
        : QGalleryDBusInterface(service, path, interface, connection, parent) {}

};

class QGalleryDBusInterfaceFactory
{
public:
    virtual ~QGalleryDBusInterfaceFactory() {}

    virtual QGalleryDBusInterfacePointer metaDataInterface() = 0;
    virtual QGalleryDBusInterfacePointer statisticsInterface() = 0;
};

QT_ADDON_GALLERY_END_NAMESPACE

#endif
