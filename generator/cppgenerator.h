/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef CPPGENERATOR_H
#define CPPGENERATOR_H

#include "generator.h"
#include "metajava.h"
#include "prigenerator.h"

class CppGenerator : public Generator {
    Q_OBJECT

public:
    /*virtual*/ QString resolveOutputDirectory() const { return cppOutputDirectory(); }

    QString cppOutputDirectory() const {
        if (!m_cpp_out_dir.isNull())
            return m_cpp_out_dir;
        return outputDirectory() + QLatin1String("/cpp");
    }
    void setCppOutputDirectory(const QString &cppOutDir) { m_cpp_out_dir = cppOutDir; }

    static QString subDirectoryForPackage(const QString &package);
    virtual QString subDirectoryForFunctional(const AbstractMetaFunctional * cls) const
    { return subDirectoryForPackage(cls->targetTypeSystem()); }

    virtual QString subDirectoryForClass(const AbstractMetaClass *cls) const {
        return subDirectoryForPackage(cls->targetTypeSystem());
    }

    static QString fixNormalizedSignatureForQt(const QString &signature);
    static void writeTypeInfo(QTextStream &s, const AbstractMetaType *type, Option option = NoOption);
    static void writeFunctionSignature(QTextStream &s, const AbstractMetaFunction *java_function,
                                       const AbstractMetaClass *implementor = nullptr,
                                       const QString &name_prefix = QString(),
                                       Option option = NoOption,
                                       const QString &classname_prefix = QString(),
                                       const QStringList &extra_arguments = QStringList(),
                                       int numArguments = -1);
    static void writeFunctionArguments(QTextStream &s, const AbstractMetaArgumentList &arguments,
                                       Option option = NoOption,
                                       int numArguments = -1);

    bool shouldGenerate(const AbstractMetaClass *java_class) const {
        return (!java_class->isNamespace() || java_class->functionsInTargetLang().size() > 0)
               && !java_class->isInterface()
               && !java_class->typeEntry()->isVariant()
               && !java_class->typeEntry()->isIterator()
               && (java_class->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp)
               && !java_class->isFake();
    }

    bool shouldGenerate(const AbstractMetaFunctional *functional) const {
        if(functional->enclosingClass()){
            return !functional->enclosingClass()->isFake()
                    && functional->enclosingClass()->typeEntry()
                    && (functional->enclosingClass()->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp);
        }
        return true; /*functional->typeEntry()->codeGeneration()==TypeEntry::GenerateCpp;*/
    }

    static QString shellClassName(const AbstractMetaClass *java_class) {
        if(java_class->typeEntry()->designatedInterface() && java_class->enclosingClass()){
            return java_class->generateShellClass()
                   ? java_class->enclosingClass()->name() + "_shell"
                   : java_class->enclosingClass()->qualifiedCppName();
        }
        return java_class->generateShellClass()
               ? java_class->name() + "_shell"
               : java_class->qualifiedCppName();
    }

    static QString shellClassName(const AbstractMetaFunctional *java_class) {
        QString _shellClassName;
        if(java_class->enclosingClass() && !java_class->enclosingClass()->isFake()){
            _shellClassName = shellClassName(java_class->enclosingClass());
            if(_shellClassName.endsWith("_shell")){
                _shellClassName = _shellClassName.chopped(6);
            }
            _shellClassName += "$";
        }
        return _shellClassName + java_class->name() + "_shell";
        //return java_class->enclosingClass()
        //        ? shellClassName(java_class->enclosingClass()) + "$" + java_class->name()
        //                                    : java_class->name() + "_shell";
    }

    static QStringList getFunctionPPConditions(const AbstractMetaFunction *java_function);

protected:
    PriGenerator *priGenerator;
    QString m_cpp_out_dir;

};


#endif // CPPGENERATOR_H
