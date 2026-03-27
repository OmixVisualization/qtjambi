/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
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

#include "pch_p.h"
#include "range_p_p.h"

#if QT_VERSION >= QT_VERSION_CHECK(6,10,0)

Q_GLOBAL_STATIC(QReadWriteLock, gLock)
typedef QHash<int, MultiRole::Type> MultiRoles;
Q_GLOBAL_STATIC(MultiRoles, gMultiRoles)
typedef QHash<int, MetaTypeUtils::DataType> DataTypes;
Q_GLOBAL_STATIC(DataTypes, gDataTypes)

MultiRole::Type MultiRole::isMultiRole(const QMetaType& metaType, AbstractContainerAccess* containerAccess){
    {
        QReadLocker locker(gLock());
        if(gMultiRoles->contains(metaType.id()))
            return gMultiRoles->value(metaType.id(), MultiRole::None);
    }
    MultiRole::Type result = MultiRole::None;
    if(containerAccess && containerAccess->isAssociative()){
        AbstractAssociativeAccess* associativeElementAccess = static_cast<AbstractAssociativeAccess*>(containerAccess);
        if(associativeElementAccess->valueMetaType()==QMetaType::fromType<QVariant>()){
            if(associativeElementAccess->keyMetaType()==QMetaType::fromType<int>()){
                result = MultiRole::Integer;
            }else if(associativeElementAccess->keyMetaType()==QMetaType::fromType<Qt::ItemDataRole>()){
                result = MultiRole::ItemDataRole;
            }else if(associativeElementAccess->keyMetaType()==QMetaType::fromType<QString>()){
                result = MultiRole::String;
            }
        }
    }
    {
        QWriteLocker locker(gLock());
        gMultiRoles->insert(metaType.id(), result);
    }
    return result;
}

MetaTypeUtils::DataType MetaTypeUtils::dataType(const QMetaType& metaType){
    auto flags = metaType.flags();
    if(flags & QMetaType::IsPointer){
        return MetaTypeUtils::Pointer;
    }else if(flags & QMetaType::SharedPointerToQObject){
        return MetaTypeUtils::QSharedPointer;
    }else if(flags & QMetaType::WeakPointerToQObject){
        return MetaTypeUtils::QWeakPointer;
    }else if(flags & QMetaType::TrackingPointerToQObject){
        return MetaTypeUtils::QPointer;
    }else{
        int id = metaType.id();
        if(id<QMetaType::LastWidgetsType){
            return MetaTypeUtils::Value;
        }
        {
            QReadLocker locker(gLock());
            if(gDataTypes->contains(metaType.id()))
                return gDataTypes->value(metaType.id(), MetaTypeUtils::Value);
        }
        MetaTypeUtils::DataType result = MetaTypeUtils::Value;
        QByteArrayView metaTypeName(metaType.name());
        if(metaTypeName.startsWith("QSharedPointer<")){
            result = MetaTypeUtils::QSharedPointer;
        }else if(metaTypeName.startsWith("QSharedDataPointer<")){
            result = MetaTypeUtils::QWeakPointer;
        }else if(metaTypeName.startsWith("QWeakPointer<")){
            result = MetaTypeUtils::QSharedDataPointer;
        }else if(metaTypeName.startsWith("QExplicitlySharedDataPointer<")){
            result = MetaTypeUtils::QExplicitlySharedDataPointer;
        }else if(metaTypeName.startsWith("QScopedPointer<")){
            result = MetaTypeUtils::QScopedPointer;
        }else if(metaTypeName.startsWith("std::shared_ptr<")){
            result = MetaTypeUtils::shared_ptr;
        }else if(metaTypeName.startsWith("std::weak_ptr<")){
            result = MetaTypeUtils::weak_ptr;
        }else if(metaTypeName.startsWith("std::shared_ptr<")){
            result = MetaTypeUtils::shared_ptr;
        }
        {
            QWriteLocker locker(gLock());
            gDataTypes->insert(metaType.id(), result);
        }
        return result;
    }
}

bool logRangeModel(){
    static bool b = []()->bool{
        QByteArray value = qgetenv("QTJAMBI_LOG_RANGEMODEL");
        return value=="1" || value.compare("true", Qt::CaseInsensitive)==1;
    }();
    return b;
}

void containerDisposer(AbstractContainerAccess* _access){
    if(_access)
        _access->dispose();
}

PropertyRef::operator QVariant() const{
    return m_isGadget ? m_property.readOnGadget(m_gadget) : m_property.read(reinterpret_cast<const QObject*>(m_gadget));
}

PropertyRef& PropertyRef::operator=(const QVariant& v){
    if(m_isGadget){
        m_property.writeOnGadget(m_gadget, v);
    }else{
        m_property.write(reinterpret_cast<QObject*>(m_gadget), v);
    }
    return *this;
}

MetaPropertyIterator::MetaPropertyIterator(void* gadget, const QMetaType& metaType, int index)
    : m_gadget(gadget),
    m_metaObject(metaType.metaObject()),
    m_isGadget(metaType.flags() & QMetaType::TypeFlag::IsGadget),
    m_index(index),
    m_current(m_metaObject->property(m_index)),
    m_currentMetaType(m_current.metaType())
{
}

MetaPropertyIterator::MetaPropertyIterator(const MetaPropertyIterator& other)
    : m_gadget(other.m_gadget),
    m_metaObject(other.m_metaObject),
    m_isGadget(other.m_isGadget),
    m_index(other.m_index),
    m_current(other.m_current),
    m_currentMetaType(other.m_currentMetaType){
}

MetaPropertyIterator::~MetaPropertyIterator(){}

PropertyRef MetaPropertyIterator::operator*(){
    return PropertyRef{m_gadget, m_isGadget, m_metaObject->property(m_index)};
}

bool MetaPropertyIterator::operator==(const MetaPropertyIterator& other) const {
    return m_gadget==other.m_gadget && m_metaObject==other.m_metaObject && m_index==other.m_index;
}

MetaPropertyIterator& MetaPropertyIterator::operator++(){
    ++m_index;
    return *this;
}
MetaPropertyIterator MetaPropertyIterator::operator++(int){
    MetaPropertyIterator copy(*this);
    ++m_index;
    return copy;
}

ConstMetaPropertyIterator::ConstMetaPropertyIterator(const void* gadget, const QMetaType& metaType, int index)
    : m_gadget(gadget),
    m_metaObject(metaType.metaObject()),
    m_isGadget(metaType.flags() & QMetaType::TypeFlag::IsGadget),
    m_index(index),
    m_current(m_metaObject->property(m_index)),
    m_currentMetaType(m_current.metaType())
{
}

ConstMetaPropertyIterator::ConstMetaPropertyIterator(const ConstMetaPropertyIterator& other)
    : m_gadget(other.m_gadget),
    m_metaObject(other.m_metaObject),
    m_isGadget(other.m_isGadget),
    m_index(other.m_index),
    m_current(other.m_current),
    m_currentMetaType(other.m_currentMetaType){
}

ConstMetaPropertyIterator::~ConstMetaPropertyIterator(){}

QVariant ConstMetaPropertyIterator::operator*(){
    QMetaProperty property = m_current;
    m_current = m_metaObject->property(m_index);
    return m_isGadget ? property.readOnGadget(m_gadget) : property.read(reinterpret_cast<const QObject*>(m_gadget));
}

bool ConstMetaPropertyIterator::operator==(const ConstMetaPropertyIterator& other) const {
    return m_gadget==other.m_gadget && m_metaObject==other.m_metaObject && m_index==other.m_index;
}

ConstMetaPropertyIterator& ConstMetaPropertyIterator::operator++(){
    ++m_index;
    return *this;
}
ConstMetaPropertyIterator ConstMetaPropertyIterator::operator++(int){
    ConstMetaPropertyIterator copy(*this);
    ++m_index;
    return copy;
}

TreeRangeData<false>::TreeRangeData(void* container, QSharedPointer<AbstractSequentialAccess>&& containerAccess, std::shared_ptr<int>&& _treeColumnCount)
    : m_container(container),
    m_treeColumnCount(std::move(_treeColumnCount)),
    m_containerAccess(std::move(containerAccess)),
    m_elementDataType(MetaTypeUtils::dataType(m_containerAccess->elementMetaType())),
    m_elementNestedContainerAccess(m_containerAccess->elementNestedContainerAccess(), &containerDisposer) {
}

TreeRangeData<false>::TreeRangeData(JNIEnv*, jobject, const TreeRangeData<false>& other)
    : m_container(nullptr),
    m_treeColumnCount(other.m_treeColumnCount),
    m_containerAccess(other.m_containerAccess),
    m_elementDataType(other.m_elementDataType),
    m_elementNestedContainerAccess(other.m_elementNestedContainerAccess)
{
}

TreeRangeData<false>::TreeRangeData()
    : m_container(nullptr),
    m_treeColumnCount(),
    m_containerAccess(),
    m_elementDataType(MetaTypeUtils::Value),
    m_elementNestedContainerAccess() {
}

TreeRangeData<false>::TreeRangeData(TreeRangeData<false>&& other)
    : m_container(std::move(other.m_container)),
    m_treeColumnCount(std::move(other.m_treeColumnCount)),
    m_containerAccess(std::move(other.m_containerAccess)),
    m_elementDataType(std::move(other.m_elementDataType)),
    m_elementNestedContainerAccess(std::move(other.m_elementNestedContainerAccess)) {
}

TreeRangeData<false>::~TreeRangeData(){
}

void* TreeRangeData<false>::container() const{
    return m_container;
}

const QMetaType& TreeRangeData<false>::elementMetaType() const{
    return m_containerAccess->elementMetaType();
}

MetaTypeUtils::DataType TreeRangeData<false>::elementDataType() const{
    return m_elementDataType;
}

const QSharedPointer<AbstractContainerAccess>& TreeRangeData<false>::elementNestedContainerAccess() const {
    return m_elementNestedContainerAccess;
}

const QSharedPointer<AbstractSequentialAccess>& TreeRangeData<false>::containerAccess() const {
    return m_containerAccess;
}

bool TreeRangeData<false>::isInitialized() const{
    return m_treeColumnCount ? true : false;
}

void TreeRangeData<false>::initialize(JNIEnv*, const TreeRangeData<false>& other){
    m_treeColumnCount = other.m_treeColumnCount;
    m_containerAccess = other.m_containerAccess;
    m_elementDataType = other.m_elementDataType;
    m_elementNestedContainerAccess = other.m_elementNestedContainerAccess;
}

int TreeRangeData<false>::treeColumnCount() const{
    Q_ASSERT(m_treeColumnCount);
    return !m_treeColumnCount ? 0 : *m_treeColumnCount;
}

AbstractIteratorBase::AbstractIteratorBase(std::unique_ptr<AbstractSequentialAccess::ElementIterator>&& _iter)
    : current_data(nullptr), current_metaType(), current_dataType(MetaTypeUtils::Value), current_containerAccess(), iter(std::move(_iter)) {
}

AbstractIteratorBase::AbstractIteratorBase(const AbstractIteratorBase& other)
    : current_data(other.current_data),
    current_metaType(other.current_metaType),
    current_dataType(other.current_dataType),
    current_containerAccess(other.current_containerAccess),
    iter(other.iter ? other.iter->clone() : nullptr){
}

AbstractIteratorBase::AbstractIteratorBase(AbstractIteratorBase&& other)
    : current_data(std::move(other.current_data)),
    current_metaType(std::move(other.current_metaType)),
    current_dataType(std::move(other.current_dataType)),
    current_containerAccess(std::move(other.current_containerAccess)),
    iter(std::move(other.iter)){
}

AbstractIteratorBase& AbstractIteratorBase::operator=(const AbstractIteratorBase& other){
    current_metaType = other.current_metaType;
    current_dataType = other.current_dataType;
    current_containerAccess = other.current_containerAccess;
    current_data = other.current_data;
    iter = other.iter ? other.iter->clone() : std::unique_ptr<AbstractSequentialAccess::ElementIterator>{nullptr};
    return *this;
}

void AbstractIteratorBase::swap(AbstractIteratorBase& other) noexcept {
    std::swap(current_metaType, other.current_metaType);
    std::swap(current_dataType, other.current_dataType);
    std::swap(current_containerAccess, other.current_containerAccess);
    std::swap(current_data, other.current_data);
    std::swap(iter, other.iter);
}

AbstractIteratorBase& AbstractIteratorBase::operator=(AbstractIteratorBase&& other){
    current_metaType = std::move(other.current_metaType);
    current_dataType = std::move(other.current_dataType);
    current_containerAccess = std::move(other.current_containerAccess);
    current_data = std::move(other.current_data);
    iter = std::move(other.iter);
    return *this;
}

bool AbstractIteratorBase::operator==(const AbstractIteratorBase& other) const{
    if(!other.iter.get()){
        return !iter.get();
    }else if(!iter.get()){
        return false;
    }else{
        return *iter==*other.iter;
    }
}

bool AbstractIteratorBase::operator!=(const AbstractIteratorBase& other) const{
    return !operator==(other);
}

bool AbstractIteratorBase::isConst() const {
    return false;
}

bool AbstractIteratorBase::operator==(const AbstractConstIteratorBase& other) const{
    if(!other.iter.get()){
        return !iter.get();
    }else if(!iter.get()){
        return false;
    }else{
        return *iter==*other.iter;
    }
}
bool AbstractIteratorBase::operator!=(const AbstractConstIteratorBase& other) const{
    return !operator==(other);
}

AbstractConstIteratorBase& AbstractConstIteratorBase::operator=(const AbstractConstIteratorBase& other){
    current_metaType = other.current_metaType;
    current_dataType = other.current_dataType;
    current_containerAccess = other.current_containerAccess;
    current_data = other.current_data;
    iter = other.iter ? other.iter->clone() : std::unique_ptr<AbstractSequentialAccess::ElementIterator>{nullptr};
    return *this;
}

AbstractConstIteratorBase& AbstractConstIteratorBase::operator=(const AbstractIteratorBase& other){
    current_metaType = other.current_metaType;
    current_dataType = other.current_dataType;
    current_containerAccess = other.current_containerAccess;
    current_data = other.current_data;
    iter = other.iter ? other.iter->clone() : std::unique_ptr<AbstractSequentialAccess::ElementIterator>{nullptr};
    return *this;
}

AbstractConstIteratorBase& AbstractConstIteratorBase::operator=(AbstractConstIteratorBase&& other){
    current_metaType = std::move(other.current_metaType);
    current_dataType = std::move(other.current_dataType);
    current_containerAccess = std::move(other.current_containerAccess);
    current_data = std::move(other.current_data);
    iter = std::move(other.iter);
    return *this;
}

AbstractConstIteratorBase& AbstractConstIteratorBase::operator=(AbstractIteratorBase&& other){
    current_metaType = std::move(other.current_metaType);
    current_dataType = std::move(other.current_dataType);
    current_containerAccess = std::move(other.current_containerAccess);
    current_data = std::move(other.current_data);
    iter = std::move(other.iter);
    return *this;
}

bool AbstractConstIteratorBase::operator==(const AbstractConstIteratorBase& other) const{
    if(!other.iter.get()){
        return !iter.get();
    }else if(!iter.get()){
        return false;
    }else{
        return *iter==*other.iter;
    }
}

bool AbstractConstIteratorBase::operator==(const AbstractIteratorBase& other) const{
    if(!other.iter.get()){
        return !iter.get();
    }else if(!iter.get()){
        return false;
    }else{
        return *iter==*other.iter;
    }
}

bool AbstractConstIteratorBase::operator!=(const AbstractConstIteratorBase& other) const{
    return !operator==(other);
}

bool AbstractConstIteratorBase::isConst() const {
    return true;
}

AbstractConstIteratorBase::AbstractConstIteratorBase(std::unique_ptr<AbstractSequentialAccess::ElementIterator>&& _iter)
    : current_data(nullptr), current_metaType(), current_dataType(MetaTypeUtils::Value), current_containerAccess(), iter(std::move(_iter)) {
}

AbstractConstIteratorBase::AbstractConstIteratorBase(const AbstractConstIteratorBase& other)
    : current_data(other.current_data),
    current_metaType(other.current_metaType),
    current_dataType(other.current_dataType),
    current_containerAccess(other.current_containerAccess),
    iter(other.iter ? other.iter->clone() : nullptr){
}

AbstractConstIteratorBase::AbstractConstIteratorBase(AbstractConstIteratorBase&& other)
    : current_data(std::move(other.current_data)),
    current_metaType(std::move(other.current_metaType)),
    current_dataType(std::move(other.current_dataType)),
    current_containerAccess(std::move(other.current_containerAccess)),
    iter(std::move(other.iter)){
}

AbstractConstIteratorBase::AbstractConstIteratorBase(const AbstractIteratorBase& other)
    : current_data(other.current_data),
    current_metaType(other.current_metaType),
    current_dataType(other.current_dataType),
    current_containerAccess(other.current_containerAccess),
    iter(other.iter ? other.iter->clone() : nullptr){
}

AbstractConstIteratorBase::AbstractConstIteratorBase(AbstractIteratorBase&& other)
    : current_data(std::move(other.current_data)),
    current_metaType(std::move(other.current_metaType)),
    current_dataType(std::move(other.current_dataType)),
    current_containerAccess(std::move(other.current_containerAccess)),
    iter(std::move(other.iter)){
}

QGenericTableItemModelImpl<GenericTable>::QGenericTableItemModelImpl(GenericTable &&model, QRangeModel *itemModel)
    : QRangeModelImplBase(itemModel)
{
    initCallFN(&QGenericTableItemModelImpl<GenericTable>::callImpl);
    switch(model.treeType){
    case TreeType::None:
        initializeTable(itemModel, std::move(model));
        break;
    default:
        initializeTree(itemModel, std::move(model));
        break;
    }
}

void QGenericTableItemModelImpl<GenericTable>::callImpl(size_t index, QtPrivate::QQuasiVirtualInterface<QRangeModelImplBase> &intf, void *ret, void *args)
{
    struct Impl{
        CallFN m_callFN;
    };
    QGenericTableItemModelImpl<GenericTable>& _this = static_cast<QGenericTableItemModelImpl<GenericTable>&>(intf);
    reinterpret_cast<Impl*>(_this.impl)->m_callFN(index, *_this.impl, ret, args);
}

#endif
