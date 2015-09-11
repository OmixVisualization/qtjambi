/*************************************************************************
 *
 * Copyright (C) 1992-2009 Nokia. All rights reserved.
 *
 * This file is part of Qt Jambi.
 *
 * ** $BEGIN_LICENSE$
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

 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 **************************************************************************/

package org.qtjambi.tools;

import org.qtjambi.qt.internal.*;

import java.util.*;
import java.io.*;

public class CacheCleaner {

    private static void doHelp() {
        System.out.println("");
        System.out.println("CacheCleaner\nA utility for cleaning up the Qt Jambi native library cache\n");
        System.out.println("USAGE:");
        System.out.println("  > java " + CacheCleaner.class.getName() + " [options] [list of keys]\n");
        System.out.println("  Options: ");
        System.out.println("    --all    -a      Removes all keys from the cache");
        System.out.println("    --list   -l      Lists used entries in the cache");
        System.out.println("    --help   -h      This help");
        System.out.println("\n\n");
    }

    public static void main(String args[]) throws Exception {

        boolean cleanAll = false;
        boolean list = false;
        List<String> names = new ArrayList<String>();

        for (String arg : args) {
            if (arg.equals("--all") || arg.equals("-a")) {
                cleanAll = true;
            } else if (arg.equals("--list") || arg.equals("-l")) {
                list = true;
            } else if (arg.equals("--help") || arg.equals("-h")) {
                doHelp();
                return;
            } else {
                names.add(arg);
            }
        }

        if (names.size() == 0 && !list && !cleanAll) {
            doHelp();
            list = true;
        }


        if (list) {
            System.out.println("Qt Jambi Caches:");
            int cutpoint = baseName().length();
            for (File d : cacheDirs()) {
                System.out.println("    \"" + d.getName().substring(cutpoint) + "\"");
            }
        } else {
            File dirs[];

            if (cleanAll) {
                dirs = cacheDirs();
            } else {
                dirs = new File[names.size()];
                for (int i=0; i<names.size(); ++i) {
                    dirs[i] = NativeLibraryManager.jambiTempDirBase(names.get(i));
                    if (!dirs[i].exists())
                        throw new FileNotFoundException(dirs[i].toString());
                }
            }

            for (File d : dirs) {
                System.out.println("Deleting \"" + d.getAbsolutePath() + "\"");
                delete(d);
            }
        }
    }

    private static void delete(File d) throws Exception {
        if (d.isDirectory()) {
            File files[] = d.listFiles();
            for (File f : files)
                delete(f);
        }
        System.out.print(" - deleting: " + d.getAbsolutePath() + "...");
        d.delete();
        System.out.println("ok!");
    }

    private static String baseName() {
        return NativeLibraryManager.jambiTempDirBase("").getName();
    }

    private static File[] cacheDirs() {
        File tmpDirBase = NativeLibraryManager.jambiTempDirBase("");
        File tmpDir = tmpDirBase.getParentFile();
        final String baseName = tmpDirBase.getName();
        File dirs[] = tmpDir.listFiles(new FileFilter() {
            public boolean accept(File path) {
                return path.isDirectory() && path.getName().startsWith(baseName);
            }
        });
        return dirs;
    }

}
