#ifndef QTJAMBI_TEST_UTIL_H
#define QTJAMBI_TEST_UTIL_H

#include <QtCore/qglobal.h>
#include <QtTest/qtestcase.h>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

#include <QtWidgets/QtWidgets>
#include <QtCore/qmetacontainer.h>
#include <QtTest/QtTest>
#include <qtjambi/qtjambi_registry.h>

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

#ifndef QT_JAMBI_RUN
namespace QTest{
void addColumn(const char* name, int id);
}
#endif
#endif // QTJAMBI_TEST_UTIL_H
