/****************************************************************************
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

#ifndef TYPEDATABASE_H_
#define TYPEDATABASE_H_

#include <QList>
#include <qstringlist.h>

#include "typeentry.h"

class TypeDatabase {
    public:
        TypeDatabase();

        static TypeDatabase *instance();

        QList<Include> extraIncludes(const QString &className);

        ComplexTypeEntry *findComplexType(const QString &name);
        PrimitiveTypeEntry *findPrimitiveType(const QString &name);
        ObjectTypeEntry *findObjectType(const QString &name);
        NamespaceTypeEntry *findNamespaceType(const QString &name);

        ContainerTypeEntry *findContainerType(const QString &name);
        PointerContainerTypeEntry *findPointerContainerType(const QString &name);

        TypeEntry *findType(const QString &name) const;

        QList<TypeEntry *> findTypes(const QString &name) const {
            return m_entries.value(name);
        }

        TypeEntryHash allEntries() {
            return m_entries;
        }

        SingleTypeEntryHash entries();

        PrimitiveTypeEntry *findTargetLangPrimitiveType(const QString &java_name);

        void addRejection(const QString &class_name, const QString &function_name,
                          const QString &field_name, const QString &enum_name);
        bool isClassRejected(const QString &class_name);
        bool isFunctionRejected(const QString &class_name, const QString &function_name);
        bool isFieldRejected(const QString &class_name, const QString &field_name);
        bool isEnumRejected(const QString &class_name, const QString &enum_name);

        void addType(TypeEntry *e) {
            m_entries[e->qualifiedCppName()].append(e);
            QString name = e->qualifiedCppName().contains("::") ? e->qualifiedCppName().split("::").last() : e->qualifiedCppName();
            m_class_name_counter[name]++;
        }

        bool isUniqueClassName(QString name) {
            return m_class_name_counter[name]<=1;
        }

        SingleTypeEntryHash flagsEntries() const {
            return m_flags_entries;
        }

        FlagsTypeEntry *findFlagsType(const QString &name) const;

        void addFlagsType(FlagsTypeEntry *fte) {
            m_flags_entries[fte->originalName()] = fte;
        }

        TemplateEntry *findTemplate(const QString &name) {
            return m_templates[name];
        }
        void addTemplate(TemplateEntry *t) {
            m_templates[t->name()] = t;
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
            m_rebuild_classes = cls;
        }

        static QString globalNamespaceClassName(const TypeEntry *te);
        QString filename() const {
            return "typesystem.txt";
        }

        bool parseFile(const QString &filename, const QStringList &importInputDirectoryList, bool generate = true, bool optional = false);

    private:
    uint m_suppressWarnings :
        1;
    uint m_includeEclipseWarnings :
        1;
    uint m_reserved :
        30;

        TypeEntryHash m_entries;
        QHash<QString,int> m_class_name_counter;
        SingleTypeEntryHash m_flags_entries;
        TemplateEntryHash m_templates;
        QStringList m_suppressedWarnings;

        QList<TypeRejection> m_rejections;
        QStringList m_rebuild_classes;
};

#endif
