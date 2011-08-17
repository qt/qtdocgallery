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

#ifndef QGALLERYTRACKERMETADATAEDIT_P_H
#define QGALLERYTRACKERMETADATAEDIT_P_H

#include "qgallery_p.h"
#include "qgallerydbusinterface_p.h"

#include <QtCore/qstringlist.h>

QT_ADDON_GALLERY_BEGIN_NAMESPACE

class QGalleryTrackerMetaDataEdit : public QObject
{
    Q_OBJECT
public:
    QGalleryTrackerMetaDataEdit(
            const QGalleryDBusInterfacePointer &metaDataInterface,
            const QString &uri,
            const QString &service,
            QObject *parent = 0);
    ~QGalleryTrackerMetaDataEdit();

    int index() const { return m_index; }
    void setIndex(int index) { m_index = index; }

    QString uri() const { return m_uri; }
    QString service() const { return m_service; }

    QStringList fields() const { return m_values.keys(); }

    QString value(const QString &field) const { return m_values.value(field); }
    void setValue(const QString &field, const QString &value, const QString &oldValue) {
        m_values[field] = value; m_oldValues[field] = oldValue;}

    QMap<QString, QString> values() const { return m_values; }

    void commit();

Q_SIGNALS:
    void finished(QGalleryTrackerMetaDataEdit *edit);

public Q_SLOTS:
    void itemsInserted(int index, int count);
    void itemsRemoved(int index, int count);

private:
    int m_index;
    QGalleryDBusInterfacePointer m_metaDataInterface;
    QString m_uri;
    QString m_service;
    QMap<QString, QString> m_values;
    QMap<QString, QString> m_oldValues;
};

QT_ADDON_GALLERY_END_NAMESPACE

#endif
