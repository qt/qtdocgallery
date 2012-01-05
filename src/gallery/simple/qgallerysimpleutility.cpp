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

#include <QUrl>
//Backend
#include "qgallerysimpleutility.h"
#include <qdocumentgallery.h>

QT_ADDON_GALLERY_BEGIN_NAMESPACE

QDocumentGallerySimpleUtility::QDocumentGallerySimpleUtility()
{

}
QDocumentGallerySimpleUtility::~QDocumentGallerySimpleUtility()
{

}

void QDocumentGallerySimpleUtility::GetDataFieldsForItemType( QStringList &propertyList, QString itemType )
{
    if (itemType == QDocumentGallery::Artist.name()) {
        propertyList.append(QDocumentGallery::artist.name() );
        propertyList.append(QLatin1String("songCount")); // The list of songs (_uuid) by this artist
        propertyList.append(QLatin1String("albums")); // The list of albums (_uuid) by this artist
        propertyList.append(QLatin1String("covers")); // List of urls to album cover thumbnails
    } else if (itemType == QDocumentGallery::AlbumArtist.name()) {
        propertyList.append(QDocumentGallery::artist.name() ); // Artist
        propertyList.append(QLatin1String("songCount")); // The list of songs (_uuid) by this artist in this album
        propertyList.append(QDocumentGallery::albumArtist.name() );
        propertyList.append(QDocumentGallery::albumTitle);
        propertyList.append(QLatin1String("albumArt"));
        propertyList.append(QLatin1String("albumId"));
    } else if (itemType == QDocumentGallery::Album.name()) {
        propertyList.append(QLatin1String("albumArt"));
        /*propertyList.append( QDocumentGallery::fileSize.name() );
        propertyList.append( QDocumentGallery::lastModified.name() );*/
        propertyList.append(QDocumentGallery::title.name() );
        propertyList.append(QDocumentGallery::artist.name() );
        propertyList.append(QDocumentGallery::albumArtist.name() );
        //propertyList.append( QDocumentGallery::mimeType.name() );
    } else if (itemType == QDocumentGallery::Audio.name()) {
        propertyList.append(QDocumentGallery::url.name() );
        /*propertyList.append( QDocumentGallery::fileSize.name() );
        propertyList.append( QDocumentGallery::lastModified.name() );*/
        propertyList.append(QDocumentGallery::title.name() );
        //propertyList.append( QDocumentGallery::mimeType.name() );
        propertyList.append(QDocumentGallery::artist.name() );
        propertyList.append(QDocumentGallery::albumTitle);
        propertyList.append(QDocumentGallery::albumArtist);
        propertyList.append(QLatin1String("albumArt"));
        //propertyList.append( QDocumentGallery::copyright.name() );
        //propertyList.append( QDocumentGallery::description.name() );
        //propertyList.append( QDocumentGallery::comments.name() );
        //propertyList.append( QDocumentGallery::rating.name() ); <-- TODO

        //propertyList.append( QDocumentGallery::duration.name() ); <-- TODO
        //propertyList.append( QDocumentGallery::performer.name() );

        //propertyList.append( QDocumentGallery::audioCodec.name() );
        //propertyList.append( QDocumentGallery::audioBitRate.name() );
        //propertyList.append( QDocumentGallery::playCount.name() );

        //propertyList.append( QDocumentGallery::sampleRate.name() );

        propertyList.append(QDocumentGallery::trackNumber);
        propertyList.append(QDocumentGallery::genre);
    /*} else if (itemType == QDocumentGallery::File.name()) {
        // From MDS base object
        propertyList.append( QDocumentGallery::url.name() );
        propertyList.append( QDocumentGallery::fileSize.name() );
        propertyList.append( QDocumentGallery::lastModified.name() );
        propertyList.append( QDocumentGallery::title.name() );
        propertyList.append( QDocumentGallery::mimeType.name() );
        // From MDS media object
        propertyList.append( QDocumentGallery::author.name() );
        propertyList.append( QDocumentGallery::copyright.name() );
        propertyList.append( QDocumentGallery::description.name() );
        propertyList.append( QDocumentGallery::comments.name() );
        propertyList.append( QDocumentGallery::rating.name() );*/
    } else if (itemType == QDocumentGallery::Image.name()) { // TODO: add the properties needed by Image Viewer
        propertyList.append(QDocumentGallery::url.name() );
        /*propertyList.append( QDocumentGallery::fileSize.name() );
        propertyList.append( QDocumentGallery::lastModified.name() );*/
        propertyList.append(QDocumentGallery::title.name() );
        /*propertyList.append( QDocumentGallery::mimeType.name() );
        propertyList.append( QDocumentGallery::author.name() );
        propertyList.append( QDocumentGallery::copyright.name() );
        propertyList.append( QDocumentGallery::description.name() ); <-- TODO*/
        propertyList.append(QDocumentGallery::comments.name() );
        /*propertyList.append( QDocumentGallery::rating.name() );

        propertyList.append( QDocumentGallery::duration.name() );
        propertyList.append( QDocumentGallery::performer.name() );

        propertyList.append( QDocumentGallery::playCount.name() );

        propertyList.append( QDocumentGallery::width.name() ); <-- TODO
        propertyList.append( QDocumentGallery::height.name() ); <-- TODO
        // From MDS image object
        propertyList.append( QDocumentGallery::orientation.name() ); <-- TODO
        propertyList.append( QDocumentGallery::dateTaken.name() ); <-- TODO
        propertyList.append( QDocumentGallery::cameraManufacturer.name() );
        propertyList.append( QDocumentGallery::cameraModel.name() );
        propertyList.append( QDocumentGallery::exposureProgram.name() );
        propertyList.append( QDocumentGallery::exposureTime.name() );
        propertyList.append( QDocumentGallery::fNumber.name() );
        propertyList.append( QDocumentGallery::flashEnabled.name() );
        propertyList.append( QDocumentGallery::focalLength.name() );
        propertyList.append( QDocumentGallery::meteringMode.name() );
        propertyList.append( QDocumentGallery::whiteBalance.name() );*/
    }/* else if (itemType == QDocumentGallery::Video.name()) { <-- TODO
        propertyList.append( QDocumentGallery::url.name() );
        propertyList.append( QDocumentGallery::fileSize.name() );
        propertyList.append( QDocumentGallery::lastModified.name() );
        propertyList.append( QDocumentGallery::title.name() );
        propertyList.append( QDocumentGallery::mimeType.name() );
        // From MDS media object
        propertyList.append( QDocumentGallery::author.name() );
        propertyList.append( QDocumentGallery::copyright.name() );
        propertyList.append( QDocumentGallery::description.name() );
        propertyList.append( QDocumentGallery::comments.name() );
        propertyList.append( QDocumentGallery::rating.name() );

        propertyList.append( QDocumentGallery::duration.name() );
        propertyList.append( QDocumentGallery::performer.name() );

        propertyList.append( QDocumentGallery::videoBitRate.name() );
        propertyList.append( QDocumentGallery::playCount.name() );

        propertyList.append( QDocumentGallery::width.name() );
        propertyList.append( QDocumentGallery::height.name() );
        // From MDS video object
        propertyList.append( QDocumentGallery::language.name() );
        propertyList.append( QDocumentGallery::frameRate.name() );
        propertyList.append( QDocumentGallery::resumePosition.name() );
    }*/
}


int QDocumentGallerySimpleUtility::GetPropertyKey( const QString &property )
{
    if (property == QDocumentGallery::url.name()) {
        return EUri;
    /*} else if (property == QDocumentGallery::fileSize.name()) {
        return EFileSize;
    } else if (property == QDocumentGallery::lastModified.name()) {
        return ELastModified;*/
    } else if (property == QDocumentGallery::title.name()) {
        return ETitle;
    /*} else if (property == QDocumentGallery::mimeType.name()) {
        return EMime;
    } else if (property == QDocumentGallery::duration.name()) {
        return EDuration;
    } else if (property == QDocumentGallery::performer.name()) {
        return EPerformer;
    } else if (property == QDocumentGallery::width.name()) {
        return EWidth;
    } else if (property == QDocumentGallery::height.name()) {
        return EHeight;
    } else if (property == QDocumentGallery::orientation.name()) {
        return EOrientation;
    } else if (property == QDocumentGallery::audioBitRate.name()) {
        return EAudioBitrate;
    } else if (property == QDocumentGallery::videoBitRate.name()) {
        return EVideoBitrate;
    } else if (property == QDocumentGallery::frameRate.name()) {
        return EFrameRate;
    } else if (property == QDocumentGallery::copyright.name()) {
        return ECopyright;
    } else if (property == QDocumentGallery::description.name()) {
        return EDescription;*/
    } else if (property == QDocumentGallery::comments.name()) {
        return EComments;
    /*} else if (property == QDocumentGallery::author.name()) {
        return EAuthor;
    } else if (property == QDocumentGallery::rating.name()) {
        return ERating;
    } else if (property == QDocumentGallery::audioCodec.name()) {
        return EAudioCodec;
    } else if (property == QDocumentGallery::playCount.name()) {
        return EPlayCount;
    } else if (property == QDocumentGallery::sampleRate.name()) {
        return ESampleRate;
    } else if (property == QDocumentGallery::dateTaken.name()) {
        return EDateTaken;
    } else if (property == QDocumentGallery::cameraManufacturer.name()) {
        return ECameraManufacturer;
    } else if (property == QDocumentGallery::cameraModel.name()) {
        return ECameraModel;
    } else if (property == QDocumentGallery::exposureProgram.name()) {
        return EExposureProgram;
    } else if (property == QDocumentGallery::exposureTime.name()) {
        return EExposureTime;
    } else if (property == QDocumentGallery::fNumber.name()) {
        return EFNumber;
    } else if (property == QDocumentGallery::flashEnabled.name()) {
        return EFlashEnabled;
    } else if (property == QDocumentGallery::focalLength.name()) {
        return EFocalLength;
    } else if (property == QDocumentGallery::meteringMode.name()) {
        return EMeteringMode;
    } else if (property == QDocumentGallery::whiteBalance.name()) {
        return EWhiteBalance;
    } else if (property == QDocumentGallery::language.name()) {
        return ELanguage;
    } else if (property == QDocumentGallery::resumePosition.name()) {
        return EResumePosition;*/
    } else if (property == QDocumentGallery::artist) {
        return EArtist;
    } else if (property == QDocumentGallery::albumArtist) {
        return EAlbumArtist;
    } else if (property == QDocumentGallery::albumTitle) {
        return EAlbumTitle;
    } else if (property == QDocumentGallery::genre) {
        return EGenre;
    } else if (property == QDocumentGallery::trackNumber) {
        return ETrackNumber;
    } else if (property == QLatin1String("albumArt")) {
        return EAlbumArt;
    //} else if (property == "count") {
    //    return ECount;
    //} else if (property == "albumCount") {
    //    return EAlbumCount;
    } else if (property == QLatin1String("songCount")) {
        return ESongCount;
    } else if (property == QLatin1String("albums")) {
        return EAlbums;
    } else if (property == QLatin1String("covers")) {
        return ECovers;
    } else if (property == QLatin1String("albumId")) {
        return EAlbumId;
    }

    return -1;
}

QVariant::Type QDocumentGallerySimpleUtility::GetPropertyType( int key )
{
    //TODO: remove compile warnings
    switch (key) {
    case EUri:
        return QVariant::Url;
    /*case EFileSize:
    case EAudioCodec:
    case EPlayCount:
        return QVariant::ULongLong;
    case ELastModified:
    case EDateTaken:
        return QVariant::DateTime;*/
    case ETitle:
    /*case EMime:
    case EAuthor:
} else if (property == "count") {
    return ECount;
}
    case ECopyright:
    case EDescription:*/
    case EComments:
    /*case EPerformer:
    case ECameraManufacturer:
    case ECameraModel:
    case ELanguage:*/
    case EArtist:
    case EAlbumArtist:
    case EAlbumTitle:
    case EAlbumArt:
    case EGenre:
    case EAlbumId:
        return QVariant::String;
    /*case ERating:
    case EAudioBitrate:
    case EWidth:
    case EHeight:
    case EOrientation:
    case EExposureProgram:
    case EFlashEnabled:
    case EFocalLength:
    case EMeteringMode:
    case EWhiteBalance:
    case EVideoBitrate:*/
    case ETrackNumber:
    //case ECount:
    //case EAlbumCount:
        return QVariant::UInt;
    /*case EDuration:
    case ESampleRate:
    case EExposureTime:
    case EFNumber:
    case EFrameRate:
    case EResumePosition:
        return QVariant::Double;*/
    default:
        return QVariant::Invalid;
    }
}


// Get the index of given property in the sql query
int QDocumentGallerySimpleUtility::GetPropertyIndex( int key, const QString& itemType )
{
    if (itemType ==QDocumentGallery::AlbumArtist.name())  {
        switch (key) {
        case EArtist:
            return 1;
        case ESongCount:
            return 2;
        case EAlbumArtist:
            return 3;
        case EAlbumTitle:
            return 4;
        case EAlbumArt:
            return 5;
        case EAlbumId:
            return 6;
        }
        return -1;
    }
    else {
        switch (key) {
        case EUri:
            return 1;
            /*case EFileSize:
    case EAudioCodec:
    case EPlayCount:
    case ELastModified:
    case EDateTaken:*/
        case ETitle:
            return 2;
            //case ECount:
            //    return 2;
        case ESongCount:
            return 2;
            /*case EMime:
    case EAuthor:
    case ECopyright:
    case EDescription:*/
            // Only in images
        case EComments:
            // In Album, Audio and in AlbumArtist
        case EArtist:
            if (itemType == QDocumentGallery::Artist.name())
                return 1;
        case EAlbums:
            return 3;
            /*case EPerformer:
    case ECameraManufacturer:
    case ECameraModel:
    case ELanguage:*/
        case EAlbumTitle:
        case ECovers:
            return 4;
        case EAlbumArtist:
            if (itemType == QDocumentGallery::Album.name())
                return 4;
        case EAlbumId:
            return 5;
        case EAlbumArt:
            if (itemType == QDocumentGallery::Album.name())
                return 1;
            return 6;
        case ETrackNumber:
            return 7;
        case EGenre:
            return 8;
            /*case ERating:
    case EAudioBitrate:
    case EWidth:
    case EHeight:
    case EOrientation:
    case EExposureProgram:
    case EFlashEnabled:
    case EFocalLength:
    case EMeteringMode:
    case EWhiteBalance:
    case EVideoBitrate:*/
            /*case EDuration:
    case ESampleRate:
    case EExposureTime:
    case EFNumber:
    case EFrameRate:
    case EResumePosition:
        return QVariant::Double;*/
        default:
            return -1;
        }
    }
}

QT_ADDON_GALLERY_END_NAMESPACE
