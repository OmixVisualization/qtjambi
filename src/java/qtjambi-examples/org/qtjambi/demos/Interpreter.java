/****************************************************************************
 **
 ** Copyright (C) 2007-2009 Nokia. All rights reserved.
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

package org.qtjambi.demos;

import java.util.Date;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Stack;
import java.util.StringTokenizer;
import java.util.Vector;

import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;

public class Interpreter {
    protected Vector<Function> infixFunctions = new Vector<Function>();
    public java.util.Hashtable<String, Function> functions = new Hashtable<String, Function>();

    public Interpreter() {
        Function function = new Function("time") {
            @Override
            public Object result(Object[] args) throws ParseException {
                checkNumberOfArguments(0, args);
                return new Date();
            }
        };
        functions.put(function.name, function);

        function = new Function("abs") {
            @Override
            public Object result(Object[] args) throws ParseException {
                checkNumberOfArguments(1, args);
                return Math.abs(Double.parseDouble(args[0].toString()));
            }
        };
        functions.put(function.name, function);

        function = new Function("pow") {
            @Override
            public Object result(Object[] args) throws ParseException {
                checkNumberOfArguments(2, args);
                return Math.pow(Double.parseDouble(args[0].toString()), Double.parseDouble(args[1].toString()));
            }
        };
        functions.put(function.name, function);

        function = new Function("cos") {
            @Override
            public Object result(Object[] args) throws ParseException {
                checkNumberOfArguments(1, args);
                return Math.cos(Double.parseDouble(args[0].toString()));
            }
        };

        functions.put(function.name, function);

        function = new Function("sin") {
            @Override
            public Object result(Object[] args) throws ParseException {
                checkNumberOfArguments(1, args);
                return Math.sin(Double.parseDouble(args[0].toString()));
            }
        };

        functions.put(function.name, function);

        function = new Function("random") {
            @Override
            public Object result(Object[] args) throws ParseException {
                checkNumberOfArguments(0, args);
                return Math.random();
            }
        };
        functions.put(function.name, function);

        function = new Function("min") {
            @Override
            public Object result(Object[] args) {
                double minimum = Double.parseDouble(args[0].toString());
                for (int i = 1; i < args.length; i++) {
                    minimum = Math.min(minimum, Double.parseDouble(args[i].toString()));
                }
                return minimum;
            }
        };
        functions.put(function.name, function);

        infixFunctions.add(new Function("*") {
            @Override
            public Object result(Object[] args) {
                double product = Double.parseDouble(args[0].toString());
                for (int i = 1; i < args.length; i++) {
                    product *= Double.parseDouble(args[i].toString());
                }
                return product;
            }
        });
        infixFunctions.add(new Function("/") {
            @Override
            public Object result(Object[] args) {
                double quotient = Double.parseDouble(args[0].toString());
                for (int i = 1; i < args.length; i++) {
                    quotient /= Double.parseDouble(args[i].toString());
                }
                return quotient;
            }
        });

        infixFunctions.add(new Function("-") {
            @Override
            public Object result(Object[] args) {
                double difference = Double.parseDouble(args[0].toString());
                for (int i = 1; i < args.length; i++) {
                    difference -= Double.parseDouble(args[i].toString());
                }
                return difference;
            }
        });

        infixFunctions.add(new Function("+") {
            @Override
            public Object result(Object[] args) {
                double sum = 0;
                for (int i = 0; i < args.length; i++) {
                    sum += Double.parseDouble(args[i].toString());
                }
                return sum;
            }
        });
    }

    public Object parseAndEvaluate(Object o) throws ParseException {
        if (o instanceof String) {
            return evaluate(parse((String) o));
        } else
            return o;
    }

    @SuppressWarnings("unchecked")
    public Object evaluate(Object o) throws ParseException {
        if (o instanceof Vector) {
            Vector<Object> vector = (Vector<Object>) o;
            if (vector.isEmpty())
                return 0;
            if (vector.size() > 1)
                throw new ParseException("Found multiple values when expecting one: " + o);
            return evaluate(vector.firstElement());
        }

        else if (o instanceof Function) {
            Function function = (Function) o;
            return function.evaluateFunction();
        }

        return o;
    }

    @SuppressWarnings("unchecked")
    public Vector<Object> parse(String expression) throws ParseException {
        Stack<Vector<Object>> stack = new Stack<Vector<Object>>();
        stack.push(new Vector<Object>());

        String delimiter = "()";
        for (Iterator<Function> iterator = infixFunctions.iterator(); iterator.hasNext();) {
            Function function = (Function) iterator.next();
            delimiter += function.name;

        }

        StringTokenizer tokenizer = new StringTokenizer(expression.trim(), delimiter, true);

        while (tokenizer.hasMoreTokens()) {
            String token = tokenizer.nextToken();
            if (token.equals("(")) {
                stack.peek().add(new Vector<Object>());
                stack.push((Vector<Object>) stack.peek().lastElement());
            } else if (token.equals(")")) {
                if (stack.isEmpty())
                    throw new ParseException("Missing starting parenthesis");
                prioritize(stack.pop());

            } else {
                String[] tmp = RetroTranslatorHelper.split(token.trim(), " ");
                for (int i = 0; i < tmp.length; i++) {
                    if (stack.peek() == null) {
                        throw new ParseException("Missing left side parenthesis");
                    }
                    if (!tmp[i].equals(""))
                        stack.peek().add(tmp[i]);
                }
            }
        }
        if (stack.isEmpty())
            throw new ParseException("Not enough staring parantheses");
        prioritize(stack.peek());
        if (stack.size() > 1)
            throw new ParseException("Not enough closing parentheses");
        return stack.peek();
    }

    @SuppressWarnings("unchecked")
    private void prioritize(Vector<Object> vector) throws ParseException {

        Function unaryMinusProt = new Function("unaryMinus") {
            @Override
            public Object result(Object[] args) {
                return -Double.parseDouble(args[0].toString());
            }
        };

        Object[] vectorArray = vector.toArray();
        int r = 0;
        for (int i = 0; i < vectorArray.length; i++) {
            Function function = functions.get(vectorArray[i].toString().toUpperCase());

            if (function != null) {
                function = (Function) function.clone();
                if ((i - r + 1) >= vectorArray.length)
                    throw new ParseException("Could not find parameters for function: " + function.name);
                if (vectorArray[i + 1] instanceof Vector)
                    function.arguments.addAll((Vector<Object>) vectorArray[i + 1]);
                vector.remove(i - r + 1);
                vector.set(i - r, function);
                r += 1;
            }
        }

        for (Iterator<Function> iterator = infixFunctions.iterator(); iterator.hasNext();) {
            Function function = (Function) iterator.next();

            vectorArray = vector.toArray();

            r = 0;
            for (int i = 0; i < vectorArray.length; i++) {
                Object element = vectorArray[i];
                if (element instanceof String) {
                    if (element.equals(function.name)) {
                        function = (Function) function.clone();
                        if ((i - r - 1 < 0 && !element.equals("-")) || i - r + 1 >= vector.size())
                            throw new ParseException("Problems at infix function:" + function.name);

                        if (i - r - 1 < 0) {
                            Function minus = (Function) unaryMinusProt.clone();
                            minus.arguments.add(vector.elementAt(i - r + 1));
                            vector.set(i - r, minus);
                            vector.remove(i - r + 1);
                            r += 1;
                            i++;
                        } else {
                            function.arguments.add(vector.elementAt(i - r - 1));

                            if (vector.elementAt(i - r + 1).equals("-")) {

                                Function minus = (Function) unaryMinusProt.clone();
                                minus.arguments.add(vector.elementAt(i - r + 2));
                                function.arguments.add(minus);

                                vector.set(i - r, function);
                                vector.remove(i - r + 2);
                                vector.remove(i - r + 1);
                                vector.remove(i - r - 1);
                                r += 3;
                                i++;

                            } else {

                                function.arguments.add(vector.elementAt(i - r + 1));

                                vector.set(i - r, function);
                                vector.remove(i - r + 1);
                                vector.remove(i - r - 1);
                                r += 2;
                                i++;
                            }
                        }
                    }
                }
            }
        }
    }

    protected abstract class Function implements Cloneable {
        protected Vector<Object> arguments;
        protected String name;

        public Function(String name) {
            this.name = name.toUpperCase();
        }

        public String getName() {
            return name;
        }

        @Override
        public String toString() {
            String signature = "";
            signature += "{function_" + name + "_ ";
            for (Iterator<Object> iterator = arguments.iterator(); iterator.hasNext();) {
                signature += iterator.next();
                if (iterator.hasNext())
                    signature += " ";
            }
            signature += "}";
            return signature;
        }

        @Override
        protected Object clone() {
            Function function = null;
            try {
                function = (Function) super.clone();
                function.arguments = new Vector<Object>();
            } catch (CloneNotSupportedException exception) {
                exception.printStackTrace();
            }

            return function;
        }

        public Object evaluateFunction() throws ParseException {
            Object[] args = new Object[arguments.size()];
            int i = 0;
            for (Iterator<Object> iterator = arguments.iterator(); iterator.hasNext();) {
                args[i] = evaluate(iterator.next());
                i++;
            }
            return result(args);
        }

        protected void checkNumberOfArguments(int size, Object[] args) throws ParseException {
            if (args.length != size)
                throw new ParseException("Wrong number of arguments to function " + name + ": Expected " + size + ".");
        }

        public abstract Object result(Object[] args) throws ParseException;
    }

    public static class ParseException extends Exception {
        private static final long serialVersionUID = 1L;

        public ParseException(String error) {
            super(error);
        }
    }
}
