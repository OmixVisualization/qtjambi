/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef UTILS_P_H
#define UTILS_P_H

#include <QtCore/qglobal.h>
#include <QtTest/qtestcase.h>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

#include <QtWidgets/QtWidgets>
#include <QtCore/qmetacontainer.h>
#include <QtTest/QtTest>
#include <QtJambi/RegistryAPI>

namespace QtJambiPrivate {
template<> struct supports_qHash<QSignalSpy> : supports_qHash<QList<QList<QVariant>>>{};
}

namespace QtMetaContainerPrivate {

template<>
class QMetaAssociationForContainer<QMap<int,QEventPoint>> : public QMetaContainerForContainer<QMap<int,QEventPoint>>
{
    typedef QMap<int,QEventPoint> C;
    friend QMetaAssociationInterface;

    static constexpr const QtPrivate::QMetaTypeInterface *getKeyMetaType()
    {
        if constexpr (QContainerInfo::has_key_type_v<C>)
            return QtPrivate::qMetaTypeInterfaceForType<typename C::key_type>();
        else
            return nullptr;
    }

    static constexpr const QtPrivate::QMetaTypeInterface *getMappedMetaType()
    {
        if constexpr (QContainerInfo::has_mapped_type_v<C>)
            return QtPrivate::qMetaTypeInterfaceForType<typename C::mapped_type>();
        else
            return nullptr;
    }

    static constexpr QMetaAssociationInterface::InsertKeyFn getInsertKeyFn()
    {
        if constexpr (QContainerInfo::can_insert_key_mapped_v<C>) {
            return [](void *c, const void *k) {
                static_cast<C *>(c)->insert(
                            *static_cast<const QContainerInfo::key_type<C> *>(k), QEventPoint());
            };
        } else {
            return nullptr;
        }
    }

    static constexpr QMetaAssociationInterface::RemoveKeyFn getRemoveKeyFn()
    {
        if constexpr (QContainerInfo::can_remove_at_key_v<C>) {
            return [](void *c, const void *k) {
                static_cast<C *>(c)->remove(*static_cast<const QContainerInfo::key_type<C> *>(k));
            };
        } else {
            return nullptr;
        }
    }

    static constexpr QMetaAssociationInterface::ContainsKeyFn getContainsKeyFn()
    {
        if constexpr (QContainerInfo::has_contains_v<C>) {
            return [](const void *c, const void *k) {
                return static_cast<const C *>(c)->contains(
                            *static_cast<const QContainerInfo::key_type<C> *>(k));
            };
        } else if (QContainerInfo::has_find_v<C>) {
            return [](const void *c, const void *k) {
                const C *container = static_cast<const C *>(c);
                return container->find(
                            *static_cast<const QContainerInfo::key_type<C> *>(k))
                        != container->end();
            };
        } else {
            return nullptr;
        }
    }

    static constexpr QMetaAssociationInterface::MappedAtKeyFn getMappedAtKeyFn()
    {
        if constexpr (QContainerInfo::can_get_at_key_v<C>) {
            return [](const void *c, const void *k, void *r) {
                *static_cast<QContainerInfo::mapped_type<C> *>(r)
                        = (*static_cast<const C *>(c))[
                                *static_cast<const QContainerInfo::key_type<C> *>(k)];
            };
        } else {
            return nullptr;
        }
    }

    static constexpr QMetaAssociationInterface::SetMappedAtKeyFn getSetMappedAtKeyFn()
    {
        if constexpr (QContainerInfo::can_set_at_key_v<C>) {
            return [](void *c, const void *k, const void *m) {
                (*static_cast<C *>(c))[*static_cast<const QContainerInfo::key_type<C> *>(k)] =
                        *static_cast<const QContainerInfo::mapped_type<C> *>(m);
            };
        } else {
            return nullptr;
        }
    }

    static constexpr QMetaAssociationInterface::CreateIteratorAtKeyFn createIteratorAtKeyFn()
    {
        if constexpr (QContainerInfo::has_find_v<C>) {
            return [](void *c, const void *k) -> void* {
                using Iterator = QContainerInfo::iterator<C>;
                return new Iterator(static_cast<C *>(c)->find(
                            *static_cast<const QContainerInfo::key_type<C> *>(k)));
            };
        } else {
            return nullptr;
        }
    }

    static constexpr QMetaAssociationInterface::CreateConstIteratorAtKeyFn createConstIteratorAtKeyFn()
    {
        if constexpr (QContainerInfo::has_find_v<C>) {
            return [](const void *c, const void *k) -> void* {
                using Iterator = QContainerInfo::const_iterator<C>;
                return new Iterator(static_cast<const C *>(c)->find(
                            *static_cast<const QContainerInfo::key_type<C> *>(k)));
            };
        } else {
            return nullptr;
        }
    }

    template<typename Iterator>
    static constexpr QMetaAssociationInterface::KeyAtIteratorFn keyAtIteratorFn()
    {
        if constexpr (QContainerInfo::iterator_has_key_v<C>) {
            return [](const void *i, void *k) {
                *static_cast<QContainerInfo::key_type<C> *>(k)
                        = static_cast<const Iterator *>(i)->key();
            };
        } else if constexpr (QContainerInfo::iterator_dereferences_to_value_v<C>
                && QContainerInfo::value_type_has_first_v<C>) {
            return [](const void *i, void *k) {
                *static_cast<QContainerInfo::key_type<C> *>(k)
                        = (*static_cast<const Iterator *>(i))->first;
            };
        } else if constexpr (QContainerInfo::iterator_dereferences_to_key_v<C>) {
            return [](const void *i, void *k) {
                *static_cast<QContainerInfo::key_type<C> *>(k)
                        = *(*static_cast<const Iterator *>(i));
            };
        } else {
            return nullptr;
        }
    }

    static constexpr QMetaAssociationInterface::KeyAtIteratorFn getKeyAtIteratorFn()
    {
        return keyAtIteratorFn<QContainerInfo::iterator<C>>();
    }

    static constexpr QMetaAssociationInterface::KeyAtIteratorFn getKeyAtConstIteratorFn()
    {
        return keyAtIteratorFn<QContainerInfo::const_iterator<C>>();
    }

    template<typename Iterator>
    static constexpr QMetaAssociationInterface::MappedAtIteratorFn mappedAtIteratorFn()
    {
        if constexpr (QContainerInfo::iterator_has_value_v<C>) {
            return [](const void *i, void *k) {
                *static_cast<QContainerInfo::mapped_type<C> *>(k)
                        = static_cast<const Iterator *>(i)->value();
            };
        } else if constexpr (QContainerInfo::iterator_dereferences_to_value_v<C>
                && QContainerInfo::value_type_has_second_v<C>) {
            return [](const void *i, void *k) {
                *static_cast<QContainerInfo::mapped_type<C> *>(k)
                        = (*static_cast<const Iterator *>(i))->second;
            };
        } else if constexpr (QContainerInfo::iterator_dereferences_to_mapped_v<C>) {
            return [](const void *i, void *k) {
                *static_cast<QContainerInfo::mapped_type<C> *>(k)
                        = *static_cast<const Iterator *>(i);
            };
        } else {
            return nullptr;
        }
    }

    static constexpr QMetaAssociationInterface::MappedAtIteratorFn getMappedAtIteratorFn()
    {
        return mappedAtIteratorFn<QContainerInfo::iterator<C>>();
    }

    static constexpr QMetaAssociationInterface::MappedAtIteratorFn getMappedAtConstIteratorFn()
    {
        return mappedAtIteratorFn<QContainerInfo::const_iterator<C>>();
    }

    static constexpr QMetaAssociationInterface::SetMappedAtIteratorFn getSetMappedAtIteratorFn()
    {
        if constexpr (QContainerInfo::can_set_mapped_at_iterator_v<C> && !std::is_const_v<C>) {
            return [](const void *i, const void *m) {
                *(*static_cast<const QContainerInfo::iterator<C> *>(i))
                        = *static_cast<const QContainerInfo::mapped_type<C> *>(m);
            };
        } else {
            return nullptr;
        }
    }

    static constexpr QMetaAssociationInterface::EraseKeyAtIteratorFn getEraseKeyAtIteratorFn()
    {
        return QMetaContainerForContainer<C>::template getEraseAtIteratorFn<
                QMetaAssociationInterface::EraseKeyAtIteratorFn>();
    }
};

}

#endif

#ifndef QTJAMBI_GENERATOR_RUNNING
namespace QTest{
void addColumn(const char* name, int id);
}
#endif
#endif // UTILS_P_H
