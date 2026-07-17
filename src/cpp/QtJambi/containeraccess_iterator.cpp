/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED
#include "pch_p.h"
#include <QtCore/QtGlobal>
#include <QtCore/private/qfactoryloader_p.h>

#include "containeraccess_p.h"
#include "containeraccess_associative.h"


AbstractSequentialConstIteratorAccess::~AbstractSequentialConstIteratorAccess(){}
AbstractSequentialConstIteratorAccess::AbstractSequentialConstIteratorAccess(){}
AbstractSequentialConstIteratorAccess::IteratorType AbstractSequentialConstIteratorAccess::iteratorType() const { return IteratorType::const_iterator; }
AbstractSequentialConstIteratorAccess::IteratorStorage AbstractSequentialConstIteratorAccess::iteratorStorage() const { return IteratorStorage::Clone; }
AbstractContainerAccess::ContainerType AbstractSequentialConstIteratorAccess::containerType() const { return ContainerType::SequentialConstIterator; }
void AbstractSequentialConstIteratorAccess::advance(JNIEnv * env, void* iterator, qsizetype n){
    if(n>0) {
        for(;n>0;--n) {
            if(isValid(iterator))
                increment(env, iterator);
            else JavaException::raiseNoSuchElementException(env, "" QTJAMBI_STACKTRACEINFO );
        }
    }else if(n<0) {
        if(!isBidirectionalIterator())
            JavaException::raiseUnsupportedOperationException(env, "retreat" QTJAMBI_STACKTRACEINFO );
        for(;n<0;++n) {
            if(isValid(iterator))
                decrement(env, iterator);
            else JavaException::raiseNoSuchElementException(env, "" QTJAMBI_STACKTRACEINFO );
        }
    }
}
bool AbstractSequentialConstIteratorAccess::advance(void* iterator, qsizetype n){
    if(n>0) {
        for(;n>0;--n) {
            if(isValid(iterator))
                increment(iterator);
            else return false;
        }
    }else if(n<0) {
        if(!isBidirectionalIterator())
            return false;
        for(;n<0;++n) {
            if(isValid(iterator))
                decrement(iterator);
            else return false;
        }
    }
    return true;
}