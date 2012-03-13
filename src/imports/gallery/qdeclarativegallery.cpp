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

#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqml.h>

#include <qdocumentgallery.h>

#include "qdeclarativedocumentgallery.h"
#include "qdeclarativegalleryfilter.h"
#include "qdeclarativegalleryitem.h"
#include "qdeclarativegalleryquerymodel.h"
#include "qdeclarativegallerytype.h"

QT_ADDON_GALLERY_BEGIN_NAMESPACE

class QGalleryDeclarativeModule : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface" FILE "gallery.json")

public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtAddOn.gallery"));

        qRegisterMetaType<QDeclarativeDocumentGallery::ItemType>();

        int major = 5;
        int minor = 0;
        qmlRegisterUncreatableType<QDeclarativeDocumentGallery>(uri, major, minor, "DocumentGallery", QDeclarativeDocumentGallery::tr("DocumentGallery is a namespace class"));
        qmlRegisterType<QDeclarativeGalleryFilterBase>();
        qmlRegisterType<QDeclarativeGalleryEqualsFilter>(uri, major, minor, "GalleryEqualsFilter");
        qmlRegisterType<QDeclarativeGalleryLessThanFilter>(uri, major, minor, "GalleryLessThanFilter");
        qmlRegisterType<QDeclarativeGalleryLessThanEqualsFilter>(uri, major, minor, "GalleryLessThanEqualsFilter");
        qmlRegisterType<QDeclarativeGalleryGreaterThanFilter>(uri, major, minor, "GalleryGreaterThanFilter");
        qmlRegisterType<QDeclarativeGalleryGreaterThanEqualsFilter>(uri, minor, minor, "GalleryGreaterThanEqualsFilter");
        qmlRegisterType<QDeclarativeGalleryContainsFilter>(uri, major, minor, "GalleryContainsFilter");
        qmlRegisterType<QDeclarativeGalleryStartsWithFilter>(uri, major, minor, "GalleryStartsWithFilter");
        qmlRegisterType<QDeclarativeGalleryEndsWithFilter>(uri, major, minor, "GalleryEndsWithFilter");
        qmlRegisterType<QDeclarativeGalleryWildcardFilter>(uri, major, minor, "GalleryWildcardFilter");
        qmlRegisterType<QDeclarativeGalleryFilterUnion>(uri, major, minor, "GalleryFilterUnion");
        qmlRegisterType<QDeclarativeGalleryFilterIntersection>(uri, major, minor, "GalleryFilterIntersection");
        qmlRegisterType<QDeclarativeDocumentGalleryItem>(uri, major, minor, "DocumentGalleryItem");
        qmlRegisterType<QDeclarativeDocumentGalleryModel>(uri, major, minor, "DocumentGalleryModel");
        qmlRegisterType<QDeclarativeDocumentGalleryType>(uri, major, minor, "DocumentGalleryType");
    }
};

#include "qdeclarativegallery.moc"

QT_ADDON_GALLERY_END_NAMESPACE
