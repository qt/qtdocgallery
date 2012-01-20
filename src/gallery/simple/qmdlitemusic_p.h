/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QMDLITEMUSIC_P_H
#define QMDLITEMUSIC_P_H

#include <private/jsondb-connection_p.h>
#include <jsondb-client.h>
#include "qmdlite_p.h"

#include <QFileInfoList>

class QMdLiteMusic;

QT_ADDON_JSONDB_BEGIN_NAMESPACE
class JsonDbConnection;
class JsonDbClient;
QT_ADDON_JSONDB_END_NAMESPACE

QT_ADDON_JSONDB_USE_NAMESPACE

class QMdLiteMusicPrivate : public QMdLitePrivate
{
    Q_OBJECT

public:
#ifdef _SQLITE_QMDBACKEND
    QMdLiteMusicPrivate(QMdLiteMusic *q);
#else
    QMdLiteMusicPrivate(QMdLiteMusic *q, JsonDbConnection *jsonDbConn);
#endif
    ~QMdLiteMusicPrivate();
    static QString albumArtImageBaseName(const QString &albumArtist, const QString &title);
    static QString albumArtImagePath(const QString &); // 512x512
    static QString albumArtImageCompressedPath(const QString &); // 512x512 .pkm
    static QString albumArtThumbnailImagePath(const QString &); // 128x128
    static QString albumArtCompressedTexturePath(const QString &); // 128x128 .pkm file
    static QString musicAlbumArtDir();

    int m_fileCount;
    int m_albumCount;
    int m_updateProgress;

    static QString m_albumArtDir;

    QMdLiteMusic *m_q;
};

#endif // QMDLITEMUSIC_P_H
