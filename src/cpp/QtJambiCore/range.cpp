/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    if(AbstractAssociativeAccess* associativeElementAccess = dynamic_cast<AbstractAssociativeAccess*>(containerAccess)){
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

bool MutableRow::initialize(void* _data, const QMetaType& _metaType, MetaTypeUtils::DataType _dataType, const QSharedPointer<AbstractContainerAccess>& _containerAccess) {
    data = _data;
    metaType = _metaType;
    dataType = _dataType;
    containerAccess = _containerAccess;
    return true;
}
MutableRow::MutableRow(void* _data, const QMetaType& _metaType, MetaTypeUtils::DataType _dataType, const QSharedPointer<AbstractContainerAccess>& _containerAccess)
    : data(_data), metaType(_metaType), dataType(_dataType), containerAccess(_containerAccess), needsDeletion(false)
{}
MutableRow::MutableRow(const MutableRow& other)
    : data(other.data), metaType(other.metaType), dataType(other.dataType), containerAccess(other.containerAccess), needsDeletion(other.needsDeletion)
{
    if(needsDeletion){
        data = metaType.create(const_cast<void*>(data));
    }
}
MutableRow::MutableRow(MutableRow&& other)
    : data(std::move(other.data)), metaType(std::move(other.metaType)), dataType(std::move(other.dataType)), containerAccess(std::move(other.containerAccess)), needsDeletion(std::move(other.needsDeletion))
{
}
MutableRow::MutableRow()
    : data(nullptr), metaType(), dataType(MetaTypeUtils::Value), containerAccess(nullptr), needsDeletion(false)
{}
MutableRow::~MutableRow(){
    if(needsDeletion && data){
        if(metaType.flags() & QMetaType::IsPointer){
            delete reinterpret_cast<void**>(data);
        }else{
            metaType.destroy(data);
        }
    }
}
MutableRow& MutableRow::operator=(const MutableRow& other){
    if(needsDeletion && data){
        if(metaType.flags() & QMetaType::IsPointer){
            delete reinterpret_cast<void**>(data);
        }else{
            metaType.destroy(data);
        }
    }
    data = other.data;
    metaType = other.metaType;
    dataType = other.dataType;
    containerAccess = other.containerAccess;
    needsDeletion = other.needsDeletion;
    if(needsDeletion){
        data = metaType.create(const_cast<void*>(data));
    }
    return *this;
}
MutableRow& MutableRow::operator=(MutableRow&& other){
    data = std::move(other.data);
    metaType = std::move(other.metaType);
    dataType = std::move(other.dataType);
    containerAccess = std::move(other.containerAccess);
    needsDeletion = std::move(other.needsDeletion);
    return *this;
}
void MutableRow::swap(MutableRow& other) noexcept{
    if(metaType.flags() & QMetaType::IsPointer){
        void* tmp = *reinterpret_cast<void**>(other.data);
        *reinterpret_cast<void**>(other.data) = *reinterpret_cast<void**>(data);
        *reinterpret_cast<void**>(data) = tmp;
    }else{
        void* tmp = metaType.create(other.data);
        metaType.destruct(other.data);
        metaType.construct(other.data, data);
        metaType.construct(data, tmp);
        metaType.destroy(tmp);
    }
}

void swap(MutableRow&& _this, MutableRow&& other) noexcept{
    _this.swap(other);
}
void swap(MutableRow& _this, MutableRow& other) noexcept{
    _this.swap(other);
}
void qSwap(MutableRow& _this, MutableRow& other) noexcept{
    _this.swap(other);
}

void* MutableRow::pointer()const{
    if(data){
        switch(dataType){
        case MetaTypeUtils::Pointer:
            return *reinterpret_cast<void**>(data);
        case MetaTypeUtils::QPointer:
            return reinterpret_cast<QPointer<QObject>*>(data)->get();
        case MetaTypeUtils::QSharedPointer:
            return reinterpret_cast<QSharedPointer<char>*>(data)->get();
        case MetaTypeUtils::QWeakPointer:
            return QSharedPointer<char>(*reinterpret_cast<QWeakPointer<char>*>(data)).get();
        case MetaTypeUtils::QSharedDataPointer:
            return const_cast<QSharedData*>(reinterpret_cast<const QSharedDataPointer<QSharedData>*>(data)->get());
        case MetaTypeUtils::QExplicitlySharedDataPointer:
            return reinterpret_cast<QExplicitlySharedDataPointer<QSharedData>*>(data)->get();
        case MetaTypeUtils::QScopedPointer:
            return reinterpret_cast<QScopedPointer<char>*>(data)->get();
        case MetaTypeUtils::shared_ptr:
            return reinterpret_cast<std::shared_ptr<char>*>(data)->get();
        case MetaTypeUtils::weak_ptr:
            return std::shared_ptr<char>(*reinterpret_cast<std::weak_ptr<char>*>(data)).get();
        case MetaTypeUtils::unique_ptr:
            return reinterpret_cast<std::unique_ptr<char>*>(data)->get();
        default:
            break;
        }
    }
    return data;
}

bool ConstRow::initialize(const void* _data, const QMetaType& _metaType, MetaTypeUtils::DataType _dataType, const QSharedPointer<AbstractContainerAccess>& _containerAccess) {
    data = _data;
    metaType = _metaType;
    dataType = _dataType;
    containerAccess = _containerAccess;
    return true;
}
ConstRow::ConstRow(const void* _data, const QMetaType& _metaType, MetaTypeUtils::DataType _dataType, const QSharedPointer<AbstractContainerAccess>& _containerAccess)
    : data(_data), metaType(_metaType), dataType(_dataType), containerAccess(_containerAccess), needsDeletion(false)
{}
ConstRow::ConstRow(const ConstRow& other)
    : data(other.data), metaType(other.metaType), dataType(other.dataType), containerAccess(other.containerAccess), needsDeletion(other.needsDeletion)
{
    if(needsDeletion){
        data = metaType.create(const_cast<void*>(data));
    }
}
ConstRow::ConstRow(const MutableRow& other)
    : data(other.data), metaType(other.metaType), dataType(other.dataType), containerAccess(other.containerAccess), needsDeletion(other.needsDeletion)
{
    if(needsDeletion){
        data = metaType.create(const_cast<void*>(data));
    }
}
ConstRow::ConstRow(ConstRow&& other)
    : data(std::move(other.data)), metaType(std::move(other.metaType)), dataType(std::move(other.dataType)), containerAccess(std::move(other.containerAccess)), needsDeletion(std::move(other.needsDeletion))
{
}
ConstRow::ConstRow()
    : data(nullptr), metaType(), dataType(MetaTypeUtils::Value), containerAccess(nullptr), needsDeletion(false)
{}
ConstRow::~ConstRow(){
    if(needsDeletion && data){
        metaType.destroy(const_cast<void*>(data));
    }
}
ConstRow& ConstRow::operator=(const ConstRow& other){
    if(needsDeletion && data){
        if(metaType.flags() & QMetaType::IsPointer){
            delete reinterpret_cast<void**>(const_cast<void*>(data));
        }else{
            metaType.destroy(const_cast<void*>(data));
        }
    }
    data = other.data;
    metaType = other.metaType;
    dataType = other.dataType;
    containerAccess = other.containerAccess;
    needsDeletion = other.needsDeletion;
    if(needsDeletion){
        data = metaType.create(const_cast<void*>(data));
    }
    return *this;
}
ConstRow& ConstRow::operator=(ConstRow&& other){
    data = std::move(other.data);
    metaType = std::move(other.metaType);
    dataType = std::move(other.dataType);
    containerAccess = std::move(other.containerAccess);
    needsDeletion = std::move(other.needsDeletion);
    return *this;
}

const void* ConstRow::pointer()const{
    if(data){
        switch(dataType){
        case MetaTypeUtils::Pointer:
            return *reinterpret_cast<void*const*>(data);
        case MetaTypeUtils::QPointer:
            return reinterpret_cast<const QPointer<QObject>*>(data)->get();
        case MetaTypeUtils::QSharedPointer:
            return reinterpret_cast<const QSharedPointer<char>*>(data)->get();
        case MetaTypeUtils::QWeakPointer:
            return QSharedPointer<char>(*reinterpret_cast<const QWeakPointer<char>*>(data)).get();
        case MetaTypeUtils::QSharedDataPointer:
            return reinterpret_cast<const QSharedDataPointer<QSharedData>*>(data)->get();
        case MetaTypeUtils::QExplicitlySharedDataPointer:
            return reinterpret_cast<const QExplicitlySharedDataPointer<QSharedData>*>(data)->get();
        case MetaTypeUtils::QScopedPointer:
            return reinterpret_cast<const QScopedPointer<char>*>(data)->get();
        case MetaTypeUtils::shared_ptr:
            return reinterpret_cast<const std::shared_ptr<char>*>(data)->get();
        case MetaTypeUtils::weak_ptr:
            return std::shared_ptr<char>(*reinterpret_cast<const std::weak_ptr<char>*>(data)).get();
        case MetaTypeUtils::unique_ptr:
            return reinterpret_cast<const std::unique_ptr<char>*>(data)->get();
        default:
            break;
        }
    }
    return data;
}

TreeRangeData<>::TreeRangeData(void* container, QSharedPointer<AbstractSequentialAccess>&& containerAccess, std::shared_ptr<int>&& _treeColumnCount)
    : m_container(container),
    m_treeColumnCount(std::move(_treeColumnCount)),
    m_containerAccess(std::move(containerAccess)),
    m_elementDataType(MetaTypeUtils::dataType(m_containerAccess->elementMetaType())),
    m_elementNestedContainerAccess(m_containerAccess->elementNestedContainerAccess(), &containerDisposer) {
}

TreeRangeData<>::TreeRangeData(JNIEnv*, jobject, const TreeRangeData<>& other)
    : m_container(nullptr),
    m_treeColumnCount(other.m_treeColumnCount),
    m_containerAccess(other.m_containerAccess),
    m_elementDataType(other.m_elementDataType),
    m_elementNestedContainerAccess(other.m_elementNestedContainerAccess)
{
}

TreeRangeData<>::TreeRangeData()
    : m_container(nullptr),
    m_treeColumnCount(),
    m_containerAccess(),
    m_elementDataType(MetaTypeUtils::Value),
    m_elementNestedContainerAccess() {
}

TreeRangeData<>::TreeRangeData(TreeRangeData&& other)
    : m_container(std::move(other.m_container)),
    m_treeColumnCount(std::move(other.m_treeColumnCount)),
    m_containerAccess(std::move(other.m_containerAccess)),
    m_elementDataType(std::move(other.m_elementDataType)),
    m_elementNestedContainerAccess(std::move(other.m_elementNestedContainerAccess)) {
}

TreeRangeData<>::~TreeRangeData(){
}

void* TreeRangeData<>::container() const{
    return m_container;
}

const QMetaType& TreeRangeData<>::elementMetaType() const{
    return m_containerAccess->elementMetaType();
}

MetaTypeUtils::DataType TreeRangeData<>::elementDataType() const{
    return m_elementDataType;
}

const QSharedPointer<AbstractContainerAccess>& TreeRangeData<>::elementNestedContainerAccess() const {
    return m_elementNestedContainerAccess;
}

const QSharedPointer<AbstractSequentialAccess>& TreeRangeData<>::containerAccess() const {
    return m_containerAccess;
}

bool TreeRangeData<>::isInitialized() const{
    return m_treeColumnCount ? true : false;
}

void TreeRangeData<>::initialize(JNIEnv*, const TreeRangeData<>& other){
    m_treeColumnCount = other.m_treeColumnCount;
    m_containerAccess = other.m_containerAccess;
    m_elementDataType = other.m_elementDataType;
    m_elementNestedContainerAccess = other.m_elementNestedContainerAccess;
}

int TreeRangeData<>::treeColumnCount() const{
    Q_ASSERT(m_treeColumnCount);
    return !m_treeColumnCount ? 0 : *m_treeColumnCount;
}

TreeRangeData<ClassInfo>::TreeRangeData(void* container, QSharedPointer<AbstractSequentialAccess>&& containerAccess, std::shared_ptr<int>&& _treeColumnCount, ClassInfo&& _classInfo)
    : TreeRangeData<>(container, std::move(containerAccess), std::move(_treeColumnCount)),
    m_classInfo(std::move(_classInfo)), m_rowObject() {
}

TreeRangeData<ClassInfo>::TreeRangeData(JNIEnv* env, jobject row, const TreeRangeData<ClassInfo>& other)
    : TreeRangeData<>(env, row, other),
    m_classInfo(other.m_classInfo), m_rowObject(env, row) {
}

TreeRangeData<ClassInfo>::TreeRangeData()
    : TreeRangeData<>(),
    m_classInfo() {
}

TreeRangeData<ClassInfo>::TreeRangeData(TreeRangeData&& other)
    : TreeRangeData<>(std::move(other)),
    m_classInfo(std::move(other.m_classInfo)) {
}

void TreeRangeData<ClassInfo>::initialize(JNIEnv* env, const TreeRangeData<ClassInfo>& other){
    TreeRangeData<>::initialize(env, other);
    m_classInfo = other.m_classInfo;
    m_rowObject = {env, env->NewObject(m_classInfo.javaClass, m_classInfo.defaultConstructor)};
}

jobject TreeRangeData<ClassInfo>::rowObject(JNIEnv* env) const{
    return m_rowObject.object(env);
}

jobject TreeRangeData<ClassInfo>::rowObject(JNIEnv* env){
    jobject result = m_rowObject.object(env);
    if(!result){
        result = env->NewObject(m_classInfo.javaClass, m_classInfo.defaultConstructor);
        m_rowObject = {env, result};
    }
    return result;
}

template<>
MetaPropertyIterator MutableMetaObjectRow<MutableRow>::begin(){
    return MetaPropertyIterator{this->pointer(), metaType};
}

template<>
MetaPropertyIterator MutableMetaObjectRow<MutableRow>::end(){
    return MetaPropertyIterator{this->pointer(), metaType, metaType.metaObject()->propertyCount()};
}

template<>
ConstMetaPropertyIterator MetaObjectRow<MutableRow,true>::begin() const{
    return ConstMetaPropertyIterator{this->pointer(), metaType};
}

template<>
ConstMetaPropertyIterator MetaObjectRow<MutableRow,true>::end() const{
    return ConstMetaPropertyIterator{this->pointer(), metaType, metaType.metaObject()->propertyCount()};
}

template<>
ConstMetaPropertyIterator MetaObjectRow<MutableRow,false>::begin() const{
    return ConstMetaPropertyIterator{this->pointer(), metaType};
}

template<>
ConstMetaPropertyIterator MetaObjectRow<MutableRow,false>::end() const{
    return ConstMetaPropertyIterator{this->pointer(), metaType, metaType.metaObject()->propertyCount()};
}

template<>
ConstMetaPropertyIterator MetaObjectRow<ConstRow,false>::begin() const{
    return ConstMetaPropertyIterator{this->pointer(), metaType};
}

template<>
ConstMetaPropertyIterator MetaObjectRow<ConstRow,false>::end() const{
    return ConstMetaPropertyIterator{this->pointer(), metaType, metaType.metaObject()->propertyCount()};
}

template<>
int MetaObjectRow<MutableRow,true>::size() const{
    return metaType.metaObject()->propertyCount();
}

template<>
int MetaObjectRow<MutableRow,false>::size() const{
    return metaType.metaObject()->propertyCount();
}

template<>
int MetaObjectRow<ConstRow,false>::size() const{
    return metaType.metaObject()->propertyCount();
}

template<>
auto ConstRangeRow<ConstRow>::begin() const -> const_iterator{
    if(auto access = dynamic_cast<AbstractSequentialAccess*>(&*containerAccess)){
        return const_iterator{access->constElementIterator(pointer())};
    }else if(auto access = dynamic_cast<AbstractAssociativeAccess*>(&*containerAccess)){
        return const_iterator{AbstractAssociativeAccess::asValueIterator(access->constKeyValueIterator(pointer()))};
    }else if(auto access = dynamic_cast<AbstractPairAccess*>(&*containerAccess)){
        return const_iterator{access->constElementIterator(pointer())};
    }else{
        return const_iterator{nullptr};
    }
}

template<>
auto ConstRangeRow<ConstRow>::end() const -> const_iterator{
    return const_iterator{nullptr};
}

template<>
auto ConstRangeRow<MutableRow>::begin() const -> const_iterator{
    if(auto access = dynamic_cast<AbstractSequentialAccess*>(&*containerAccess)){
        return const_iterator{access->constElementIterator(pointer())};
    }else if(auto access = dynamic_cast<AbstractAssociativeAccess*>(&*containerAccess)){
        return const_iterator{AbstractAssociativeAccess::asValueIterator(access->constKeyValueIterator(pointer()))};
    }else if(auto access = dynamic_cast<AbstractPairAccess*>(&*containerAccess)){
        return const_iterator{access->constElementIterator(pointer())};
    }else{
        return const_iterator{nullptr};
    }
}

template<>
auto ConstRangeRow<MutableRow>::end() const -> const_iterator{
    return const_iterator{nullptr};
}

template<>
auto MutableRangeRow<ConstRow>::begin() -> iterator{
    if(auto access = dynamic_cast<AbstractSequentialAccess*>(&*containerAccess)){
        return iterator{access->elementIterator(pointer())};
    }else if(auto access = dynamic_cast<AbstractAssociativeAccess*>(&*containerAccess)){
        return iterator{AbstractAssociativeAccess::asValueIterator(access->keyValueIterator(pointer()))};
    }else if(auto access = dynamic_cast<AbstractPairAccess*>(&*containerAccess)){
        return iterator{access->elementIterator(pointer())};
    }else{
        return iterator{nullptr};
    }
}

template<>
auto MutableRangeRow<ConstRow>::end() -> iterator{
    return iterator{nullptr};
}

template<>
auto MutableRangeRow<MutableRow>::begin() -> iterator{
    if(auto access = dynamic_cast<AbstractSequentialAccess*>(&*containerAccess)){
        return iterator{access->elementIterator(pointer())};
    }else if(auto access = dynamic_cast<AbstractAssociativeAccess*>(&*containerAccess)){
        return iterator{AbstractAssociativeAccess::asValueIterator(access->keyValueIterator(pointer()))};
    }else if(auto access = dynamic_cast<AbstractPairAccess*>(&*containerAccess)){
        return iterator{access->elementIterator(pointer())};
    }else{
        return iterator{nullptr};
    }
}

template<>
auto MutableRangeRow<MutableRow>::end() -> iterator{
    return iterator{nullptr};
}

AbstractIterator::AbstractIterator(std::unique_ptr<AbstractSequentialAccess::ElementIterator>&& _iter)
    : current_data(nullptr), current_metaType(), current_dataType(MetaTypeUtils::Value), current_containerAccess(), iter(std::move(_iter)) {
    if(iter){
        if(!iter->isConst()){
            operator++();
        }else{
            iter.reset();
        }
    }
}

AbstractIterator::AbstractIterator(const AbstractIterator& other)
    : current_data(other.current_data),
    current_metaType(other.current_metaType),
    current_dataType(other.current_dataType),
    current_containerAccess(other.current_containerAccess),
    iter(other.iter ? other.iter->clone() : nullptr){
}

AbstractIterator::AbstractIterator(AbstractIterator&& other)
    : current_data(std::move(other.current_data)),
    current_metaType(std::move(other.current_metaType)),
    current_dataType(std::move(other.current_dataType)),
    current_containerAccess(std::move(other.current_containerAccess)),
    iter(std::move(other.iter)){
}

AbstractIterator& AbstractIterator::operator=(const AbstractIterator& other){
    current_metaType = other.current_metaType;
    current_dataType = other.current_dataType;
    current_containerAccess = other.current_containerAccess;
    current_data = other.current_data;
    iter = other.iter ? other.iter->clone() : std::unique_ptr<AbstractSequentialAccess::ElementIterator>{nullptr};
    return *this;
}

void AbstractIterator::swap(AbstractIterator& other) noexcept {
    std::swap(current_metaType, other.current_metaType);
    std::swap(current_dataType, other.current_dataType);
    std::swap(current_containerAccess, other.current_containerAccess);
    std::swap(current_data, other.current_data);
    std::swap(iter, other.iter);
}

void swap(AbstractIterator& _this, AbstractIterator& other) noexcept{
    _this.swap(other);
}

void qSwap(AbstractIterator& _this, AbstractIterator& other) noexcept{
    _this.swap(other);
}

AbstractIterator& AbstractIterator::operator=(AbstractIterator&& other){
    current_metaType = std::move(other.current_metaType);
    current_dataType = std::move(other.current_dataType);
    current_containerAccess = std::move(other.current_containerAccess);
    current_data = std::move(other.current_data);
    iter = std::move(other.iter);
    return *this;
}

bool AbstractIterator::operator==(const AbstractIterator& other) const{
    if(!other.iter.get()){
        return !iter.get();
    }else if(!iter.get()){
        return false;
    }else{
        return *iter==*other.iter;
    }
}

bool AbstractIterator::operator!=(const AbstractIterator& other) const{
    return !operator==(other);
}

AbstractIterator& AbstractIterator::operator++(){
    if(iter){
        if(iter->hasNext()){
            QMetaType nextType = iter->elementMetaType();
            if(nextType!=current_metaType){
                current_dataType = MetaTypeUtils::dataType(nextType);
            }
            current_metaType = std::move(nextType);
            current_containerAccess = QSharedPointer<AbstractContainerAccess>(iter->elementNestedContainerAccess(), &containerDisposer);
            current_data = iter->mutableNext();
        }else{
            current_data = nullptr;
            iter.reset();
        }
    }
    return *this;
}

AbstractIterator AbstractIterator::operator++(int){
    AbstractIterator copy(*this);
    operator++();
    return copy;
}

bool AbstractIterator::isConst() const {
    return false;
}

bool AbstractIterator::operator==(const AbstractConstIterator& other) const{
    if(!other.iter.get()){
        return !iter.get();
    }else if(!iter.get()){
        return false;
    }else{
        return *iter==*other.iter;
    }
}
bool AbstractIterator::operator!=(const AbstractConstIterator& other) const{
    return !operator==(other);
}

AbstractConstIterator::AbstractConstIterator(std::unique_ptr<AbstractSequentialAccess::ElementIterator>&& _iter)
    : current_data(nullptr), current_metaType(), current_dataType(MetaTypeUtils::Value), current_containerAccess(), iter(std::move(_iter)) {
    operator++();
}

AbstractConstIterator::AbstractConstIterator(const AbstractConstIterator& other)
    : current_data(other.current_data),
    current_metaType(other.current_metaType),
    current_dataType(other.current_dataType),
    current_containerAccess(other.current_containerAccess), iter(other.iter ? other.iter->clone() : nullptr){
}

AbstractConstIterator::AbstractConstIterator(const AbstractIterator& other)
    : current_data(other.current_data),
    current_metaType(other.current_metaType),
    current_dataType(other.current_dataType),
    current_containerAccess(other.current_containerAccess), iter(other.iter ? other.iter->clone() : nullptr){
}

AbstractConstIterator::AbstractConstIterator(AbstractConstIterator&& other)
    : current_data(std::move(other.current_data)),
    current_metaType(std::move(other.current_metaType)),
    current_dataType(std::move(other.current_dataType)),
    current_containerAccess(std::move(other.current_containerAccess)), iter(std::move(other.iter)){
}

AbstractConstIterator::AbstractConstIterator(AbstractIterator&& other)
    : current_data(std::move(other.current_data)),
    current_metaType(std::move(other.current_metaType)),
    current_dataType(std::move(other.current_dataType)),
    current_containerAccess(std::move(other.current_containerAccess)), iter(std::move(other.iter)){
}

AbstractConstIterator& AbstractConstIterator::operator=(const AbstractConstIterator& other){
    current_metaType = other.current_metaType;
    current_dataType = other.current_dataType;
    current_containerAccess = other.current_containerAccess;
    current_data = other.current_data;
    iter = other.iter ? other.iter->clone() : std::unique_ptr<AbstractSequentialAccess::ElementIterator>{nullptr};
    return *this;
}

AbstractConstIterator& AbstractConstIterator::operator=(const AbstractIterator& other){
    current_metaType = other.current_metaType;
    current_dataType = other.current_dataType;
    current_containerAccess = other.current_containerAccess;
    current_data = other.current_data;
    iter = other.iter ? other.iter->clone() : std::unique_ptr<AbstractSequentialAccess::ElementIterator>{nullptr};
    return *this;
}

void AbstractConstIterator::swap(AbstractConstIterator& other) noexcept{
    std::swap(current_metaType, other.current_metaType);
    std::swap(current_dataType, other.current_dataType);
    std::swap(current_containerAccess, other.current_containerAccess);
    std::swap(current_data, other.current_data);
    std::swap(iter, other.iter);
}

void swap(AbstractConstIterator& _this, AbstractConstIterator& other) noexcept{
    _this.swap(other);
}

void qSwap(AbstractConstIterator& _this, AbstractConstIterator& other) noexcept{
    _this.swap(other);
}

AbstractConstIterator& AbstractConstIterator::operator=(AbstractConstIterator&& other){
    current_metaType = std::move(other.current_metaType);
    current_dataType = std::move(other.current_dataType);
    current_containerAccess = std::move(other.current_containerAccess);
    current_data = std::move(other.current_data);
    iter = std::move(other.iter);
    return *this;
}

AbstractConstIterator& AbstractConstIterator::operator=(AbstractIterator&& other){
    current_metaType = std::move(other.current_metaType);
    current_dataType = std::move(other.current_dataType);
    current_containerAccess = std::move(other.current_containerAccess);
    current_data = std::move(other.current_data);
    iter = std::move(other.iter);
    return *this;
}

bool AbstractConstIterator::operator==(const AbstractConstIterator& other) const{
    if(!other.iter.get()){
        return !iter.get();
    }else if(!iter.get()){
        return false;
    }else{
        return *iter==*other.iter;
    }
}

bool AbstractConstIterator::operator==(const AbstractIterator& other) const{
    if(!other.iter.get()){
        return !iter.get();
    }else if(!iter.get()){
        return false;
    }else{
        return *iter==*other.iter;
    }
}

bool AbstractConstIterator::operator!=(const AbstractConstIterator& other) const{
    return !operator==(other);
}

bool AbstractConstIterator::isConst() const {
    return true;
}

AbstractConstIterator& AbstractConstIterator::operator++(){
    if(iter){
        if(iter->hasNext()){
            QMetaType nextType = iter->elementMetaType();
            if(nextType!=current_metaType){
                current_dataType = MetaTypeUtils::dataType(nextType);
            }
            current_metaType = std::move(nextType);
            current_containerAccess = QSharedPointer<AbstractContainerAccess>(iter->elementNestedContainerAccess(), &containerDisposer);
            current_data = iter->constNext();
        }else{
            current_data = nullptr;
            iter.reset();
        }
    }
    return *this;
}

AbstractConstIterator AbstractConstIterator::operator++(int){
    AbstractConstIterator copy(*this);
    operator++();
    return copy;
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
