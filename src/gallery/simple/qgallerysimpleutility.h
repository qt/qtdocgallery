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

#ifndef QDOCUMENTGALLERYSIMPLEUTILITY_H
#define QDOCUMENTGALLERYSIMPLEUTILITY_H

#include <QString>
#include <QDateTime>
//API
#include "qgalleryqueryrequest.h"

QT_ADDON_GALLERY_BEGIN_NAMESPACE

class QDocumentGallerySimpleUtility
{
public:
    enum TPropertyKeys {
        EUri = 0,
        //EFileSize,
        //ELastModified,
        ETitle,
        //EMime,
        //EAuthor,
        EArtist,
        EAlbumArtist,
        EAlbumTitle,
        EAlbumArt,
        ECount,
        EAlbumCount,
        EGenre,
        ETrackNumber,
        ESongCount,
        EAlbums,
        ECovers,
        EAlbumId,
        //ECopyright,
        //EDescription,
        EComments/*,
        ERating,
        EDuration,
        EPerformer,
        EAudioCodec,
        EAudioBitrate,
        EPlayCount,
        ESampleRate,
        EWidth,
        EHeight,
        EOrientation,
        EDateTaken,
        ECameraManufacturer,
        ECameraModel,
        EExposureProgram,
        EExposureTime,
        EFNumber,
        EFlashEnabled,
        EFocalLength,
        EMeteringMode,
        EWhiteBalance,
        EVideoBitrate,
        ELanguage,
        EFrameRate,
        EResumePosition*/
    };

    QDocumentGallerySimpleUtility();
    ~QDocumentGallerySimpleUtility();

    /**
     * Maps Document Gallery itemtype to MdE object class
     *
     */
    static void GetDataFieldsForItemType(QStringList &propertyList, QString itemType);
    static int GetPropertyKey(const QString &property);
    static QVariant::Type GetPropertyType(int key);
    static int GetPropertyIndex(int key, const QString &itemType);
};

QT_ADDON_GALLERY_END_NAMESPACE

#endif // QDOCUMENTGALLERYSIMPLEUTILITY_H
