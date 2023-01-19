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

#include "prigenerator.h"
#include "reporthandler.h"
#include "bufferedoutputstream.h"

PriGenerator::PriGenerator(TS::TypeDatabase* database) : m_database(database) {}

QString PriGenerator::subDirectoryForPackage(const QString &typeSystem, const QString &package, OutputDirectoryType type) const {
    TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(m_database->findType(typeSystem));
    switch (type) {
        case CppDirectory:
            {
                if(typeSystemEntry && !typeSystemEntry->qtLibrary().isEmpty()){
                    if(typeSystemEntry->qtLibrary().startsWith("Qt") && !typeSystemEntry->qtLibrary().startsWith("QtJambi")){
                        QString libName = typeSystemEntry->qtLibrary();
                        return "QtJambi" + libName.mid(2);
                    }else
                        return typeSystemEntry->qtLibrary();
                }else if(typeSystemEntry && !typeSystemEntry->targetName().isEmpty()){
                    return typeSystemEntry->targetName();
                }else
                    return QString(package).replace(".", "_");
            }
        case JavaDirectory:
            {
                QString pkgDir = QString(package).replace(".", "/");
                if(!typeSystemEntry)
                    return pkgDir;
                else if(!typeSystemEntry->module().isEmpty())
                    return typeSystemEntry->module() + "/" + pkgDir;
                else if(!typeSystemEntry->qtLibrary().isEmpty()){
                    if(typeSystemEntry->qtLibrary().startsWith("Qt") && !typeSystemEntry->qtLibrary().startsWith("QtJambi")){
                        QString libName = typeSystemEntry->qtLibrary();
                        return "QtJambi" + libName.mid(2) + "/" + pkgDir;
                    }else
                        return typeSystemEntry->qtLibrary() + "/" + pkgDir;
                }else if(typeSystemEntry && !typeSystemEntry->targetName().isEmpty()){
                    return typeSystemEntry->targetName() + "/" + pkgDir;
                }else
                    return pkgDir;
            }
    }
    return QString(); // kill nonsense warnings
}

QString PriGenerator::subDirectoryForClass(const MetaClass *cls, OutputDirectoryType type) const{
    Q_ASSERT(cls);
    return subDirectoryForPackage(cls->targetTypeSystem(), cls->package(), type);
}

QString PriGenerator::subDirectoryForClass(const MetaFunctional *cls, OutputDirectoryType type) const{
    Q_ASSERT(cls);
    return subDirectoryForPackage(cls->targetTypeSystem(), cls->package(), type);
}

void PriGenerator::addHeader(const QString &folder, const QString &header) {
    QWriteLocker locker(&m_lock);
    Q_UNUSED(locker)
    if(!priHash[folder].headers.contains(header))
        priHash[folder].headers << header;
}

void PriGenerator::addSource(const QString &folder, const QString &source) {
    QWriteLocker locker(&m_lock);
    Q_UNUSED(locker)
    if(!priHash[folder].sources.contains(source))
        priHash[folder].sources << source;
}

void PriGenerator::generate() {
    QReadLocker locker(&m_lock);
    Q_UNUSED(locker)
    QMapIterator<QString, Pri> pri(priHash);
    while (pri.hasNext()) {
        pri.next();

        BufferedOutputStream stream(QFileInfo(resolveOutputDirectory() + "/" + pri.key()));
        stream << "HEADERS += \\\n";
        QStringList list = pri.value().headers;
        std::sort(list.begin(), list.end());
        for(const QString &entry : list) {
            stream << "           $$PWD/" << entry << " \\\n";
        }
        stream << "\n";

        stream << "SOURCES += \\\n";
        list = pri.value().sources;
        std::sort(list.begin(), list.end());
        for(const QString &entry : list) {
            stream << "           $$PWD/" << entry << " \\\n";
        }
        stream << "\n\n";

        if (stream.finish())
            ++m_num_generated_written;
        ++m_num_generated;
    }
}
