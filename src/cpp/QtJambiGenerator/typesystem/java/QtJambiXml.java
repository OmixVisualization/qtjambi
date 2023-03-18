/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package generator;

import io.qt.*;
import io.qt.xml.*;

class QDomDocument___ extends QDomDocument {
    public static final class Result {
        private Result(boolean success, String errorMessage, int errorLine, int errorColumn) {
            this.success = success;
            this.errorMessage = errorMessage;
            this.errorLine = errorLine;
            this.errorColumn = errorColumn;
        }

        public final boolean success;
        public final String errorMessage;
        public final int errorLine;
        public final int errorColumn;
    }
}// class

class QXmlNamespaceSupport___ extends QXmlNamespaceSupport {
    public static final class ProcessedName {
        private ProcessedName(String nsuri, String localName) {
            this.nsuri = nsuri;
            this.localName = localName;
        }

        public final String nsuri;
        public final String localName;
    }

    public static final class SplitName {
        private SplitName(String prefix, String localname) {
            this.prefix = prefix;
            this.localname = localname;
        }

        public final String prefix;
        public final String localname;
    }
}// class

interface QXmlEntityResolver___ extends QXmlEntityResolver {

    public static final class ResolvedEntity {

            public final @NativeAccess boolean error;
            public final @NativeAccess QXmlInputSource inputSource;

            @NativeAccess
            public ResolvedEntity(boolean error, QXmlInputSource inputSource) {

                this.error = error;
                this.inputSource = inputSource;

            }

    }

}// class

