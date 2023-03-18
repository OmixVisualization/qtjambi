/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of QtJambi.
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

#ifndef CPPGENERATOR_H
#define CPPGENERATOR_H

#include "abstractgenerator.h"
#include "metalang.h"
#include "prigenerator.h"
#include "typedatabase.h"

class CppGenerator : public AbstractGenerator {
public:
    CppGenerator(PriGenerator *_priGenerator);
    QString resolveOutputDirectory() const override;

    QString cppOutputDirectory() const;
    void setCppOutputDirectory(const QString &cppOutDir);

    QString subDirectoryForPackage(const QString &package) const;
    QString subDirectoryForFunctional(const MetaFunctional * cls) const override;
    QString subDirectoryForClass(const MetaClass *cls) const override;
    static QString fixNormalizedSignatureForQt(const QString &signature);
    static void writeTypeInfo(QTextStream &s, const MetaType *type, Option option = NoOption);
    void writeFunctionSignature(QTextStream &s, const MetaFunction *java_function,
                                       const MetaClass *implementor = nullptr,
                                       const QString &name_prefix = QString(),
                                       Option option = NoOption,
                                       const QString &classname_prefix = QString(),
                                       const QStringList &extra_arguments = QStringList(),
                                       int numArguments = -1) const;
    void writeFunctionArguments(QTextStream &s, const MetaArgumentList &arguments,
                                       const MetaFunction *java_function,
                                       Option option = NoOption,
                                       int numArguments = -1) const;

    bool shouldGenerate(const MetaClass *java_class) const override;

    static bool shouldGenerateCpp(const MetaClass *java_class);

    bool shouldGenerate(const MetaFunctional *functional) const override;

    static QString shellClassName(const MetaClass *java_class);

    static QString shellClassName(const MetaFunctional *java_class);
    static QStringList getFunctionPPConditions(const MetaFunction *java_function);

    static QString translateType(const MetaType *java_type, Option option = NoOption);

    static QString marshalledArguments(const MetaFunction *java_function);

    enum JNISignatureFormat {
        Underscores = 0x0001,        //!< Used in the jni exported function names
        SlashesAndStuff = 0x0010,     //!< Used for looking up functions through jni
        ReturnType = 0x0100,
        NoQContainers = 0x0200,
        NoModification = 0x1000
    };

    static QString jni_signature(const QString &_full_name, JNISignatureFormat format);
    static QString jni_signature(const MetaFunction *function, JNISignatureFormat format);
    static QString jni_signature(const MetaFunctional *function, JNISignatureFormat format);
    static QString jni_signature(const MetaType *java_type, JNISignatureFormat format);
    inline TS::TypeDatabase* database() const {return priGenerator->database();}
protected:
    QByteArray jniName(const QString &name) const;
    PriGenerator *priGenerator;
    QString m_cpp_out_dir;
};


#endif // CPPGENERATOR_H
