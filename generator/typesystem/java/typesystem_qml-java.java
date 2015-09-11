/****************************************************************************
**
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

class __QQmlComponent extends QQmlComponent{

	@QtBlockedSlot
	public final void setData(String data, org.qtjambi.qt.core.QUrl baseUrl)    {
		setData(new org.qtjambi.qt.core.QByteArray(data), baseUrl);
	}

	@QtBlockedSlot
	public final void setData(byte[] data, org.qtjambi.qt.core.QUrl baseUrl)    {
		setData(new org.qtjambi.qt.core.QByteArray(data), baseUrl);
	}
	
	@QtBlockedSlot
	public <T extends org.qtjambi.qt.core.QObject> T create(Class<T> type)    {
		return create(type, (org.qtjambi.qt.qml.QQmlContext)null);
	}

	@QtBlockedSlot
	public <T extends org.qtjambi.qt.core.QObject> T create(Class<T> type, org.qtjambi.qt.qml.QQmlContext context)    {
		String packageName = type.getName();
		int idx = packageName.lastIndexOf('.');
		if(idx>0){
			packageName = packageName.substring(0, idx);
		}
		Utilities.initializePackage(packageName);
		return type.cast(create(context));
	}
}// class

class __QtQml extends QtQml{

    private QtQml(){}

    public static native int qmlRegisterType(Class<?> type);
    
    public static native int qmlRegisterTypeNotAvailable(String uri, int versionMajor, int versionMinor, String qmlName, String message);
    
    public static native int qmlRegisterUncreatableType(Class<?> type, String uri, int versionMajor, int versionMinor, String qmlName, String reason);
    
    public static native int qmlRegisterUncreatableType(Class<?> type, int metaObjectRevision, String uri, int versionMajor, int versionMinor, String qmlName, String reason);
    
    public static native int qmlRegisterType(Class<?> type, String uri, int versionMajor, int versionMinor, String qmlName);
    
    public static native int qmlRegisterType(Class<?> type, int metaObjectRevision, String uri, int versionMajor, int versionMinor, String qmlName);
    
    public static native int qmlRegisterRevision(Class<?> type, int metaObjectRevision, String uri, int versionMajor, int versionMinor);

    public static native int qmlRegisterExtendedType(Class<?> type, Class<?> extendedType);

    public static native int qmlRegisterExtendedType(Class<?> type, Class<?> extendedType, String uri, int versionMajor, int versionMinor, String qmlName);

    public static native int qmlRegisterInterface(Class<?> type, String typeName);

    public static QObject qmlAttachedPropertiesObject(QObject obj){
    	return qmlAttachedPropertiesObject(obj, true);
    }

    public static native QObject qmlAttachedPropertiesObject(QObject obj, boolean create);

    public static native void qmlRegisterBaseTypes(String uri, int versionMajor, int versionMinor);

    public static native int qmlRegisterType(QUrl url, String uri, int versionMajor, int versionMinor, String qmlName);
    
    public static native boolean qmlProtectModule(String uri, int majVersion);

//  public static native int qmlRegisterSingletonType(String uri, int versionMajor, int versionMinor, String typeName, QJSValue (*callback)(QQmlEngine *, QJSEngine *));

//  public static native int qmlRegisterSingletonType(String uri, int versionMajor, int versionMinor, String typeName, QObject *(*callback)(QQmlEngine *, QJSEngine *));

//  public static native int qmlRegisterCustomType(Class<?> type, String uri, int versionMajor, int versionMinor, String qmlName, QQmlCustomParser parser);

}// class

class QQmlExpression__{
	public static class Result{
    	public Result(boolean valueIsUndefined, Object value) {
			super();
			this.valueIsUndefined = valueIsUndefined;
			this.value = value;
		}
		public final boolean valueIsUndefined;
		public final java.lang.Object value;
    }

    @QtBlockedSlot
    public final Result evaluate() {
    	org.qtjambi.qt.QNativePointer np = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
    	java.lang.Object value = evaluate(np);
        return new Result(np.booleanValue(), value);
    }
}// class