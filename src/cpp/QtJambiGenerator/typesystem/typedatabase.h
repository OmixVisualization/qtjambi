/****************************************************************************
**
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

#ifndef TYPEDATABASE_H_
#define TYPEDATABASE_H_

#include <QList>
#include <QSet>
#include <qstringlist.h>
#include <QVersionNumber>

#include "typeentry.h"

namespace TS{

class TypeDatabase {
public:
    typedef std::function<bool(const QString& name)> DefinedPtr;

    TypeDatabase();

    static TypeDatabase* instance();

    QList<Include> extraIncludes(const QString &className);

    TypeSystemTypeEntry *findTypeSystem(const QString &name) const;
    ComplexTypeEntry *findComplexType(const QString &name) const;
    FunctionalTypeEntry *findFunctionalType(const QString &name) const;
    FunctionalTypeEntry *findFunctionalTypeByUsing(const QString &containingClassName, const QString &_using) const;
    PrimitiveTypeEntry *findPrimitiveType(const QString &name) const;
    ObjectTypeEntry *findObjectType(const QString &name) const;
    NamespaceTypeEntry *findNamespaceType(const QString &name) const;

    ContainerTypeEntry *findContainerType(const QString &name) const;
    IteratorTypeEntry *findIteratorType(const QString &name) const;
    IteratorTypeEntry *findIteratorType(const ComplexTypeEntry * container) const;
    PointerContainerTypeEntry *findPointerContainerType(const QString &name) const;
    InitializerListTypeEntry *findInitializerListType(const QString &name) const;

    bool isPixmapType(const TypeEntry *e) const{
        return e==m_pixmapType || e==m_bitmapType;
    }

    TypeEntry *findType(const QString &name) const;

    QList<TypeEntry *> findTypes(const QString &name) const {
        return m_entries.value(name);
    }

    const TypeEntryHash& allEntries() {
        return m_entries;
    }

    SingleTypeEntryHash entries();

    PrimitiveTypeEntry *findTargetLangPrimitiveType(const QString &java_name) const;

    void addRejection(const QString &class_name, const QString &function_name,
                      const QString &field_name, const QString &enum_name);
    bool isClassRejected(const QString &class_name);
    bool isFunctionRejected(const QString &class_name, const QString &function_name);
    bool isFieldRejected(const QString &class_name, const QString &field_name);
    bool isEnumRejected(const QString &class_name, const QString &enum_name);

    void addType(TypeEntry *e);

    bool isUniqueClassName(QString name) {
        return m_class_name_counter[name]<=1;
    }

    SingleTypeEntryHash flagsEntries() const {
        return m_flags_entries;
    }

    FlagsTypeEntry *findFlagsType(const QString &name) const;

    void addFlagsType(FlagsTypeEntry *fte) {
        m_flags_entries[fte->flagsTemplate()] = fte;
    }

    TemplateEntry *findTemplate(const QString &name) {
        return m_templates[name];
    }

    TemplateTypeEntry *findTemplateType(const QString &name) {
        return m_templateTypes[name];
    }

    void addTemplate(TemplateEntry *t) {
        m_templates[t->name()] = t;
    }
    void addTemplateType(TemplateTypeEntry *t) {
        m_templateTypes[t->name()] = t;
    }

    void setIncludeEclipseWarnings(bool on) {
        m_includeEclipseWarnings = on;
    }
    bool includeEclipseWarnings() const {
        return m_includeEclipseWarnings;
    }

    void setSuppressWarnings(bool on) {
        m_suppressWarnings = on;
    }
    void addSuppressedWarning(const QString &s) {
        m_suppressedWarnings.append(s);
    }

    bool isSuppressedWarning(const QString &s);

    void setRebuildClasses(const QStringList &cls) {
        m_rebuildClasses = cls;
    }

    QString filename() const {
        return "typesystem.txt";
    }

    void initialize(const QString &filename, const QStringList &importInputDirectoryList, const QStringList &typeystemDirectoryList, const QVersionNumber& qtVersion, bool generate = true);

    bool parseFile(const QString &filename, const QStringList &importInputDirectoryList, const QStringList &typeystemDirectoryList, bool generate = true, bool optional = false);

    bool defined(QString name);

    void setDefined(DefinedPtr&& function);

    const QMap<QString,TypeSystemTypeEntry*>& typeSystemsByQtLibrary() const { return m_typeSystemsByQtLibrary; }

    QList<TypeSystemTypeEntry*> typeSystems() const;

    inline const QVersionNumber& qtVersion() const {return m_qtVersion;}

    ComplexTypeEntry * qvariantType() const{
        return m_qvariantType;
    }

    ComplexTypeEntry * qstringType() const{
        return m_qstringType;
    }

    ComplexTypeEntry * qcharType() const{
        return m_qcharType;
    }

    bool useNativeIds() const;
    void setUseNativeIds(bool newUseNativeIds);

    const QStringList &rebuildClasses() const;

private:
    QVersionNumber m_qtVersion;
    uint m_suppressWarnings : 1;
    uint m_includeEclipseWarnings : 1;
    uint m_reserved : 30;

    TypeEntryHash m_entries;
    QMap<QString,int> m_class_name_counter;
    SingleTypeEntryHash m_flags_entries;
    TemplateEntryHash m_templates;
    TemplateTypeEntryHash m_templateTypes;
    QStringList m_suppressedWarnings;

    QList<TypeRejection> m_rejections;
    QStringList m_rebuildClasses;
    QMap<QString,bool> m_rejectedClasses;

    DefinedPtr m_defined;
    QMap<QString,TypeSystemTypeEntry*> m_typeSystemsByQtLibrary;
    QSet<QString> m_loadedTypesystems;
    QList<TypeSystemTypeEntry*> m_otherTypeSystems;
    ComplexTypeEntry * m_pixmapType;
    ComplexTypeEntry * m_bitmapType;
    ComplexTypeEntry * m_qstringType;
    ComplexTypeEntry * m_qvariantType;
    ComplexTypeEntry * m_qcharType;
    bool m_useNativeIds = true;
};

}

extern QString resolveFilePath(QString fileName, int opts, const QStringList &list);

#endif
