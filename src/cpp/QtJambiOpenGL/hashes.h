/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#ifndef QTJAMBIOPENGL_HASHES_H
#define QTJAMBIOPENGL_HASHES_H

#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtOpenGL/QtOpenGL>
#include <QtJambiGui/hashes.h>

inline hash_type qHash(const QOpenGLDebugMessage &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(value.source()));
    hashCode = hashCode * 31 + qHash(int(value.type()));
    hashCode = hashCode * 31 + qHash(int(value.severity()));
    hashCode = hashCode * 31 + qHash(value.id());
    hashCode = hashCode * 31 + qHash(value.message());
    return hashCode;
}

inline hash_type qHash(const QOpenGLFramebufferObjectFormat &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.samples());
    hashCode = hashCode * 31 + qHash(value.mipmap());
    hashCode = hashCode * 31 + qHash(int(value.attachment()));
    hashCode = hashCode * 31 + qHash(value.textureTarget());
    hashCode = hashCode * 31 + qHash(value.internalTextureFormat());
    return hashCode;
}

#endif // QTJAMBI_GUI_QHASHES_H
