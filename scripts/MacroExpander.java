/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import java.io.*;
import java.util.*;
import java.util.jar.*;
import java.util.zip.*;

public class MacroExpander {
    // should not hardcode this
    static String lisenceHeader =
        "** This file may be used under the terms of the GNU General Public\n"+
        "** License version 2.0 as published by the Free Software Foundation\n"+
        "** and appearing in the file LICENSE.GPL included in the packaging of\n"+
        "** this file.  Please review the following information to ensure GN\n"+
        "** General Public Licensing requirements will be met:\n"+
        "** http://www.trolltech.com/products/qt/opensource.html\n"+
        "**\n"+
        "** If you are unsure which license is appropriate for your use, please\n"+
        "** review the following information:\n"+
        "** http://www.trolltech.com/products/qt/licensing.html or contact the\n"+
        "** sales department at sales@trolltech.com.\n";

    public static String expandMacros(String rawHtml)
    {
        rawHtml = rawHtml.replaceAll("\\$THISYEAR\\$","2007");
        rawHtml = rawHtml.replaceAll("\\$TROLLTECH\\$", "Trolltech ASA");
        rawHtml = rawHtml.replaceAll("\\$PRODUCT\\$", "Qt Jambi");
        rawHtml = rawHtml.replaceAll("\\$LICENSE\\$", lisenceHeader);
        rawHtml = rawHtml.replaceAll("\\$JAVA_LICENSE\\$", lisenceHeader);
        rawHtml = rawHtml.replaceAll("\\$CPP_LICENSE\\$", lisenceHeader);

        return rawHtml;
    }

    public static void doJarFile(File jarFile, File outFile) throws IOException
    {
        JarOutputStream jarOut = new JarOutputStream(
                new FileOutputStream(outFile));
        jarOut.setLevel(9);
        jarOut.setMethod(ZipOutputStream.DEFLATED);

        JarFile jar = new JarFile(jarFile);
        Enumeration<JarEntry> entries = jar.entries();
        while (entries.hasMoreElements()) {
            JarEntry entry = entries.nextElement();
            InputStream in = jar.getInputStream(entry);

            byte buff[] = new byte[4096];
            ByteArrayOutputStream bout = new ByteArrayOutputStream();
            while(in.available() != 0) {
                int read = in.read(buff);
                bout.write(buff, 0, read);
            }
            byte buffer[] = bout.toByteArray();
            in.close();

            if (entry.getName().endsWith(".html")) {
                String expandedFile = expandMacros(new String(buffer));
                buffer = expandedFile.getBytes();
            }

            JarEntry entryOut = new JarEntry(entry.getName());
            jarOut.putNextEntry(entryOut);
            jarOut.write(buffer, 0, buffer.length);
            jarOut.closeEntry();
        }
        jarOut.close();
    }

    public static void main(String[] args) throws IOException {
        if (args.length != 2) {
            System.err.println("Usage: MacroExpander inJarFile outJarFile");
            System.exit(1);
        }

        MacroExpander.doJarFile(new File(args[0]), new File(args[1]));
        System.err.println("Finished");
    }

}
