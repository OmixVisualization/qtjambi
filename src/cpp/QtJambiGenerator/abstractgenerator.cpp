/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "abstractgenerator.h"
#include "reporthandler.h"
#include "bufferedoutputstream.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

AbstractGenerator::AbstractGenerator() {
    m_num_generated = 0;
    m_num_generated_written = 0;
    m_qtVersionMajor = QT_VERSION_MAJOR;
    m_qtVersionMinor = QT_VERSION_MINOR;
    m_qtVersionPatch = QT_VERSION_PATCH;
    m_qtjambiVersionPatch = 0;
}

void AbstractGenerator::generate() {
    if (m_classes.size() == 0) {
        ReportHandler::warning(QString("%1: no java classes, skipping")
                               .arg(metaObject()->className()));
        return;
    }

    for(MetaClass *cls : m_classes) {
        for(MetaFunctional* functional : cls->functionals()){
            if (!shouldGenerate(functional)){
                continue;
            }
            QString fileName = fileNameForFunctional(functional);
            ReportHandler::debugSparse(QString("generating: %1").arg(fileName));

            BufferedOutputStream stream(QFileInfo(resolveOutputDirectory() + "/" + subDirectoryForFunctional(functional) + "/" + fileName));
            write(stream, functional);

            if (stream.finish())
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

            BufferedOutputStream stream(QFileInfo(resolveOutputDirectory() + "/" + subDirectoryForClass(cls) + "/" + fileName));
            write(stream, cls);

            if (stream.finish())
                ++m_num_generated_written;
            ++m_num_generated;
        }
    }
}

void AbstractGenerator::writeInclude(QTextStream &s, const TS::Include &inc, QSet<QString> &included) {
    if (inc.name.isEmpty())
        return;

    QString incString;
    if (inc.type == Include::LocalPath)
        incString = "\"" + inc.name + "\"";
    else if (inc.type == Include::IncludePath)
        incString = "<" + inc.name + ">";
    else
        incString = inc.name;

    if (included.contains(incString))
        return;    // no need to emit this #include it is already in the generared file

    included.insert(incString);

    if (!inc.suppressed){
        if (inc.type == TS::Include::TargetLangImport)
            s << "import " << incString << ";" << Qt::endl;
        else{
            /*if(incString==QLatin1String("qquickgraphicsdevice.h")
                    || incString==QLatin1String("QtQuick/QQuickGraphicsDevice")){
                s << "#ifdef QT_FEATURE_vulkan" << Qt::endl
                  << "#undef QT_FEATURE_vulkan" << Qt::endl
                  << "#define QT_FEATURE_vulkan -1" << Qt::endl
                  << "#endif" << Qt::endl;
            }*/
            for(const QString& feature : inc.requiredFeatures.keys()){
                QString configFile = inc.requiredFeatures[feature];
                if(!configFile.isEmpty()){
                    if(configFile=="qtgui-config.h"){
                        configFile = "QtGui/qtguiglobal.h";
                    }
                    writeInclude(s, TS::Include(TS::Include::IncludePath, configFile), included);
                }
                s << "#if QT_CONFIG(" << feature << ")" << Qt::endl;
            }
            s << "#include " << incString << Qt::endl;
            for(const QString& feature : inc.requiredFeatures.keys()){
                s << "#endif //QT_CONFIG(" << feature << ")" << Qt::endl;
            }
        }
    }
}

void AbstractGenerator::printClasses() {
    QTextStream s(stdout);

    QVector<MetaClass*> classes = m_classes.toVector();
    std::sort(classes.begin(), classes.end(), [](MetaClass *a, MetaClass *b) ->bool {return a->name() < b->name();});

    for(MetaClass *cls : classes) {
        if (!shouldGenerate(cls))
            continue;
        write(s, cls);
        s << Qt::endl << Qt::endl;
    }
}

void AbstractGenerator::verifyDirectoryFor(const QFile &file) {
    QDir dir = QFileInfo(file).dir();
    if (!dir.exists()) {
        if (!dir.mkpath(dir.absolutePath()))
            ReportHandler::warning(QString("unable to create directory '%1'")
                                   .arg(dir.absolutePath()));
    }
}

bool AbstractGenerator::hasDefaultConstructor(const MetaType *type) {
    QString full_name = type->typeEntry()->qualifiedTargetLangName();
    QString class_name = type->typeEntry()->targetLangName();

    for(const MetaClass *java_class : m_classes) {
        if (java_class->typeEntry()->qualifiedTargetLangName() == full_name) {
            MetaFunctionList functions = java_class->functions();
            for(const MetaFunction *function : functions) {
                if (function->arguments().size() == 0 && function->name() == class_name)
                    return true;
            }
            return false;
        }
    }
    return false;
}

bool AbstractGenerator::hasPublicDefaultConstructor(const MetaType *type) {
    QString full_name = type->typeEntry()->qualifiedTargetLangName();
    QString class_name = type->typeEntry()->targetLangName();

    for(const MetaClass *java_class : m_classes) {
        if (java_class->typeEntry()->qualifiedTargetLangName() == full_name) {
            MetaFunctionList functions = java_class->functions();
            bool hasConstructor = false;
            for(const MetaFunction *function : functions) {
                if (function->isConstructor()){
                    hasConstructor = true;
                    if(function->wasPublic()){
                        for(MetaArgument* a : function->arguments()){
                            if(a->originalDefaultValueExpression().isEmpty()){
                                return false;
                            }
                        }
                        return true;
                    }
                }
            }
            return !hasConstructor;
        }
    }
    return false;
}

bool AbstractGenerator::hasPublicAssignmentOperator(const MetaType *type) {
    QString full_name = type->typeEntry()->qualifiedTargetLangName();
    QString class_name = "operator=";

    for(const MetaClass *java_class : m_classes) {
        if (java_class->typeEntry()->qualifiedTargetLangName() == full_name) {
            MetaFunctionList functions = java_class->functions();
            for(const MetaFunction *function : functions) {
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

bool AbstractGenerator::shouldGenerate(const MetaClass *) const {return true;}
bool AbstractGenerator::shouldGenerate(const MetaFunctional *) const {return true;}

QString AbstractGenerator::subDirectoryForClass(const MetaClass *) const {
    Q_ASSERT(false);
    return QString();
}

QString AbstractGenerator::subDirectoryForFunctional(const MetaFunctional *) const {
    Q_ASSERT(false);
    return QString();
}

void AbstractGenerator::generateFake(const MetaClass *) {
}

QString AbstractGenerator::fileNameForClass(const MetaClass *) const {
    Q_ASSERT(false);
    return QString();
}

QString AbstractGenerator::fileNameForFunctional(const MetaFunctional *) const {
    Q_ASSERT(false);
    return QString();
}

void AbstractGenerator::write(QTextStream &, const MetaClass *, int) {
    Q_ASSERT(false);
}

void AbstractGenerator::write(QTextStream &, const MetaFunctional *, int) {
    Q_ASSERT(false);
}
