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

#include "qtjambiapi.h"
#include <QtCore/QReadWriteLock>
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include <QtCore/QAssociativeIterable>
#include "containeraccess_p.h"
#include "functionpointer.h"
#include "registryutil_p.h"
#include "java_p.h"
#include "utils_p.h"
#include "qtjambilink_p.h"
#include "java_p.h"
#include "coreapi.h"

QT_WARNING_DISABLE_GCC("-Winaccessible-base")
QT_WARNING_DISABLE_CLANG("-Winaccessible-base")

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#else
QReadWriteLock* containerAccessLock();
QMap<int, QtMetaContainerPrivate::QMetaAssociationInterface>& metaAssociationHash();

QSharedPointer<AutoMapAccess> getMapAccess(const QtPrivate::QMetaTypeInterface *iface){
    return findContainerAccess(iface->typeId.loadAcquire()).dynamicCast<AutoMapAccess>();
}
void AutoMapAccess::defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr){
    if(QSharedPointer<class AutoMapAccess> access = getMapAccess(iface)){
        access->constructContainer(ptr);
    }
}
void AutoMapAccess::copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other){
    if(QSharedPointer<class AutoMapAccess> access = getMapAccess(iface)){
        access->constructContainer(ptr, other);
    }
}
void AutoMapAccess::moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other){
    if(QSharedPointer<class AutoMapAccess> access = getMapAccess(iface)){
        access->constructContainer(ptr, other);
    }
}
void AutoMapAccess::dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr){
    if(QSharedPointer<class AutoMapAccess> access = getMapAccess(iface)){
        access->destructContainer(ptr);
    }
}
bool AutoMapAccess::equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2){
    if(QSharedPointer<class AutoMapAccess> access = getMapAccess(iface)){
        return access->equal(ptr1, ptr2);
    }
    return false;
}
void AutoMapAccess::debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr){
    if(QSharedPointer<class AutoMapAccess> access = getMapAccess(iface)){
        access->debugStream(s, ptr);
    }
}
void AutoMapAccess::dataStreamOutFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr){
    if(QSharedPointer<class AutoMapAccess> access = getMapAccess(iface)){
        access->dataStreamOut(s, ptr);
    }
}
void AutoMapAccess::dataStreamInFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr){
    if(QSharedPointer<class AutoMapAccess> access = getMapAccess(iface)){
        access->dataStreamIn(s, ptr);
    }
}

void* AutoMapAccess::constructContainer(JNIEnv*, void* result, const ContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

void* AutoMapAccess::constructContainer(void* result, void* container) {
    const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container);
    return new(result) MapDataPointer(std::move(d));
}

AutoMapAccess::iterator AutoMapAccess::begin(const void* container) {
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
        return iterator(begin(*d), m_offset1, m_offset2);
    }else{
        return {};
    }
}

AutoMapAccess::iterator AutoMapAccess::end(const void* container) {
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
        return iterator(end(*d), m_offset1, m_offset2);
    }else{
        return {};
    }
}

void AutoMapAccess::dataStreamOut(QDataStream &s, const void *ptr){
    QtPrivate::writeAssociativeContainer(s, ConstContainer{ptr, this});
}

void AutoMapAccess::dataStreamIn(QDataStream &s, void *ptr){
    Container container(ptr, this);
    QtPrivate::readAssociativeContainer(s, container);
}

void AutoMapAccess::debugStream(QDebug &dbg, const void *ptr){
    QtPrivate::printAssociativeContainer(dbg, "QMap", ConstContainer{ptr, this});
}

void* AutoMapAccess::TreeNode::data(qsizetype offset){
    return reinterpret_cast<char*>(this)+offset;
}

const void* AutoMapAccess::TreeNode::data(qsizetype offset) const{
    return reinterpret_cast<const char*>(this)+offset;
}

AutoMapAccess::extended_iterator::extended_iterator(const void* _container, const QSharedPointer<class AutoMapAccess>& _access, AutoMapAccess::TreeNode* _node) : node_iterator(_node), container(_container), access(_access) {
}

AutoMapAccess::extended_iterator::extended_iterator(const void* _container, const QSharedPointer<class AutoMapAccess>& _access, const node_iterator& other) : node_iterator(other), container(_container), access(_access) {
}

AutoMapAccess::node_iterator::node_iterator(AutoMapAccess::TreeNode* _node) : node(_node) {
}

AutoMapAccess::node_iterator::node_iterator(const node_iterator& other) : node(other.node) {
}

AutoMapAccess::TreeNode* AutoMapAccess::node_iterator::operator->() const{
    return node;
}

AutoMapAccess::TreeNode& AutoMapAccess::node_iterator::operator*() const{
    Q_ASSERT(node);
    return *node;
}

AutoMapAccess::node_iterator& AutoMapAccess::node_iterator::operator++(){
    if(node)
        node = next(node);
    return *this;
}
AutoMapAccess::node_iterator AutoMapAccess::node_iterator::operator++(int){
    node_iterator iter(*this);
    ++*this;
    return iter;
}

AutoMapAccess::node_iterator& AutoMapAccess::node_iterator::operator--(){
    if(node)
        node = prev(node);
    return *this;
}
AutoMapAccess::node_iterator AutoMapAccess::node_iterator::operator--(int){
    node_iterator iter(*this);
    ++*this;
    return iter;
}

AutoMapAccess::iterator::iterator() {}
AutoMapAccess::iterator::iterator(node_iterator iter, size_t offset1, size_t offset2)
    : m_iter(iter), m_offset1(offset1), m_offset2(offset2){}

AutoMapAccess::iterator& AutoMapAccess::iterator::operator++(){
    ++m_iter;
    return *this;
}
AutoMapAccess::iterator AutoMapAccess::iterator::operator++(int){
    iterator _this = *this;
    ++m_iter;
    return _this;
}
AutoMapAccess::iterator& AutoMapAccess::iterator::operator--(){
    --m_iter;
    return *this;
}
AutoMapAccess::iterator AutoMapAccess::iterator::operator--(int){
    iterator _this = *this;
    --m_iter;
    return _this;
}
bool AutoMapAccess::iterator::operator==(const iterator& right) const{
    return m_iter==right.m_iter;
}
void* AutoMapAccess::iterator::key()const{
    return m_iter->data(m_offset1);
}
void* AutoMapAccess::iterator::value()const{
    return m_iter->data(m_offset2);
}


bool AutoMapAccess::node_iterator::operator==(const node_iterator& right) const{
    if(node==nullptr){
        if(right.node==nullptr
#if defined(Q_CC_MSVC)
            || !right.node->isNil
#endif
            ){
            return true;
        }
    }
    if(right.node==nullptr){
        if(node==nullptr
#if defined(Q_CC_MSVC)
            || !right.node->isNil
#endif
            ){
            return true;
        }
    }
    return node==right.node;
}
bool AutoMapAccess::node_iterator::operator!=(const node_iterator& right) const{
    return !operator==(right.node);
}

AutoMapAccess::TreeNode* AutoMapAccess::node_iterator::next(TreeNode* node){
#if defined(Q_CC_MSVC)
    if (node->right->isNil) {
        TreeNode* rnode{nullptr};
        while (!(rnode = node->parent)->isNil && node == rnode->right) {
            node = rnode;
        }
        node = rnode;
    } else {
        node = AutoMapAccess::treeMin(node->right);
    }
    return node;
#elif defined(_LIBCPP_VERSION)
    return reinterpret_cast<TreeNode*>(std::__tree_next_iter<std::__tree_node_base<void*>*>(reinterpret_cast<std::__tree_node_base<void*>*>(node)));
#elif defined(__GLIBCXX__)
    return reinterpret_cast<TreeNode*>(std::_Rb_tree_increment(reinterpret_cast<std::_Rb_tree_node_base*>(node)));
#endif
}

AutoMapAccess::TreeNode* AutoMapAccess::node_iterator::prev(TreeNode* node){
#if defined(Q_CC_MSVC)
    if (node->isNil) {
        node = node->right;
    } else if (node->left->isNil) {
        TreeNode* lnode{nullptr};
        while (!(lnode = node->parent)->isNil && node == lnode->left) {
            node = lnode;
        }

        if (!node->isNil) {
            node = lnode;
        }
    } else {
        node = AutoMapAccess::treeMax(node->left);
    }
    return node;
#elif defined(_LIBCPP_VERSION)
    return reinterpret_cast<TreeNode*>(std::__tree_prev_iter<std::__tree_node_base<void*>*>(reinterpret_cast<std::__tree_node_base<void*>*>(node)));
#elif defined(__GLIBCXX__)
    return reinterpret_cast<TreeNode*>(std::_Rb_tree_decrement(reinterpret_cast<std::_Rb_tree_node_base*>(node)));
#endif
}

std::pair<const QtPrivate::QMetaTypeInterface *,const QtPrivate::QMetaTypeInterface *> AutoMapAccess::metaTypes(){
    return {m_keyMetaType.iface(), m_valueMetaType.iface()};
}

qsizetype AutoMapAccess::erase(MapData& data, const void* key){
    return erase(data, eqrange(data, key));
}

qsizetype AutoMapAccess::erase(MapData& data, const void* key, const void* value){
    node_iterator i = find(data, key);
    node_iterator end1 = end(data);
    while(i != end1){
        if(m_valueMetaType.compare(value, i->data(m_offset2))==0){
            erase(data, i);
            return 1 + erase(data, key, value);
        }
        ++i;
        if(m_keyMetaType.compare(key, i->data(m_offset1))!=0)
            break;
    }
    return 0;
}

qsizetype AutoMapAccess::erase(MapData& data, QPair<TreeNode*,TreeNode*> pair){
    const node_iterator first = pair.first;
    const node_iterator last = pair.second;
    const auto count = static_cast<qsizetype>(std::distance(first, last));
    erase(data, first, last);
    return count;
}

void AutoMapAccess::destroy(TreeNode* node){
#if defined(Q_CC_MSVC)
    if(!node->isNil)
#endif
    {
        void* value = node->data(m_offset2);
        m_valueMetaType.destruct(value);
        void* key = node->data(m_offset1);
        m_keyMetaType.destruct(key);
    }
    operator delete(node);
}

AutoMapAccess::TreeNode* AutoMapAccess::extract(MapData& data, node_iterator where){
    TreeNode* erasednode = where.data(); // node to erase
#if defined(Q_CC_MSVC)
    ++where; // save successor iterator for return

    TreeNode* fixnode{nullptr}; // the node to recolor as needed
    TreeNode* fixnodeparent{nullptr}; // parent of _Fixnode (which may be nil)
    TreeNode* pnode = erasednode;

    if (pnode->left->isNil) {
        fixnode = pnode->right; // stitch up right subtree
    } else if (pnode->right->isNil) {
        fixnode = pnode->left; // stitch up left subtree
    } else { // two subtrees, must lift successor node to replace erased
        pnode   = &*where; // _Pnode is successor node
        fixnode = pnode->right; // _Fixnode is only subtree
    }

    if (pnode == erasednode) { // at most one subtree, relink it
        fixnodeparent = erasednode->parent;
        if (!fixnode->isNil) {
            fixnode->parent = fixnodeparent; // link up
        }

        if (data.head->parent == erasednode) {
            data.head->parent = fixnode; // link down from root
        } else if (fixnodeparent->left == erasednode) {
            fixnodeparent->left = fixnode; // link down to left
        } else {
            fixnodeparent->right = fixnode; // link down to right
        }

        if (data.head->left == erasednode) {
            data.head->left = fixnode->isNil ? fixnodeparent // smallest is parent of erased node
                                             : treeMin(fixnode); // smallest in relinked subtree
        }

        if (data.head->right == erasednode) {
            data.head->right = fixnode->isNil ? fixnodeparent // largest is parent of erased node
                                              : treeMax(fixnode); // largest in relinked subtree
        }
    } else { // erased has two subtrees, _Pnode is successor to erased
        erasednode->left->parent = pnode; // link left up
        pnode->left               = erasednode->left; // link successor down

        if (pnode == erasednode->right) {
            fixnodeparent = pnode; // successor is next to erased
        } else { // successor further down, link in place of erased
            fixnodeparent = pnode->parent; // parent is successor's
            if (!fixnode->isNil) {
                fixnode->parent = fixnodeparent; // link fix up
            }

            fixnodeparent->left        = fixnode; // link fix down
            pnode->right               = erasednode->right; // link next down
            erasednode->right->parent = pnode; // right up
        }

        if (data.head->parent == erasednode) {
            data.head->parent = pnode; // link down from root
        } else if (erasednode->parent->left == erasednode) {
            erasednode->parent->left = pnode; // link down to left
        } else {
            erasednode->parent->right = pnode; // link down to right
        }

        pnode->parent = erasednode->parent; // link successor up
        std::swap(pnode->color, erasednode->color); // recolor it
    }

    if (erasednode->color == Black) { // erasing black link, must recolor/rebalance tree
        for (; fixnode != data.head->parent && fixnode->color == Black; fixnodeparent = fixnode->parent) {
            if (fixnode == fixnodeparent->left) { // fixup left subtree
                pnode = fixnodeparent->right;
                if (pnode->color == Red) { // rotate red up from right subtree
                    pnode->color         = Black;
                    fixnodeparent->color = Red;
                    rotateLeft(data, fixnodeparent);
                    pnode = fixnodeparent->right;
                }

                if (pnode->isNil) {
                    fixnode = fixnodeparent; // shouldn't happen
                } else if (pnode->left->color == Black
                           && pnode->right->color == Black) { // redden right subtree with black children
                    pnode->color = Red;
                    fixnode       = fixnodeparent;
                } else { // must rearrange right subtree
                    if (pnode->right->color == Black) { // rotate red up from left sub-subtree
                        pnode->left->color = Black;
                        pnode->color        = Red;
                        rotateRight(data, pnode);
                        pnode = fixnodeparent->right;
                    }

                    pnode->color         = fixnodeparent->color;
                    fixnodeparent->color = Black;
                    pnode->right->color = Black;
                    rotateLeft(data, fixnodeparent);
                    break; // tree now recolored/rebalanced
                }
            } else { // fixup right subtree
                pnode = fixnodeparent->left;
                if (pnode->color == Red) { // rotate red up from left subtree
                    pnode->color         = Black;
                    fixnodeparent->color = Red;
                    rotateRight(data, fixnodeparent);
                    pnode = fixnodeparent->left;
                }

                if (pnode->isNil) {
                    fixnode = fixnodeparent; // shouldn't happen
                } else if (pnode->right->color == Black
                           && pnode->left->color == Black) { // redden left subtree with black children
                    pnode->color = Red;
                    fixnode       = fixnodeparent;
                } else { // must rearrange left subtree
                    if (pnode->left->color == Black) { // rotate red up from right sub-subtree
                        pnode->right->color = Black;
                        pnode->color         = Red;
                        rotateLeft(data, pnode);
                        pnode = fixnodeparent->left;
                    }

                    pnode->color         = fixnodeparent->color;
                    fixnodeparent->color = Black;
                    pnode->left->color  = Black;
                    rotateRight(data, fixnodeparent);
                    break; // tree now recolored/rebalanced
                }
            }
        }

        fixnode->color = Black; // stopping node is black
    }
#elif defined(_LIBCPP_VERSION)
    node_iterator __r(where);
    ++__r;
    if (data.begin == where.data())
        data.begin = __r.data();
    --data.size;
    std::__tree_remove(reinterpret_cast<std::__tree_node_base<void*>*>(data.header.left),
                         reinterpret_cast<std::__tree_node_base<void*>*>(where.data()));
    erasednode = where.data();
#elif defined(__GLIBCXX__)
    TreeNode* header = &data.header;
    erasednode = reinterpret_cast<TreeNode*>(std::_Rb_tree_rebalance_for_erase(reinterpret_cast<std::_Rb_tree_node_base*>(erasednode), *reinterpret_cast<std::_Rb_tree_node_base*>(header)));
#endif

    if (0 < data.size) {
        --data.size;
    }

    return erasednode;
}

void AutoMapAccess::eraseTree(MapData& data, TreeNode* node){
#if defined(Q_CC_MSVC)
    while (node && !node->isNil) {
#else
    while (node && node->color==Black) {
#endif
        eraseTree(data, node->right);
        destroy(std::exchange(node, node->left));
    }
}

#if defined(Q_CC_MSVC)
AutoMapAccess::TreeNode* AutoMapAccess::treeMax(TreeNode* node) {
    while (!node->right->isNil) {
        node = node->right;
    }

    return node;
}

AutoMapAccess::TreeNode* AutoMapAccess::treeMin(TreeNode* node) {
    while (!node->left->isNil) {
        node = node->left;
    }
    return node;
}

void AutoMapAccess::rotateLeft(MapData& data, TreeNode* node){
    TreeNode* pnode    = node->right;
    node->right = pnode->left;

    if (!pnode->left->isNil) {
        pnode->left->parent = node;
    }

    pnode->parent = node->parent;

    if (node == data.head->parent) {
        data.head->parent = pnode;
    } else if (node == node->parent->left) {
        node->parent->left = pnode;
    } else {
        node->parent->right = pnode;
    }

    pnode->left       = node;
    node->parent = pnode;
}

void AutoMapAccess::rotateRight(MapData& data, TreeNode* node){
    TreeNode* pnode   = node->left;
    node->left = pnode->right;

    if (!pnode->right->isNil) {
        pnode->right->parent = node;
    }

    pnode->parent = node->parent;

    if (node == data.head->parent) {
        data.head->parent = pnode;
    } else if (node == node->parent->right) {
        node->parent->right = pnode;
    } else {
        node->parent->left = pnode;
    }

    pnode->right      = node;
    node->parent = pnode;
}

void AutoMapAccess::orphanPtr(MapData& data, const TreeNode* node) {
#if _ITERATOR_DEBUG_LEVEL == 2
    std::_Lockit _Lock(_LOCK_DEBUG);
    struct const_iterator : std::_Iterator_base12{
        const TreeNode* _Ptr;
    };
    QSet<std::_Iterator_base12*> visited;
    std::_Iterator_base12** pnext = &data._Myproxy->_Myfirstiter;
    while (*pnext && !visited.contains(*pnext)) {
        visited.insert(*pnext);
        const TreeNode* pnextptr = reinterpret_cast<const_iterator&>(**pnext)._Ptr;
        if (pnextptr == data.head || (node != nullptr && pnextptr != node)) {
            pnext = &(*pnext)->_Mynextiter;
        } else { // orphan the iterator
            (*pnext)->_Myproxy = nullptr;
            *pnext             = (*pnext)->_Mynextiter;
        }
    }
#else // ^^^ _ITERATOR_DEBUG_LEVEL == 2 / _ITERATOR_DEBUG_LEVEL != 2 vvv
    (void) data;
    (void) node;
#endif // ^^^ _ITERATOR_DEBUG_LEVEL != 2 ^^^
}

#elif defined(_LIBCPP_VERSION)
AutoMapAccess::TreeNode*& AutoMapAccess::findLeafHigh(MapData& data, TreeEndNode*& parent, const void* key){
    TreeNode* __nd = data.header.left;
    if (__nd != nullptr)
    {
        while (true)
        {
            if (m_keyMetaType.compare(key, __nd->data(m_offset1))<0)
            {
                if (__nd->left != nullptr)
                    __nd = static_cast<TreeNode*>(__nd->left);
                else
                {
                    parent = static_cast<TreeEndNode*>(__nd);
                    return parent->left;
                }
            }
            else
            {
                if (__nd->right != nullptr)
                    __nd = static_cast<TreeNode*>(__nd->right);
                else
                {
                    parent = static_cast<TreeEndNode*>(__nd);
                    return __nd->right;
                }
            }
        }
    }
    parent = static_cast<TreeEndNode*>(&data.header);
    return parent->left;
}

AutoMapAccess::TreeNode*& AutoMapAccess::findLeafLow(MapData& data, TreeEndNode*& parent, const void* key){
    TreeNode* __nd = data.header.left;
    if (__nd != nullptr)
    {
        while (true)
        {
            if (m_keyMetaType.compare(__nd->data(m_offset1), key)<0)
            {
                if (__nd->right != nullptr)
                    __nd = static_cast<TreeNode*>(__nd->right);
                else
                {
                    parent = static_cast<TreeEndNode*>(__nd);
                    return __nd->right;
                }
            }
            else
            {
                if (__nd->left != nullptr)
                    __nd = static_cast<TreeNode*>(__nd->left);
                else
                {
                    parent = static_cast<TreeEndNode*>(__nd);
                    return parent->left;
                }
            }
        }
    }
    parent = static_cast<TreeEndNode*>(&data.header);
    return parent->left;
}

AutoMapAccess::TreeNode*& AutoMapAccess::findLeaf(MapData& data, node_iterator node, TreeEndNode*& parent, const void* key){
    if(!node.data())
        return findLeafHigh(data, parent, key);
    if (node == end(data) || !(m_keyMetaType.compare(node->data(m_offset1), key)<0))  // check before
    {
        // key <= *node
        node_iterator __prior = node;
        if (__prior == begin(data) || !(m_keyMetaType.compare(key, (--__prior)->data(m_offset1))<0))
        {
            // *prev(node) < key < *node
            if (node->left == nullptr)
            {
                parent = node.data();
                return parent->left;
            }
            else
            {
                parent = __prior.data();
                return __prior->right;
            }
        }
        // key <= *prev(node)
        return findLeafHigh(data, parent, key);
    }
    return findLeafLow(data, parent, key);
}

AutoMapAccess::TreeNode*& AutoMapAccess::findEqual(MapData& data, TreeEndNode*& parent, const void* key){
    TreeNode* __nd = data.header.left;
    TreeNode** __nd_ptr = &data.header.left;
    if (__nd != nullptr)
    {
        while (true)
        {
            if (m_keyMetaType.compare(key, __nd->data(m_offset1))<0)
            {
                if (__nd->left != nullptr) {
                    __nd_ptr = &__nd->left;
                    __nd = static_cast<TreeNode*>(__nd->left);
                } else {
                    parent = static_cast<TreeEndNode*>(__nd);
                    return parent->left;
                }
            }
            else if (m_keyMetaType.compare(__nd->data(m_offset1), key)<0)
            {
                if (__nd->right != nullptr) {
                    __nd_ptr = std::addressof(__nd->right);
                    __nd = static_cast<TreeNode*>(__nd->right);
                } else {
                    parent = static_cast<TreeEndNode*>(__nd);
                    return __nd->right;
                }
            }
            else
            {
                parent = static_cast<TreeEndNode*>(__nd);
                return *__nd_ptr;
            }
        }
    }
    parent = static_cast<TreeEndNode*>(&data.header);
    return parent->left;
}

AutoMapAccess::TreeNode*& AutoMapAccess::findEqualHint(MapData& data, node_iterator node, TreeEndNode*& parent, TreeNode*& dummy, const void* key){
    if(!node.data())
        return findEqual(data, parent, key);
    if (node == end(data) || m_keyMetaType.compare(key, node->data(m_offset1))<0)  // check before
    {
        // key < *node
        node_iterator __prior = node;
        if (__prior == begin(data) || m_keyMetaType.compare((--__prior)->data(m_offset1), key)<0)
        {
            // *prev(node) < key < *node
            if (node->left == nullptr)
            {
                parent = node.data();
                return parent->left;
            }
            else
            {
                parent = __prior.data();
                return __prior->right;
            }
        }
        // key <= *prev(node)
        return findEqual(data, parent, key);
    }
    else if (m_keyMetaType.compare(node->data(m_offset1), key)<0)  // check after
    {
        // *node < key
        node_iterator __next = std::next(node);
        if (__next == data.end() || m_keyMetaType.compare(key, __next->data(m_offset1))<0)
        {
            // *node < key < *std::next(node)
            if (node.data()->right == nullptr)
            {
                parent = node.data();
                return node.data()->right;
            }
            else
            {
                parent = node.data();
                return node.data()->left;
            }
        }
        // *next(node) <= key
        return findEqual(data, parent, key);
    }
    // else key == *node
    parent = node.data();
    dummy = node.data();
    return dummy;
}

#elif defined(__GLIBCXX__)

std::pair<AutoMapAccess::TreeNode*, AutoMapAccess::TreeNode*> AutoMapAccess::getInsertUniquePos(MapData& data, const void* key){
    typedef std::pair<TreeNode*, TreeNode*> Res;
    TreeNode* node = data.header.left;
    TreeNode* node2 = &data.header;
    bool isLess = true;
    while (node && node!=&data.header)
    {
        node2 = node;
        isLess = m_keyMetaType.compare(key, node->data(m_offset1))<0;
        node = isLess ? node->left : node->right;
    }
    node_iterator bgn = begin(data);
    node_iterator iter = node_iterator(node2);
    if (isLess)
    {
        if (iter == bgn)
            return Res(node, node2);
        else
            --iter;
    }
    if (m_keyMetaType.compare(iter->data(m_offset1), key)<0)
        return Res(node, node2);
    return Res(iter.data(), 0);
}

std::pair<AutoMapAccess::TreeNode*, AutoMapAccess::TreeNode*> AutoMapAccess::getInsertHintUniquePos(MapData& data, TreeNode* node, const void* key){
    typedef std::pair<TreeNode*,TreeNode*> Res;

    // end()
    if (node == &data.header)
    {
        if (data.size > 0
//            && data.header.right
//            && data.header.right->color==Black
            && m_keyMetaType.compare(data.header.right->data(m_offset1), key)<0)
            return Res(0, data.header.right);
        else
            return getInsertUniquePos(data, key);
    }
    else if (m_keyMetaType.compare(key, node->data(m_offset1))<0)
    {
        // First, try before...
        node_iterator before = node;
        if (node == data.header.left) // begin()
            return Res(data.header.left, data.header.left);
        else if (m_keyMetaType.compare((--before)->data(m_offset1), key)<0)
        {
            if (!before->right)
                return Res(nullptr, before.data());
            else
                return Res(node, node);
        }
        else
            return getInsertUniquePos(data, key);
    }
    else if (m_keyMetaType.compare(node->data(m_offset1), key)<0)
    {
        // ... then try after.
        node_iterator after = node;
        if (node == data.header.right)
            return Res(0, data.header.right);
        else if (m_keyMetaType.compare(key, (++after)->data(m_offset1))<0)
        {
            if (!node->right)
                return Res(0, node);
            else
                return Res(after.data(), after.data());
        }
        else
            return getInsertUniquePos(data, key);
    }
    else{
        // Equivalent keys.
        return Res(node, 0);
    }
}


std::pair<AutoMapAccess::TreeNode*, AutoMapAccess::TreeNode*> AutoMapAccess::getInsertEqualPos(MapData& data, const void* key){
    typedef std::pair<TreeNode*,TreeNode*> Res;
    TreeNode* node(data.header.left);
    TreeNode* node2(&data.header);
    while (node && node!=&data.header){
        node2 = node;
        node = m_keyMetaType.compare(key, node->data(m_offset1))<0 ?
                   node->left : node->right;
    }
    return Res(node, node2);
}

std::pair<AutoMapAccess::TreeNode*, AutoMapAccess::TreeNode*> AutoMapAccess::getInsertHintEqualPos(MapData& data, TreeNode* node, const void* key){
    typedef std::pair<TreeNode*,TreeNode*> Res;
    // end()
    if (node == &data.header)
    {
        if (data.size > 0
            && !(m_keyMetaType.compare(key, data.header.right->data(m_offset1))<0))
            return Res(nullptr, data.header.right);
        else
            return getInsertEqualPos(data, key);
    }
    else if (!(m_keyMetaType.compare(node->data(m_offset1), key)<0))
    {
        // First, try before...
        node_iterator before(node);
        if (node == data.header.left) // begin()
            return Res(data.header.left, data.header.left);
        else if (!(m_keyMetaType.compare(key, (--before)->data(m_offset1))<0))
        {
            if (!before->right)
                return Res(nullptr, before.data());
            else
                return Res(node, node);
        }
        else
            return getInsertEqualPos(data, key);
    }
    else
    {
        // ... then try after.
        node_iterator after(node);
        if (node == data.header.right)
            return Res(nullptr, data.header.right);
        else if (!(m_keyMetaType.compare((++after)->data(m_offset1), key)<0))
        {
            if (!node->right)
                return Res(nullptr, node);
            else
                return Res(after.data(), after.data());
        }
        else
            return Res(nullptr, nullptr);
    }
}

#endif

AutoMapAccess::TreeNode* AutoMapAccess::erase(MapData& data, node_iterator first, node_iterator last){
    const node_iterator _begin = begin(data);
    if (first == _begin
#if defined(Q_CC_MSVC)
        && last->isNil
#endif
        ) {
        // erase all
        clear(data);
        return &*last;
    }

    // partial erase, one at a time
    while (first != last) {
        erase(data, first++);
    }
    return &*last;
}

AutoMapAccess::node_iterator AutoMapAccess::erase(MapData& data, node_iterator iter){
    node_iterator successor = iter;
    ++successor;
    TreeNode* _Erasednode = extract(data, iter); // node to erase
#if defined(Q_CC_MSVC)
    orphanPtr(data, _Erasednode);
#endif
    destroy(_Erasednode);
    return successor;
}

QPair<AutoMapAccess::TreeNode*,AutoMapAccess::TreeNode*> AutoMapAccess::eqrange(const MapData& data, const void* key){
#if defined(Q_CC_MSVC)
    TreeNode* pnode   = data.head->parent;
    TreeNode* lonode  = data.head;
    TreeNode* hinode  = data.head;

    while (!pnode->isNil) {
        const void* nodekey = pnode->data(m_offset1);
        if (m_keyMetaType.compare(nodekey, key)<0) {
            pnode = pnode->right;
        } else {
            if (hinode->isNil && m_keyMetaType.compare(key, nodekey)>0) {
                hinode = pnode;
            }

            lonode = pnode;
            pnode  = pnode->left;
        }
    }

    pnode = hinode->isNil ? data.head->parent : hinode->left; // continue scan for upper bound
    while (!pnode->isNil) {
        const void* nodekey = pnode->data(m_offset1);
        auto cmp = m_keyMetaType.compare(key, nodekey);
        if (cmp<0) {
            hinode = pnode;
            pnode  = pnode->left;
        } else {
            pnode = pnode->right;
        }
    }

    return {lonode, hinode};
#elif defined(_LIBCPP_VERSION)
    TreeNode* __result = reinterpret_cast<TreeNode*>(&data.header);
    TreeNode* __rt = data.header.left;
    while (__rt != nullptr)
    {
        const void* nodekey = __rt->data(m_offset1);
        auto cmp = m_keyMetaType.compare(key, nodekey);
        if (cmp<0)
        {
            __result = __rt;
            __rt = __rt->left;
        }
        else if (cmp>0)
            __rt = __rt->right;
        else{
            node_iterator next(__rt);
            node_iterator _end(end(data));
            do{
                ++next;
                if(_end==next)
                    break;
                nodekey = next->data(m_offset1);
            }while(m_keyMetaType.compare(nodekey, key)==0);

            node_iterator bgn(begin(data));
            node_iterator prev(__rt);
            do{
                __rt = prev.data();
                if(bgn==prev)
                    break;
                --prev;
                nodekey = prev->data(m_offset1);
            }while(m_keyMetaType.compare(nodekey, key)==0);

            return {__rt, next.data()};
        }
    }
    return {__result, __result};
#elif defined(__GLIBCXX__)
    TreeNode* pnode = data.header.parent;
    TreeNode* lonode = &data.header;
    while (pnode)
    {
        const void* nodekey = pnode->data(m_offset1);
        auto cmp = m_keyMetaType.compare(nodekey, key);
        if (cmp<0) {
            pnode = pnode->right;
        } else if (cmp>0) {
            lonode = pnode;
            pnode  = pnode->left;
        } else {
            node_iterator next(pnode);
            node_iterator end(&data.header);
            do{
                ++next;
                if(end==next)
                    break;
                nodekey = next->data(m_offset1);
            }while(m_keyMetaType.compare(nodekey, key)==0);

            node_iterator begin(data.header.parent);
            node_iterator prev(pnode);
            do{
                pnode = prev.data();
                if(begin==prev)
                    break;
                --prev;
                nodekey = prev->data(m_offset1);
            }while(m_keyMetaType.compare(nodekey, key)==0);

            return {pnode, next.data()};
        }
    }
    return {lonode, lonode};
#endif
}

AutoMapAccess::node_iterator AutoMapAccess::find(const MapData& data, const void* key){
    TreeNode* found = findLowerBound(data, key);
    if (found
#if defined(Q_CC_MSVC)
            && !found->isNil
#else
            && found!=end(data).data()
#endif
        && m_keyMetaType.compare(found->data(m_offset1), key)==0) {
        return found;
    }
    return end(data);
}

#if defined(Q_CC_MSVC)
AutoMapAccess::TreeFindResult AutoMapAccess::findLowerBound(const MapData& data, const void* key){
    TreeFindResult result{{data.head->parent, TreeChild::Right}, data.head};
    TreeNode* trynode = data.head->parent;
    while (trynode && !trynode->isNil) {
        result.location.parent = trynode;
        const void* trykey = trynode->data(m_offset1);
        auto cmp = m_keyMetaType.compare(trykey, key);
        if (cmp<0) {
            result.location.child = TreeChild::Right;
            trynode = trynode->right;
        } else{
            result.location.child = TreeChild::Left;
            result.bound = trynode;
            trynode = trynode->left;
        }
    }
    return result;
}

AutoMapAccess::TreeNode* AutoMapAccess::findUpperBound(const MapData& data, const void* key){
    TreeFindResult result{{data.head->parent, TreeChild::Right}, data.head};
    TreeNode* trynode = data.head->parent;
    while (trynode && !trynode->isNil) {
        result.location.parent = trynode;
        const void* trykey = trynode->data(m_offset1);
        auto cmp = m_keyMetaType.compare(key, trykey);
        if (cmp<0) {
            result.location.child = TreeChild::Left;
            result.bound = trynode;
            trynode = trynode->left;
        } else{
            result.location.child = TreeChild::Right;
            trynode = trynode->right;
        }
    }
    return result;
}

AutoMapAccess::TreeNode* AutoMapAccess::insertNode(MapData& data, Location location, TreeNode* newNode){
    ++data.size;
    const auto head  = data.head;
    newNode->parent = location.parent;
    if (location.parent == head) { // first node in tree, just set head values
        head->left     = newNode;
        head->parent   = newNode;
        head->right    = newNode;
        newNode->color = Black; // the root is black
        return newNode;
    }

    if (location.child == TreeChild::Right) { // add to right of loc.parent
        location.parent->right = newNode;
        if (location.parent == head->right) { // remember rightmost node
            head->right = newNode;
        }
    } else { // add to left of loc.parent
        location.parent->left = newNode;
        if (location.parent == head->left) { // remember leftmost node
            head->left = newNode;
        }
    }

    for (TreeNode* pnode = newNode; pnode->parent->color == Red;) {
        if (pnode->parent == pnode->parent->parent->left) { // fixup red-red in left subtree
            const auto parent_sibling = pnode->parent->parent->right;
            if (parent_sibling->color == Red) { // parent's sibling has two red children, blacken both
                pnode->parent->color          = Black;
                parent_sibling->color          = Black;
                pnode->parent->parent->color = Red;
                pnode                           = pnode->parent->parent;
            } else { // parent's sibling has red and black children
                if (pnode == pnode->parent->right) { // rotate right child to left
                    pnode = pnode->parent;
                    rotateLeft(data, pnode);
                }

                pnode->parent->color          = Black; // propagate red up
                pnode->parent->parent->color = Red;
                rotateRight(data, pnode->parent->parent);
            }
        } else { // fixup red-red in right subtree
            const auto parent_sibling = pnode->parent->parent->left;
            if (parent_sibling->color == Red) { // parent's sibling has two red children, blacken both
                pnode->parent->color          = Black;
                parent_sibling->color          = Black;
                pnode->parent->parent->color = Red;
                pnode                           = pnode->parent->parent;
            } else { // parent's sibling has red and black children
                if (pnode == pnode->parent->left) { // rotate left child to right
                    pnode = pnode->parent;
                    rotateRight(data, pnode);
                }

                pnode->parent->color          = Black; // propagate red up
                pnode->parent->parent->color = Red;
                rotateLeft(data, pnode->parent->parent);
            }
        }
    }

    head->parent->color = Black; // root is always black
    return newNode;
}
#else
AutoMapAccess::TreeNode* AutoMapAccess::findLowerBound(const MapData& data, const void* key){
#if defined(_LIBCPP_VERSION)
    TreeNode* __result = reinterpret_cast<AutoMapAccess::TreeNode*>(&data.header);
    TreeNode* __root = data.header.left;
#elif defined(__GLIBCXX__)
    TreeNode* __result = &data.header;
    TreeNode* __root = data.header.parent;
#endif
    while (__root && __root!=&data.header)
    {
        const void* trykey = __root->data(m_offset1);
        auto cmp = m_keyMetaType.compare(trykey, key);
        if(cmp<0){
            __root = __root->right;
        }else{
            __result = __root;
            __root = __root->left;
        }
    }
    return __result;
}

AutoMapAccess::TreeNode* AutoMapAccess::findUpperBound(const MapData& data, const void* key){
#if defined(_LIBCPP_VERSION)
    TreeNode* __result = reinterpret_cast<AutoMapAccess::TreeNode*>(&data.header);
    TreeNode* __root = data.header.left;
#elif defined(__GLIBCXX__)
    TreeNode* __result = &data.header;
    TreeNode* __root = data.header.parent;
#endif
    while (__root && __root!=&data.header){
        const void* trykey = __root->data(m_offset1);
        auto cmp = m_keyMetaType.compare(key, trykey);
        if (cmp<0) {
            __result = __root;
            __root = __root->left;
        } else{
            __root = __root->right;
        }
    }
    return __result;
}
#endif

void AutoMapAccess::copyNode(QList<TreeNode*>& nextNodes, QHash<TreeNode*,TreeNode*> &nodesMap, TreeNode* node){
    if(node && !nodesMap.contains(node)){
        TreeNode* newNode = new(operator new(m_size)) TreeNode();
#if defined(Q_CC_MSVC)
        if(!node->isNil){
#endif
            m_keyMetaType.construct(newNode->data(m_offset1), node->data(m_offset1));
            m_valueMetaType.construct(newNode->data(m_offset2), node->data(m_offset2));
#if defined(Q_CC_MSVC)
        }
        newNode->isNil = node->isNil;
#endif
        newNode->color = node->color;
        nodesMap[node] = newNode;
        nextNodes << node->left;
#if !defined(_LIBCPP_VERSION)
        nextNodes << node->parent;
#endif
        nextNodes << node->right;
    }
};

void AutoMapAccess::insert(void* container, const void* key, const void* value){
    insertOrAssign(detach(*reinterpret_cast<MapDataPointer*>(container)), key, value);
}

AutoMapAccess::MapData& AutoMapAccess::detach(MapDataPointer& d){
    if(!d){
        MapData* mapData = createMapData();
        d.reset(mapData);
    } else if (d->ref.loadRelaxed() != 1) {
        QList<TreeNode*> nextNodes;
        QHash<TreeNode*,TreeNode*> nodesMap;
        MapData* mapData = createMapData();
        mapData->size = d->size;
#if defined(Q_CC_MSVC)
        nodesMap[d->head] = mapData->head;
        nextNodes << d->head->left;
        nextNodes << d->head->parent;
        nextNodes << d->head->right;
#elif defined(_LIBCPP_VERSION)
        nodesMap[reinterpret_cast<TreeNode*>(&d->header)] = reinterpret_cast<TreeNode*>(&mapData->header);
        nextNodes << d->header.left;
        nextNodes << d->begin;
#elif defined(__GLIBCXX__)
        nodesMap[&d->header] = &mapData->header;
        nextNodes << d->header.left;
        nextNodes << d->header.parent;
        nextNodes << d->header.right;
#endif
        while(!nextNodes.isEmpty()){
            QList<TreeNode*> copy;
            nextNodes.swap(copy);
            while(!copy.isEmpty()){
                copyNode(nextNodes, nodesMap, copy.takeLast());
            }
        }
        QHash<TreeNode*,TreeNode*> nodesMap2 = nodesMap;
#if defined(_LIBCPP_VERSION)
        nodesMap2.take(reinterpret_cast<TreeNode*>(&d->header));
#elif defined(__GLIBCXX__)
        nodesMap2.take(&d->header);
#endif
        nodesMap2.take(nullptr);
        for(auto iter = nodesMap2.constKeyValueBegin(); iter!=nodesMap2.constKeyValueEnd(); ++iter){
            iter->second->left = nodesMap[iter->first->left];
#if defined(_LIBCPP_VERSION)
            iter->second->parent = nodesMap[reinterpret_cast<TreeNode*>(iter->first->parent)];
#elif defined(Q_CC_MSVC) || defined(__GLIBCXX__)
            iter->second->parent = nodesMap[iter->first->parent];
#endif
            iter->second->right = nodesMap[iter->first->right];
        }
#if defined(Q_CC_MSVC)
#elif defined(_LIBCPP_VERSION)
        mapData->begin = nodesMap[d->begin];
        mapData->header.left = nodesMap[d->header.left];
#elif defined(__GLIBCXX__)
        mapData->header.left = nodesMap[d->header.left];
        mapData->header.parent = nodesMap[d->header.parent];
        mapData->header.right = nodesMap[d->header.right];
#endif
        d.reset(mapData);
    }
    return *d;
}

AutoMapAccess::MapData* AutoMapAccess::createMapData(){
    MapData *newData = new MapData;
#if defined(Q_CC_MSVC)
#if _ITERATOR_DEBUG_LEVEL == 2
    std::allocator<std::_Container_proxy> _Alproxy;
    std::_Container_proxy_ptr<std::allocator<std::_Container_proxy>> _Proxy(_Alproxy, *newData);
#endif
    TreeNode* node = reinterpret_cast<TreeNode*>(operator new(m_size));
    node->left = node;
    node->parent = node;
    node->right = node;
    node->color = Black;
    node->isNil = true;
    newData->head = node;
#if _ITERATOR_DEBUG_LEVEL == 2
    _Proxy._Release();
#endif
#elif defined(_LIBCPP_VERSION)
    newData->begin = reinterpret_cast<TreeNode*>(&newData->header);
#elif defined(__GLIBCXX__)
    newData->header.left = &newData->header;
    newData->header.parent = nullptr;
    newData->header.right = &newData->header;
    newData->header.color = Red;
#endif
    return newData;
}

#if defined(_LIBCPP_VERSION)
AutoMapAccess::TreeNode* AutoMapAccess::MapData::end(){
    return reinterpret_cast<AutoMapAccess::TreeNode*>(&header);
}
#endif

AutoMapAccess::node_iterator AutoMapAccess::end(const MapData& data){
#if defined(Q_CC_MSVC)
    return data.head;
#elif defined(_LIBCPP_VERSION)
    return reinterpret_cast<AutoMapAccess::TreeNode*>(&data.header);
#elif defined(__GLIBCXX__)
    return &data.header;
#endif
}

AutoMapAccess::node_iterator AutoMapAccess::begin(const MapData& data){
#if defined(Q_CC_MSVC)
    return data.head->left;
#elif defined(_LIBCPP_VERSION)
    return data.begin;
#elif defined(__GLIBCXX__)
    return data.header.left;
#endif
}

void AutoMapAccess::insertOrAssign(MapData& data, const void* key, const void* value){
#if defined(Q_CC_MSVC)
    TreeFindResult loc = findLowerBound(data, key);
    if (!isMulti() && loc.bound && !loc.bound->isNil && m_keyMetaType.compare(loc.bound->data(m_offset1), key)==0) {
        m_valueMetaType.destruct(loc.bound->data(m_offset2));
        m_valueMetaType.construct(loc.bound->data(m_offset2), value);
    }else{
        TreeNode* newNode = new(operator new(m_size)) TreeNode();
        newNode->left = data.head;
        newNode->parent = data.head;
        newNode->right = data.head;
        newNode->color = Red;
        newNode->isNil = false;
        m_keyMetaType.construct(newNode->data(m_offset1), key);
        m_valueMetaType.construct(newNode->data(m_offset2), value);
        insertNode(data, loc.location, newNode);
    }
#else
    TreeNode* lowerBound = findLowerBound(data, key);
    if (!isMulti() && lowerBound && lowerBound!=end(data).data() && m_keyMetaType.compare(lowerBound->data(m_offset1), key)==0) {
        m_valueMetaType.destruct(lowerBound->data(m_offset2));
        m_valueMetaType.construct(lowerBound->data(m_offset2), value);
    }else{
#if defined(_LIBCPP_VERSION)
        TreeEndNode* __parent{nullptr};
        TreeNode* __dummy{nullptr};
        TreeNode*& __child = isMulti() ? findLeaf(data, lowerBound, __parent, key) : findEqualHint(data, lowerBound, __parent, __dummy, key);
        if(__child==nullptr || isMulti()){
            TreeNode* newNode = new(operator new(m_size)) TreeNode();
            newNode->left = nullptr;
            newNode->right = nullptr;
            newNode->parent = __parent;
            m_keyMetaType.construct(newNode->data(m_offset1), key);
            m_valueMetaType.construct(newNode->data(m_offset2), value);
            __child = newNode;
            if (data.begin->left != nullptr)
                data.begin = data.begin->left;
            std::__tree_balance_after_insert(reinterpret_cast<std::__tree_node_base<void*>*>(data.header.left), reinterpret_cast<std::__tree_node_base<void*>*>(__child));
            ++data.size;
        }
#elif defined(__GLIBCXX__)
        TreeNode* newNode = new(operator new(m_size)) TreeNode();
        newNode->left = nullptr;
        newNode->parent = nullptr;
        newNode->right = nullptr;
        m_keyMetaType.construct(newNode->data(m_offset1), key);
        m_valueMetaType.construct(newNode->data(m_offset2), value);
        std::pair<TreeNode*,TreeNode*> res = isMulti() ? getInsertHintEqualPos(data, lowerBound, key) : getInsertHintUniquePos(data, lowerBound, key);
        ++data.size;
        TreeNode* header = &data.header;
        bool insert_left = (res.first || res.second == header
                            || m_keyMetaType.compare(res.second->data(m_offset1), key)>=0);
        std::_Rb_tree_insert_and_rebalance(insert_left,
                                           reinterpret_cast<std::_Rb_tree_node_base*>(newNode),
                                           reinterpret_cast<std::_Rb_tree_node_base*>(res.second),
                                           *reinterpret_cast<std::_Rb_tree_node_base*>(header));
#endif
    }
#endif
}

void AutoMapAccess::insertOrAssign(MapData& data, const void* key, JNIEnv *env, jobject value){
#if defined(Q_CC_MSVC)
    TreeFindResult loc = findLowerBound(data, key);
    if (!isMulti() && loc.bound && !loc.bound->isNil && m_keyMetaType.compare(loc.bound->data(m_offset1), key)==0) {
        jvalue jv;
        jv.l = value;
        void* target = loc.bound->data(m_offset2);
        m_valueExternalToInternalConverter(env, nullptr, jv, target, jValueType::l);
    }else{
        TreeNode* newNode = new(operator new(m_size)) TreeNode();
        newNode->left = data.head;
        newNode->parent = data.head;
        newNode->right = data.head;
        newNode->color = Red;
        newNode->isNil = false;
        m_keyMetaType.construct(newNode->data(m_offset1), key);
        void* target = newNode->data(m_offset2);
        m_valueMetaType.construct(target);
        jvalue jv;
        jv.l = value;
        m_valueExternalToInternalConverter(env, nullptr, jv, target, jValueType::l);
        insertNode(data, loc.location, newNode);
    }
#else
    TreeNode* lowerBound = findLowerBound(data, key);
    if (!isMulti() && lowerBound && lowerBound!=end(data).data() && m_keyMetaType.compare(lowerBound->data(m_offset1), key)==0) {
        m_valueMetaType.destruct(lowerBound->data(m_offset2));
        m_valueMetaType.construct(lowerBound->data(m_offset2), value);
    }else{
#if defined(_LIBCPP_VERSION)
        TreeEndNode* __parent{nullptr};
        TreeNode* __dummy{nullptr};
        TreeNode*& __child = isMulti() ? findLeaf(data, lowerBound, __parent, key) : findEqualHint(data, lowerBound, __parent, __dummy, key);
        if(__child==nullptr || isMulti()){
            TreeNode* newNode = new(operator new(m_size)) TreeNode();
            newNode->left = nullptr;
            newNode->right = nullptr;
            newNode->parent = __parent;
            m_keyMetaType.construct(newNode->data(m_offset1), key);
            void* target = newNode->data(m_offset2);
            m_valueMetaType.construct(target);
            jvalue jv;
            jv.l = value;
            m_valueExternalToInternalConverter(env, nullptr, jv, target, jValueType::l);
            __child = newNode;
            if (data.begin->left != nullptr)
                data.begin = data.begin->left;
            std::__tree_balance_after_insert(reinterpret_cast<std::__tree_node_base<void*>*>(data.header.left), reinterpret_cast<std::__tree_node_base<void*>*>(__child));
            ++data.size;
        }
#elif defined(__GLIBCXX__)
        TreeNode* newNode = new(operator new(m_size)) TreeNode();
        newNode->left = nullptr;
        newNode->parent = nullptr;
        newNode->right = nullptr;
        m_keyMetaType.construct(newNode->data(m_offset1), key);
        void* target = newNode->data(m_offset2);
        m_valueMetaType.construct(target);
        jvalue jv;
        jv.l = value;
        m_valueExternalToInternalConverter(env, nullptr, jv, target, jValueType::l);
        std::pair<TreeNode*,TreeNode*> res = isMulti() ? getInsertHintEqualPos(data, lowerBound, key) : getInsertHintUniquePos(data, lowerBound, key);
        ++data.size;
        TreeNode* header = &data.header;
        bool insert_left = (res.first || res.second == header
                            || m_keyMetaType.compare(res.second->data(m_offset1), key)>=0);
        std::_Rb_tree_insert_and_rebalance(insert_left,
                                           reinterpret_cast<std::_Rb_tree_node_base*>(newNode),
                                           reinterpret_cast<std::_Rb_tree_node_base*>(res.second),
                                           *reinterpret_cast<std::_Rb_tree_node_base*>(header));
#endif
    }
#endif
}

jint AutoMapAccess::copyIfNotEquivalentTo(MapData& data, const MapData& copyFrom, const void* key){
    jint result = 0;
    node_iterator iter(begin(copyFrom));
    node_iterator cend(end(copyFrom));
    while(iter != cend){
        const void* theirKey = iter->data(m_offset1);
        if (m_keyMetaType.compare(key, theirKey)==0) {
            ++result;
        }else{
            const void* theirValue = iter->data(m_offset2);
            insertOrAssign(data, theirKey, theirValue);
        }
        ++iter;
    }
    return result;
}

jint AutoMapAccess::copyIfNotEquivalentTo(MapData& data, const MapData& copyFrom, const void* key, const void* value){
    jint result = 0;
    node_iterator iter(begin(copyFrom));
    node_iterator cend(end(copyFrom));
    while(iter != cend){
        const void* theirKey = iter->data(m_offset1);
        const void* theirValue = iter->data(m_offset2);
        if (m_keyMetaType.compare(key, theirKey)==0 && m_valueMetaType.compare(value, theirValue)==0) {
            ++result;
        }else{
            const void* theirValue = iter->data(m_offset2);
            insertOrAssign(data, theirKey, theirValue);
        }
        ++iter;
    }
    return result;
}

void AutoMapAccess::clear(MapData& data){
#if defined(Q_CC_MSVC)
    orphanPtr(data, nullptr);
    auto _Head = data.head;
    eraseTree(data, _Head->parent);
    _Head->parent  = _Head;
    _Head->left    = _Head;
    _Head->right   = _Head;
#elif defined(_LIBCPP_VERSION)
    eraseTree(data, data.header.left);
    data.begin = reinterpret_cast<TreeNode*>(&data.header);
    data.header.left = nullptr;
#elif defined(__GLIBCXX__)
    eraseTree(data, data.header.parent);
    data.header.parent = nullptr;
    data.header.left = &data.header;
    data.header.right = &data.header;
#endif
    data.size = 0;
}
#endif // QT_VERSION

AutoMapAccess::AutoMapAccess(const AutoMapAccess & other)
    :AbstractMapAccess(), AbstractNestedAssociativeAccess(),
      m_keyMetaType(other.m_keyMetaType
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                       .id()),
      m_keyAlign(other.m_keyAlign
#endif
                                  ),
      m_keyHashFunction(other.m_keyHashFunction),
      m_keyInternalToExternalConverter(other.m_keyInternalToExternalConverter),
      m_keyExternalToInternalConverter(other.m_keyExternalToInternalConverter),
      m_keyNestedContainerAccess(other.m_keyNestedContainerAccess),
      m_valueMetaType(other.m_valueMetaType
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                          .id()),
      m_valueAlign(other.m_valueAlign
#endif
                                     ),
      m_valueHashFunction(other.m_valueHashFunction),
      m_valueInternalToExternalConverter(other.m_valueInternalToExternalConverter),
      m_valueExternalToInternalConverter(other.m_valueExternalToInternalConverter),
      m_valueNestedContainerAccess(other.m_valueNestedContainerAccess),
      m_align(other.m_align),
      m_offset1(other.m_offset1),
      m_offset2(other.m_offset2),
      m_size(other.m_size),
      m_keyOwnerFunction(other.m_keyOwnerFunction),
      m_valueOwnerFunction(other.m_valueOwnerFunction),
      m_keyDataType(other.m_keyDataType),
      m_valueDataType(other.m_valueDataType)
{
}

AutoMapAccess::~AutoMapAccess(){}

std::unique_ptr<AbstractMapAccess::KeyValueIterator> AutoMapAccess::keyValueIterator(const void* container) {
    class KeyValueIterator : public AbstractMapAccess::KeyValueIterator{
        AutoMapAccess* access;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        Node* current;
        Node* end;
    public:
        KeyValueIterator(AutoMapAccess* _access, QMapDataBase *const* map)
            :access(_access), current(beginNode(map)), end(endNode(map)){}
#else
        node_iterator current;
        node_iterator end;
    public:
        KeyValueIterator(AutoMapAccess* _access, const MapDataPointer& d)
            :access(_access), current(d ? _access->begin(*d) : node_iterator{}), end(d? _access->end(*d) : node_iterator{}){}
#endif
        bool hasNext() override{
            return current!=end;
        }
        QPair<jobject,jobject> next(JNIEnv * env) override{
            jvalue k;
            k.l = nullptr;
            jvalue v;
            v.l = nullptr;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            access->m_keyInternalToExternalConverter(env, nullptr, reinterpret_cast<char*>(current)+access->m_offset1, k, true);
            access->m_valueInternalToExternalConverter(env, nullptr, reinterpret_cast<char*>(current)+access->m_offset2, v, true);
            current = current->nextNode();
#else
            access->m_keyInternalToExternalConverter(env, nullptr, current->data(access->m_offset1), k, true);
            access->m_valueInternalToExternalConverter(env, nullptr, current->data(access->m_offset2), v, true);
            ++current;
#endif
            return {k.l, v.l};
        }
        QPair<const void*,const void*> next() override {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            void* key = reinterpret_cast<char*>(current)+access->m_offset1;
            void* value = reinterpret_cast<char*>(current)+access->m_offset2;
            current = current->nextNode();
#else
            void* key = current->data(access->m_offset1);
            void* value = current->data(access->m_offset2);
            ++current;
#endif
            if(access->m_keyDataType & AbstractContainerAccess::PointersMask){
                key = *reinterpret_cast<void**>(key);
            }
            if(access->m_valueDataType & AbstractContainerAccess::PointersMask){
                value = *reinterpret_cast<void**>(value);
            }
            return {key, value};
        }
    };
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return std::unique_ptr<AbstractMapAccess::KeyValueIterator>(new KeyValueIterator(this, reinterpret_cast<QMapDataBase *const*>(container)));
#else
    return std::unique_ptr<AbstractMapAccess::KeyValueIterator>(new KeyValueIterator(this, *reinterpret_cast<const MapDataPointer*>(container)));
#endif
}

AutoMapAccess::AutoMapAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                int keyMetaType,
                size_t keyAlign,
#else
                const QMetaType& keyMetaType,
#endif
                const QtJambiUtils::QHashFunction& keyHashFunction,
                const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
                const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
                const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
                PtrOwnerFunction keyOwnerFunction,
                AbstractContainerAccess::DataType keyDataType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                int valueMetaType,
                size_t valueAlign,
#else
                const QMetaType& valueMetaType,
#endif
                const QtJambiUtils::QHashFunction& valueHashFunction,
                const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
                const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
                const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess,
                PtrOwnerFunction valueOwnerFunction,
                AbstractContainerAccess::DataType valueDataType
        )
    : AbstractMapAccess(), AbstractNestedAssociativeAccess(),
      m_keyMetaType(keyMetaType),
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
      m_keyAlign(keyAlign),
#endif
      m_keyHashFunction(keyHashFunction),
      m_keyInternalToExternalConverter(keyInternalToExternalConverter),
      m_keyExternalToInternalConverter(keyExternalToInternalConverter),
      m_keyNestedContainerAccess(keyNestedContainerAccess),
      m_valueMetaType(valueMetaType),
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
      m_valueAlign(valueAlign),
#endif
      m_valueHashFunction(valueHashFunction),
      m_valueInternalToExternalConverter(valueInternalToExternalConverter),
      m_valueExternalToInternalConverter(valueExternalToInternalConverter),
      m_valueNestedContainerAccess(valueNestedContainerAccess),
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
      m_align(qMax(keyAlign, valueAlign)),
#else
      m_align(qMax(m_keyMetaType.alignOf(), m_valueMetaType.alignOf())),
#endif
      m_offset1(0),
      m_offset2(0),
      m_size(0),
      m_keyOwnerFunction(keyOwnerFunction),
      m_valueOwnerFunction(valueOwnerFunction),
      m_keyDataType(keyDataType),
      m_valueDataType(valueDataType)
{
    Q_ASSERT(m_keyInternalToExternalConverter);
    Q_ASSERT(m_keyExternalToInternalConverter);
    Q_ASSERT(m_valueInternalToExternalConverter);
    Q_ASSERT(m_valueExternalToInternalConverter);
    Q_ASSERT(m_keyMetaType.id()!=QMetaType::UnknownType
            && m_keyMetaType.id()!=QMetaType::Void);
    Q_ASSERT(m_valueMetaType.id()!=QMetaType::UnknownType
            && m_valueMetaType.id()!=QMetaType::Void);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    m_offset1 = sizeof(QMapNodeBase);
    if(m_offset1%m_keyAlign>0)
        m_offset1 += m_keyAlign-m_offset1%m_keyAlign;
    m_offset2 = m_offset1 + m_keyMetaType.sizeOf();
    if(m_offset2%m_valueAlign>0)
        m_offset2 += m_valueAlign-m_offset2%m_valueAlign;
    m_size = m_offset2 + m_valueMetaType.sizeOf();
    if(m_size%m_align>0)
        m_size += m_align-m_size%m_align;
#else
#if defined(Q_CC_MSVC)
    m_offset1 = qsizetype(&static_cast<TreeNode*>(0)->isNil) + sizeof(decltype(std::declval<TreeNode>().isNil));
#elif defined(_LIBCPP_VERSION)
    m_offset1 = qsizetype(&static_cast<TreeNode*>(0)->color) + sizeof(decltype(std::declval<TreeNode>().color));
#elif defined(__GLIBCXX__)
    m_offset1 = qsizetype(&static_cast<TreeNode*>(0)->right) + sizeof(decltype(std::declval<TreeNode>().right));
#endif
    auto pairAlign = qMax(m_valueMetaType.alignOf(), m_keyMetaType.alignOf());
    if(m_offset1%pairAlign>0)
        m_offset1 += pairAlign-m_offset1%pairAlign;
    m_offset2 = m_offset1 + m_keyMetaType.sizeOf();
    if(m_offset2%m_valueMetaType.alignOf()>0)
        m_offset2 += m_valueMetaType.alignOf()-m_offset2%m_valueMetaType.alignOf();
    m_size = m_offset2 + m_valueMetaType.sizeOf();
#endif
}

void AutoMapAccess::dispose(){ delete this; }
AbstractMapAccess* AutoMapAccess::clone(){ return new AutoMapAccess(*this); }

const QMetaType& AutoMapAccess::keyMetaType(){
    return m_keyMetaType;
}

const QMetaType& AutoMapAccess::valueMetaType(){
    return m_valueMetaType;
}

AbstractContainerAccess::DataType AutoMapAccess::keyType() {return m_keyDataType;}
AbstractContainerAccess::DataType AutoMapAccess::valueType() {return m_valueDataType;}

AbstractContainerAccess* AutoMapAccess::keyNestedContainerAccess() {
    return m_keyNestedContainerAccess ? m_keyNestedContainerAccess->clone() : nullptr;
}

AbstractContainerAccess* AutoMapAccess::valueNestedContainerAccess() {
    return m_valueNestedContainerAccess ? m_valueNestedContainerAccess->clone() : nullptr;
}

const QSharedPointer<AbstractContainerAccess>& AutoMapAccess::sharedKeyNestedContainerAccess(){
    return m_keyNestedContainerAccess;
}
const QSharedPointer<AbstractContainerAccess>& AutoMapAccess::sharedValueNestedContainerAccess(){
    return m_valueNestedContainerAccess;
}
bool AutoMapAccess::hasKeyNestedContainerAccess() {
    return !m_keyNestedContainerAccess.isNull();
}
bool AutoMapAccess::hasKeyNestedPointers() {
    if(hasKeyNestedContainerAccess()){
        if(auto daccess = dynamic_cast<AbstractSequentialAccess*>(m_keyNestedContainerAccess.data())){
            return (daccess->elementType() & PointersMask) || daccess->hasNestedPointers();
        }else if(auto daccess = dynamic_cast<AbstractAssociativeAccess*>(m_keyNestedContainerAccess.data())){
            return (daccess->keyType() & PointersMask) || daccess->hasKeyNestedPointers() || (daccess->valueType() & PointersMask) || daccess->hasValueNestedPointers();
        }else if(auto daccess = dynamic_cast<AbstractPairAccess*>(m_keyNestedContainerAccess.data())){
            return (daccess->firstType() & PointersMask) || daccess->hasFirstNestedPointers() || (daccess->secondType() & PointersMask) || daccess->hasSecondNestedPointers();
        }
    }
    return false;
}
bool AutoMapAccess::hasValueNestedContainerAccess() {
    return !m_valueNestedContainerAccess.isNull();
}
bool AutoMapAccess::hasValueNestedPointers() {
    if(hasValueNestedContainerAccess()){
        if(auto daccess = dynamic_cast<AbstractSequentialAccess*>(m_valueNestedContainerAccess.data())){
            return (daccess->elementType() & PointersMask) || daccess->hasNestedPointers();
        }else if(auto daccess = dynamic_cast<AbstractAssociativeAccess*>(m_valueNestedContainerAccess.data())){
            return (daccess->keyType() & PointersMask) || daccess->hasKeyNestedPointers() || (daccess->valueType() & PointersMask) || daccess->hasValueNestedPointers();
        }else if(auto daccess = dynamic_cast<AbstractPairAccess*>(m_valueNestedContainerAccess.data())){
            return (daccess->firstType() & PointersMask) || daccess->hasFirstNestedPointers() || (daccess->secondType() & PointersMask) || daccess->hasSecondNestedPointers();
        }
    }
    return false;
}

const QObject* AutoMapAccess::getOwner(const void* container){
    if(hasOwnerFunction() && size(container)>0){
        auto iter = keyValueIterator(container);
        while(iter->hasNext()){
            auto current = iter->next();
            if(m_keyOwnerFunction){
                if(const QObject* owner = m_keyOwnerFunction(current.first))
                    return owner;
            }else if(m_valueOwnerFunction){
                if(const QObject* owner = m_valueOwnerFunction(current.second))
                    return owner;
            }else if(m_keyNestedContainerAccess){
                if(const QObject* owner = m_keyNestedContainerAccess->getOwner(current.first))
                    return owner;
            }else if(m_valueNestedContainerAccess){
                if(const QObject* owner = m_valueNestedContainerAccess->getOwner(current.second))
                    return owner;
            }
        }
    }
    return nullptr;
}

bool AutoMapAccess::hasOwnerFunction(){
    if(m_keyOwnerFunction && !(keyType() & PointersMask))
        return true;
    if(m_valueOwnerFunction && !(valueType() & PointersMask))
        return true;
    if(!(keyType() & PointersMask) && m_keyNestedContainerAccess && m_keyNestedContainerAccess->hasOwnerFunction())
        return true;
    if(!(valueType() & PointersMask) && m_valueNestedContainerAccess && m_valueNestedContainerAccess->hasOwnerFunction())
        return true;
    return false;
}

jint AutoMapAccess::size(JNIEnv *,const void* container){
    return jint(size(container));
}

qsizetype AutoMapAccess::size(const void* container){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    return (*map)->size;
#else
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
        return d->size;
    }
    return 0;
#endif
}

void AutoMapAccess::insert(JNIEnv *env, const ContainerInfo& container, jobject key, jobject value){
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container.container);
        QMapDataBase*& d = *map;

        detach(map);

        Node *n = rootNode(map);
        Node *y = endNode(map);
        Node *lastNode = nullptr;
        bool  left = true;
        while (n) {
            y = n;
            if (!qMapLessThanKey(*n, akey)) {
                lastNode = n;
                left = true;
                n = n->left;
            } else {
                left = false;
                n = n->right;
            }
        }
        if (lastNode && !qMapLessThanKey(akey, *lastNode)) {
            assignValue(env, value, lastNode);
            return;
        }
        Node* newNode = d->createNode(int(m_size), int(m_align), y, left);
        char* newNodeData = reinterpret_cast<char*>(newNode);
        m_keyMetaType.construct(newNodeData + m_offset1, akey);
        m_valueMetaType.construct(newNodeData + m_offset2);
        assignValue(env, value, newNode);
#else
        insertOrAssign(detach(*reinterpret_cast<MapDataPointer*>(container.container)), akey, env, value);
#endif
    }
}

jboolean AutoMapAccess::contains(JNIEnv *env,const void* container, jobject key)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase * d = *map;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l))
        return findNode(d, akey) != nullptr;
#else
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
        jvalue jv;
        jv.l = key;
        void* akey = nullptr;
        QtJambiScope scope;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            auto i = find(*d, akey);
            return i != end(*d);
        }
    }
#endif
    return false;
}

jobject AutoMapAccess::begin(JNIEnv * env, const ExtendedContainerInfo& container)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container.container);
    detach(map);
    Node* begin = beginNode(map);
    return createIterator(env, container.nativeId, new void*(begin));
#else
    return createIterator(env, container.nativeId, new iterator(begin(detach(*reinterpret_cast<MapDataPointer*>(container.container))), m_offset1, m_offset2));
#endif
}

jobject AutoMapAccess::end(JNIEnv * env, const ExtendedContainerInfo& container)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container.container);
    detach(map);
    Node* end = endNode(map);
    return createIterator(env, container.nativeId, new void*(end));
#else
    return createIterator(env, container.nativeId, new iterator(end(detach(*reinterpret_cast<MapDataPointer*>(container.container))), m_offset1, m_offset2));
#endif
}

jobject AutoMapAccess::find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container.container);
    detach(map);
    QMapDataBase* d = *map;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        Node *n = findNode(d, akey);
        return createIterator(env, container.nativeId, new void*(n ? n : endNode(map)));
    }
    return createIterator(env, container.nativeId, new void*(endNode(map)));
#else
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            return createIterator(env, container.nativeId, new iterator(find(*d, akey), m_offset1, m_offset2));
        }
    }
    return end(env, container);
#endif
}

jobject AutoMapAccess::constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container.container);
    Node* begin = beginNode(map);
    return createConstIterator(env, container.nativeId, new void*(begin));
#else
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
        return createConstIterator(env, container.nativeId, new iterator(begin(*d), m_offset1, m_offset2));
    }else{
        return constEnd(env, container);
    }
#endif
}

jobject AutoMapAccess::constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container.container);
    Node* end = endNode(map);
    return createConstIterator(env, container.nativeId, new void*(end));
#else
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
        return createConstIterator(env, container.nativeId, new iterator(end(*d), m_offset1, m_offset2));
    }else{
        return createConstIterator(env, container.nativeId, new iterator());
    }
#endif
}

jobject AutoMapAccess::constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container.container);
    QMapDataBase* d = *map;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        Node *n = findNode(d, akey);
        return createConstIterator(env, container.nativeId, new void*(n ? n : endNode(map)));
    }
    return createConstIterator(env, container.nativeId, new void*(endNode(map)));
#else
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            return createConstIterator(env, container.nativeId, new iterator(find(*d, akey), m_offset1, m_offset2));
        }
    }
    return constEnd(env, container);
#endif
}

jobject AutoMapAccess::constLowerBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container.container);
    QMapDataBase * d = *map;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        Node *lb = rootNode(&d) ? lowerBound(rootNode(&d), akey) : nullptr;
        if (!lb)
            lb = endNode(&d);
        return createConstIterator(env, container.nativeId, new void*(lb));
    }
    return createConstIterator(env, container.nativeId, new void*(endNode(map)));
#else
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            return createConstIterator(env, container.nativeId, new iterator(node_iterator(findLowerBound(*d, akey)), m_offset1, m_offset2));
        }
    }
    return constEnd(env, container);
#endif
}

jobject AutoMapAccess::constUpperBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container.container);
    QMapDataBase * d = *map;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        Node *ub = rootNode(&d) ? upperBound(rootNode(&d), akey) : nullptr;
        if (!ub)
            ub = endNode(&d);
        return createConstIterator(env, container.nativeId, new void*(ub));
    }
    return createConstIterator(env, container.nativeId, new void*(endNode(map)));
#else
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            return createConstIterator(env, container.nativeId, new iterator(findUpperBound(*d, akey), m_offset1, m_offset2));
        }
    }
    return constEnd(env, container);
#endif
}

bool AutoMapAccess::keyLessThan(JNIEnv * env, jobject key1, jobject key2)
{
    jvalue jv;
    jv.l = key1;
    QtJambiScope scope;
    void* akey1 = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey1, jValueType::l)){
        jv.l = key2;
        void* akey2 = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey2, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            return isLessThan(m_keyMetaType, akey1, akey2);
#else
            return m_keyMetaType.compare(akey1, akey2)<0;
#endif
        }
    }
    return false;
}

void AutoMapAccess::clear(JNIEnv *, const ContainerInfo& container)
{
    clear(container.container);
}

void AutoMapAccess::clear(void* container)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const QMapDataBase* empty = &QMapDataBase::shared_null;
    assign(container, &empty);
#else
    MapDataPointer& d = *reinterpret_cast<MapDataPointer*>(container);
    if (!d)
        return;

    if (!d.isShared())
        clear(*d);
    else
        d.reset();
#endif
}

void* AutoMapAccess::constructContainer(JNIEnv*, void* result, const ConstContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

void* AutoMapAccess::constructContainer(void* result)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return new(result) const QMapDataBase*(&QMapDataBase::shared_null);
#else
    return new(result) MapDataPointer();
#endif

}

void* AutoMapAccess::constructContainer(void* result, const void* container)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    constructContainer(result);
    assign(result, container);
    return result;
#else
    const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container);
    return new(result) MapDataPointer(d);
#endif
}

bool AutoMapAccess::destructContainer(void* container){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container);
    QMapDataBase*& d = *map;
    if (!d->ref.deref()) destroy(d);
#else
    MapDataPointer& d = *reinterpret_cast<MapDataPointer*>(container);
    if(d){
        if (!d.isShared()){
            clear(*d);
#if defined(Q_CC_MSVC)
#if _ITERATOR_DEBUG_LEVEL == 2
            std::allocator<std::_Container_proxy> _Alproxy;
            _Delete_plain_internal(_Alproxy, d->_Myproxy);
#endif
#endif
        }
        d.reset();
    }
#endif
    return true;
}

jint AutoMapAccess::count(JNIEnv *env, const void* container, jobject key)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    jint result = 0;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        Node *firstNode(nullptr);
        Node *lastNode(nullptr);
        nodeRange(d, akey, &firstNode, &lastNode);

        while (firstNode != lastNode) {
            ++result;
            firstNode = firstNode->nextNode();
        }
    }
    return result;
#else
    jint result = 0;
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            node_iterator i = find(*d, akey);
            node_iterator _end = end(*d);
            while(i!=_end){
                ++result;
                const void* trykey = i->data(m_offset1);
                auto cmp = m_keyMetaType.compare(trykey, akey);
                if(cmp!=0)
                    break;
                ++i;
            }
        }
    }
    return result;
#endif
}

jobject AutoMapAccess::first(JNIEnv * env, const void* container)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    if(d->size==0)
        return nullptr;
    Node* first = beginNode(map);
    return nodeValue(env, first);
#else
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
        node_iterator iter = begin(*d);
        if(iter!=end(*d)){
            jvalue jv;
            jv.l = nullptr;
            m_valueInternalToExternalConverter(env, nullptr, iter->data(m_offset2), jv, true);
            return jv.l;
        }
    }
    return nullptr;
#endif
}

jobject AutoMapAccess::firstKey(JNIEnv *env, const void* container)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    if(d->size==0)
        return nullptr;
    Node* first = beginNode(map);
    return nodeKey(env, first);
#else
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
        node_iterator iter = begin(*d);
        if(iter!=end(*d)){
            jvalue jv;
            jv.l = nullptr;
            m_keyInternalToExternalConverter(env, nullptr, iter->data(m_offset1), jv, true);
            return jv.l;
        }
    }
    return nullptr;
#endif
}

jobject AutoMapAccess::last(JNIEnv * env, const void* container)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    if(d->size==0)
        return nullptr;
    Node* end = endNode(map);
    return nodeValue(env, end->previousNode());
#else
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
        node_iterator iter = end(*d);
        --iter;
        if(iter!=end(*d)){
            jvalue jv;
            jv.l = nullptr;
            m_valueInternalToExternalConverter(env, nullptr, iter->data(m_offset2), jv, true);
            return jv.l;
        }
    }
    return nullptr;
#endif
}

jobject AutoMapAccess::lastKey(JNIEnv *env, const void* container)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    if(d->size==0)
        return nullptr;
    Node* end = endNode(map);
    return nodeKey(env, end->previousNode());
#else
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
        node_iterator iter = end(*d);
        --iter;
        if(iter!=end(*d)){
            jvalue jv;
            jv.l = nullptr;
            m_keyInternalToExternalConverter(env, nullptr, iter->data(m_offset1), jv, true);
            return jv.l;
        }
    }
    return nullptr;
#endif
}

jobject AutoMapAccess::key(JNIEnv *env, const void* container, jobject value, jobject defaultKey)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    jvalue jv;
    jv.l = value;
    QtJambiScope scope;
    void* avalue = nullptr;
    if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
        Node *firstNode(beginNode(map));
        Node *lastNode(endNode(map));
        while (firstNode != lastNode) {
            if(isEquals(m_valueMetaType, reinterpret_cast<char*>(firstNode)+m_offset2, avalue)){
                return nodeKey(env, firstNode);
            }
            firstNode = firstNode->nextNode();
        }
    }
    return defaultKey;
#else
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
        jvalue jv;
        jv.l = value;
        QtJambiScope scope;
        void* avalue = nullptr;
        if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
            node_iterator end1 = end(*d);
            node_iterator iter = begin(*d);
            while(iter!=end1){
                const void* value1 = iter->data(m_offset2);
                if(m_valueMetaType.compare(value1, avalue)==0){
                    jvalue jv;
                    jv.l = nullptr;
                    m_keyInternalToExternalConverter(env, nullptr, iter->data(m_offset1), jv, true);
                    return jv.l;
                }
                ++iter;
            }
        }
    }
    return defaultKey;
#endif
}

jobject AutoMapAccess::value(JNIEnv *env, const void* container, jobject key, jobject defaultValue)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        Node *n = findNode(d, akey);
        return n ? nodeValue(env, n) : defaultValue;
    }else{
        return defaultValue;
    }
#else
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            node_iterator i = find(*d, akey);
            node_iterator end1 = end(*d);
            if(i != end1){
                jvalue jv;
                jv.l = nullptr;
                m_valueInternalToExternalConverter(env, nullptr, i->data(m_offset2), jv, true);
                return jv.l;
            }
        }
    }
    return defaultValue;
#endif
}

void AutoMapAccess::assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other){
    assign(container.container, other.container);
}

void AutoMapAccess::assign(void* container, const void* other)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container);
    QMapDataBase*& d = *map;
    QMapDataBase *const* map2 = reinterpret_cast<QMapDataBase *const*>(other);
    QMapDataBase* d2 = *map2;
    if(d!=d2 && d2->ref.ref()){
        if (!d->ref.deref())
            destroy(d);
        d = d2;
    }
#else
    MapDataPointer& d = *reinterpret_cast<MapDataPointer*>(container);
    const MapDataPointer& d2 = *reinterpret_cast<const MapDataPointer*>(other);
    if(!d2){
        if (!d)
            return;
        if (!d.isShared())
            clear(*d);
        else
            d.reset();
    }else{
        d = d2;
    }
#endif
}

IsBiContainerFunction AutoMapAccess::getIsBiContainerFunction(){
    return ContainerAPI::getAsQMap;
}

bool AutoMapAccess::isMulti() const{
    return false;
}

bool AutoMapAccess::equal(const void* a, const void* b){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(a);
    QMapDataBase* d = *map;
    QMapDataBase *const* map2 = reinterpret_cast<QMapDataBase *const*>(b);
    QMapDataBase* d2 = *map2;
    if(d==d2)
        return true;
    if(d->size!=d2->size)
        return false;
    Node *firstNode(beginNode(map));
    Node *lastNode(endNode(map));
    Node *firstNode2(beginNode(map2));
    Node *lastNode2(endNode(map2));
    while (firstNode != lastNode && firstNode2 != lastNode2) {
        void* key = reinterpret_cast<char*>(firstNode)+m_offset1;
        void* key2 = reinterpret_cast<char*>(firstNode2)+m_offset1;
        if(!isEquals(m_keyMetaType, key, key2))
            return false;
        void* value = reinterpret_cast<char*>(firstNode)+m_offset2;
        void* value2 = reinterpret_cast<char*>(firstNode2)+m_offset2;
        if(!isEquals(m_valueMetaType, value, value2))
            return false;
        firstNode = firstNode->nextNode();
        firstNode2 = firstNode2->nextNode();
    }
    return true;
#else
    const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(a);
    const MapDataPointer& d2 = *reinterpret_cast<const MapDataPointer*>(b);
    if(d==d2){
        return true;
    }
    if(!d || d->size==0){
        if(!d2 || d2->size==0){
            return false;
        }
    }
    if(d && d2 && d->size==d2->size){
        node_iterator end1 = end(*d);
        node_iterator end2 = end(*d2);
        node_iterator iter1 = begin(*d);
        node_iterator iter2 = begin(*d2);
        while(iter1!=end1 && iter2!=end2){
            const void* key1 = iter1->data(m_offset1);
            const void* key2 = iter2->data(m_offset1);
            if(m_keyMetaType.compare(key1, key2)!=0)
                return false;
            const void* value1 = iter1->data(m_offset2);
            const void* value2 = iter2->data(m_offset2);
            if(m_valueMetaType.compare(value1, value2)!=0)
                return false;
            ++iter1;
            ++iter2;
        }
        return true;
    }
    return false;
#endif
}

jboolean AutoMapAccess::equal(JNIEnv *env, const void* container, jobject other)
{
    void* ptr{nullptr};
    if ((*getIsBiContainerFunction())(env, other, keyMetaType(), valueMetaType(), ptr)) {
        return equal(container, ptr);
    }else{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
        QMapDataBase* d = *map;
        if(d->size!=Java::Runtime::Map::size(env, other))
            return false;
        jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
        QtJambiScope scope;
        jvalue _value;
        void *_qvaluePtr;
        Node *firstNode(beginNode(map));
        Node *lastNode(endNode(map));
        while(firstNode != lastNode && QtJambiAPI::hasJavaIteratorNext(env, iterator)){
            jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);
            _value.l = QtJambiAPI::keyOfJavaMapEntry(env, entry);
            _qvaluePtr = nullptr;
            if(!m_keyExternalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l))
                return false;
            void* key = reinterpret_cast<char*>(firstNode)+m_offset1;
            if(!isEquals(m_keyMetaType, key, _qvaluePtr))
                return false;
            _value.l = QtJambiAPI::valueOfJavaMapEntry(env, entry);
            _qvaluePtr = nullptr;
            if(!m_valueExternalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l))
                return false;
            void* value = reinterpret_cast<char*>(firstNode)+m_offset2;
            if(!isEquals(m_valueMetaType, value, _qvaluePtr))
                return false;
            firstNode = firstNode->nextNode();
        }
        return firstNode == lastNode && !QtJambiAPI::hasJavaIteratorNext(env, iterator);
#else
        if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
            if(!other){
                if(d->size==0)
                    return true;
            }else if(size(env, container)==Java::Runtime::Map::size(env, other)){
                jobject jiter = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
                QtJambiScope scope;
                jvalue _value;
                void *_qvaluePtr;
                node_iterator end1 = end(*d);
                node_iterator iter1 = begin(*d);
                while(iter1!=end1 && QtJambiAPI::hasJavaIteratorNext(env, jiter)){
                    jobject entry = QtJambiAPI::nextOfJavaIterator(env, jiter);
                    _value.l = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                    _qvaluePtr = nullptr;
                    if(!m_keyExternalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l))
                        return false;
                    const void* key = iter1->data(m_offset1);
                    if(m_keyMetaType.compare(key, _qvaluePtr)!=0)
                        return false;
                    _value.l = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                    _qvaluePtr = nullptr;
                    if(!m_valueExternalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l))
                        return false;
                    void* value = iter1->data(m_offset2);
                    if(m_valueMetaType.compare(value, _qvaluePtr)!=0)
                        return false;
                    ++iter1;
                }
                return true;
            }
        }else if(!other || Java::Runtime::Map::size(env, other)==0){
            return true;
        }
        return false;
#endif
    }
}

int AutoMapAccess::registerContainer(const QByteArray& typeName)
{
    int newMetaType = QMetaType::type(typeName);
    if(newMetaType==QMetaType::UnknownType){
        QSharedPointer<AutoMapAccess> access(dynamic_cast<AutoMapAccess*>(this->clone()), &containerDisposer);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        auto kiface = m_keyMetaType.iface();
        auto viface = m_valueMetaType.iface();
#endif
        newMetaType = registerContainerMetaType(typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                                       qtjambi_function_pointer<16,void(void*)>([access](void* ptr){
                                            access->destructContainer(ptr);
                                       }, qHash(typeName)),
                                       [](void* ptr, const void * other) -> void* {
                                            if(other){
                                                QMapDataBase *const* map2 = reinterpret_cast<QMapDataBase *const*>(other);
                                                QMapDataBase* d2 = *map2;
                                                if(d2!=&QMapDataBase::shared_null && d2->ref.ref())
                                                    return new (ptr) QMapDataBase *(d2);
                                            }
                                            return new (ptr) QMapDataBase const*(&QMapDataBase::shared_null);
                                       },
#endif //QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                                       uint(sizeOf()),
                                       alignOf(),
#else
                                       AutoMapAccess::defaultCtr,
                                       AutoMapAccess::copyCtr,
                                       AutoMapAccess::moveCtr,
                                       AutoMapAccess::dtor,
                                       (kiface->equals
                                                    || (kiface->flags & QMetaType::IsPointer)
                                                    || (kiface->flags & QMetaType::IsEnumeration))
                                                && (viface->equals
                                                    || (viface->flags & QMetaType::IsPointer)
                                                    || (viface->flags & QMetaType::IsEnumeration)) ? AutoMapAccess::equalsFn : nullptr,
                                       nullptr,
                                       (kiface->debugStream
                                                    || (kiface->flags & QMetaType::IsPointer)
                                                    || (kiface->flags & QMetaType::IsEnumeration))
                                                && (viface->debugStream
                                                    || (viface->flags & QMetaType::IsPointer)
                                                    || (viface->flags & QMetaType::IsEnumeration)) ? AutoMapAccess::debugStreamFn : nullptr,
                                       (kiface->dataStreamOut
                                                    || (kiface->flags & QMetaType::IsEnumeration))
                                                && (viface->dataStreamOut
                                                    || (viface->flags & QMetaType::IsEnumeration)) ? AutoMapAccess::dataStreamOutFn : nullptr,
                                       (kiface->dataStreamIn
                                                    || (kiface->flags & QMetaType::IsEnumeration))
                                                && (viface->dataStreamIn
                                                    || (viface->flags & QMetaType::IsEnumeration)) ? AutoMapAccess::dataStreamInFn : nullptr,
                                       nullptr,
                                       uint(sizeOf()),
                                       alignOf(),
                                       QMetaType::UnknownType,
#endif
                                       QMetaType::NeedsConstruction
                                                   | QMetaType::NeedsDestruction
                                                   | QMetaType::MovableType,
                                       nullptr,
                                       nullptr,
                                       access);
        if(m_keyHashFunction && m_valueHashFunction){
            insertHashFunctionByMetaType(newMetaType,
                                            [access]
                                            (const void* ptr, hash_type seed)->hash_type{
                                                if(ptr){
                                                    hash_type hashCode = seed;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(ptr);
                                                    Node *firstNode(beginNode(map));
                                                    Node *lastNode(endNode(map));
                                                    while (firstNode != lastNode) {
                                                        void* key = reinterpret_cast<char*>(firstNode)+access->m_offset1;
                                                        void* value = reinterpret_cast<char*>(firstNode)+access->m_offset2;
                                                        hash_type heyHash = access->m_keyHashFunction(key, 0);
                                                        hash_type valueHash = access->m_valueHashFunction(value, 0);
                                                        hash_type pairSeed = 0;
                                                        pairSeed = pairSeed ^ (heyHash + 0x9e3779b9 + (pairSeed << 6) + (pairSeed >> 2));
                                                        pairSeed = pairSeed ^ (valueHash + 0x9e3779b9 + (pairSeed << 6) + (pairSeed >> 2));
                                                        hashCode = hashCode + pairSeed;
                                                        firstNode = firstNode->nextNode();
                                                    }
#else
                                                    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(ptr)){
                                                        node_iterator end1 = access->end(*d);
                                                        node_iterator iter = access->begin(*d);
                                                        while(iter!=end1){
                                                            const void* key = iter->data(access->m_offset1);
                                                            const void* value = iter->data(access->m_offset2);
                                                            hash_type heyHash = access->m_keyHashFunction(key, 0);
                                                            hash_type valueHash = access->m_valueHashFunction(value, 0);
                                                            hash_type pairSeed = 0;
                                                            pairSeed = pairSeed ^ (heyHash + 0x9e3779b9 + (pairSeed << 6) + (pairSeed >> 2));
                                                            pairSeed = pairSeed ^ (valueHash + 0x9e3779b9 + (pairSeed << 6) + (pairSeed >> 2));
                                                            hashCode = hashCode + pairSeed;
                                                            ++iter;
                                                        }
                                                    }
#endif
                                                    return hashCode;
                                                }else{
                                                    return 0;
                                                }
                                            });
        }

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        {
            int to = qMetaTypeId<QtMetaTypePrivate::QAssociativeIterableImpl>();
            struct ConverterFunction : QtPrivate::AbstractConverterFunction{
                int _metaType_id_first;
                uint _metaType_flags_first;
                int _metaType_id_second;
                uint _metaType_flags_second;
                QtMetaTypePrivate::QAssociativeIterableImpl::sizeFunc _size;
                QtMetaTypePrivate::QAssociativeIterableImpl::findFunc _find;
                QtMetaTypePrivate::QAssociativeIterableImpl::beginFunc _begin;
                QtMetaTypePrivate::QAssociativeIterableImpl::beginFunc _end;
                QtMetaTypePrivate::QAssociativeIterableImpl::advanceFunc _advance;
                QtMetaTypePrivate::QAssociativeIterableImpl::getFunc _getKey;
                QtMetaTypePrivate::QAssociativeIterableImpl::getFunc _getValue;
                QtMetaTypePrivate::QAssociativeIterableImpl::destroyIterFunc _destroyIter;
                QtMetaTypePrivate::QAssociativeIterableImpl::equalIterFunc _equalIter;
                QtMetaTypePrivate::QAssociativeIterableImpl::copyIterFunc _copyIter;

                ConverterFunction(QSharedPointer<AutoMapAccess> access, int newMetaType, const QMetaType& keyMetaType, const QMetaType& valueMetaType, size_t offset1, size_t offset2)
                    : QtPrivate::AbstractConverterFunction(convert),
                      _metaType_id_first(keyMetaType.id()),
                      _metaType_flags_first(AbstractContainerAccess::isPointerType(keyMetaType)),
                      _metaType_id_second(valueMetaType.id()),
                      _metaType_flags_second(AbstractContainerAccess::isPointerType(valueMetaType)),
                      _size([](const void *p) -> int {
                                QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(p);
                                return (*map)->size;
                            }),
                      _find(qtjambi_function_pointer<16,void(const void *, const void *, void **)>([access](const void *container, const void *key, void **iterator){
                                QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
                                QMapDataBase* d = *map;
                                *iterator = access->findNode(d, key);
                            }, newMetaType)),
                      _begin([](const void *p, void ** iterator){
                                QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(p);
                                *iterator = beginNode(map);
                            }),
                      _end([](const void *p, void ** iterator){
                                QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(p);
                                *iterator = endNode(map);
                            }),
                      _advance([](void **p, int n){
                                Node* node = reinterpret_cast<Node*>(*p);
                                for(int i=0; node!=nullptr && i<n; ++i){
                                    node = node->nextNode();
                                }
                                *p = node;
                            }),
                      _getKey(qtjambi_function_pointer<16,QtMetaTypePrivate::VariantData(void*const*,int,uint)>([offset1](void * const *p, int metaTypeId, uint flags)->QtMetaTypePrivate::VariantData{
                                char* node = reinterpret_cast<char*>(*p);
                                return QtMetaTypePrivate::VariantData(metaTypeId, node+offset1, flags);
                            })),
                      _getValue(qtjambi_function_pointer<16,QtMetaTypePrivate::VariantData(void*const*,int,uint)>([offset2](void * const *p, int metaTypeId, uint flags)->QtMetaTypePrivate::VariantData{
                                char* node = reinterpret_cast<char*>(*p);
                                return QtMetaTypePrivate::VariantData(metaTypeId, node+offset2, flags);
                            })),
                      _destroyIter([](void **){}),
                      _equalIter([](void * const *p, void * const *other)->bool{ return *p==*other; }),
                      _copyIter([](void ** iter1, void * const *iter2){
                                    *reinterpret_cast<Node**>(iter1) = *reinterpret_cast<Node*const*>(iter2);
                                })
                {
                }

                static bool convert(const AbstractConverterFunction *_f, const void *src, void*target){
                    const ConverterFunction* f = static_cast<const ConverterFunction*>(_f);
                    QtMetaTypePrivate::QAssociativeIterableImpl* a = new(target) QtMetaTypePrivate::QAssociativeIterableImpl();
                    a->_iterable = src;
                    a->_metaType_id_key = f->_metaType_id_first;
                    a->_metaType_flags_key = f->_metaType_flags_first;
                    a->_metaType_id_value = f->_metaType_id_second;
                    a->_metaType_flags_value = f->_metaType_flags_second;
                    a->_size = f->_size;
                    a->_find = f->_find;
                    a->_begin = f->_begin;
                    a->_end = f->_end;
                    a->_advance = f->_advance;
                    a->_getKey = f->_getKey;
                    a->_getValue = f->_getValue;
                    a->_destroyIter = f->_destroyIter;
                    a->_equalIter = f->_equalIter;
                    a->_copyIter = f->_copyIter;
                    return true;
                }
            };
            registerConverter(new ConverterFunction(access, newMetaType, m_keyMetaType, m_valueMetaType, m_offset1, m_offset2), newMetaType, to);
        }
        if((QMetaType::hasRegisteredComparators(m_keyMetaType.id())
            || registeredComparator(m_keyMetaType.id())
            || m_keyMetaType.id()<QMetaType::User
            || AbstractContainerAccess::isPointerType(m_keyMetaType))
                && (QMetaType::hasRegisteredComparators(m_valueMetaType.id())
                || registeredComparator(m_valueMetaType.id())
                || m_valueMetaType.id()<QMetaType::User
                || AbstractContainerAccess::isPointerType(m_valueMetaType))){
            struct Comparator : QtPrivate::AbstractComparatorFunction{
                static bool equals(const AbstractComparatorFunction *_c, const void *a, const void *b){
                    const Comparator* c = static_cast<const Comparator*>(_c);
                    return c->access->equal(a, b);
                }
                static void destroy(AbstractComparatorFunction *_c){
                    delete static_cast<Comparator*>(_c);
                }
                QSharedPointer<AutoMapAccess> access;

                Comparator(const QSharedPointer<AutoMapAccess>& _access)
                    : QtPrivate::AbstractComparatorFunction(nullptr, equals, destroy),
                      access(_access)
                {

                }
            };
            RegistryAPI::registerComparator(new Comparator(access), newMetaType);
        }
        if((QMetaType::hasRegisteredDebugStreamOperator(m_keyMetaType.id())
                && QMetaType::hasRegisteredDebugStreamOperator(m_valueMetaType.id()))
                || (registeredDebugStreamOperator(m_keyMetaType.id())
                    && registeredDebugStreamOperator(m_valueMetaType.id()))
                || (m_keyMetaType.id()<QMetaType::User
                    && m_valueMetaType.id()<QMetaType::User)
                || (AbstractContainerAccess::isPointerType(m_keyMetaType)
                    || AbstractContainerAccess::isPointerType(m_valueMetaType))){
            struct DebugStreamFunction : QtPrivate::AbstractDebugStreamFunction{
                static void stream(const AbstractDebugStreamFunction *_f, QDebug& d, const void *ptr){
                    const DebugStreamFunction* f = static_cast<const DebugStreamFunction*>(_f);
                    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(ptr);
                    Node *firstNode(beginNode(map));
                    Node *lastNode(endNode(map));
                    d = d.nospace().noquote();
                    if(f->access->isMulti())
                        d << "QMultiMap";
                    else
                        d << "QMap";
                    d << "(";
                    while (firstNode != lastNode) {
                        void* key = reinterpret_cast<char*>(firstNode)+f->access->m_offset1;
                        void* value = reinterpret_cast<char*>(firstNode)+f->access->m_offset2;
                        d << "(";
                        CoreAPI::appendToDebugStream(d, f->access->m_keyMetaType.id(), key);
                        d << ",";
                        CoreAPI::appendToDebugStream(d, f->access->m_valueMetaType.id(), value);
                        d << ")";
                        firstNode = firstNode->nextNode();
                    }
                    d << ")";
                }
                static void destroy(AbstractDebugStreamFunction *_f){
                    delete static_cast<DebugStreamFunction*>(_f);
                }
                QSharedPointer<AutoMapAccess> access;

                DebugStreamFunction(const QSharedPointer<AutoMapAccess>& _access)
                    : QtPrivate::AbstractDebugStreamFunction(stream, destroy),
                      access(_access)
                {

                }
            };
            RegistryAPI::registerDebugStreamOperator(new DebugStreamFunction(access), newMetaType);
        }
        {
            size_t offset1 = m_offset1;
            size_t offset2 = m_offset2;
            int keyMetaType_id = m_keyMetaType.id();
            int valueMetaType_id = m_valueMetaType.id();
            QMetaType::registerStreamOperators(newMetaType,
                                               qtjambi_function_pointer<16,void(QDataStream &, const void *)>(
                                                                               [keyMetaType_id,valueMetaType_id,offset1,offset2](QDataStream &s, const void * p){
                                                                                       QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(p);
                                                                                       Node *firstNode(beginNode(map));
                                                                                       Node *lastNode(endNode(map));
                                                                                       s << quint32((*map)->size);
                                                                                       while (firstNode != lastNode) {
                                                                                           void* key = reinterpret_cast<char*>(firstNode)+offset1;
                                                                                           void* value = reinterpret_cast<char*>(firstNode)+offset2;
                                                                                           QMetaType::save(s, keyMetaType_id, key);
                                                                                           QMetaType::save(s, valueMetaType_id, value);
                                                                                           firstNode = firstNode->nextNode();
                                                                                       }
                                                                               }, newMetaType),
                                               qtjambi_function_pointer<16,void(QDataStream &, void *)>(
                                                                               [access](QDataStream &s, void * p){
                                                                                    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(p);
                                                                                    const QMapDataBase* empty = &QMapDataBase::shared_null;
                                                                                    access->assign(p, &empty);
                                                                                    access->detach(map);
                                                                                    quint32 size = 0;
                                                                                    s >> size;
                                                                                    for(quint32 i=0; i<size; ++i){
                                                                                        void* key = QMetaType::create(access->m_keyMetaType.id(), nullptr);
                                                                                        QMetaType::load(s, access->m_keyMetaType.id(), key);

                                                                                        Node *n = rootNode(map);
                                                                                        Node *y = endNode(map);
                                                                                        bool  left = true;
                                                                                        while (n) {
                                                                                            y = n;
                                                                                            if (!isLessThan(access->m_keyMetaType, reinterpret_cast<const char*>(n) + access->m_offset1, key)) {
                                                                                                left = true;
                                                                                                n = n->left;
                                                                                            } else {
                                                                                                left = false;
                                                                                                n = n->right;
                                                                                            }
                                                                                        }
                                                                                        Node* newNode = (*map)->createNode(int(access->m_size), int(access->m_align), y, left);
                                                                                        char* newNodeData = reinterpret_cast<char*>(newNode);
                                                                                        QMetaType::construct(access->m_keyMetaType.id(), newNodeData + access->m_offset1, key);
                                                                                        QMetaType::destroy(access->m_keyMetaType.id(), key);
                                                                                        QMetaType::construct(access->m_valueMetaType.id(), newNodeData + access->m_offset2, nullptr);
                                                                                        QMetaType::load(s, access->m_valueMetaType.id(), newNodeData + access->m_offset2);
                                                                                    }
                                                                               }, newMetaType)
                                               );
        }
#else
        {
            const QMetaType to = QMetaType::fromType<QAssociativeIterable>();
            QMetaType::registerMutableViewFunction([newMetaType](void *src, void *target)->bool{
                new(target) QIterable<QMetaAssociation>(QMetaAssociation(createMetaAssociationInterface(newMetaType)), reinterpret_cast<void **>(src));
                return true;
            }, QMetaType(newMetaType), to);
            QMetaType::registerConverterFunction([newMetaType](const void *src, void *target)->bool{
                new(target) QIterable<QMetaAssociation>(QMetaAssociation(createMetaAssociationInterface(newMetaType)), reinterpret_cast<void const*const*>(src));
                return true;
            }, QMetaType(newMetaType), to);
        }
#endif
    }else{
        registerContainerAccess(newMetaType, this);
    }
    return newMetaType;
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
QtMetaContainerPrivate::QMetaAssociationInterface* AutoMapAccess::createMetaAssociationInterface(int newMetaType){
    {
        QReadLocker locker(containerAccessLock());
        Q_UNUSED(locker)
        if(metaAssociationHash().contains(newMetaType))
            return &metaAssociationHash()[newMetaType];
    }
    using namespace QtMetaContainerPrivate;
    QMetaAssociationInterface* metaAssociationInterface;
    {
        QWriteLocker locker(containerAccessLock());
        metaAssociationInterface = &metaAssociationHash()[newMetaType];
    }
    QSharedPointer<class AutoMapAccess> access = getMapAccess(QMetaType(newMetaType).iface());
    metaAssociationInterface->keyMetaType = access->m_keyMetaType.iface();
    metaAssociationInterface->mappedMetaType = access->m_valueMetaType.iface();
    metaAssociationInterface->iteratorCapabilities = InputCapability | ForwardCapability;
    metaAssociationInterface->sizeFn = [](const void *c) -> qsizetype {
        if(!c)
            return 0;
        const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(c);
        return !d ? 0 : d->size;
    };

    metaAssociationInterface->clearFn = qtjambi_function_pointer<16,void(void *)>([newMetaType](void *c) {
        if(!c)
            return;
        QSharedPointer<class AutoMapAccess> access = getMapAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        access->clear(c);
    }, newMetaType);
    metaAssociationInterface->createIteratorFn = qtjambi_function_pointer<16,void*(void *,QMetaContainerInterface::Position)>(
        [newMetaType](void *c, QMetaContainerInterface::Position p) -> void* {
            if(!c)
                return nullptr;
            QSharedPointer<class AutoMapAccess> access = getMapAccess(QMetaType(newMetaType).iface());
            if(!access)
                return nullptr;
            switch (p) {
            case QMetaContainerInterface::AtBegin:
                return new extended_iterator(c, access, access->begin(access->detach(*reinterpret_cast<MapDataPointer*>(c))));
            default:
                return new extended_iterator(c, access, access->end(access->detach(*reinterpret_cast<MapDataPointer*>(c))));
            }
            return nullptr;
        }, newMetaType
        );
    metaAssociationInterface->destroyConstIteratorFn = [](const void *i) {
        delete static_cast<const extended_iterator *>(i);
    };
    metaAssociationInterface->compareConstIteratorFn = [](const void *i, const void *j) {
        return *static_cast<const extended_iterator *>(i) == *static_cast<const extended_iterator *>(j);
    };
    metaAssociationInterface->copyConstIteratorFn = [](void *i, const void *j) {
        *static_cast<extended_iterator *>(i) = *static_cast<const extended_iterator *>(j);
    };
    metaAssociationInterface->advanceConstIteratorFn = [](void *i, qsizetype step) {
        extended_iterator& it = *reinterpret_cast<extended_iterator*>(i);
        if(step>0){
            for(;step>0; --step)
                ++it;
        }
    };
    metaAssociationInterface->diffConstIteratorFn = [](const void *i, const void *j) -> qsizetype {
        extended_iterator it = *reinterpret_cast<const extended_iterator*>(i);
        extended_iterator jt = *reinterpret_cast<const extended_iterator*>(j);
        const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(it.container);
        extended_iterator end(it.container, it.access, d ? it.access->end(*d) : node_iterator{nullptr});
        if(it==jt || it==end)
            return 0;
        int counter = 0;
        while(it!=jt){
            ++counter;
            ++it;
            if(it==end && jt!=end)
                return 0;
        }
        return counter;
    };
    metaAssociationInterface->createConstIteratorFn = qtjambi_function_pointer<16,void*(const void *,QMetaContainerInterface::Position)>(
        [newMetaType](const void *c, QMetaContainerInterface::Position p) -> void* {
            if(!c)
                return nullptr;
            QSharedPointer<class AutoMapAccess> access = getMapAccess(QMetaType(newMetaType).iface());
            if(!access)
                return nullptr;
            const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(c);
            if(d){
                switch (p) {
                case QMetaContainerInterface::AtBegin:
                    return new extended_iterator(c, access, access->begin(*d));
                default:
                    return new extended_iterator(c, access, access->end(*d));
                }
            }
            return new extended_iterator(c, access);
        }, newMetaType
        );
    metaAssociationInterface->destroyIteratorFn = metaAssociationInterface->destroyConstIteratorFn;
    metaAssociationInterface->compareIteratorFn = metaAssociationInterface->compareConstIteratorFn;
    metaAssociationInterface->copyIteratorFn = metaAssociationInterface->copyConstIteratorFn;
    metaAssociationInterface->advanceIteratorFn = metaAssociationInterface->advanceConstIteratorFn;
    metaAssociationInterface->diffIteratorFn = metaAssociationInterface->diffConstIteratorFn;
    metaAssociationInterface->insertKeyFn = qtjambi_function_pointer<16,void(void *, const void *)>([newMetaType](void *c, const void *k) {
        if(!c)
            return;
        QSharedPointer<class AutoMapAccess> access = getMapAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        access->detach(*reinterpret_cast<MapDataPointer*>(c));
        MapDataPointer& d = *reinterpret_cast<MapDataPointer*>(c);
        access->insertOrAssign(*d, k, nullptr);
    }, newMetaType);
    metaAssociationInterface->removeKeyFn = qtjambi_function_pointer<16,void(void *, const void *)>([newMetaType](void *c, const void *k) {
        if(!c)
            return;
        QSharedPointer<class AutoMapAccess> access = getMapAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        access->detach(*reinterpret_cast<MapDataPointer*>(c));
        MapDataPointer& d = *reinterpret_cast<MapDataPointer*>(c);
        if(!d.isShared()){
            access->erase(*d, k);
        }else{
            MapData *newData = access->createMapData();
            access->copyIfNotEquivalentTo(*newData, *d, k);
            d.reset(newData);
        }
    }, newMetaType);
    metaAssociationInterface->containsKeyFn = qtjambi_function_pointer<16,bool(const void *, const void*)>([newMetaType](const void *c, const void* k) -> bool {
        if(!c)
            return false;
        QSharedPointer<class AutoMapAccess> access = getMapAccess(QMetaType(newMetaType).iface());
        const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(c);
        auto i = access->find(*d, k);
        return i != access->end(*d);
        return false;
    }, newMetaType);
    metaAssociationInterface->mappedAtKeyFn = access->isMulti() ? QMetaAssociationInterface::MappedAtKeyFn(nullptr) : qtjambi_function_pointer<16,void(const void *, const void *, void *)>([newMetaType](const void *c, const void *k, void *r) {
        if(!c)
            return;
        QSharedPointer<class AutoMapAccess> access = getMapAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(c);
        node_iterator i = access->find(*d, k);
        node_iterator end1 = access->end(*d);
        if(i != end1){
            access->m_valueMetaType.destruct(r);
            access->m_valueMetaType.construct(r, i->data(access->m_offset2));
        }
    }, newMetaType);
    metaAssociationInterface->setMappedAtKeyFn = access->isMulti() ? QMetaAssociationInterface::SetMappedAtKeyFn(nullptr) : qtjambi_function_pointer<16,void(void *, const void *, const void *)>([newMetaType](void *c, const void *k, const void *r) {
        if(!c)
            return;
        QSharedPointer<class AutoMapAccess> access = getMapAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        access->insertOrAssign(access->detach(*reinterpret_cast<MapDataPointer*>(c)), k, r);
    }, newMetaType);
    metaAssociationInterface->createIteratorAtKeyFn = qtjambi_function_pointer<16,void*(void *, const void *)>([newMetaType](void *c, const void *k) ->void* {
        if(!c)
            return nullptr;
        QSharedPointer<class AutoMapAccess> access = getMapAccess(QMetaType(newMetaType).iface());
        if(!access)
            return nullptr;
        return new extended_iterator(c, access, access->find(access->detach(*reinterpret_cast<MapDataPointer*>(c)), k));
    }, newMetaType);
    metaAssociationInterface->createConstIteratorAtKeyFn = qtjambi_function_pointer<16,void*(const void *, const void *)>([newMetaType](const void *c, const void *k) ->void* {
        if(!c)
            return nullptr;
        QSharedPointer<class AutoMapAccess> access = getMapAccess(QMetaType(newMetaType).iface());
        if(!access)
            return nullptr;
        const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(c);
        return new extended_iterator(c, access, access->find(*d, k));
    }, newMetaType);
    metaAssociationInterface->keyAtIteratorFn = [](const void *i, void *k) {
        const extended_iterator* it = reinterpret_cast<const extended_iterator*>(i);
        it->access->m_keyMetaType.destruct(k);
        it->access->m_keyMetaType.construct(k, (*it)->data(it->access->m_offset1));
    };
    metaAssociationInterface->keyAtConstIteratorFn = metaAssociationInterface->keyAtIteratorFn;
    metaAssociationInterface->mappedAtIteratorFn = [](const void *i, void *k) {
        const extended_iterator* it = reinterpret_cast<const extended_iterator*>(i);
        it->access->m_valueMetaType.destruct(k);
        it->access->m_valueMetaType.construct(k, (*it)->data(it->access->m_offset2));
    };
    metaAssociationInterface->mappedAtConstIteratorFn = metaAssociationInterface->mappedAtIteratorFn;
    metaAssociationInterface->setMappedAtIteratorFn = [](const void *i, const void *v) {
        const extended_iterator* it = reinterpret_cast<const extended_iterator*>(i);
        it->access->m_valueMetaType.destruct((*it)->data(it->access->m_offset2));
        it->access->m_valueMetaType.construct((*it)->data(it->access->m_offset2), v);
    };
    metaAssociationInterface->eraseKeyAtIteratorFn = [](void *c, const void *i) {
        const extended_iterator* it = reinterpret_cast<const extended_iterator*>(i);
        it->access->erase(it->access->detach(*reinterpret_cast<MapDataPointer*>(c)), it);
    };
    return metaAssociationInterface;
}
#endif

ContainerAndAccessInfo AutoMapAccess::keys(JNIEnv *env, const ConstContainerInfo& container)
{
    ContainerAndAccessInfo result;
    AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(SequentialContainerType::QList, m_keyMetaType));
    if(!listAccess)
        listAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(
                                                                           env,
                                                                           SequentialContainerType::QList,
                                                                           m_keyMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                           m_keyAlign,
                                                                           m_keyMetaType.sizeOf(),
                                                                           AbstractContainerAccess::isStaticType(m_keyMetaType),
#else
                                                                           m_keyMetaType.alignOf(),
                                                                           m_keyMetaType.sizeOf(),
#endif
                                                                           AbstractContainerAccess::isPointerType(m_keyMetaType),
                                                                           m_keyHashFunction,
                                                                           m_keyInternalToExternalConverter,
                                                                           m_keyExternalToInternalConverter,
                                                                           m_keyNestedContainerAccess,
                                                                           m_keyOwnerFunction
                                                                           ));
    if(listAccess){
        CHECK_CONTAINER_ACCESS(env, listAccess)
        result.container = listAccess->createContainer();
        result.object = ContainerAPI::objectFromQList(env, result.container, listAccess);
        result.access = listAccess;
        jint idx = listAccess->size(env, result.container);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container.container);
        QMapDataBase* d = *map;
        Node *firstNode(beginNode(map));
        Node *lastNode(endNode(map));
        listAccess->reserve(env, result, d->size);
        while(firstNode != lastNode){
            if(listAccess->append(result.container, reinterpret_cast<char*>(firstNode)+m_offset1)){
                idx++;
            }else{
                listAccess->insert(env, result, idx++, 1, nodeKey(env, firstNode));
            }
            firstNode = firstNode->nextNode();
        }
#else
        if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
            listAccess->reserve(env, result, d->size);
            node_iterator end1 = end(*d);
            node_iterator iter1 = begin(*d);
            jvalue jv;
            while(iter1!=end1){
                jv.l = nullptr;
                if(listAccess->append(result.container, iter1->data(m_offset1))){
                    idx++;
                }else{
                    m_keyInternalToExternalConverter(env, nullptr, iter1->data(m_offset1), jv, true);
                    listAccess->insert(env, result, idx++, 1, jv.l);
                }
                ++iter1;
            }
        }
#endif
    }
    return result;
}

ContainerAndAccessInfo AutoMapAccess::keys(JNIEnv *env, const ConstContainerInfo& container, jobject value)
{
    ContainerAndAccessInfo result;
    AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(SequentialContainerType::QList, m_keyMetaType));
    if(!listAccess)
        listAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(
                                                                           env,
                                                                           SequentialContainerType::QList,
                                                                           m_keyMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                           m_keyAlign,
                                                                           m_keyMetaType.sizeOf(),
                                                                           AbstractContainerAccess::isStaticType(m_keyMetaType),
#else
                                                                           m_keyMetaType.alignOf(),
                                                                           m_keyMetaType.sizeOf(),
#endif
                                                                           AbstractContainerAccess::isPointerType(m_keyMetaType),
                                                                           m_keyHashFunction,
                                                                           m_keyInternalToExternalConverter,
                                                                           m_keyExternalToInternalConverter,
                                                                           m_keyNestedContainerAccess,
                                                                           m_keyOwnerFunction
                                                                           ));
    if(listAccess){
        CHECK_CONTAINER_ACCESS(env, listAccess)
        result.container = listAccess->createContainer();
        result.object = ContainerAPI::objectFromQList(env, result.container, listAccess);
        result.access = listAccess;
        QtJambiScope scope;
        jvalue _value;
        void *_qvaluePtr;
        _value.l = value;
        _qvaluePtr = nullptr;
        jint idx = listAccess->size(env, result.container);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container.container);
        if((*map)->size>0 && m_valueExternalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
            Node *firstNode(beginNode(map));
            Node *lastNode(endNode(map));
            while(firstNode != lastNode){
                void* value = reinterpret_cast<char*>(firstNode)+m_offset2;
                if(isEquals(m_valueMetaType, value, _qvaluePtr)){
                    if(listAccess->append(result.container, reinterpret_cast<char*>(firstNode)+m_offset1)){
                        idx++;
                    }else{
                        listAccess->insert(env, result, idx++, 1, nodeKey(env, firstNode));
                    }
                }
                firstNode = firstNode->nextNode();
            }
        }
#else
        if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
            if(m_valueExternalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
                listAccess->reserve(env, result, d->size);
                node_iterator end1 = end(*d);
                node_iterator iter1 = begin(*d);
                jvalue jv;
                while(iter1!=end1){
                    if(m_valueMetaType.compare(_qvaluePtr, iter1->data(m_offset2))==0){
                        if(listAccess->append(result.container, iter1->data(m_offset1))){
                            idx++;
                        }else{
                            jv.l = nullptr;
                            m_keyInternalToExternalConverter(env, nullptr, iter1->data(m_offset1), jv, true);
                            listAccess->insert(env, result, idx++, 1, jv.l);
                        }
                    }
                    ++iter1;
                }
            }
        }
#endif
    }
    return result;
}

jint AutoMapAccess::remove(JNIEnv *env, const ContainerInfo& container, jobject key)
{
    int n = 0;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container.container);
        QMapDataBase*& d = *map;
        detach(map);
        while (Node *node = findNode(d, akey)) {
            deleteNode(d, node);
            ++n;
        }
#else
        if(MapDataPointer& d = *reinterpret_cast<MapDataPointer*>(container.container)){
            if(!d.isShared()){
                n = jint(erase(*d, akey));
            }else{
                MapData *newData = createMapData();
                n = copyIfNotEquivalentTo(*newData, *d, akey);
                d.reset(newData);
            }
        }
#endif
    }
    return n;
}

jobject AutoMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key)
{
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container.container);
    QMapDataBase*& d = *map;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        detach(map);
        Node *node = findNode(d, akey);
        jobject result = node ? nodeValue(env, node) : nullptr;
        deleteNode(d, node);
        return result;
    }
#else
    MapDataPointer& d = *reinterpret_cast<MapDataPointer*>(container.container);
    if (!d)
        return {};
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        const MapDataPointer copy = d.isShared() ? d : MapDataPointer();
        MapData& data = detach(d);
        node_iterator node = find(data, akey);
        if (node!=end(data)) {
            jvalue jv;
            jv.l = nullptr;
            m_valueInternalToExternalConverter(env, nullptr, node->data(m_offset2), jv, true);
            erase(*d, node);
            return jv.l;
        }
    }
#endif
    return nullptr;
}

ContainerAndAccessInfo AutoMapAccess::values(JNIEnv *env, const ConstContainerInfo& container)
{
    ContainerAndAccessInfo result;
    AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(SequentialContainerType::QList, m_valueMetaType));
    if(!listAccess)
        listAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(
                                                                           env,
                                                                           SequentialContainerType::QList,
                                                                           m_valueMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                           m_valueAlign,
                                                                           m_valueMetaType.sizeOf(),
                                                                           AbstractContainerAccess::isStaticType(m_valueMetaType),
#else
                                                                           m_valueMetaType.alignOf(),
                                                                           m_valueMetaType.sizeOf(),
#endif
                                                                           AbstractContainerAccess::isPointerType(m_valueMetaType),
                                                                           m_valueHashFunction,
                                                                           m_valueInternalToExternalConverter,
                                                                           m_valueExternalToInternalConverter,
                                                                           m_valueNestedContainerAccess,
                                                                           m_valueOwnerFunction
                                                                           ));
    if(listAccess){
        CHECK_CONTAINER_ACCESS(env, listAccess)
        result.container = listAccess->createContainer();
        result.object = ContainerAPI::objectFromQList(env, result.container, listAccess);
        result.access = listAccess;
        jint idx = listAccess->size(env, result.container);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container.container);
        Node *firstNode(beginNode(map));
        Node *lastNode(endNode(map));
        while(firstNode != lastNode){
            if(listAccess->append(result.container, reinterpret_cast<char*>(firstNode)+m_offset2)){
                idx++;
            }else{
                listAccess->insert(env, result, idx++, 1, nodeValue(env, firstNode));
            }
            firstNode = firstNode->nextNode();
        }
#else
        if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
            node_iterator end1 = end(*d);
            node_iterator iter1 = begin(*d);
            jvalue jv;
            jv.l = nullptr;
            while(iter1!=end1){
                if(listAccess->append(result.container, iter1->data(m_offset2))){
                    idx++;
                }else{
                    m_valueInternalToExternalConverter(env, nullptr, iter1->data(m_offset2), jv, true);
                    listAccess->insert(env, result, idx++, 1, jv.l);
                }
                ++iter1;
            }
        }
#endif
    }
    return result;
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
void AutoMapAccess::deleteNode(QMapDataBase * d, Node* node){
    m_keyMetaType.destruct(reinterpret_cast<char*>(node) + m_offset1);
    m_valueMetaType.destruct(reinterpret_cast<char*>(node) + m_offset2);
    d->freeNodeAndRebalance(node);
}

void AutoMapAccess::detach(QMapDataBase ** map){
    QMapDataBase* d = *map;
    if (d->ref.isShared()) detach_helper(map);
}

void AutoMapAccess::detach_helper(QMapDataBase ** map){
    QMapDataBase*& d = *map;
    QMapDataBase *x = QMapDataBase::createData();
    if (d->header.left) {
        x->header.left = copyNode(static_cast<Node *>(d->header.left), x);
        x->header.left->setParent(&x->header);
    }
    if (!d->ref.deref())
        destroy(d);
    d = x;
    d->recalcMostLeftNode();
}

void AutoMapAccess::destroy(QMapDataBase* data){
    if (rootNode(&data)) {
        destroySubTree(rootNode(&data));
        data->freeTree(data->header.left, int(m_align));
    }
    QMapDataBase::freeData(data);
}

void AutoMapAccess::destroySubTree(Node* node)
{
    m_keyMetaType.destruct(reinterpret_cast<char*>(node) + m_offset1);
    m_valueMetaType.destruct(reinterpret_cast<char*>(node) + m_offset2);
    if (node->left)
        destroySubTree(node->left);
    if (node->right)
        destroySubTree(node->right);
}

AutoMapAccess::Node* AutoMapAccess::copyNode(Node* node, QMapDataBase* d){
    Node* n = d->createNode(int(m_size), int(m_align), nullptr, false);
    char* nodeData = reinterpret_cast<char*>(node);
    char* nData = reinterpret_cast<char*>(n);
    QT_TRY {
        m_keyMetaType.construct(nData+m_offset1, nodeData+m_offset1);
        QT_TRY {
            m_valueMetaType.construct(nData+m_offset2, nodeData+m_offset2);
        } QT_CATCH(...) {
            m_keyMetaType.destruct(nData + m_offset1);
            QT_RETHROW;
        }
    } QT_CATCH(...) {
        d->freeNodeAndRebalance(n);
        QT_RETHROW;
    }
    n->setColor(node->color());
    if (node->left) {
        n->left = copyNode(node->left, d);
        n->left->setParent(n);
    } else {
        n->left = nullptr;
    }
    if (node->right) {
        n->right = copyNode(node->right, d);
        n->right->setParent(n);
    } else {
        n->right = nullptr;
    }
    return n;
}

AutoMapAccess::Node* AutoMapAccess::rootNode(QMapDataBase *const* map){
    QMapDataBase* d = *map;
    return reinterpret_cast<Node*>(d->header.left);
}

AutoMapAccess::Node* AutoMapAccess::beginNode(QMapDataBase *const* map){
    QMapDataBase* d = *map;
    if (rootNode(map))
        return static_cast<Node*>(d->mostLeftNode);
    return endNode(map);
    return reinterpret_cast<Node*>(d->header.left);
}

AutoMapAccess::Node* AutoMapAccess::endNode(QMapDataBase *const* map){
    QMapDataBase* d = *map;
    return reinterpret_cast<Node*>(&d->header);
}

AutoMapAccess::Node* AutoMapAccess::lowerBound(Node* n, const void* akey){
    Node *lastNode = nullptr;
    while (n) {
        if (!qMapLessThanKey(*n, akey)) {
            lastNode = n;
            n = n->left;
        } else {
            n = n->right;
        }
    }
    return lastNode;
}

AutoMapAccess::Node* AutoMapAccess::upperBound(Node* n, const void* akey){
    Node *lastNode = nullptr;
    while (n) {
        if (qMapLessThanKey(akey, *n)) {
            lastNode = n;
            n = n->left;
        } else {
            n = n->right;
        }
    }
    return lastNode;
}

AutoMapAccess::Node* AutoMapAccess::findNode(QMapDataBase * d, const void* akey){
    if (Node *r = rootNode(&d)) {
        Node *lb = lowerBound(r, akey);
        if (lb && !qMapLessThanKey(akey, *lb))
            return lb;
    }
    return nullptr;
}

void AutoMapAccess::nodeRange(QMapDataBase* base, const void* akey, Node **firstNode, Node **lastNode){
    Node *n = rootNode(&base);
    Node *l = endNode(&base);
    while (n) {
        if (qMapLessThanKey(akey, *n)) {
            l = n;
            n = n->left;
        } else if (qMapLessThanKey(*n, akey)) {
            n = n->right;
        } else {
            *firstNode = n->left ? lowerBound(n->left, akey) : nullptr;
            if (!*firstNode)
                *firstNode = n;
            *lastNode = n->right ? upperBound(n->right, akey) : nullptr;
            if (!*lastNode)
                *lastNode = l;
            return;
        }
    }
    *firstNode = *lastNode = l;
}

void AutoMapAccess::assignValue(JNIEnv * env,jobject value, Node* node){
    jvalue jv;
    jv.l = value;
    void* _qt_value = reinterpret_cast<char*>(node) + m_offset2;
    m_valueExternalToInternalConverter(env, nullptr, jv, _qt_value, jValueType::l);
}

jobject AutoMapAccess::nodeKey(JNIEnv * env, Node* node)
{
    jvalue jv;
    jv.l = nullptr;
    if(node)
        m_keyInternalToExternalConverter(env, nullptr, reinterpret_cast<char*>(node)+m_offset1, jv, true);
    return jv.l;
}

jobject AutoMapAccess::nodeValue(JNIEnv * env, Node* node)
{
    jvalue jv;
    jv.l = nullptr;
    if(node)
        m_valueInternalToExternalConverter(env, nullptr, reinterpret_cast<char*>(node)+m_offset2, jv, true);
    return jv.l;
}

bool AutoMapAccess::qMapLessThanKey(const void* akey, const Node& right){
    return isLessThan(m_keyMetaType, akey, reinterpret_cast<const char*>(&right) + m_offset1);
}

bool AutoMapAccess::qMapLessThanKey(const Node& left, const void* akey){
    return isLessThan(m_keyMetaType, reinterpret_cast<const char*>(&left) + m_offset1, akey);
}
#endif

jobject AutoMapAccess::createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    AutoAssociativeConstIteratorAccess* containerAccess = new AutoAssociativeConstIteratorAccess(m_valueInternalToExternalConverter,
                                                                     [](AutoAssociativeConstIteratorAccess*,void*ptr){
                                                                        Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                        *reinterpret_cast<Node**>(ptr) = cursor->nextNode();
                                                                     },
                                                                     [](AutoAssociativeConstIteratorAccess*,void*ptr){
                                                                        Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                        *reinterpret_cast<Node**>(ptr) = cursor->previousNode();
                                                                     },
                                                                     [](AutoAssociativeConstIteratorAccess* access, const void* ptr)->const void*{
                                                                        char* cursor = *reinterpret_cast<char*const*>(ptr);
                                                                        return cursor+access->valueOffset();
                                                                     },
                                                                     {},
                                                                     [](AutoAssociativeConstIteratorAccess*,const void*ptr1,const void*ptr2)->bool{
                                                                        return *reinterpret_cast<Node*const*>(ptr1)==*reinterpret_cast<Node*const*>(ptr2);
                                                                     },
                                                                     m_keyInternalToExternalConverter,
                                                                     [](AutoAssociativeConstIteratorAccess* access, const void* ptr)->const void*{
                                                                        char* cursor = *reinterpret_cast<char*const*>(ptr);
                                                                        return cursor+access->keyOffset();
                                                                     },
                                                                    m_keyMetaType,
                                                                    m_valueMetaType,
                                                                    m_offset1,
                                                                    m_offset2
                                            );
    return QtJambiAPI::convertQAssociativeIteratorToJavaObject(env, ownerId, iteratorPtr, [](void* ptr,bool){
        delete reinterpret_cast<void**>(ptr);
    }, containerAccess);
#else
    AbstractAssociativeConstIteratorAccess* containerAccess = new AutoAssociativeConstIteratorAccess(m_valueInternalToExternalConverter,
            [](AutoAssociativeConstIteratorAccess*,void*ptr){
                iterator& cursor = *reinterpret_cast<iterator*>(ptr);
                ++cursor;
            },
            [](AutoAssociativeConstIteratorAccess*,void*ptr){
                iterator& cursor = *reinterpret_cast<iterator*>(ptr);
                --cursor;
            },
            [](AutoAssociativeConstIteratorAccess*,const void*ptr)->const void*{
                const iterator& cursor = *reinterpret_cast<const iterator*>(ptr);
                return cursor.value();
            },
            nullptr,
            [](AutoAssociativeConstIteratorAccess*,const void*ptr1,const void*ptr2)->bool{
                const iterator& n1 = *reinterpret_cast<const iterator*>(ptr1);
                const iterator& n2 = *reinterpret_cast<const iterator*>(ptr2);
                return n1==n2;
            },
            m_keyInternalToExternalConverter,
            [](AutoAssociativeConstIteratorAccess*,const void*ptr)->const void*{
                const iterator& cursor = *reinterpret_cast<const iterator*>(ptr);
                return cursor.key();
            },
            m_keyMetaType,
            m_valueMetaType,
            /*offsets not required*/ 0, 0
        );
    return QtJambiAPI::convertQAssociativeIteratorToJavaObject(env, ownerId, iteratorPtr, [](void* ptr,bool){
            delete reinterpret_cast<iterator*>(ptr);
        }, containerAccess);
#endif
}

jobject AutoMapAccess::createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    AbstractAssociativeIteratorAccess* containerAccess = new AutoAssociativeIteratorAccess(m_valueInternalToExternalConverter,
                                                                     [](AutoAssociativeIteratorAccess*, void*ptr){
                                                                        Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                        *reinterpret_cast<Node**>(ptr) = cursor->nextNode();
                                                                     },
                                                                     [](AutoAssociativeIteratorAccess*, void*ptr){
                                                                        Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                        *reinterpret_cast<Node**>(ptr) = cursor->previousNode();
                                                                     },
                                                                     [](AutoAssociativeIteratorAccess* access, const void*ptr)->const void*{
                                                                        char* cursor = *reinterpret_cast<char*const*>(ptr);
                                                                        return cursor+access->valueOffset();
                                                                     },
                                                                     {},
                                                                     [](AutoAssociativeIteratorAccess*, const void*ptr1,const void*ptr2)->bool{
                                                                        Node* n1 = *reinterpret_cast<Node*const*>(ptr1);
                                                                        Node* n2 = *reinterpret_cast<Node*const*>(ptr2);
                                                                        return n1==n2;
                                                                     },
                                                                     m_keyInternalToExternalConverter,
                                                                     [](AutoAssociativeIteratorAccess* access, const void*ptr)->const void*{
                                                                        char* cursor = *reinterpret_cast<char*const*>(ptr);
                                                                        return cursor+access->keyOffset();
                                                                     },
                                                                     m_valueExternalToInternalConverter,
                                                                     [](AutoAssociativeIteratorAccess* access, void*ptr)->void*{
                                                                        char* cursor = *reinterpret_cast<char**>(ptr);
                                                                        return cursor+access->valueOffset();
                                                                     },
                                                                    m_keyMetaType,
                                                                    m_valueMetaType,
                                                                    m_offset1,
                                                                    m_offset2
                                            );
    return QtJambiAPI::convertQAssociativeIteratorToJavaObject(env, ownerId, iteratorPtr, [](void* ptr,bool){
        delete reinterpret_cast<void**>(ptr);
    }, containerAccess);
#else
    AbstractAssociativeIteratorAccess* containerAccess = new AutoAssociativeIteratorAccess(m_valueInternalToExternalConverter,
            [](AutoAssociativeIteratorAccess*, void*ptr){
                iterator& cursor = *reinterpret_cast<iterator*>(ptr);
                ++cursor;
            },
            [](AutoAssociativeIteratorAccess*, void*ptr){
                iterator& cursor = *reinterpret_cast<iterator*>(ptr);
                --cursor;
            },
            [](AutoAssociativeIteratorAccess*, const void*ptr)->const void*{
                const iterator& cursor = *reinterpret_cast<const iterator*>(ptr);
                return cursor.value();
            },
            nullptr,
            [](AutoAssociativeIteratorAccess*, const void*ptr1,const void*ptr2)->bool{
                const iterator& n1 = *reinterpret_cast<const iterator*>(ptr1);
                const iterator& n2 = *reinterpret_cast<const iterator*>(ptr2);
                return n1==n2;
            },
            m_keyInternalToExternalConverter,
            [](AutoAssociativeIteratorAccess*, const void*ptr)->const void*{
                const iterator& cursor = *reinterpret_cast<const iterator*>(ptr);
                return cursor.key();
            },
            m_valueExternalToInternalConverter,
            [](AutoAssociativeIteratorAccess*, void*ptr)->void*{
                iterator& cursor = *reinterpret_cast<iterator*>(ptr);
                return cursor.value();
            },
            m_keyMetaType,
            m_valueMetaType,
            /*offsets not required*/ 0, 0
        );
    return QtJambiAPI::convertQAssociativeIteratorToJavaObject(env, ownerId, iteratorPtr, [](void* ptr,bool){
            delete reinterpret_cast<iterator*>(ptr);
        }, containerAccess);
#endif
}

size_t AutoMapAccess::sizeOf() {
    return sizeof(QMap<char,char>);
}

ushort AutoMapAccess::alignOf() const{
    return ushort(sizeof(QMap<char,char>));
}

bool AutoMapAccess::isDetached(const void* container){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    return map && *map && !(*map)->ref.isShared();
#else
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
        return !d.isShared();
    }
    return true;
#endif
}

void AutoMapAccess::detach(const ContainerInfo& container){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container.container);
    detach(map);
#else
    if(MapDataPointer& d = *reinterpret_cast<MapDataPointer*>(container.container)){
        detach(d);
    }
#endif
}

bool AutoMapAccess::isSharedWith(const void* container, const void* container2){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase *const* map2 = reinterpret_cast<QMapDataBase *const*>(container2);
    if(map && map2){
        return (*map)==(*map2);
    }
#else
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
        if(const MapDataPointer& d2 = *reinterpret_cast<const MapDataPointer*>(container2)){
            return d.data()==d2.data();
        }
    }
#endif
    return false;
}

void AutoMapAccess::swap(JNIEnv *, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMapDataBase *& map = *reinterpret_cast<QMapDataBase **>(container.container);
    QMapDataBase *& map2 = *reinterpret_cast<QMapDataBase **>(container2.container);
    qSwap(map, map2);
#else
    MapDataPointer& d = *reinterpret_cast<MapDataPointer*>(container.container);
    MapDataPointer& d2 = *reinterpret_cast<MapDataPointer*>(container2.container);
    d.swap(d2);
#endif
}

KeyPointerRCAutoMapAccess::KeyPointerRCAutoMapAccess(KeyPointerRCAutoMapAccess& other)
    : AutoMapAccess(other), ReferenceCountingSetContainer() {}

KeyPointerRCAutoMapAccess* KeyPointerRCAutoMapAccess::clone(){
    return new KeyPointerRCAutoMapAccess(*this);
}

void KeyPointerRCAutoMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = keyValueIterator(container.container);
    while(iterator->hasNext()){
        const void* content = iterator->next().first;
        jobject obj{nullptr};
        switch(keyType()){
        case PointerToQObject:
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(reinterpret_cast<const QObject*>(content))){
                obj = link->getJavaObjectLocalRef(env);
            }
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(keyMetaType())){
                if(FunctionalResolver resolver = registeredFunctionalResolver(*typeId)){
                    bool success = false;
                    obj = resolver(env, content, &success);
                    break;
                }
            }
            Q_FALLTHROUGH();
        case Pointer:
            for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(content)){
                obj = link->getJavaObjectLocalRef(env);
                break;
            }
            break;
        default:
            break;
        }
        if(obj)
            Java::Runtime::Collection::add(env, set, obj);
    }
    clearRC(env, container.object);
    addAllRC(env, container.object, set);
}

void KeyPointerRCAutoMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoMapAccess::swap(env, container, container2);
    if(KeyPointerRCAutoMapAccess* access = dynamic_cast<KeyPointerRCAutoMapAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void KeyPointerRCAutoMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoMapAccess::assign(env, container, container2);
    if(KeyPointerRCAutoMapAccess* access = dynamic_cast<KeyPointerRCAutoMapAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void KeyPointerRCAutoMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoMapAccess::clear(env, container);
    clearRC(env, container.object);
}

void KeyPointerRCAutoMapAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMapAccess::insert(env, container, key, value);
    addUniqueRC(env, container.object, key);
}

jint KeyPointerRCAutoMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = AutoMapAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, container.object, key);
    }
    return result;
}

jobject KeyPointerRCAutoMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoMapAccess::take(env, container, key);
    removeRC(env, container.object, key);
    return result;
}

ValuePointerRCAutoMapAccess::ValuePointerRCAutoMapAccess(ValuePointerRCAutoMapAccess& other)
    : AutoMapAccess(other), ReferenceCountingSetContainer() {}

ValuePointerRCAutoMapAccess* ValuePointerRCAutoMapAccess::clone(){
    return new ValuePointerRCAutoMapAccess(*this);
}

void ValuePointerRCAutoMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = keyValueIterator(container.container);
    while(iterator->hasNext()){
        const void* content = iterator->next().second;
        jobject obj{nullptr};
        switch(valueType()){
        case PointerToQObject:
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(reinterpret_cast<const QObject*>(content))){
                obj = link->getJavaObjectLocalRef(env);
            }
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(valueMetaType())){
                if(FunctionalResolver resolver = registeredFunctionalResolver(*typeId)){
                    bool success = false;
                    obj = resolver(env, content, &success);
                    break;
                }
            }
            Q_FALLTHROUGH();
        case Pointer:
            for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(content)){
                obj = link->getJavaObjectLocalRef(env);
                break;
            }
            break;
        default:
            break;
        }
        if(obj)
            Java::Runtime::Collection::add(env, set, obj);
    }
    clearRC(env, container.object);
    addAllRC(env, container.object, set);
}

void ValuePointerRCAutoMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoMapAccess::swap(env, container, container2);
    if(ValuePointerRCAutoMapAccess* access = dynamic_cast<ValuePointerRCAutoMapAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void ValuePointerRCAutoMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoMapAccess::assign(env, container, container2);
    if(ValuePointerRCAutoMapAccess* access = dynamic_cast<ValuePointerRCAutoMapAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void ValuePointerRCAutoMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoMapAccess::clear(env, container);
    clearRC(env, container.object);
}

void ValuePointerRCAutoMapAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jobject oldValue = AutoMapAccess::value(env, container.container, key, nullptr);
    AutoMapAccess::insert(env, container, key, value);
    removeRC(env, container.object, oldValue);
    addRC(env, container.object, value);
}

jint ValuePointerRCAutoMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jobject oldValue = AutoMapAccess::value(env, container.container, key, nullptr);
    jint result = AutoMapAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, container.object, oldValue);
    }
    return result;
}

jobject ValuePointerRCAutoMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoMapAccess::take(env, container, key);
    removeRC(env, container.object, result);
    return result;
}

PointersRCAutoMapAccess::PointersRCAutoMapAccess(PointersRCAutoMapAccess& other)
    : AutoMapAccess(other), ReferenceCountingMapContainer(other) {}

PointersRCAutoMapAccess* PointersRCAutoMapAccess::clone(){
    return new PointersRCAutoMapAccess(*this);
}

void PointersRCAutoMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject map = Java::Runtime::HashMap::newInstance(env, 0);
    auto iterator = keyValueIterator(container.container);
    while(iterator->hasNext()){
        auto content = iterator->next();
        jobject key{nullptr};
        switch(keyType()){
        case PointerToQObject:
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(reinterpret_cast<const QObject*>(content.first))){
                key = link->getJavaObjectLocalRef(env);
            }
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(keyMetaType())){
                if(FunctionalResolver resolver = registeredFunctionalResolver(*typeId)){
                    bool success = false;
                    key = resolver(env, content.first, &success);
                    break;
                }
            }
            Q_FALLTHROUGH();
        case Pointer:
            for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(content.first)){
                key = link->getJavaObjectLocalRef(env);
                break;
            }
            break;
        default:
            break;
        }
        jobject value{nullptr};
        switch(valueType()){
        case PointerToQObject:
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(reinterpret_cast<const QObject*>(content.second))){
                value = link->getJavaObjectLocalRef(env);
            }
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(valueMetaType())){
                if(FunctionalResolver resolver = registeredFunctionalResolver(*typeId)){
                    bool success = false;
                    value = resolver(env, content.second, &success);
                    break;
                }
            }
            Q_FALLTHROUGH();
        case Pointer:
            for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(content.second)){
                value = link->getJavaObjectLocalRef(env);
                break;
            }
            break;
        default:
            break;
        }
        Java::Runtime::Map::put(env, map, key, value);
    }
    clearRC(env, container.object);
    putAllRC(env, container.object, map);
}

void PointersRCAutoMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoMapAccess::swap(env, container, container2);
    if(PointersRCAutoMapAccess* access = dynamic_cast<PointersRCAutoMapAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void PointersRCAutoMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoMapAccess::assign(env, container, container2);
    if(PointersRCAutoMapAccess* access = dynamic_cast<PointersRCAutoMapAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void PointersRCAutoMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoMapAccess::clear(env, container);
    clearRC(env, container.object);
}

void PointersRCAutoMapAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMapAccess::insert(env, container, key, value);
    putRC(env, container.object, key, value);
}

jint PointersRCAutoMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = AutoMapAccess::remove(env, container, key);
    removeRC(env, container.object, key, result);
    return result;
}

jobject PointersRCAutoMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoMapAccess::take(env, container, key);
    removeRC(env, container.object, key);
    return result;
}

NestedPointersRCAutoMapAccess::NestedPointersRCAutoMapAccess(NestedPointersRCAutoMapAccess& other)
    : AutoMapAccess(other), ReferenceCountingSetContainer() {}

NestedPointersRCAutoMapAccess* NestedPointersRCAutoMapAccess::clone(){
    return new NestedPointersRCAutoMapAccess(*this);
}

void NestedPointersRCAutoMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    if(size(env, container.container)==0){
        clearRC(env, container.object);
    }else{
        JniLocalFrame frame(env, 200);
        jobject set = Java::Runtime::HashSet::newInstance(env);
        auto access1 = keyNestedContainerAccess();
        auto access2 = valueNestedContainerAccess();
        auto iterator = keyValueIterator(container.container);
        while(iterator->hasNext()){
            auto current = iterator->next();
            unfoldAndAddContainer(env, set, current.first, keyType(), keyMetaType(), access1);
            unfoldAndAddContainer(env, set, current.second, valueType(), valueMetaType(), access2);
        }
        if(access1)
            access1->dispose();
        if(access2)
            access2->dispose();
        addAllRC(env, container.object, set);
    }
}

void NestedPointersRCAutoMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoMapAccess::swap(env, container, container2);
    if(NestedPointersRCAutoMapAccess* access = dynamic_cast<NestedPointersRCAutoMapAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCAutoMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoMapAccess::assign(env, container, container2);
    if(NestedPointersRCAutoMapAccess* access = dynamic_cast<NestedPointersRCAutoMapAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCAutoMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoMapAccess::clear(env, container);
    clearRC(env, container.object);
}

void NestedPointersRCAutoMapAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMapAccess::insert(env, container, key, value);
    addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
    addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), value);
}

jint NestedPointersRCAutoMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = AutoMapAccess::remove(env, container, key);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jobject NestedPointersRCAutoMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoMapAccess::take(env, container, key);
    updateRC(env, container);
    return result;
}
