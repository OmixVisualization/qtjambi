/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

    foreach(AbstractMetaClass *cls, m_classes) {
        if (!shouldGenerate(cls))
            continue;

        QString fileName = fileNameForClass(cls);
        ReportHandler::debugSparse(QString("generating: %1").arg(fileName));

        FileOut fileOut(resolveOutputDirectory() + "/" + subDirectoryForClass(cls) + "/" + fileName);
        write(fileOut.stream, cls);

        if (fileOut.done())
            ++m_num_generated_written;
        ++m_num_generated;
    }
}

void Generator::printClasses() {
    QTextStream s(stdout);

    AbstractMetaClassList classes = m_classes;
    qSort(classes);

    foreach(AbstractMetaClass *cls, classes) {
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

QString Generator::fileNameForClass(const AbstractMetaClass *) const {
    Q_ASSERT(false);
    return QString();
}

void Generator::write(QTextStream &, const AbstractMetaClass *, int) {
    Q_ASSERT(false);
}

bool Generator::hasDefaultConstructor(const AbstractMetaType *type) {
    QString full_name = type->typeEntry()->qualifiedTargetLangName();
    QString class_name = type->typeEntry()->targetLangName();

    foreach(const AbstractMetaClass *java_class, m_classes) {
        if (java_class->typeEntry()->qualifiedTargetLangName() == full_name) {
            AbstractMetaFunctionList functions = java_class->functions();
            foreach(const AbstractMetaFunction *function, functions) {
                if (function->arguments().size() == 0 && function->name() == class_name)
                    return true;
            }
            return false;
        }
    }
    return false;
}
