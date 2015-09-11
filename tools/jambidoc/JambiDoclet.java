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

package jambidoc;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.Writer;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import com.sun.javadoc.ClassDoc;
import com.sun.javadoc.ConstructorDoc;
import com.sun.javadoc.DocErrorReporter;
import com.sun.javadoc.ExecutableMemberDoc;
import com.sun.javadoc.LanguageVersion;
import com.sun.javadoc.MethodDoc;
import com.sun.javadoc.PackageDoc;
import com.sun.javadoc.Parameter;
import com.sun.javadoc.RootDoc;
import com.sun.javadoc.SourcePosition;
import com.sun.javadoc.Type;
import com.sun.tools.doclets.formats.html.HtmlDoclet;

/**
 * This Doclet class generates documentation for functions created by
 * the Jambi generator. This doclet is a preliminary solution
 * and may be removed or changed without notice.
 *
 * @exclude
 */
public class JambiDoclet
{
    private static String qtJambiDir = System.getenv().get("JAMBI");
    private static String qtJdocDir = qtJambiDir + "/doc/html/org/qtjambi/qt/";
    private static String jambiDocletFileDir = qtJambiDir + "/tools/jambidoc/files/";
    private static String newLineChar = System.getProperty("line.separator");

    private Map<String, FileWriter> writers = new HashMap<String, FileWriter>();
    private Map<String, String> docFileContents = new HashMap<String, String>();
    private Map<String, String> packageDocs = new HashMap<String, String>();
    private List<String> excludeList = new LinkedList<String>();
    private DocMap jdocFileDocuments = new DocMap();

    public static boolean validOptions(String options[][],
               DocErrorReporter reporter) {
        try {
            return ExcludeDoclet.validOptions(options, reporter);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return false;
    }

    public static int optionLength(String option) {
        return ExcludeDoclet.optionLength(option);
    }

    public static LanguageVersion languageVersion() {
        return HtmlDoclet.languageVersion();
    }

    public static boolean start(RootDoc root)
    {
        JambiDoclet doclet = new JambiDoclet();
        doclet.resolveDocumentation(root);

        boolean ret = false;

        /*
        try {
            HtmlDoclet.start(root);
        } catch (Exception e) {
            e.printStackTrace();
        }*/

        try {
            ret = ExcludeDoclet.start(root);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return ret;
    }

    private void documentClasses(ClassDoc classes[])
    {
        for (ClassDoc currClass : classes) {
            //if (currClass.containingPackage().name().equals("org.qtjambi.qt"))
            //  continue;

            checkForTable (currClass);
            if (!currClass.getRawCommentText().equals("")) {
                currClass.setRawCommentText(fixColon(currClass.getRawCommentText()));
            }

            if ( currClass.superclass() != null && currClass.superclass().name().equals("QFlags")) {
                documentFlagsClass(currClass);
                continue;
            }

            if (isQtEnum(currClass)) {
                String enumStr = currClass.getRawCommentText();
                if (enumStr.equals("")) {
                    currClass.setRawCommentText("Press link for info on "+currClass.name());
                }
                continue;
            }

            if (currClass.containingClass() != null) {
                continue;
            }

            for (ConstructorDoc currConstructor : currClass.constructors()) {
                if (currConstructor.getRawCommentText().equals(""))
                    currConstructor.setRawCommentText(
                            getDocumentation(currConstructor, getSignature(currConstructor)));

                if (currConstructor.getRawCommentText().equals("") &&
                    !hasQtDocumentation(currConstructor))
                    documentFunction(currConstructor);
                    String constrDesc = currConstructor.getRawCommentText();
                    currConstructor.setRawCommentText(fixColon(constrDesc));
            }

            for (MethodDoc currMethod : currClass.methods()) {
                if (currMethod.getRawCommentText().equals(""))
                    currMethod.setRawCommentText(
                            getDocumentation(currMethod, getSignature(currMethod)));

                if (currMethod.getRawCommentText().equals("") &&
                    !hasQtDocumentation(currMethod)) {
                    documentFunction(currMethod);
                    String functionDesc = currMethod.getRawCommentText();
                    currMethod.setRawCommentText(fixColon(functionDesc));
                }
            }
        }
    }

    private void checkForTable(ClassDoc currClass) {
        String comment = currClass.getRawCommentText();
        if (comment.trim().startsWith("<p><table") ||
            comment.trim().startsWith("<table")) {
            currClass.setRawCommentText("Press link for information on "+currClass.name()+". ");
        }
    }

    private void fillPackageDocs()
    {
        packageDocs.put("org.qtjambi.qt", "<body>Classes that implement the Qt Jambi framework.</body>");
        packageDocs.put("org.qtjambi.qt.core", "<body>Core non-GUI classes used by other modules.</body>");
        packageDocs.put("org.qtjambi.qt.gui", "<body>Graphical user interface components.</body>");
        packageDocs.put("org.qtjambi.qt.network", "<body>Classes for network programming.</body>");
        packageDocs.put("org.qtjambi.qt.opengl", "<body>OpenGL support classes.</body>");
        packageDocs.put("org.qtjambi.qt.sql", "<body>Classes for database integration using SQL.</body>");
        packageDocs.put("org.qtjambi.qt.svg", "<body>Classes for displaying the contents of SVG files.</body>");
        packageDocs.put("org.qtjambi.qt.xml", "<body>Classes for handling XML.</body>");
        packageDocs.put("org.qtjambi.qt.designer", "<body>Classes for extending Qt Designer</body>");
    }

    public void resolveDocumentation(RootDoc root)
    {
        fillPackageDocs();

        for (PackageDoc pack : root.specifiedPackages()) {
            String name = pack.name();
            System.err.println("Package name is: "+name);
            String doc = packageDocs.get(name);
            if (doc != null) {
                String packageFile = qtJambiDir + "/" + name.replace(".", "/") + "/package.html";
                //System.err.println("Package file is: " + packageFile);

                try {
                    Writer writer = new FileWriter(packageFile);
                    writer.write(doc);
                    writer.flush();
                    writer.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        setupFileContents(root);
        documentClasses(root.classes());

        for (String key : writers.keySet()) {
            FileWriter writer = writers.get(key);
            try {
                writer.flush();
                writer.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        // We hold this off for now.
        /*try {
            writeCommentsToSource();
        } catch (IOException e) {
            e.printStackTrace();
        }*/
    }

    public String fixColon (String fixMe)
    {
        int colon = fixMe.indexOf(':');
        int dot = fixMe.indexOf('.');

        if (colon != -1) {
            if (dot == -1 || colon < dot) {
                fixMe = fixMe.replaceFirst(":", ".");
            }
        }

        return fixMe;
    }

    private boolean isQtEnum(ClassDoc classDoc)
    {
        for (ClassDoc interf : classDoc.interfaces()) {
            if (interf.name().equals("QtEnumerator"))
                return true;
        }
        return false;
    }

    public void documentFlagsClass(ClassDoc flags)
    {
        StringBuffer doc = new StringBuffer();
        ConstructorDoc cons[] = flags.constructors();
        String enumTypeName = cons[1].parameters()[0].type().qualifiedTypeName();

        doc.append("This QFlag class provides flags for the ");
        doc.append(enumTypeName);
        doc.append(" enum.");

        flags.setRawCommentText(doc.toString());

        cons[0].setRawCommentText("Creates flags with value set as only flag."+newLineChar+
                                  "@param value the value for the flag to be set");

        cons[1].setRawCommentText("Creates flags with the specified "+enumTypeName+" flags set."+newLineChar+
                                  "@param args an array with the values to set");

        if (cons.length > 2)
            cons[2].setRawCommentText("Creates a copy of the specified "+flags.name()+"."+newLineChar+
                                      "@param other the "+flags.name()+" that is to be copied into");
    }

    private void documentFunction(ExecutableMemberDoc method)
    {
        method.setRawCommentText(getDocumentationForFunction(method));
        addComment(method);
    }

    private String getDocumentationForFunction(ExecutableMemberDoc method)
    {
        String documentation = null;
        String str = getDocumentation(method, getSignature(method));

        String signature = method.containingClass().name()+"."+method.name()+method.signature();
        String currentDocs = docFileContents.get(method.containingClass().containingPackage().name());

        int loc = currentDocs.indexOf("$$"+signature);
        if (loc != -1) {
                    int startPos = currentDocs.indexOf("/**", loc + signature.length()) + 3;
                    documentation = currentDocs.substring(startPos,
                                                          currentDocs.indexOf("*/", startPos));
        } else {
            if (method.name().equals("compareTo")) {
                            documentation = "{@inheritDoc}";
            }
            else if (method.isConstructor() && method.parameters().length > 0 &&
                method.parameters()[0].typeName().equals("QtJambiObject.QPrivateConstructor")) {
                documentation = "@exclude";
            }
            else if (method.name().equals("fromNativePointer")) {
                documentation = generateFromNativePointerDocumentation((MethodDoc) method);
            }
            else if (method.name().equals("nativePointerArray")) {
                documentation = generateNativePointerArrayDocumentation((MethodDoc) method);
            }
            else if (method.name().equals("equals")) {
                documentation = "{@inheritDoc}";
            }
            else if (method.name().startsWith("__qt")) {
                documentation = "@exclude";
            }
            else if (method.name().startsWith("readFrom")) {
                documentation = "Reads a "+method.containingClass().name()+" from "
                                +method.parameters()[0].name()+".";
            }
            else if (method.name().equals("writeTo")) {
                documentation = "Writes this"+method.containingClass().name()
                                +" to "+method.parameters()[0].name()+".";
            }
            else if (method.name().equals("toString")) {
                documentation = "Returns a string representation of ";
                if (method.parameters().length > 0) {
                    documentation+=method.parameters()[0].name()+".";
                } else
                    documentation+="this "+method.containingClass().name()+".";
            }
            else if ((method instanceof MethodDoc) &&
                     ((MethodDoc) method).overriddenMethod() != null) {
                documentation = "{@inheritDoc}";
            }
            else if (str != null && !str.equals("")) {
                documentation = str;
            }
            else {
                // This is temporary until the new tool is in place.
                System.err.println("JambiDoclet: Missing docs for "+ getSignature(method));
                documentation = "@exclude";
                /*
                documentation = generateDocForFunction(method);

                writeDocumentation(method.containingClass(),
                        "$$"+signature+newLineChar+"/**"+newLineChar+documentation+"*//*"+newLineChar);*/
            }
        }

        return documentation;
    }

    Map<ClassDoc, List<ExecutableMemberDoc>> comments = new
        HashMap<ClassDoc, List<ExecutableMemberDoc>>();

    public void addComment(ExecutableMemberDoc doc)
    {
        List<ExecutableMemberDoc> list = comments.get(doc.containingClass());

        if (list == null) {
            LinkedList<ExecutableMemberDoc> newList = new LinkedList<ExecutableMemberDoc>();
            comments.put(doc.containingClass(), newList);
            list = newList;
        }

        SourcePosition pos = doc.position();
        if (pos != null && pos.file().exists() && pos.line() != 0) {
            list.add(doc);
        }
    }

    public void writeCommentsToSource() throws IOException
    {
        for (ClassDoc doc : comments.keySet()) {
            List<ExecutableMemberDoc> list = comments.get(doc);

            if (list == null || list.size() < 1) {
                continue;
            }

            Collections.sort (list, new Comparator<ExecutableMemberDoc>() {
                public int compare(ExecutableMemberDoc p1, ExecutableMemberDoc p2) {
                    return p1.position().line() - p2.position().line();
                }});

            File file = doc.position().file();

            ByteArrayOutputStream bout = new ByteArrayOutputStream();
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(bout));
            BufferedReader reader = new BufferedReader(new FileReader(file));

            int i = 0;
            for (ExecutableMemberDoc method : list) {
                for (; i < method.position().line() - 1; ++i)
                    writer.write(reader.readLine() + newLineChar);

                String comment = "/**" + newLineChar + method.getRawCommentText()
                                 + newLineChar + "*/" + newLineChar;

                writer.write(comment);
            }

            String str;
            while ( (str = reader.readLine()) != null)
                writer.write(str + newLineChar);

            writer.flush();
            reader.close();

            FileWriter fileWriter = new FileWriter(doc.position().file());
            fileWriter.write(bout.toString().toCharArray());
            fileWriter.flush();
            fileWriter.close();
        }
    }

    private String generateFromNativePointerDocumentation(MethodDoc method)
    {
        StringBuffer doc = new StringBuffer();
        Parameter param = method.parameters()[0];

        doc.append("This function returns the ");
        doc.append(method.containingClass().name());
        doc.append(" instance pointed to by ");
        doc.append(param.name());
        doc.append(newLineChar);
        doc.append("@param ");
        doc.append(param.name());
        doc.append(" the QNativePointer of which object should be returned.");
        doc.append(newLineChar);
        return doc.toString();
    }

    private String generateNativePointerArrayDocumentation(MethodDoc method)
    {
        StringBuffer doc = new StringBuffer();

        doc.append("This function returns a QNativePointer that is pointing to the specified ");
        doc.append(method.containingClass().name());
        doc.append(" array.");
        doc.append(newLineChar);
        doc.append("@param ");
        doc.append(method.parameters()[0].name());
        doc.append(" the array that the returned pointer will point to.");
        doc.append(newLineChar);
        doc.append("@return a QNativePointer that is pointing to the specified array.");
        doc.append(newLineChar);

        return doc.toString();
    }

    private void setupFileContents(RootDoc root)
    {
        for (PackageDoc pack : root.specifiedPackages()) {
            File docFile = new File(jambiDocletFileDir+"jambidoclet_"+pack.name()+".txt");

            if (docFile.exists()) {
                try {
                    FileReader reader = new FileReader(docFile);
                    StringBuffer buffer = new StringBuffer();

                    char buff[] = new char[4096];
                    while(reader.read(buff) != -1) {
                        buffer.append(buff);
                    }
                    reader.close();
                    docFileContents.put(pack.name(), buffer.toString());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            } else
                docFileContents.put(pack.name(), "");
        }
    }

    private void writeDocumentation(ClassDoc classDoc, String documentation)
    {
        String packageName = classDoc.containingPackage().name();
        FileWriter writer = writers.get(packageName);
        if (writer == null) {
            try {
                File docFile = new File(jambiDocletFileDir+"jambidoclet_"+packageName+".txt");

                writer = new FileWriter(docFile, true);
                writers.put(packageName, writer);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        try {
            writer.write(newLineChar+newLineChar+documentation);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    private String generateDocForFunction(ExecutableMemberDoc method)
    {
        StringBuffer doc = new StringBuffer();

        doc.append("This is a JambiDoclet generated javadoc comment for ");
        doc.append(method.qualifiedName());
        doc.append(".");
        doc.append(newLineChar);
        doc.append(newLineChar);

        for (Parameter param : method.parameters()) {
            doc.append("<tt>");
            doc.append(param.name());
            doc.append("</tt>");
            doc.append(newLineChar);
        }

        for (Type exception : method.thrownExceptionTypes()) {
            doc.append(newLineChar);
            doc.append("@throws ");
            doc.append(exception.qualifiedTypeName());
        }

        doc.append(newLineChar);

        return doc.toString();
    }

    private boolean hasQtDocumentation(ExecutableMemberDoc method)
    {
        ClassDoc classDoc = method.containingClass();
        String className = classDoc.name();
        File jdocFile = new File(qtJdocDir + className.toLowerCase() + ".jdoc");

        if (!classDoc.isOrdinaryClass() || classDoc.containingClass() != null || classDoc.isEnum()
            || !jdocFile.exists())
            return false;

        String str = jdocFileDocuments.getFunctionDocumentation(method, getSignature(method));
        if (str != null && !str.equals(""))
            return true;

        return false;
    }

    public String getDocumentation(ExecutableMemberDoc method, String signature)
    {
        String docs = null;

        try {
            docs = jdocFileDocuments.getFunctionDocumentation(method, signature);
            if (docs != null && !docs.equals(""))
                docs = docs.substring(3, docs.length()-3); //remove /** */
        } catch (Exception e) {
            e.printStackTrace();
        }

        return docs;
    }

    private String getSignature(ExecutableMemberDoc method)
    {
        StringBuffer signature = new StringBuffer();

        signature.append(method.modifiers());
        signature.append(" ");
        if (method.isMethod()) {
            signature.append(((MethodDoc) method).returnType().qualifiedTypeName());
            String dim = ((MethodDoc) method).returnType().dimension();
            if (dim != null && dim.length() > 0)
                signature.append(dim);
            signature.append(" ");
        }
        signature.append(method.name());
        signature.append("(");

        Parameter params[] = method.parameters();
        for (int i = 0; i < params.length; ++i) {
            signature.append(params[i].type().qualifiedTypeName());
            String pdim = params[i].type().dimension();
            if (pdim != null && pdim.length() > 0)
                signature.append(pdim);
            signature.append(" ");
            signature.append(params[i].name());
            if (i + 1 < params.length)
                signature.append(", ");
        }
        signature.append(")");

        return signature.toString();
    }

    class DocMap
    {
        private Map<String, Document> map = new HashMap<String, Document>();

        public Document get(String key)
        {
            Document doc = map.get(key.toLowerCase());

            if (doc == null) {
                File jdocFile = new File(qtJdocDir + key.toLowerCase().replace('_', '-') + ".jdoc");

                if (jdocFile.exists()) {
                    Document document = null;
                    try {
                        document = DocumentBuilderFactory.newInstance()
                                   .newDocumentBuilder().parse(jdocFile);

                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    map.put(key.toLowerCase(), document);
                    doc = document;
                }
            }

            return doc;
        }

        public String getFunctionDocumentation(ExecutableMemberDoc method, String signature)
        {
            Document doc = get(method.containingClass().name());
            Element element;

            if (doc == null)
                return null;

            NodeList nodeList = doc.getElementsByTagName("method");
            for (int i = 0; i < nodeList.getLength(); ++i) {
                element = (Element) nodeList.item(i);
                if (element.getAttribute("name").equals(signature)) {
                    return element.getAttribute("doc");
                }
            }
            return null;
        }
    }
}
