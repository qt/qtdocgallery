/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtDocGallery module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGALLERYGLOBAL_H
#define QGALLERYGLOBAL_H

#include <QtCore/qglobal.h>

#ifndef QT_STATIC
#  if defined(QT_BUILD_DOCGALLERY_LIB)
#    define Q_GALLERY_EXPORT Q_DECL_EXPORT
#  else
#    define Q_GALLERY_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_GALLERY_EXPORT
#endif

#if defined(QT_NAMESPACE)
#define QT_DOCGALLERY_BEGIN_NAMESPACE namespace QT_NAMESPACE { namespace QDocGallery {
#define QT_DOCGALLERY_END_NAMESPACE } }
#define QT_USE_DOCGALLERY_NAMESPACE using namespace QT_NAMESPACE::QDocGallery;
#define QT_DOCGALLERY_PREPEND_NAMESPACE(name) ::QT_NAMESPACE::QDocGallery::name
#else
#define QT_DOCGALLERY_BEGIN_NAMESPACE namespace QDocGallery {
#define QT_DOCGALLERY_END_NAMESPACE }
#define QT_USE_DOCGALLERY_NAMESPACE using namespace QDocGallery;
#define QT_DOCGALLERY_PREPEND_NAMESPACE(name) ::QDocGallery::name
#endif

#endif // QGALLERYGLOBAL_H