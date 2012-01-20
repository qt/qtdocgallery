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

#include "qmdlitequeryresultsqlite_p.h"

#include <QSqlQuery>
#include <QVariant>

int QMdLiteQueryResult::at () const
{
    return d->m_query.at ();
}

bool QMdLiteQueryResult::first ()
{
    return d->m_query.first ();
}

bool QMdLiteQueryResult::isActive () const
{
    return d->m_query.isActive();
}

bool QMdLiteQueryResult::last ()
{
    return d->m_query.last();
}

bool QMdLiteQueryResult::next ()
{
    return d->m_query.next();
}

bool QMdLiteQueryResult::previous ()
{
    return d->m_query.previous ();
}

bool QMdLiteQueryResult::seek ( int index, bool relative )
{
    return d->m_query.seek(index, relative);
}

int QMdLiteQueryResult::size () const
{
    return d->m_query.size();
}

QVariant QMdLiteQueryResult::value ( int index ) const
{
    return d->m_query.value(index);
}


void QMdLiteQueryResult::setSqlQuery ( QSqlQuery& query  )
{
    d->m_query = query;
}

void QMdLiteQueryResult::isValid ( )
{
    return d->m_query.isValid();
}

