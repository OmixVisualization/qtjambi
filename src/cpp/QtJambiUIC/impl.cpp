/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtDesigner/private/ui4_p.h>
#include <QtJambi/CoreAPI>
#include <QtJambi/RegistryAPI>
#include <QtJambi/qtjambi_cast.h>

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_uic_java_WriteClass_getExternalTypeName(JNIEnv *env, jclass, jstring externalName){
    jstring result = nullptr;
    QTJAMBI_TRY{
        QString internalName = CoreAPI::externalTypeTame(env, qtjambi_cast<QString>(env, externalName));
        result = qtjambi_cast<jstring>(env, internalName);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

void initialize_meta_info_UIC(){
    using namespace RegistryAPI;
    registerContainerAccessFactory(typeid(QHash<DomWidget*,QString>), NewContainerAccessFunction(&QtJambiPrivate::QHashAccess<DomWidget*,QString>::newInstance));
    registerContainerAccessFactory(typeid(QHash<DomSpacer*,QString>), NewContainerAccessFunction(&QtJambiPrivate::QHashAccess<DomSpacer*,QString>::newInstance));
    registerContainerAccessFactory(typeid(QHash<DomLayout*,QString>), NewContainerAccessFunction(&QtJambiPrivate::QHashAccess<DomLayout*,QString>::newInstance));
    registerContainerAccessFactory(typeid(QHash<DomActionGroup*,QString>), NewContainerAccessFunction(&QtJambiPrivate::QHashAccess<DomActionGroup*,QString>::newInstance));
    registerContainerAccessFactory(typeid(QHash<DomButtonGroup*,QString>), NewContainerAccessFunction(&QtJambiPrivate::QHashAccess<DomButtonGroup*,QString>::newInstance));
    registerContainerAccessFactory(typeid(QHash<DomAction*,QString>), NewContainerAccessFunction(&QtJambiPrivate::QHashAccess<DomAction*,QString>::newInstance));
    registerContainerAccessFactory(typeid(QHash<QString,bool>), NewContainerAccessFunction(&QtJambiPrivate::QHashAccess<QString,bool>::newInstance));
    registerContainerAccessFactory(typeid(QHash<QString,DomProperty*>), NewContainerAccessFunction(&QtJambiPrivate::QHashAccess<QString,DomProperty*>::newInstance));
}
