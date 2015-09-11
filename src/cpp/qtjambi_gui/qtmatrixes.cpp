/****************************************************************************
**
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#include "qtmatrixes.h"
#include <QtGui/QtGui>

#define QTJAMBIMATRIX_IMPL(M,N)\
QTJAMBIMATRIX(M,N)::QTJAMBIMATRIX(M,N)():QMATRIX(M,N)(){}\
QTJAMBIMATRIX(M,N)::QTJAMBIMATRIX(M,N)(const QMATRIX(M,N)& other):QMATRIX(M,N)(other){}\
QTJAMBIMATRIX(M,N)::QTJAMBIMATRIX(M,N)(const float *values):QMATRIX(M,N)(values){}\
\
QMATRIX(M,N)& QTJAMBIMATRIX(M,N)::divide(float divisor){\
    return QMATRIX(M,N)::operator/=(divisor);\
}\


QTJAMBIMATRIX_IMPL(2,2)
QTJAMBIMATRIX_IMPL(2,3)
QTJAMBIMATRIX_IMPL(2,4)
QTJAMBIMATRIX_IMPL(3,2)
QTJAMBIMATRIX_IMPL(3,3)
QTJAMBIMATRIX_IMPL(3,4)
QTJAMBIMATRIX_IMPL(4,2)
QTJAMBIMATRIX_IMPL(4,3)
