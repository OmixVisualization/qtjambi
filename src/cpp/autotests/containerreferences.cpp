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

#include "containerreferences.h"

const QList<QString>& ContainerReferences::constList() const {return m_list;}
QList<QString>& ContainerReferences::listRef() {return m_list;}

const QList<QList<float>>& ContainerReferences::constListList() const {return m_listlist;}
QList<QList<float>>& ContainerReferences::listListRef() {return m_listlist;}

const QQueue<QString>& ContainerReferences::constQueue() const {return m_queue;}
QQueue<QString>& ContainerReferences::queueRef() {return m_queue;}

const QSet<QString>& ContainerReferences::constSet() const {return m_set;}
QSet<QString>& ContainerReferences::setRef() {return m_set;}

const QStack<QPair<int,QString>>& ContainerReferences::constStack() const {return m_stack;}
QStack<QPair<int,QString>>& ContainerReferences::stackRef() {return m_stack;}

const QMap<QString,QPoint>& ContainerReferences::constMap() const {return m_map;}
QMap<QString,QPoint>& ContainerReferences::mapRef() {return m_map;}

const QHash<int,QPoint>& ContainerReferences::constHash() const {return m_hash;}
QHash<int,QPoint>& ContainerReferences::hashRef() {return m_hash;}

const QMultiMap<QString,QPoint>& ContainerReferences::constMultiMap() const {return m_multimap;}
QMultiMap<QString,QPoint>& ContainerReferences::multiMapRef() {return m_multimap;}

const QMultiHash<int,QPoint>& ContainerReferences::constMultiHash() const {return m_multihash;}
QMultiHash<int,QPoint>& ContainerReferences::multiHashRef() {return m_multihash;}

const QStringList& ContainerReferences::constStringList() const {return m_stringlist;}
QStringList& ContainerReferences::stringListRef() {return m_stringlist;}

const QByteArrayList& ContainerReferences::constByteArrayList() const {return m_bytearraylist;}
QByteArrayList& ContainerReferences::byteArrayListRef() {return m_bytearraylist;}
