/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "generator.h"
#include "reporthandler.h"
#include "fileout.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

Generator::Generator() {
    m_num_generated = 0;
    m_num_generated_written = 0;
}

void Generator::generate() {
    if (m_classes.size() == 0) {
        ReportHandler::warning(QString("%1: no java classes, skipping")
                               .arg(metaObject()->className()));
        return;
    }

    for(AbstractMetaClass *cls : m_classes) {
        for(AbstractMetaFunctional* functional : cls->functionals()){
            if (!shouldGenerate(functional)){
                continue;
            }
            QString fileName = fileNameForFunctional(functional);
            ReportHandler::debugSparse(QString("generating: %1").arg(fileName));

            FileOut fileOut(resolveOutputDirectory() + "/" + subDirectoryForFunctional(functional) + "/" + fileName);
            write(fileOut.stream, functional);

            if (fileOut.done())
                ++m_num_generated_written;
            ++m_num_generated;
        }

        {
            if (!shouldGenerate(cls)){
                if(cls->isFake()){
                    generateFake(cls);
                }
                continue;
            }
            QString fileName = fileNameForClass(cls);
            ReportHandler::debugSparse(QString("generating: %1").arg(fileName));

            FileOut fileOut(resolveOutputDirectory() + "/" + subDirectoryForClass(cls) + "/" + fileName);
            write(fileOut.stream, cls);

            if (fileOut.done())
                ++m_num_generated_written;
            ++m_num_generated;
        }
    }
}

void Generator::writeInclude(QTextStream &s, const Include &inc, QSet<QString> &dedupe) {
    if (inc.name.isEmpty())
        return;

    QString incString;
    if (inc.type == Include::LocalPath)
        incString = "\"" + inc.name + "\"";
    else if (inc.type == Include::IncludePath)
        incString = "<" + inc.name + ">";
    else
        incString = inc.name;

    if (dedupe.contains(incString))
        return;    // no need to emit this #include it is already in the generared file

    dedupe.insert(incString);

    if (!inc.suppressed){
        if (inc.type == Include::TargetLangImport)
            s << "import " << incString << ";" << endl;
        else{
            for(const QString& feature : inc.requiredFeatures.keys()){
                QString configFile = inc.requiredFeatures[feature];
                if(!configFile.isEmpty()){
                    writeInclude(s, Include(Include::IncludePath, configFile), dedupe);
                }
                s << "#if QT_CONFIG(" << feature << ")" << endl;
            }
            s << "#include " << incString << endl;
            for(const QString& feature : inc.requiredFeatures.keys()){
                s << "#endif //QT_CONFIG(" << feature << ")" << endl;
            }
        }
    }
}

void Generator::printClasses() {
    QTextStream s(stdout);

    QVector<AbstractMetaClass*> classes = m_classes.toVector();
    std::sort(classes.begin(), classes.end(), [](AbstractMetaClass *a, AbstractMetaClass *b) ->bool {return a->name() < b->name();});

    for(AbstractMetaClass *cls : classes) {
        if (!shouldGenerate(cls))
            continue;
        write(s, cls);
        s << endl << endl;
    }
}

void Generator::verifyDirectoryFor(const QFile &file) {
    QDir dir = QFileInfo(file).dir();
    if (!dir.exists()) {
        if (!dir.mkpath(dir.absolutePath()))
            ReportHandler::warning(QString("unable to create directory '%1'")
                                   .arg(dir.absolutePath()));
    }
}

QString Generator::subDirectoryForClass(const AbstractMetaClass *) const {
    Q_ASSERT(false);
    return QString();
}

QString Generator::subDirectoryForFunctional(const AbstractMetaFunctional *) const {
    Q_ASSERT(false);
    return QString();
}

void Generator::generateFake(const AbstractMetaClass *) {
}

QString Generator::fileNameForClass(const AbstractMetaClass *) const {
    Q_ASSERT(false);
    return QString();
}

QString Generator::fileNameForFunctional(const AbstractMetaFunctional *) const {
    Q_ASSERT(false);
    return QString();
}

void Generator::write(QTextStream &, const AbstractMetaClass *, int) {
    Q_ASSERT(false);
}

void Generator::write(QTextStream &, const AbstractMetaFunctional *, int) {
    Q_ASSERT(false);
}

bool Generator::hasDefaultConstructor(const AbstractMetaType *type) {
    QString full_name = type->typeEntry()->qualifiedTargetLangName();
    QString class_name = type->typeEntry()->targetLangName();

    for(const AbstractMetaClass *java_class : m_classes) {
        if (java_class->typeEntry()->qualifiedTargetLangName() == full_name) {
            AbstractMetaFunctionList functions = java_class->functions();
            for(const AbstractMetaFunction *function : functions) {
                if (function->arguments().size() == 0 && function->name() == class_name)
                    return true;
            }
            return false;
        }
    }
    return false;
}

bool Generator::hasPublicDefaultConstructor(const AbstractMetaType *type) {
    QString full_name = type->typeEntry()->qualifiedTargetLangName();
    QString class_name = type->typeEntry()->targetLangName();

    for(const AbstractMetaClass *java_class : m_classes) {
        if (java_class->typeEntry()->qualifiedTargetLangName() == full_name) {
            AbstractMetaFunctionList functions = java_class->functions();
            for(const AbstractMetaFunction *function : functions) {
                if (function->name() == class_name && function->wasPublic()){
                    for(AbstractMetaArgument* a : function->arguments()){
                        if(a->originalDefaultValueExpression().isEmpty()){
                            return false;
                        }
                    }
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

bool Generator::hasPublicAssignmentOperator(const AbstractMetaType *type) {
    QString full_name = type->typeEntry()->qualifiedTargetLangName();
    QString class_name = "operator=";

    for(const AbstractMetaClass *java_class : m_classes) {
        if (java_class->typeEntry()->qualifiedTargetLangName() == full_name) {
            AbstractMetaFunctionList functions = java_class->functions();
            for(const AbstractMetaFunction *function : functions) {
                if (function->name() == class_name
                        && function->wasPublic()
                        && function->arguments().size()==1
                        && function->arguments()[0]->type()->typeEntry()==type->typeEntry()){
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}
