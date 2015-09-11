/****************************************************************************
 **
 ** Copyright (C) 2008-2009 Nokia. All rights reserved.
 **
 ** This file is part of Qt Jambi.
 **
 ** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

package org.qtjambi.examples.xmlpatterns;

import org.qtjambi.examples.QtJambiExample;
import org.qtjambi.examples.XMLHighlighter;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QLineEdit;
import org.qtjambi.qt.widgets.QTextBrowser;
import org.qtjambi.qt.widgets.QWidget;
import org.qtjambi.qt.xmlpatterns.QAbstractXmlNodeModel;
import org.qtjambi.qt.xmlpatterns.QXmlItem;
import org.qtjambi.qt.xmlpatterns.QXmlQuery;
import org.qtjambi.qt.xmlpatterns.QXmlResultItems;

@QtJambiExample(name = "XML Query")
public class XMLQuery extends QWidget {

    private QLineEdit queryLine;
    private QTextBrowser sourceBrowser;
    private QTextBrowser resultBrowser;

    private final String fileName = "classpath:org/qtjambi/examples/frank.xbel";

    public XMLQuery() {
        this(null);
    }

    public XMLQuery(QWidget parent) {
        super(parent);
        QGridLayout layout = new QGridLayout(this);

        sourceBrowser = new QTextBrowser(this);
        sourceBrowser.setToolTip(tr("This window contains the source file used in the example.\n" +
                "The file is accessed using the classpath file engine. '"
                + fileName + "'"));
        resultBrowser = new QTextBrowser(this);
        resultBrowser.setToolTip(tr("This is the result of the query."));
        queryLine = new QLineEdit(this);
        queryLine.setToolTip(tr("Try to change query, and press enter."));

        layout.addWidget(sourceBrowser, 1, 1);
        layout.addWidget(resultBrowser, 1, 2);
        layout.addWidget(queryLine, 2, 1, 1, 2);

        queryLine.returnPressed.connect(this, "executeQuery()");

        QFile file = new QFile(fileName);
        if (file.open(QIODevice.OpenModeFlag.ReadOnly)) {
            sourceBrowser.setPlainText(file.readAll().toString());
            new XMLHighlighter(sourceBrowser.document());
        } else {
            sourceBrowser.setPlainText(tr("Could not open file: ") + fileName);
        }

        queryLine.setText("doc(\"" + fileName + "\")/xbel/folder/title");
        executeQuery();

        setWindowTitle("XML Query");
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
    }

    private void executeQuery() {
        resultBrowser.clear();

        QXmlQuery query = new QXmlQuery();
        query.setQuery(queryLine.text());

        String res = "";

        if (query.isValid()) {

            QXmlResultItems result = new QXmlResultItems();
            query.evaluateTo(result);

            QXmlItem item = result.next();
            while (!item.isNull()) {

                if (item.isNode()) {
                    QAbstractXmlNodeModel model = item.toNodeModelIndex().model();
                    res += model.stringValue(item.toNodeModelIndex()) + "\n";
                } else if (item.isAtomicValue()) {
                    res += item.toAtomicValue() + "\n";
                }

                item = result.next();
            }
        } else {
            res = "Query was not valid.\n";
        }
        resultBrowser.setPlainText(res);
    }

    public static void main(String[] args) {
        QApplication.initialize(args);

        XMLQuery test = new XMLQuery();
        test.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
