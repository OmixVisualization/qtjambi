/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

package generator;

import org.qtjambi.qt.QtBlockedSlot;
import org.qtjambi.qt.xmlpatterns.QAbstractXmlNodeModel;
import org.qtjambi.qt.xmlpatterns.QXmlNodeModelIndex;

class QXmlName___ extends QXmlName {

        /**
         * Constructs a <code>QXmlName</code> instance that inserts <code>localName</code>,
         * <code>namespaceURI</code> and <code>prefix</code> into <code>namePool</code> if they aren't
         * already there. The accessor functions <code>namespaceUri()</code>, <code>prefix()</code>,
         * <code>localName()</code>, and <code>toClarkName()</code> must be passed the <code>namePool</code>
         * used here, so the <code>namePool</code> must remain in scope while the
         * accessor functions might be used. However, two instances can
         * be compared with <code>==</code> or <code>!=</code> and copied without the
         * <code>namePool</code>.
         *
         * The user guarantees that the string components are valid for a
         * <code>QName</code>. In particular, the local name, and the prefix (if present),
         * must be valid {@link <a href="http://www.w3.org/TR/REC-xml-names/#NT-NCName">NCNames</a>}
         * The function <code>isNCName()</code> can be used to test validity
         * of these names. The namespace URI should be an absolute URI.
         * <code>QUrl.isRelative()</code> can be used to test whether the namespace URI
         * is relative or absolute. Finally, providing a prefix is not valid
         * when no namespace URI is provided.
         *
         * <code>namePool</code> is not copied. Nor is the reference to it retained
         * in this instance. This constructor inserts the three strings
         * into <code>namePool</code>.
         */
        public QXmlName(QXmlNamePool namePool, String localName, String namespaceURI, String prefix) {
            this(namePool.nativePointer(), localName, namespaceURI, prefix);
        }

        /**
         * Equivalent to calling QXmlName(namePool, localName, namespaceURI, null);
         */
        public QXmlName(QXmlNamePool namePool, String localName, String namespaceURI) {
            this(namePool, localName, namespaceURI, null);
        }

        /**
         * Equivalent to calling QXmlName(namePool, localName, null, null)
         */
        public QXmlName(QXmlNamePool namePool, String localName) {
            this(namePool, localName, null);
        }


}// class

class QXmlQuery___ extends QXmlQuery {
        public final String evaluateTo() {
            QNativePointer result = new QNativePointer(QNativePointer.Type.String);

            boolean ok = evaluateTo_private(result);

            return ok ? result.stringValue() : null;
        }


}// class

class QSimpleXmlNodeModel___ extends QSimpleXmlNodeModel {

        /**
         * Returns the name pool that is associated with this model. The implementation of <code>name()</code>
         * would use this to create names.
         */
        public final QXmlNamePool namePool() {
            return QXmlNamePool.fromNativePointer(namePool_private());
        }

        @Deprecated
        public static org.qtjambi.qt.QNativePointer nativePointerArray(QSimpleXmlNodeModel array[]) {
            throw new RuntimeException("This object cannot be made into a native pointer. The method is present for purposes of backwards compatibility.");
        }

        @Deprecated
        public QSimpleXmlNodeModel clone() {
            throw new RuntimeException("This object cannot be cloned. The method is present for purposes of backwards compatibility.");
        }

}// class

class QAbstractXmlNodeModel___ extends QAbstractXmlNodeModel {
        @Deprecated
        public static org.qtjambi.qt.QNativePointer nativePointerArray(QAbstractXmlNodeModel array[]) {
            throw new RuntimeException("This object cannot be made into a native pointer. The method is present for purposes of backwards compatibility.");
        }

        @Deprecated
        public QAbstractXmlNodeModel clone() {
            throw new RuntimeException("This object cannot be cloned. The method is present for purposes of backwards compatibility.");
        }

}// class
