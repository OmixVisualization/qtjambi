/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <qtjambi_core.h>
#include <QtCore/QString>
#include <QtCore/QStringList>

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage:\n"
               "    jambi <vm options> [classname] options\n");
        return 0;
    }
    int start = 2;
    QStringList vm_parameters;
    for(int i=1; i<argc; i++){
        QString arg = QLatin1String(argv[i]);
        if(arg.startsWith("-")){
            vm_parameters << arg;
            start++;
        }else{
            break;
        }
    }
    qtjambi_initialize_vm(vm_parameters);

    JNIEnv *env = qtjambi_current_environment();

    QString class_name = QLatin1String(argv[start-1]);

    jclass java_class = env->FindClass(class_name.replace('.', '/').toLatin1());
    if (!java_class) {
        printf("failed to find class: '%s'\n", qPrintable(class_name));
        return 0;
    }

    jmethodID main_id = env->GetStaticMethodID(java_class, "main", "([Ljava/lang/String;)V");
    if (!main_id) {
        printf("failed to find main(String[])\n");
        return 0;
    }

    jclass string_class = env->FindClass("java/lang/String");
    Q_ASSERT(string_class);


    jobjectArray args = env->NewObjectArray(argc - start, string_class, 0);
    for (int i = start; i < argc; ++i) {
        env->SetObjectArrayElement(args, i - start, qtjambi_from_qstring(env, QLatin1String(argv[i])));
    }

    env->CallStaticVoidMethod(java_class, main_id, args);

    qtjambi_exception_check(env);

    qtjambi_destroy_vm();

    return 0;
}
