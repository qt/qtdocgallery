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

#ifndef QGALLERYPROPERTY_H
#define QGALLERYPROPERTY_H

#include <qgalleryfilter.h>

#include <QtCore/qstring.h>

QT_ADDON_GALLERY_BEGIN_NAMESPACE

struct Q_GALLERY_EXPORT QGalleryProperty
{
    enum Attribute
    {
        CanRead  = 0x01,
        CanWrite = 0x02,
        CanSort = 0x04,
        CanFilter = 0x08
    };

    Q_DECLARE_FLAGS(Attributes, Attribute)

    QString name() const { return QString::fromLatin1(m_name, m_length); }

    inline operator QString() const { return QString::fromLatin1(m_name, m_length); }

    QGalleryMetaDataFilter operator <(const QVariant &value) const {
        return QGalleryMetaDataFilter(QLatin1String(m_name), value, QGalleryFilter::LessThan); }
    QGalleryMetaDataFilter operator <=(const QVariant &value) const {
        return QGalleryMetaDataFilter(QLatin1String(m_name), value, QGalleryFilter::LessThanEquals); }

    QGalleryMetaDataFilter operator ==(const QVariant &value) const {
        return QGalleryMetaDataFilter(QLatin1String(m_name), value, QGalleryFilter::Equals); }

    QGalleryMetaDataFilter operator >=(const QVariant &value) const {
        return QGalleryMetaDataFilter(QLatin1String(m_name), value, QGalleryFilter::GreaterThanEquals); }
    QGalleryMetaDataFilter operator >(const QVariant &value) const {
        return QGalleryMetaDataFilter(QLatin1String(m_name), value, QGalleryFilter::GreaterThan); }

    QGalleryMetaDataFilter contains(const QString &string) const {
        return QGalleryMetaDataFilter(QLatin1String(m_name), string, QGalleryFilter::Contains); }
    QGalleryMetaDataFilter startsWith(const QString &string) const {
        return QGalleryMetaDataFilter(QLatin1String(m_name), string, QGalleryFilter::StartsWith); }
    QGalleryMetaDataFilter endsWith(const QString &string) const {
        return QGalleryMetaDataFilter(QLatin1String(m_name), string, QGalleryFilter::EndsWith); }
    QGalleryMetaDataFilter wildcard(const QString &string) const {
        return QGalleryMetaDataFilter(QLatin1String(m_name), string, QGalleryFilter::Wildcard); }
    QGalleryMetaDataFilter regExp(const QString &regExp) const {
        return QGalleryMetaDataFilter(QLatin1String(m_name), regExp, QGalleryFilter::RegExp); }
    QGalleryMetaDataFilter regExp(const QRegExp &regExp) const {
        return QGalleryMetaDataFilter(QLatin1String(m_name), regExp, QGalleryFilter::RegExp); }

    QString ascending() const { return QLatin1Char('+') + QLatin1String(m_name); }
    QString descending() const { return QLatin1Char('-') + QLatin1String(m_name); }

    const char * const m_name;
    const int m_length;
};

QT_ADDON_GALLERY_END_NAMESPACE

Q_DECLARE_OPERATORS_FOR_FLAGS(QT_ADDON_GELLERY_PREPEND_NAMESPACE(QGalleryProperty::Attributes))

#define Q_DEFINE_GALLERY_PROPERTY(scope, name) const QGalleryProperty scope::name = {#name, sizeof(#name) - 1};

#endif
