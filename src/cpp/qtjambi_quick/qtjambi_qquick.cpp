/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#include <QtQuick/QQuickItem>
#include <QtQml/QQmlPropertyValueSource>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_qml.h>

class QmlPropertyValueSource : public QQmlPropertyValueSource{
    void setTarget(const QQmlProperty &);
};

void QmlPropertyValueSource::setTarget(const QQmlProperty &){}

class ErrorDummyQuickItem : public QQuickItem{
public:
    ErrorDummyQuickItem(int _vsCast, int _viCast);
    ~ErrorDummyQuickItem();
private:
    int vsCast;
    int viCast;
};

ErrorDummyQuickItem::ErrorDummyQuickItem(int _vsCast, int _viCast)
    : QQuickItem(), vsCast(_vsCast), viCast(_viCast)
{
    if(vsCast>0){
        void * vsCastPtr = reinterpret_cast<void*>(qintptr(this)+vsCast);
        new(vsCastPtr) QmlPropertyValueSource();
    }
    if(viCast>0){
        void * viCastPtr = reinterpret_cast<void*>(qintptr(this)+viCast);
        Q_UNUSED(viCastPtr)
    }
}

ErrorDummyQuickItem::~ErrorDummyQuickItem(){
    if(vsCast>0){
        QmlPropertyValueSource * vsCastPtr = reinterpret_cast<QmlPropertyValueSource*>(qintptr(this)+vsCast);
        vsCastPtr->~QmlPropertyValueSource();
    }
    if(viCast>0){
        void * viCastPtr = reinterpret_cast<void*>(qintptr(this)+viCast);
        Q_UNUSED(viCastPtr)
    }
}

extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGSimpleMaterialShader_isOpenGL)
(JNIEnv *, jclass)
{
#if QT_CONFIG(opengl)
    return true;
#else
    return false;
#endif
}

void initialize_meta_info_registerParserStatusCaster(){
    registerCreateQmlErrorDummyObjectFunction([](const QMetaObject* metaObject, void* placement, int vsCast, int viCast) -> QObject*{
        if(metaObject->inherits(&QQuickItem::staticMetaObject)){
            return new(placement) ErrorDummyQuickItem(vsCast, viCast);
        }
        return nullptr;
    });
}

