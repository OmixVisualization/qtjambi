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

import com.sun.javadoc.*;
import com.sun.tools.doclets.standard.Standard;
import com.sun.tools.javadoc.Main;
import java.lang.reflect.*;
import java.util.ArrayList;
import java.util.List;

// Thanks to Chris Nokleberg for releasing this Doclet
// in the public domain.
public class ExcludeDoclet
{
    public static void main(String[] args)
    {
        String name = ExcludeDoclet.class.getName();
        Main.execute(name, name, args);
    }

    public static boolean validOptions(String[][] options, DocErrorReporter reporter)
    throws java.io.IOException
    {
        return Standard.validOptions(options, reporter);
    }

    public static int optionLength(String option)
    {
        return Standard.optionLength(option);
    }

    public static boolean start(RootDoc root)
    throws java.io.IOException
    {
        return Standard.start((RootDoc)process(root, RootDoc.class));
    }

    private static boolean exclude(Doc doc)
    {
        if (doc instanceof ProgramElementDoc) {
            if (((ProgramElementDoc)doc).containingPackage().tags("exclude").length > 0)
                return true;
        }
        return doc.tags("exclude").length > 0;
    }

    private static Object process(Object obj, Class expect)
    {
        if (obj == null)
            return null;
        Class cls = obj.getClass();
        if (cls.getName().startsWith("com.sun.")) {
            return Proxy.newProxyInstance(cls.getClassLoader(),
                                          cls.getInterfaces(),
                                          new ExcludeHandler(obj));
        } else if (obj instanceof Object[]) {
            Class componentType = expect.getComponentType();
            Object[] array = (Object[])obj;
            List list = new ArrayList(array.length);
            for (int i = 0; i < array.length; i++) {
                Object entry = array[i];
                if ((entry instanceof Doc) && exclude((Doc)entry))
                    continue;
                list.add(process(entry, componentType));
            }
            return list.toArray((Object[])Array.newInstance(componentType, list.size()));
        } else {
            return obj;
        }
    }

    private static class ExcludeHandler
    implements InvocationHandler
    {
        private Object target;

        public ExcludeHandler(Object target)
        {
            this.target = target;
        }

        public Object invoke(Object proxy, Method method, Object[] args)
        throws Throwable
        {
            if (args != null) {
                String methodName = method.getName();
                if (methodName.equals("compareTo") ||
                    methodName.equals("equals") ||
                    methodName.equals("overrides") ||
                    methodName.equals("subclassOf")) {
                    args[0] = unwrap(args[0]);
                }
            }
            try {
                return process(method.invoke(target, args), method.getReturnType());
            } catch (InvocationTargetException e) {
                throw e.getTargetException();
            }
        }

        private Object unwrap(Object proxy)
        {
            if (proxy instanceof Proxy)
                return ((ExcludeHandler)Proxy.getInvocationHandler(proxy)).target;
            return proxy;
        }
    }
}
