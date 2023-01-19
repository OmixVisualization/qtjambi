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
** Alternatively, this file may be used under the terms of the GNU Lesser
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

package io.qt.tools.ant;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;

abstract class Util {

    public static File LOCATE_EXEC(String name, String prepend, String append) {
        String searchPath = "";

        if(prepend != null && !prepend.equals(""))
            searchPath += prepend + File.pathSeparator;

        String envPath = System.getenv("PATH");
        if(envPath != null)
            searchPath += envPath;

        if(append != null && !append.equals(""))
            searchPath += File.pathSeparator + append;

        StringTokenizer tokenizer = new StringTokenizer(searchPath, File.pathSeparator);
        while(tokenizer.hasMoreTokens()) {
            File exec = new File(tokenizer.nextToken() + File.separator + name);
            //System.out.println("Trying " + exec.getAbsolutePath() + ": " + exec.isFile());
            if(exec.isFile())
                return makeCanonical(exec);
        }

        throw new BuildException("Could not find executable: " + name);
    }

    public static File LOCATE_EXEC(String name, List<String> prependList, String append) {
        String prependString = null;
        if(prependList != null) {
            StringBuffer sb = new StringBuffer();
            for(String s : prependList) {
                if(sb.length() > 0)
                    sb.append(File.pathSeparator);
                sb.append(s);
            }
            prependString = sb.toString();
        }
        return LOCATE_EXEC(name, prependString, append);
    }

    public static void redirectOutput(Process proc) {
        try {
            StreamConsumer std = new StreamConsumer(proc.getInputStream(), System.out);
            StreamConsumer err = new StreamConsumer(proc.getErrorStream(), System.err);
            std.start();
            err.start();
            proc.waitFor();
            std.join();
            err.join();
        } catch(InterruptedException e) {
            e.printStackTrace();
        }
    }

    public static void redirectOutput(Process proc, File outputFile, File errorFile) {
        FileOutputStream outputFileStream = null;
        FileOutputStream errorFileStream = null;
        try {
            try {
                outputFileStream = new FileOutputStream(outputFile);
                errorFileStream = new FileOutputStream(errorFile);
                StreamConsumer std = new StreamConsumer(proc.getInputStream(), new java.io.PrintStream(outputFileStream));
                StreamConsumer err = new StreamConsumer(proc.getErrorStream(), new java.io.PrintStream(errorFileStream));
                std.start();
                err.start();
                proc.waitFor();
                std.join();
                err.join();
            } catch(InterruptedException e) {
                e.printStackTrace();
            }finally{
                if(outputFileStream!=null){
                    outputFileStream.close();
                }
                if(errorFileStream!=null){
                    errorFileStream.close();
                }
            }
        } catch(Exception e) {
            e.printStackTrace();
        }
    }


    public static boolean copy(File src, File dst) throws IOException {
        boolean bf = false;

        File destDir = dst.getParentFile();
        if(!destDir.exists()) {
            if(!destDir.mkdirs())
                throw new IOException("File#mkdirs(\"" + destDir.getAbsolutePath() + "\"): failed");
        }

        InputStream in = null;
        OutputStream out = null;
        try {
            in = new FileInputStream(src);
            out = new FileOutputStream(dst);

            byte buffer[] = new byte[1024 * 8];
            int n;
            while((n = in.read(buffer)) > 0) {
                out.write(buffer, 0, n);
            }

            out.close();
            out = null;
            in.close();
            in = null;

            bf = true;
        } finally {
            if(out != null) {
                try {
                    out.close();
                } catch(IOException eat) {
                }
                out = null;
            }
            if(in != null) {
                try {
                    in.close();
                } catch(IOException eat) {
                }
                in = null;
            }
        }
        return bf;
    }


    public static void copyRecursive(File src, File target) throws IOException {
        if(src.isDirectory()) {
            File entries[] = src.listFiles();
            for(File e : entries) {
                copyRecursive(e, new File(target, e.getName()));
            }
        } else {
            copy(src, target);
        }
    }

    public static File findInPath(String name) {
        String PATH[] = System.getenv("PATH").split(File.pathSeparator);
        for(String p : PATH) {
            File f = new File(p, name);
            if(f.exists())
                return f;
        }
        return null;
    }

    public static File findInLibraryPath(String name, String javaLibDir) {
        String libraryPath;
        if(javaLibDir != null) {
            libraryPath = javaLibDir;
        } else {
            libraryPath = System.getProperty("java.library.path");
        }
        //System.out.println("library path is: " + libraryPath);

        // Make /usr/lib an implicit part of library path
        if(OSInfo.os() == OSInfo.OS.Linux) {
            String archName = OSInfo.crossOSArchName();
            boolean match = false;
            if(archName.equals(OSInfo.K_LINUX_X86)) {
                // (some non-FHS) Linux 32bit might have lib32 directory most Linux
                //  distros (FHS compliant) will not have a /usr/lib32.
                File lib32Dir = new File("/usr/lib32");
                if(lib32Dir.exists() && lib32Dir.isDirectory()) {
                    libraryPath += File.pathSeparator + lib32Dir.getAbsolutePath();
                    match = true;
                }
            } else if(archName.equals(OSInfo.K_LINUX_X64)) {
                // Linux 64bit
                libraryPath += File.pathSeparator + "/usr/lib64";
                match = true;
            }
            if(!match) {
                // Normal Unix
                libraryPath += File.pathSeparator + "/usr/lib";
            }
        }

        String PATH[] = libraryPath.split(File.pathSeparator);
        for(String p : PATH) {
            File f = new File(p, name);
            if(f.exists()) {
                // FIXME: We should check the path/file we pickup relates to the 32bit
                //  or 64bit we are building for.  i.e. weed out invalid matches due
                //  32/64 mismatch.
                // We should emit warning if we skipped anything (out of order) to find
                //  a match also if the only match is known to be wrong.
                return f;
            }
        }
        return null;
    }


    public static File makeCanonical(String file) throws BuildException {
        if(file == null)
            return null;
        return makeCanonical(new File(file));
    }

    public static File makeCanonical(File file) throws BuildException {
        if(file == null)
            return null;
        try {
            return file.getCanonicalFile();
        } catch(IOException e) {
            e.printStackTrace();
            throw new BuildException("Path : " + file.getAbsolutePath() + " failed to create canonical form.", e);
        }
    }

    public static String arrayToString(Object[] array) {
        StringBuffer sb = new StringBuffer();
        sb.append("[");
        boolean first = true;
        for(Object o : array) {
            if(first)
                first = false;
            else
                sb.append(", ");
            sb.append("\"");
            sb.append(o.toString());
            sb.append("\"");
        }
        sb.append("]");
        return sb.toString();
    }

    public static String safeArrayToString(Object[] array) {
        if(array == null)
            return null;
        return arrayToString(array);
    }

    public static String stripLeadingAndTrailingWhitespace(String s) {
        final int len = s.length();
        int lastContigiousWhitespaceIndex = -1;
        int state = 0;
        StringBuffer sb = new StringBuffer();
        for(int i = 0; i < len; i++) {
            char c = s.charAt(i);

            boolean isSpace;
            if(c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == '\013' || c == '\f')
                isSpace = true;
            else
                isSpace = false;

            if(state == 0) {    // leading
                if(isSpace)
                    continue;   // skip
                else
                    state = 1;  // next-state && emit
            } else {
                // always emit as we truncate accordingly at end
                if(isSpace) {
                    if(lastContigiousWhitespaceIndex < 0)
                        lastContigiousWhitespaceIndex = i;
                } else {
                    lastContigiousWhitespaceIndex = -1;
                }
            }
            sb.append(c);
        }
        if(lastContigiousWhitespaceIndex >= 0)
            s = sb.substring(0, lastContigiousWhitespaceIndex);
        else
            s = sb.toString();
        return s;
    }

    public static String arrayJoinToString(Object[] array, CharSequence delim) {
        StringBuffer sb = new StringBuffer();
        boolean first = true;
        for(Object o : array) {
            if(first)
                first = false;
            else
                sb.append(delim);
            sb.append(o.toString());
        }
        return sb.toString();
    }

    public static String safeArrayJoinToString(Object[] array, CharSequence delim) {
        if(array == null)
            return null;
        return arrayJoinToString(array, delim);
    }

    public static List<String> splitStringTokenizer(String s) {
        List<String> list = new ArrayList<String>();
        StringTokenizer tokenizer = new StringTokenizer(s);
        while(tokenizer.hasMoreTokens())
            list.add(tokenizer.nextToken());
        return list;
    }

    public static List<String> safeSplitStringTokenizer(String s) {
        if(s == null)
            return null;
        return splitStringTokenizer(s);
    }

    // FIXME: This is implement enough for our needs only.  See perlre quotemeta()
    public static String regexEscape(String s) {
        StringBuilder sb = new StringBuilder();
        final int len = s.length();
        for(int i = 0; i < len; i++) {
            char c = s.charAt(i);
            switch(c) {
            case '\\':
                sb.append("\\\\");
                break;

            default:
                sb.append(c);
                break;
            }
        }
        return sb.toString();
    }

    public static String pathCanon(String[] sA, String separator) {
        if(sA == null)
            return null;
        StringBuilder sb = new StringBuilder();
        for(String s : sA) {
            if(s == null)
                continue;
            // Split by "/" since this is a path expressed in XML
            String[] ssA = s.split(regexEscape(separator));
            for(String ss : ssA) {
                if(ss.length() == 0)
                    continue;
                if(sb.length() > 0)
                    sb.append(separator);
                sb.append(ss);
            }
        }
        return sb.toString();
    }

    public static String pathCanon(String[] sA) {
        return pathCanon(sA, File.separator);
    }

    public static void emitDebugLog(Project project, String[] sA) {
        if(sA == null)
            return;
        for(String s : sA) {
            project.log(s, Project.MSG_DEBUG);
        }
    }

    public static String firstWord(String s) {
        final int l = s.length();
        int i = 0;
        while(i < l) {  // skip leading ASCII whitespace
            char c = s.charAt(i);
            if(c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\f') {
                i++;
                continue;
            }
            break;
        }
        int startOffset = i;
        while(i < l) {  // consume until next ASCII whitespace or end of string
            char c = s.charAt(i);
            if(c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\f')
                break;
            i++;
        }
        int endOffset = i;
        return s.substring(startOffset, endOffset);
    }

    public static String safeFirstWord(String s) {
        if(s == null)
            return null;
        return firstWord(s);
    }

}
