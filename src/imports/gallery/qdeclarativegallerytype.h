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

#ifndef QDECLARATIVEGALLERYTYPE_H
#define QDECLARATIVEGALLERYTYPE_H

#include <qgallerytyperequest.h>

#include <QtCore/qpointer.h>
#include <QtCore/qurl.h>
#include <QtDeclarative/qdeclarative.h>
#include <QtDeclarative/qdeclarativepropertymap.h>

#include "qdeclarativedocumentgallery.h"
#include "qdeclarativegalleryfilter.h"

QT_ADDON_GALLERY_BEGIN_NAMESPACE

class QDeclarativeGalleryType : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_ENUMS(Status)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QStringList properties READ propertyNames WRITE setPropertyNames NOTIFY propertyNamesChanged)
    Q_PROPERTY(bool autoUpdate READ autoUpdate WRITE setAutoUpdate NOTIFY autoUpdateChanged)
    Q_PROPERTY(bool available READ available NOTIFY availableChanged)
    Q_PROPERTY(QObject *metaData READ metaData NOTIFY metaDataChanged)
public:
    enum Status
    {
        Null        = QGalleryAbstractRequest::Inactive,
        Active      = QGalleryAbstractRequest::Active,
        Canceling   = QGalleryAbstractRequest::Canceling,
        Canceled    = QGalleryAbstractRequest::Canceled,
        Idle        = QGalleryAbstractRequest::Idle,
        Finished    = QGalleryAbstractRequest::Finished,
        Error       = QGalleryAbstractRequest::Error
    };

    ~QDeclarativeGalleryType();

    Status status() const { return m_status; }

    qreal progress() const;

    QStringList propertyNames() { return m_request.propertyNames(); }
    void setPropertyNames(const QStringList &names);

    bool autoUpdate() const { return m_request.autoUpdate(); }
    void setAutoUpdate(bool enabled);

    bool available() const { return m_request.isValid(); }

    QObject *metaData() const { return m_metaData; }

    void componentComplete();

public Q_SLOTS:
    void reload();
    void cancel();
    void clear();

Q_SIGNALS:
    void statusChanged();
    void progressChanged();
    void availableChanged();
    void metaDataChanged();

    void galleryChanged();
    void propertyNamesChanged();
    void autoUpdateChanged();

protected:
    enum UpdateStatus
    {
        Incomplete,
        NoUpdate,
        PendingUpdate,
        CanceledUpdate
    };

    explicit QDeclarativeGalleryType(QObject *parent = 0);

    void deferredExecute();

    bool event(QEvent *event);

    QGalleryTypeRequest m_request;
    QDeclarativePropertyMap *m_metaData;
    QHash<int, QString> m_propertyKeys;
    Status m_status;
    UpdateStatus m_updateStatus;

private Q_SLOTS:
    void _q_stateChanged();
    void _q_typeChanged();
    void _q_metaDataChanged(const QList<int> &keys);
};

class QDeclarativeDocumentGalleryType : public QDeclarativeGalleryType
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeDocumentGallery::ItemType itemType READ itemType WRITE setItemType NOTIFY itemTypeChanged)
public:
    explicit QDeclarativeDocumentGalleryType(QObject *parent = 0);
    ~QDeclarativeDocumentGalleryType();

    void classBegin();

    QDeclarativeDocumentGallery::ItemType itemType() const;
    void setItemType(QDeclarativeDocumentGallery::ItemType itemType);

Q_SIGNALS:
    void itemTypeChanged();

};

QT_ADDON_GALLERY_END_NAMESPACE

QML_DECLARE_TYPE(QT_ADDON_GELLERY_PREPEND_NAMESPACE(QDeclarativeDocumentGalleryType))

#endif