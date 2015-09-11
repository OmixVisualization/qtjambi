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

package org.qtjambi.qt.internal;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.lang.reflect.Field;
import java.lang.reflect.GenericArrayType;
import java.lang.reflect.Method;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;

import org.qtjambi.qt.QNativePointer;
import org.qtjambi.qt.QSignalEmitter;
import org.qtjambi.qt.QtJambiGeneratedClass;
import org.qtjambi.qt.QtProperty;
import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QDataStream;
import org.qtjambi.qt.core.QEvent;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QRegExp;
import org.qtjambi.qt.internal.QSignalEmitterInternal.AbstractSignalInternal.ReflectiveConnection;
import org.qtjambi.qt.internal.QSignalEmitterInternal.AbstractSignalInternal.SlotObjectConnection;
import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;
import org.qtjambi.qt.widgets.QMessageBox;

public class QtJambiInternal {

    public static final char SlotPrefix = '1';
    public static final char SignalPrefix = '2';

    public static void setupDefaultPluginPath() {
        try {
            if (org.qtjambi.qt.internal.NativeLibraryManager.isUsingDeploymentSpec()) {
                List<String> paths = new ArrayList<String>();

                List<String> pluginPaths = org.qtjambi.qt.internal.NativeLibraryManager.pluginPaths();
                if (pluginPaths != null)
                    paths.addAll(pluginPaths);

                List<String> pluginDesignerPaths = org.qtjambi.qt.internal.NativeLibraryManager.pluginDesignerPaths();
                if (pluginDesignerPaths != null)
                    paths.addAll(pluginDesignerPaths);

                // We don't override the existing values (which are based on envvars)
                //  as envvars should continue to take priority even for Java Qt use.
                if (paths.size() > 0) {
                    Collections.reverse(paths);
                    for (String p : paths)
                        QCoreApplication.addLibraryPath(p);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     *  @exclude
     */
    static class QMetaCallEvent extends QEvent {

        public static final QEvent.Type MetaCallEventType = QEvent.Type
                .resolve(512);

        private QObject sender;

        public QMetaCallEvent(QSignalEmitter.AbstractSignalInternal.Connection connection, QObject sender,
                Object... arguments) {
            super(MetaCallEventType);
            this.arguments = arguments;
            this.connection = connection;
            this.sender = sender;
        }

        public final Object[] getArguments() {
            return arguments;
        }

        public final void setArguments(Object[] arguments) {
            this.arguments = arguments;
        }

        public final QSignalEmitter.AbstractSignalInternal.Connection getConnection() {
            return connection;
        }

        public final void setConnection(QSignalEmitter.AbstractSignalInternal.ReflectiveConnection connection) {
            this.connection = connection;
        }

        @SuppressWarnings({ "unchecked", "rawtypes" })
		final void execute() {
            boolean updateSender = sender != null;
            long oldSender = 0;
            QSignalEmitterInternal oldEmitter = null;
            Object resolvedReceiver = connection.resolveReceiver();
            if (resolvedReceiver == null)
                return;
            Object lambdaOwner = null;
            if(connection instanceof SlotObjectConnection){
            	lambdaOwner = ((SlotObjectConnection) connection).lambdaOwner();
            }
            if (updateSender) {
            	if(lambdaOwner instanceof QObject){
                    oldSender = QtJambiInternal.setQObjectSender(((QObject) lambdaOwner).nativeId(),
                            sender.nativeId());
            	}else{
                    oldSender = QtJambiInternal.setQObjectSender(((QObject) resolvedReceiver).nativeId(),
                            sender.nativeId());
            	}
                oldEmitter = QSignalEmitterInternal.currentSender.get();
                QSignalEmitterInternal.currentSender.set(sender);
            }
            try {
	            if(connection instanceof ReflectiveConnection){
	            	ReflectiveConnection methodConnection = (ReflectiveConnection)connection;
		            try {
		            	methodConnection.slot.invoke(resolvedReceiver, arguments);
		            } catch (IllegalAccessException e) {
		                QtJambiInternal.invokeSlot(resolvedReceiver, methodConnection.slotId,
		                		methodConnection.returnType,
		                		methodConnection.args, methodConnection.convertTypes);
		            } catch (Exception e) {
		                System.err.printf("Exception while executing queued connection: sender=%s, receiver=%s %s\n",
		                        sender != null ? sender.getClass().getName() : "N/A",
		                        resolvedReceiver,
		                        methodConnection.slot.toString());
		                if (e.getCause() != null)
		                    e.getCause().printStackTrace();
		                else
		                    e.printStackTrace();
		            }
	            }else if(connection instanceof SlotObjectConnection){
	            	SlotObjectConnection slotObjectConnection = (SlotObjectConnection)connection;
	            	try {
	            		switch(slotObjectConnection.args.length){
                    	case 0:
                   			((QSignalEmitter.Slot0) resolvedReceiver).invoke();
                    		break;
                    	case 1:
                   			((QSignalEmitter.Slot1) resolvedReceiver).invoke(
                   					slotObjectConnection.args[0]
               					);
                    		break;
                    	case 2:
                   			((QSignalEmitter.Slot2) resolvedReceiver).invoke(
                   					slotObjectConnection.args[0], slotObjectConnection.args[1]
               					);
                    		break;
                    	case 3:
                   			((QSignalEmitter.Slot3) resolvedReceiver).invoke(
                   					slotObjectConnection.args[0], slotObjectConnection.args[1], slotObjectConnection.args[2]
               					);
                    		break;
                    	case 4:
                   			((QSignalEmitter.Slot4) resolvedReceiver).invoke(
                   					slotObjectConnection.args[0], slotObjectConnection.args[1], slotObjectConnection.args[2], 
                   					slotObjectConnection.args[3]
               					);
                    		break;
                    	case 5:
                   			((QSignalEmitter.Slot5) resolvedReceiver).invoke(
                   					slotObjectConnection.args[0], slotObjectConnection.args[1], slotObjectConnection.args[2], 
                   					slotObjectConnection.args[3], slotObjectConnection.args[4]
               					);
                    		break;
                    	case 6:
                   			((QSignalEmitter.Slot6) resolvedReceiver).invoke(
                   					slotObjectConnection.args[0], slotObjectConnection.args[1], slotObjectConnection.args[2], 
                   					slotObjectConnection.args[3], slotObjectConnection.args[4], slotObjectConnection.args[5]
               					);
                    		break;
                    	case 7:
                   			((QSignalEmitter.Slot7) resolvedReceiver).invoke(
                   					slotObjectConnection.args[0], slotObjectConnection.args[1], slotObjectConnection.args[2], 
                   					slotObjectConnection.args[3], slotObjectConnection.args[4], slotObjectConnection.args[5], 
                   					slotObjectConnection.args[6]
               					);
                    		break;
                    	case 8:
                   			((QSignalEmitter.Slot8) resolvedReceiver).invoke(
                   					slotObjectConnection.args[0], slotObjectConnection.args[1], slotObjectConnection.args[2], 
                   					slotObjectConnection.args[3], slotObjectConnection.args[4], slotObjectConnection.args[5], 
                   					slotObjectConnection.args[6], slotObjectConnection.args[7]
               					);
                    		break;
                    	case 9:
                   			((QSignalEmitter.Slot9) resolvedReceiver).invoke(
                   					slotObjectConnection.args[0], slotObjectConnection.args[1], slotObjectConnection.args[2], 
                   					slotObjectConnection.args[3], slotObjectConnection.args[4], slotObjectConnection.args[5], 
                   					slotObjectConnection.args[6], slotObjectConnection.args[7], slotObjectConnection.args[8]
               					);
                    		break;
                    	}
	            	} catch (Exception e) {
		                System.err.printf("Exception while executing queued connection: sender=%s, receiver=%s\n",
		                        sender != null ? sender.getClass().getName() : "N/A",
		                        resolvedReceiver);
		                if (e.getCause() != null)
		                    e.getCause().printStackTrace();
		                else
		                    e.printStackTrace();
		            }
	            }
            } finally {
                if (updateSender) {
                	if(lambdaOwner instanceof QObject){
                		QtJambiInternal.resetQObjectSender(((QObject) lambdaOwner).nativeId(), oldSender);
                	}else{
	                    QtJambiInternal.resetQObjectSender(((QObject) resolvedReceiver).nativeId(), oldSender);
                	}
                    QSignalEmitterInternal.currentSender.set(oldEmitter);
                }
            }
        }

        private Object arguments[];
        private QSignalEmitterInternal.AbstractSignalInternal.Connection connection;
    }

    public static int[] resolveConversionSchema(Class<?> inputParameterTypes[], Class<?> outputParameterTypes[]) {
        int returned[] = new int[outputParameterTypes.length];
        for (int i = 0; i < returned.length; ++i) {
            returned[i] = 'L';
            if (outputParameterTypes[i].isPrimitive())
                returned[i] = QtJambiInternal.primitiveToByte(inputParameterTypes[i]);
        }

        return returned;
    }

    public static byte typeConversionCode(Class<?> cls) {
        if (cls.isPrimitive())
            return QtJambiInternal.primitiveToByte(cls);
        else
            return 'L';
    }

    static Class<?> getComplexType(Class<?> primitiveType) {
        if (!primitiveType.isPrimitive())
            throw new RuntimeException("Primitive type required");

        if (primitiveType.equals(Integer.TYPE))
            return Integer.class;
        else if (primitiveType.equals(Double.TYPE))
            return Double.class;
        else if (primitiveType.equals(Long.TYPE))
            return Long.class;
        else if (primitiveType.equals(Float.TYPE))
            return Float.class;
        else if (primitiveType.equals(Short.TYPE))
            return Short.class;
        else if (primitiveType.equals(Boolean.TYPE))
            return Boolean.class;
        else if (primitiveType.equals(Character.TYPE))
            return Character.class;
        else if (primitiveType.equals(Byte.TYPE))
            return Byte.class;
        else
            throw new RuntimeException("Unrecognized primitive type: "
                    + primitiveType);
    }

    public static byte primitiveToByte(Class<?> primitiveType) {
        if (primitiveType.equals(Integer.class) || primitiveType.equals(Integer.TYPE)) {
            return 'I';
        } else if (primitiveType.equals(Long.class) || primitiveType.equals(Long.TYPE)) {
            return 'J';
        } else if (primitiveType.equals(Short.class) || primitiveType.equals(Short.TYPE)) {
            return 'S';
        } else if (primitiveType.equals(Boolean.class) || primitiveType.equals(Boolean.TYPE)) {
            return 'Z';
        } else if (primitiveType.equals(Byte.class) || primitiveType.equals(Byte.TYPE)) {
            return 'B';
        } else if (primitiveType.equals(Float.class) || primitiveType.equals(Float.TYPE)) {
            return 'F';
        } else if (primitiveType.equals(Double.class) || primitiveType.equals(Double.TYPE)) {
            return 'D';
        } else if (primitiveType.equals(Character.class) || primitiveType.equals(Character.TYPE)) {
            return 'C';
        } else if (primitiveType.equals(Void.class) || primitiveType.equals(Void.TYPE)) {
            return 'V';
        } else {
            throw new RuntimeException(
                    "Error in conversion to primitive for complex type "
                            + primitiveType);
        }
    }

    public static QSignalEmitterInternal.AbstractSignalInternal lookupSignal(QSignalEmitterInternal signalEmitter, String name)
    {
        if (name == null || signalEmitter == null) {
            System.err.println("lookupSignal: Name or object is null");
            return null;
        }

        QSignalEmitterInternal.AbstractSignalInternal returned = null;
        for (Class<?> cls = signalEmitter.getClass();
             QSignalEmitterInternal.class.isAssignableFrom(cls) && returned == null;
             cls = cls.getSuperclass()) {

            Field[] fields = cls.getDeclaredFields();
            Field f = findField(fields, name);
            if(f == null)
                continue;
            //no inspection EmptyCatchBlock
            try {
                f.setAccessible(true);
            } catch (SecurityException e) {
            }

            if (QSignalEmitterInternal.AbstractSignalInternal.class.isAssignableFrom(f.getType())) {
                try {
                    returned = (QSignalEmitterInternal.AbstractSignalInternal) f.get(signalEmitter);
                } catch (Exception e) {
                    returned = fetchSignal(signalEmitter, f);
                }
            }
        }

        return returned;
    }

    public static Method lookupSlot(Object object, String signature) {
        return lookupSlot(object, signature, false);
    }

    public static Method lookupSlot(Object object, String signature, boolean slotMustBeGenerated) {
        Class<?> cls = slotMustBeGenerated ? findGeneratedSuperclass(object) : object.getClass();

        int pos = signature.indexOf('(');
        if (pos < 0) {
            throw new RuntimeException("Wrong syntax in slot signature: '"
                                       + signature + "'");
        }
        int spacePos = signature.trim().lastIndexOf(' ', pos);
        if (pos > spacePos && spacePos > 0) {
            throw new RuntimeException(
                    "Do not specify return type in slot signature: '"
                            + signature + "'");
        }

        String name = signature.substring(0, pos).trim();

        int pos2 = signature.indexOf(')', pos);
        if (pos2 < 0) {
            throw new RuntimeException("Wrong syntax in slot signature: '"
                                       + signature + "'");
        }
        String strTypes = signature.substring(pos + 1, pos2).trim();

        String argumentTypes[];

        if (strTypes.length() == 0)
            argumentTypes = new String[0];
        else
            argumentTypes = RetroTranslatorHelper.split(strTypes, ",");

        for (int i = 0; i < argumentTypes.length; ++i)
            argumentTypes[i] = argumentTypes[i].replace(" ", "");

        return findFunctionRecursive(cls, name, argumentTypes);
    }

    private static Method findFunctionRecursive(Class<?> cls, String functionName,
            String argumentTypes[]) {
        Method methods[] = cls.getDeclaredMethods();

        for (Method m : methods) {
            boolean found;
            if (!m.getName().equals(functionName))
                continue;

            Class<?> a[] = m.getParameterTypes();
            if (a.length != argumentTypes.length)
                continue;

            found = true;
            for (int i = 0; i < a.length; ++i) {
                String arg = a[i].getName();

                Class<?> t = a[i];

                if(t.isArray()){
                    String brackets = "";

                    do {
                        t = t.getComponentType();
                        brackets += "[]";
                    }
                    while(t.isArray());

                    arg = t.getName() + brackets;
                }

                if (argumentTypes[i].indexOf('.') < 0) {
                    arg = arg.substring(arg.lastIndexOf('.') + 1);
                }

                String argLenient = a[i].isMemberClass() ? arg.replace('$', '.') : null;
                if (!arg.equals(argumentTypes[i]) && (argLenient == null || !argLenient.equals(argumentTypes[i]))) {
                    found = false;
                    break;
                }
            }

            if (found)
                return m;
        }

        cls = cls.getSuperclass();
        if (cls == null)
            return null;
        else
            return findFunctionRecursive(cls, functionName, argumentTypes);
    }

    /**
     * Sets the current QObject sender for receiver to sender. A
     * function call to setQObjectSender must always be followed by a
     * call to resetQObjectSender where the data returned from set is
     * passed to reset.
     * Package visibility due to handling of raw memory pointers in
     *  long type.
     * @param receiver The receiver QObject
     * @param sender The sender QObject
     * @return A magic data to be used in the following reset call.
     */
    static native long setQObjectSender(long receiver, long sender);

    /**
     * Resets the current sender for the object in receiver.
     * Package visibility due to handling of raw memory pointers in
     *  long type.
     * @param receiver The receiver QObject.
     * @param data A magic value which must be the return value from
     * the recent setQObjectSender call.
     */
    static native void resetQObjectSender(long receiver, long data);

    private static boolean signalMatchesSlot(String signal, String slot) {
        // void slots always match...
        if (slot.contains("()"))
            return true;

        int signalIndex = signal.indexOf('<');

        // Match only if () slot which is covered above already...
        if (signalIndex < 0)
            return false;

        int slotIndex = slot.indexOf('(');
        if (slotIndex < 0) {
            throw new IllegalArgumentException("slot doesn't contain () as expected, '"
                                               + slot + "'");
        }

        String substr = signal.substring(signalIndex + 1, signal.length() - 1);
        String signalArguments[] = RetroTranslatorHelper.split(substr, ",");
        substr = slot.substring(slotIndex + 1, slot.length() - 1);
        String slotArguments[] = RetroTranslatorHelper.split(substr, ",");

        if (slotArguments.length > signalArguments.length)
            return false;

        for (int i=0; i<slotArguments.length; ++i) {
            if (!matchTypes(signalArguments[i], slotArguments[i]))
                return false;
        }

        return true;
    }


    private static HashMap<String, String> typeMap;
    static {
        typeMap = new HashMap<String, String>();
        typeMap.put("java.lang.Boolean", "boolean");
        typeMap.put("java.lang.Byte", "byte");
        typeMap.put("java.lang.Char", "char");
        typeMap.put("java.lang.Short", "short");
        typeMap.put("java.lang.Integer", "int");
        typeMap.put("java.lang.Long", "long");
        typeMap.put("java.lang.Float", "float");
        typeMap.put("java.lang.Double", "double");
    }
    private static boolean matchTypes(String a, String b) {
        return (a.equals(b) || (typeMap.get(a) != null && typeMap.get(a).equals(b)));
    }

    public static void disconnect(QSignalEmitter sender, Object receiver) {
        Class<?> cls = sender.getClass();
        while (QSignalEmitterInternal.class.isAssignableFrom(cls)) {
            Field fields[] = cls.getDeclaredFields();

            for (Field f : fields) {
                if (isSignal(f.getType())) {
                    QSignalEmitter.AbstractSignal signal;
                    try {
                        f.setAccessible(true);
                        signal = (QSignalEmitter.AbstractSignal) f.get(sender);
                    } catch (Exception e) {
                        signal = (QSignalEmitter.AbstractSignal) fetchSignal(sender, f);
                    }

                    signal.disconnect(receiver);
                }
            }

            cls = cls.getSuperclass();
        }
    }

    public static native QSignalEmitter sender(QObject receiver);

    public static Method findEmitMethod(QSignalEmitterInternal.AbstractSignalInternal signal) {
    	Class<?> cls = signal.getClass();
        Method slotMethod = null;
        while(slotMethod==null && cls!=QSignalEmitterInternal.AbstractSignalInternal.class){
        	Method methods[] = cls.getDeclaredMethods();
	
	        for (Method method : methods) {
	            if (method.getName().equals("emit")) {
	                slotMethod = method;
	                break;
	            }
	        }
	        cls = cls.getSuperclass();
        }
        return slotMethod;
    }

    private static Field findField(Field[] fields, String fieldName) {
        Field found = null;
        for (Field field : fields) {
            if (field.getName().equals(fieldName)) {
                found = field;
                break;
            }
        }
        return found;
    }

    /**
     * Returns true if the class cl represents a Signal.
     * @return True if the class is a signal
     * @param cl The class to check
     */
    public static boolean isSignal(Class<?> cl) {
        return QSignalEmitterInternal.AbstractSignalInternal.class.isAssignableFrom(cl);
    }

    public static QtJambiInternal.ResolvedSignal resolveSignal(Field field, Class<?> declaringClass) {
        QtJambiInternal.ResolvedSignal resolvedSignal = new ResolvedSignal();
        resolvedSignal.name = field.getName();

        Type t = field.getGenericType();

        // either t is a parameterized type, or it is Signal0
        if (t instanceof ParameterizedType) {
            ParameterizedType p = (ParameterizedType) t;
            Type actualTypes[] = p.getActualTypeArguments();

            resolvedSignal.types = new Class[actualTypes.length];
            resolvedSignal.arrayDimensions = new int[actualTypes.length];
            for (int j = 0; j < resolvedSignal.types.length; ++j) {

                Type actualType = actualTypes[j];
                int arrayDims = 0;
                while (actualType instanceof GenericArrayType
                        || actualType instanceof ParameterizedType) {
                    if (actualType instanceof GenericArrayType) {
                        actualType = ((GenericArrayType) actualType)
                                .getGenericComponentType();
                        ++arrayDims;
                    } else { // ParameterizedType
                        actualType = ((ParameterizedType) actualType)
                                .getRawType();
                    }
                }

                if (actualType instanceof Class) {
                    Class<?> actualTypeClass = (Class<?>) actualType;
                    // JRE7 returns Class as the top level item, but isArray() is set (for each dimension, hence we loop)
                    // JRE5/JRE6 returns GenericArrayTypeImpl for each dimension and the final unwrapping returns a Class with isArray() not set
                    // So this while loop below is needed since JRE7
                    while (actualTypeClass.isArray()) {
                        arrayDims++;
                        actualTypeClass = actualTypeClass.getComponentType();
                    }
                    // If we do not do this assignment here, we need to uncomment the section in QSignalEmitterInternal#matchTwoTypes()
                    //  to unwrap things there as well (or unit tests continue to fail).
                    actualType = actualTypeClass;
                    // End of JRE7 changes
                    resolvedSignal.types[j] = (Class<?>) actualType;
                    resolvedSignal.arrayDimensions[j] = arrayDims;
                } else {
                    throw new RuntimeException(
                            "Signals of generic types not supported: "
                                    + actualTypes[j]
                                    .toString());
                }
            }
        }

        return resolvedSignal;
    }

    static native QSignalEmitterInternal.AbstractSignalInternal fetchSignal(QSignalEmitterInternal signalEmitter, Field field);

    static native long resolveSlot(Method method);

    static native void invokeSlot(Object receiver, long m,
            byte returnType, Object args[], int slotTypes[]);

    /*private static native void setField(QSignalEmitter object, Field f,
            QObject.AbstractSignalInternal newValue);*/

    static native boolean cppDisconnect(QObject sender, String signal, QObject receiver, String slot);

    /**
     * Searches the object's class and its superclasses for a method of the given name and returns
     * its signature.
     */
    static private HashMap<String, String> signalMethodSignatureCash = new HashMap<String, String>();
    static String findSignalMethodSignature(QSignalEmitter signalEmitter, String name) throws NoSuchFieldException, IllegalAccessException {

        Class<?> cls = signalEmitter.getClass();
        String fullName = cls + "." + name;
        String found = signalMethodSignatureCash.get(fullName);

        if (found != null) {
            return found;
        }

        while (cls != null) {
            Method methods[] = cls.getDeclaredMethods();
            for (Method method : methods) {
                if (method.getName().equals(name)) {
                    found = name + "(";

                    Class<?> params[] = method.getParameterTypes();
                    for (int j = 0; j < params.length; ++j) {
                        if (j > 0) {
                            found += ",";
                        }
                        found += params[j].getName();
                    }
                    found = found + ")";
                    break;
                }
            }

            cls = cls.getSuperclass();
        }
        signalMethodSignatureCash.put(fullName, found);
        return found;
    }

    public static List<QObject> findChildren(QObject qobject, Class<?> cl,
            String name) {
        List<QObject> children = qobject.children();
        List<QObject> matching = new ArrayList<QObject>();
        for (QObject current : children) {
            if ((name == null || name.equals(current.objectName()))
                    && (cl == null || cl.isAssignableFrom(current.getClass()))) {
                matching.add(current);
            }
            matching.addAll(current.findChildren(cl, name));
        }

        return matching;
    }

    public static List<QObject> findChildren(QObject qobject, Class<?> cl,
            QRegExp name) {
        List<QObject> children = qobject.children();
        List<QObject> matching = new ArrayList<QObject>();
        for (QObject current : children) {
            if ((name == null || name.indexIn(current.objectName()) >= 0)
                    && (cl == null || cl.isAssignableFrom(current.getClass()))) {
                matching.add(current);
            }
            matching.addAll(current.findChildren(cl, name));
        }

        return matching;
    }

    public static QObject findChild(QObject qobject, Class<?> cl, String name) {
        List<QObject> children = qobject.children();
        for (QObject current : children) {
            if ((name == null || name.equals(current.objectName()))
                && (cl == null || cl.isAssignableFrom(current.getClass()))) {
                return current;
            }
            QObject object = findChild(current, cl, name);
            if (object != null)
                return object;
        }
        return null;
    }

    public native static Object createExtendedEnum(int value, int ordinal,
            Class<?> cl, String name);


    private static class MutableInteger {
        int value;
    }

    private static HashMap<Class<?>, MutableInteger> expensesTable;
    public static void countExpense(Class<?> cl, int cost, int limit) {
        if (expensesTable == null)
            expensesTable = new HashMap<Class<?>, MutableInteger>();

        MutableInteger mi = expensesTable.get(cl);
        if (mi == null) {
            mi = new MutableInteger();
            expensesTable.put(cl, mi);
        }

        mi.value += cost;
        if (mi.value > limit) {
            mi.value = 0;
            System.gc();
        }
    }

    public static void setField(Object owner, Class<?> declaringClass, String fieldName, Object newValue) {

        Field f = null;
        try {
            f = declaringClass.getDeclaredField(fieldName);
            f.setAccessible(true);
            f.set(owner, newValue);
        } catch (Exception e) {
            if (!setFieldNative(owner, f, newValue)) {
                throw new RuntimeException("Cannot set field '" + fieldName);
            }
        }
    }
    public static native boolean setFieldNative(Object owner, Field field, Object newValue);

    public static Object fetchField(Object owner, Class<?> declaringClass, String fieldName) {

        Field f = null;
        try {
            f = declaringClass.getDeclaredField(fieldName);
            f.setAccessible(true);
            return f.get(owner);
        } catch (Exception e) {
            return fetchFieldNative(owner, f);
        }
    }
    public static native Object fetchFieldNative(Object owner, Field field);

    public static String charPointerToString(QNativePointer np) {
        int pos = 0; byte b;

        String returned = "";
        while ((b = np.byteAt(pos++)) != 0)
            returned += Byte.toString(b);

        return returned;
    }

    public static QNativePointer intArrayToNativePointer(int data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Int, data.length);
        for (int i=0; i<data.length; ++i)
            np.setIntAt(i, data[i]);
        return np;
    }

    public static QNativePointer byteArrayToNativePointer(byte data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Byte, data.length);
        for (int i=0; i<data.length; ++i)
            np.setByteAt(i, data[i]);
        return np;
    }

    /**
     * Returns wether a class is an actual implementor of a function or
     * if the function is simply a shell around a native implementation
     * provided by default by the Qt Jambi bindings.
     *
     * @param method The function to match.
     * @return wether the implements the function or not.
     */
    public static boolean isImplementedInJava(Method method) {
        return method.getDeclaringClass().getAnnotation(QtJambiGeneratedClass.class) == null;
    }


    /**
     * Returns the field entry for all declared signales in o and its base
     * classes.
     */
    private static List<Field> findSignals(QObject o) {
        Class<?> c = o.getClass();
        List<Field> fields = new ArrayList<Field>();
        while (c != null) {
            Field declared[] = c.getDeclaredFields();
            for (Field f : declared) {
                if (QtJambiInternal.isSignal(f.getType())) {
                    fields.add(f);
                }
            }
            c = c.getSuperclass();
        }
        return fields;
    }

    private static Class<?> objectClass(Class<?> cl) {
        if (cl == boolean.class)
            return java.lang.Boolean.class;
        if (cl == byte.class)
            return java.lang.Byte.class;
        if (cl == char.class)
            return java.lang.Character.class;
        if (cl == short.class)
            return java.lang.Short.class;
        if (cl == int.class)
            return java.lang.Integer.class;
        if (cl == long.class)
            return java.lang.Long.class;
        if (cl == float.class)
            return java.lang.Float.class;
        if (cl == double.class)
            return java.lang.Double.class;
        return cl;
    }

    /**
     * Compares the signatures and does a connect if the signatures match up.
     */
    private static void tryConnect(QObject receiver, Method method, QObject sender, Field signal) {
        Class<?> params[] = method.getParameterTypes();
        Type type = signal.getGenericType();

        if (type instanceof ParameterizedType) {
            ParameterizedType pt = (ParameterizedType) type;
            Type types[] = pt.getActualTypeArguments();

            // If the signal has too few arguments, we abort...
            if (types.length < params.length)
                return;

            for (int i = 0; i < params.length; ++i) {
                Class<?> signal_type = (Class<?>) types[i];
                Class<?> param_type = params[i];

                if (signal_type.isPrimitive())
                    signal_type = objectClass(signal_type);

                if (param_type.isPrimitive())
                    param_type = objectClass(param_type);

                // Parameter types don't match.
                if (signal_type != param_type)
                    return;
            }

        } else if (params.length != 0) {
            throw new RuntimeException("Don't know how to autoconnect to: "
                    + signal.getDeclaringClass().getName() + "."
                    + signal.getName());
        }

        // Do the connection...
        Object signal_object = null;
        try {
            signal_object = signal.get(sender);
        } catch (IllegalAccessException e) {
            e.printStackTrace();
            return;
        }

        ((QSignalEmitter.AbstractSignal) signal_object).connect(receiver, MetaObjectTools.methodSignature(method));
    }

    /**
     * Dunno what to make of this, so I'm making it internal.
     * @exclude
     */
    public static void connectSlotsByName(QObject object) {
        List<QObject> children = object.findChildren();
        Class<?> objectClass = object.getClass();
        while (objectClass != null) {
            Method methods[] = objectClass.getDeclaredMethods();
            for (QObject child : children) {
                String prefix = "on_" + child.objectName() + "_";
                List<Field> fields = findSignals(child);
                for (Field f : fields) {
                    String slot_name = prefix + f.getName();
                    for (int i = 0; i < methods.length; ++i) {
                        if (methods[i].getName().equals(slot_name)) {
                            tryConnect(object, methods[i], child, f);
                        }
                    }
                }
            }
            objectClass = objectClass.getSuperclass();
        }
    }

    /**
     * Returns the closest superclass of <tt>obj's</tt> class which
     * is a generated class, or null if no such class is found. 
     */
    public static Class<?> findGeneratedSuperclass(Object obj){
        Class<?> clazz = obj.getClass();
        while(clazz!=null && !clazz.isAnnotationPresent(QtJambiGeneratedClass.class)){
            clazz = clazz.getSuperclass();
        }

        return clazz;
    }

    public static void writeSerializableJavaObject(QDataStream s, Object o) throws IOException {
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        ObjectOutputStream out = null;
        try {
            out = new ObjectOutputStream(bos);
            out.writeObject(o);
            ObjectOutputStream tmpOut = out;
            out = null;  // don't call close() twice
            tmpOut.close();
        } finally {
            if(out != null) {
                try {
                    out.close();
                } catch(IOException eat) {
                }
                out = null;
            }
        }
        s.writeBytes(bos.toByteArray());
    }

    public static boolean isGeneratedClass(Class<?> clazz) {
        return clazz.isAnnotationPresent(QtJambiGeneratedClass.class);
    }

    public static class ResolvedSignal {
        public Class<?> types[] = new Class[0];
        public int arrayDimensions[] = new int[0];
        public String name = "";
    }

    public static String signalParameters(QSignalEmitterInternal.AbstractSignalInternal signal) {
        return MetaObjectTools.bunchOfClassNamesInARow(signal.resolveSignal(), signal.arrayDimensions());
    }

    /**
     * Package visibility due to handling of raw memory pointers in
     *  long type.
     * @param nativeId
     * @return
     */
    static QtProperty userProperty(long nativeId) {
        List<QtProperty> properties = properties(nativeId);

        for (QtProperty property : properties) {
            if (property.isUser())
                return property;
        }

        return null;
    }

    /**
     * Package visibility due to handling of raw memory pointers in
     *  long type.
     * @param nativeId
     * @return
     */
    native static List<QtProperty> properties(long nativeId);

    /**
     * Package visibility due to handling of raw memory pointers in
     *  long type.
     * @param nativeId
     * @param name
     * @return
     */
    static int indexOfProperty(long nativeId, String name) {
        List<QtProperty> properties = properties(nativeId);

        for (int i=0; i<properties.size(); ++i) {
            if (name.equals(properties.get(i).name()))
                return i;
        }

        return -1;
    }

    public static Object readSerializableJavaObject(final QDataStream s) throws ClassNotFoundException, IOException {
        Object res = null;
        ObjectInputStream in = null;
        try {
            in = new ObjectInputStream(new InputStream(){
                @Override
                public int read() throws IOException {
                    return s.readByte();
                }
            });
            res = in.readObject();
            ObjectInputStream tmpIn = in;
            in = null;  // don't call close() twice
            tmpIn.close();
        } finally {
            if(in != null) {
                try {
                    in.close();
                } catch(IOException eat) {
                }
                in = null;
            }
        }
        return res;
    }
}
