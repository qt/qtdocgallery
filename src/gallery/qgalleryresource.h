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

#ifndef QGALLERYRESOURCE_H
#define QGALLERYRESOURCE_H

#include "qgalleryglobal.h"

#include <QtCore/qurl.h>
#include <QtCore/qvariant.h>

QT_DOCGALLERY_BEGIN_NAMESPACE

class Q_GALLERY_EXPORT QGalleryResource
{
public:
    QGalleryResource();
    QGalleryResource(const QUrl &url);
    QGalleryResource(const QUrl &url, QMap<int, QVariant> attributes);
    QGalleryResource(const QGalleryResource &resource);
    ~QGalleryResource();

    QGalleryResource &operator =(const QGalleryResource &resource);

    bool operator ==(const QGalleryResource &resource) const;
    bool operator !=(const QGalleryResource &resource) const;

    QUrl url() const;
    QMap<int, QVariant> attributes() const;

    QVariant attribute(int key) const;

private:
    QUrl m_url;
    QMap<int, QVariant> m_attributes;
};

QT_DOCGALLERY_END_NAMESPACE

#endif
