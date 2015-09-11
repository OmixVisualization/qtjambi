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

import org.qtjambi.qt.*;
import org.qtjambi.qt.xml.*;

class QDomDocument___ extends QDomDocument {

        public class Result {
                private Result(boolean success, QNativePointer errorMessage, QNativePointer errorLine, QNativePointer errorColumn) {
                    this.success = success;
                    this.errorMessage = errorMessage.stringValue();
                    this.errorLine = errorLine.intValue();
                    this.errorColumn = errorColumn.intValue();
                }

                public boolean success;
                public String errorMessage;
                public int errorLine;
                public int errorColumn;

        }

        public final Result setContent(QXmlInputSource source, boolean namespaceProcessing) {
            QNativePointer errorStr = new QNativePointer(QNativePointer.Type.String);
            QNativePointer errorLine = new QNativePointer(QNativePointer.Type.Int);
            QNativePointer errorColumn = new QNativePointer(QNativePointer.Type.Int);

            boolean success = setContent(source, namespaceProcessing, errorStr, errorLine, errorColumn);

            return new Result(success, errorStr, errorLine, errorColumn);
        }

}// class

class QXmlNamespaceSupport___ extends QXmlNamespaceSupport {

        public static class ProcessedName {
                public ProcessedName(String nsuri, String localName) {
                    this.nsuri = nsuri;
                    this.localName = localName;
                }

                public String nsuri;
                public String localName;
        }

        public final ProcessedName processName(String qname, boolean isAttribute) {
            QNativePointer nsUri = new QNativePointer(QNativePointer.Type.String);
            QNativePointer localName = new QNativePointer(QNativePointer.Type.String);
            processName(qname, isAttribute, nsUri, localName);

            return new ProcessedName(nsUri.stringValue(), localName.stringValue());
        }

        public static class SplitName {
                public SplitName(String prefix, String localname) {
                    this.prefix = prefix;
                    this.localname = localname;
                }

                public String prefix;
                public String localname;
        }

        public final SplitName splitName(String qname) {
            QNativePointer prefix = new QNativePointer(QNativePointer.Type.String);
            QNativePointer localName = new QNativePointer(QNativePointer.Type.String);
            splitName(qname, prefix, localName);

            return new SplitName(prefix.stringValue(), localName.stringValue());
        }

}// class

interface QXmlEntityResolver___ extends QXmlEntityResolver {

    public static class ResolvedEntity {

            public boolean error;
            public QXmlInputSource inputSource;

            public ResolvedEntity(boolean error, QXmlInputSource inputSource) {

                this.error = error;
                this.inputSource = inputSource;

            }

    }

}// class


