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

#include "qgallerytrackerchangenotifier_p.h"

#include "qgallerytrackerschema_p.h"
#include <QtCore/qdebug.h>

QT_ADDON_GALLERY_BEGIN_NAMESPACE

QGalleryTrackerChangeNotifier::QGalleryTrackerChangeNotifier(
        const QString &service,
        const QGalleryDBusInterfacePointer &daemonInterface, QObject *parent)
    : QObject(parent)
    , m_daemonInterface(daemonInterface)
    , m_service(service)
{
    connect(
        m_daemonInterface.data(), SIGNAL(GraphUpdated(QString,QVector<QGalleryTrackerGraphUpdate>,QVector<QGalleryTrackerGraphUpdate>)),
        this, SLOT(graphUpdated(QString,QVector<QGalleryTrackerGraphUpdate>,QVector<QGalleryTrackerGraphUpdate>)));
}

void QGalleryTrackerChangeNotifier::itemsEdited(const QString &service)
{
    emit itemsChanged(QGalleryTrackerSchema::serviceUpdateId(service));
}

void QGalleryTrackerChangeNotifier::graphUpdated(
        const QString &className,
        const QVector<QGalleryTrackerGraphUpdate> &,
        const QVector<QGalleryTrackerGraphUpdate> &)
{
    /*
     * className ends with e.g. ...nfo#Audio and m_service contains "nfo:Audio".
     */
    QString identifier(m_service);
    identifier.replace(QLatin1Char(':'), QLatin1Char('#'));
    if (className.endsWith(identifier)
            || (m_service == QLatin1String("nmm:Artist") && className.endsWith(QLatin1String("nfo#Audio")))) {
        emit itemsChanged(QGalleryTrackerSchema::serviceUpdateId(m_service));
    }
}

QT_ADDON_GALLERY_END_NAMESPACE
