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

#ifndef QMDLITEMUSIC_H
#define QMDLITEMUSIC_H

#include <QString>
#include <private/jsondb-connection_p.h>
#include "qmdlite.h"

const QString cJsondbNamespace = QLatin1String("com.nokia.mp.content.");

class QMdLiteMusicPrivate;
Q_ADDON_JSONDB_BEGIN_NAMESPACE
class JsonDbConnection;
Q_ADDON_JSONDB_END_NAMESPACE

Q_USE_JSONDB_NAMESPACE

class QMdLiteMusic : public QMdLite
{
    Q_OBJECT

public:
    explicit QMdLiteMusic(QObject *parent = 0, JsonDbConnection *jsonDbConn = 0);
    ~QMdLiteMusic();

    QMdLiteQueryResultIf *createAlbumQuery(const QStringList &properties, const QString &filterStr = QString(),
                                           const QStringList &sortProperties = QStringList());
    //QMdLiteQueryResult *createAlbumArtistQuery(const QString &filterStr = QString());
    QMdLiteQueryResultIf *createArtistQuery(const QString &filterStr = QString());
    QMdLiteQueryResultIf *createArtistAlbumsQuery(const QString &artist, const QString &filterStr);
    QMdLiteQueryResultIf *createMusicQuery(const QStringList &properties, const QString &filterStr = QString(),
                                           const QStringList &sortProperties = QStringList());

    QMdLite::DBType dbType();

private:
    friend class QMdLiteMusicPrivate;
    QScopedPointer<QMdLiteMusicPrivate> d;

    Q_DISABLE_COPY(QMdLiteMusic)
};

#endif // QMDLITEMUSIC_H
