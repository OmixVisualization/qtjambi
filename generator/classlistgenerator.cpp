/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "classlistgenerator.h"

QString ClassListGenerator::fileName() const { return "qtjambi-classes.qdoc"; }

static bool class_sorter(AbstractMetaClass *a, AbstractMetaClass *b) {
    return a->name() < b->name();
}

void ClassListGenerator::generate() {
    QFile f(fileName());
    if (f.open(QFile::WriteOnly)) {
        QTextStream s(&f);

        s << "/****************************************************************************" << endl
        << "**" << endl
        << "** This is a generated file, please don't touch." << endl
        << "**" << endl
        << "****************************************************************************/" << endl << endl;

        s << "/*!" << endl
        << "\\page qtjambi-classes.html" << endl << endl
        << "\\title Qt Jambi's classes" << endl << endl
        << "This is a list of all Qt Jambi classes." << endl << endl
        << "\\table 100%" << endl;

        AbstractMetaClassList classes = Generator::classes();
        qSort(classes.begin(), classes.end(), class_sorter);

        int numColumns = 4;
        int numRows = (classes.size() + numColumns - 1) / numColumns;

        for (int i = 0; i < numRows; ++i) {
            s << endl << "\\row ";
            for (int j = 0; j < numColumns; ++j) {
                if (classes.value(i + j * numRows)) {
                    s << "\\o \\l{" << classes.value(i + j * numRows)->qualifiedCppName()
                    << "}{" << classes.value(i + j * numRows)->name() << "} ";
                }
            }

        }

        s << endl << "\\endtable" << endl
        << "*/" << endl;
    }
}
