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

#include "metajava.h"

/**
 * do not allow java keywords as argument name
 */
QString MetaJavaArgument::argumentName() const {
    QString n = AbstractMetaArgument::argumentName();
    // variable name must not be java key word !
    if(n=="interface"
            || n=="extends"
            || n=="strictfp"
            || n=="assert"
            || n=="abstract"
            || n=="boolean"
            || n=="byte"
            || n=="final"
            || n=="finally"
            || n=="implements"
            || n=="instanceof"
            || n=="import"
            || n=="native"
            || n=="package"
            || n=="super"
            || n=="synchronized"
            || n=="throws"
            || n=="throw"
            || n=="null"){
        return "arg__"+n;
    }else{
        return n;
    }
}
