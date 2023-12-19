# Characteristics Of QtJambi

## Initializing Qt

Initializing Qt in C++ is done by creating an instance of
`QCoreApplication` (or `QGuiApplication`/`QApplication`):

``` c++
// main method in C++
int main(int argc, char* argv[]){
    QApplication app(argc, argv);
    return app.exec();
    // finally, app is deleted when leaving scope
}
```

However, in Java you simply call the static methods `initialize(args)`
at the beginning of the main method and `shutdown()` when leaving the
main method. Also `exec()` is static.

``` Java
// main method in Java
public static void main(String[] args) {
    QApplication.initialize(args); // creating an instance of QApplication
    QApplication.exec();
    QApplication.shutdown(); // deleting instance of QApplication
}
```

If you want to use your custom subclass of `QCoreApplication` (or
`QGuiApplication`/`QApplication`) call `initialize(...)` with
constructor handle:

``` Java
import io.qt.widgets.*;
public class MyApplication extends QApplication{
    private MyApplication(String[] args){
        super(args);
    }
    
    public static void main(String[] args) {
        QApplication.initialize(args, MyApplication::new); // creating an instance of MyApplication
        QApplication.exec();
        QApplication.shutdown(); // deleting instance of MyApplication
    }
}
```

Make sure to remove all top-level widgets of your application prior to
`QApplication.shutdown();` either by removing all references (`widget = null;`) 
or by disposing the object (`widget.dispose();`).

## Qt Types and Java Types

Almost all classes and namespaces declared by Qt API have counterparts in Java. 
The Java classes provide all methods of the native Qt classes with similar method signature.

### Primitive Types

C++ and Qt-specific primitive types are mapped to the corresponding Java primitive types.
All unsigned types are used as signed Java types, as Java does not support unsigned integers.

| native                             | java   |
|------------------------------------|--------|
| int, qint32, quint32               | int    |
| short, qint16, quint16             | short  |
| char, qint8, quint8                | byte   |
| qint64, quint64, qintptr, quintptr | long   |
| bool, 1-bit field                  | byte   |
| QChar, char_16                     | char   |
| float                              | float  |
| double                             | double |
| void                               | void   |

### Value and Object Types

Qt provides two different categories of classes: object types and value types.
An instance of an object type is a unique entity. Entities cannot be copied and two entities are never equals.
In Qt's native API they are mostly represented as pointer argument types, e.g. `QObject* object`.

In contrast, value types can be copied and compared. The unmodified copies of a value are always equal.
In Qt's native API they are mostly represented as const-reference-typed argument or copy argument, e.g. `const QColor& color`.

### Null Value and Nullness

If you submit `null` as object type argument (showing a `*` pointer in Qt API) it is translated to a native null pointer (`nullptr`).
However, there are a few cases where `null` is prohibited according to Qt's documentation.
If you submit `null` as value type argument it is converted to the default value. For instance, calling `widget.setWindowIcon(null);`
is equivalent to `widget.setWindowIcon(new QIcon());` (but faster).
Only in cases where the native Qt API expects a **non-constant reference** the corresponding Java method does not accept `null`.

QtJambi API uses type annotations for arguments to show their *nullness*, i.e. whether their C++ representation is pointer-based (`@Nullable`), value-based/const-reference-based (`@NonNull`) or reference-based (`@StrictNonNull`).
In Java, `@Nullable` and `@NonNull` have no further meaning. You can submit `null` to `@NonNull`-annotated method argument. Qt will use the default value as described above.
`@StrictNonNull` annotated methods will throw `NullPointerException` or `IllegalArgumentException` when submitting `null`. Nullness annotations are primarily used to specify type nullness for programming language **Kotlin**.

### Pointers and References

Java does not support constant types (`const`), C-pointers (`*`) or
C-references (`&`). Corresponding function argument types are simply used as Java reference types: 
`const QSize& size` is `QSize size` and `QGraphicsItem* item` as well as 
`const QGraphicsItem* item` is `QGraphicsItem item` in Java.

If a Qt function returns a constant reference to a value the corresponding Java method returns a copy of the value.

Sometimes, occurences of call-by-reference or call-by-value-pointer in Qt are
represented in QtJambi as wrapped return values.
For example, the `QFormLayout` C++-method `void getItemPosition(int index, int *rowPtr, QFormLayout::ItemRole *rolePtr) const` 
where `rowPtr` and `rolePtr` are pointers for storing the method output, is represented in Java by 
`public final ItemInfo getItemPosition(int index)` whereas
`ItemInfo` provides `role` and `row` as public member fields.

### Array Pointers

In some cases, array pointer of primitive type are mapped to Java NIO
buffers. For instance, `QSharedMemory.data()`, `QImage.bits()` and
`QUdpSocket.readDatagram(ByteBuffer,HostInfo)`. If the native Qt API
specifies constant pointers, the given Java buffer is read only.

In rare cases, `void*` or `const void*` maps to `QNativePointer` as the most generic way to represent native pointers.

### Strings

The Qt classes `QString`, `QLatin1String`, `QStringView`,
`QUtf8StringView`, `QAnyStringView` and `QStringRef` are mapped to
`java.lang.String`. Also in many cases `const char*` is used as Java
String.

The java class `io.qt.core.QString` represents the mutable string type `QString`.
However, this class is only available to provide data conversion and formatting features of `QString`.
In nearly all cases you have to call `toString()` to use `QString` as string method parameter.

### Enums and Flags

Enums declared by Qt are also available as Java enum. Java enums cannot
convert from or to integer values. Instead, all Qt enum Java classes
provide a `value()` providing the enum value and a `resolve(int)` method
for converting an `int` to the corrensponding enum entry. In case of 64
Bit enums, the corresponding methods point to `long`.
In most cases, Qt enumerator arguments don't accept `null`. 

#### Extensible Enums

Certain enum types in Qt are expected to be extensible, i.e. the
predefined set of enum entries can be extended by custom entries. This
is also supported by QtJambi. The `resolve(int value)` method of an
extensible enum provides a new enum entry if the requested value is not
yet available. Alternatively, you can specify `resolve(int value, String name)` 
to request a new entry with a specific enum name. If you want to
develop custom extensible enums use the annotation `@QtExtensibleEnum`.

#### Flags

There are enums in Qt used as flags. This is also availale in Java by
providing a `QFlags` type for the enum. For instance, the class
`Qt.Edges` is available as flags type for enum `Qt.Edge`.

In C++ you combine different enum entries to a flag value by inclusive
or operator (`Qt::TopEdge | Qt::RightEdge`). In Java, there are multiple
ways available to create flags:

``` Java
// three equivalent ways to create a flag
Qt.Edges flags1 = Qt.Edge.TopEdge.combined(Qt.Edge.RightEdge);
Qt.Edges flags2 = Qt.Edge.flags(Qt.Edge.TopEdge, Qt.Edge.RightEdge);
Qt.Edges flags3 = new Qt.Edges(Qt.Edge.TopEdge, Qt.Edge.RightEdge);
```

In most cases where methods take flags as argument, an overloaded method
is provided taking the corresponding enum type as variadic argument, for
instance:

`QWindow.startSystemResize(io.qt.core.Qt.Edge ... edges)` for

`QWindow.startSystemResize(io.qt.core.Qt.Edges edges)`

In most cases, QFlags arguments don't accept `null`. 

### Function Pointers

Function pointers are made available as functional interfaces and can be
used with lambda expressions.

Examples:

``` java
QEasingCurve ec = new QEasingCurve();
ec.setCustomType(value -> value < 0.5 ? 0.2 : 0.8);
```

### Containers

Qt provides following container types:

  - `QList`
  - `QQueue`
  - `QStack`
  - `QSet`
  - `QHash`
  - `QMap`
  - `QMultiHash`
  - `QMultiMap`
  - `QVector` (Qt5 only)
  - `QLinkedList` (Qt5 only)

These types are all available as Java class in QtJambi. However, when
instantiating such a container class, you need to specify the element
type in the constructor. Example:

``` c++
// using containers in C++
QSet<int> intSet;
intSet << 1 << 2 << 3;

QList<double> doubleList{1.0, 2.0, 3.0};

QMap<QString,QList<int>> mapOfIntegers;
mapOfIntegers["first"] = QList<int>{1, 2, 3};
```

``` java
// using containers in Java
QSet<Integer> intSet = new QSet<>(int.class);
intSet.append(1);
intSet.append(2);
intSet.append(3);

QList<Double> doubleList = QList.of(1.0, 2.0, 3.0);

QMap<String,QList<Integer>> mapOfIntegers = new QMap<>(String.class, 
                                                        QMetaType.fromType(
                                                                  QList.class, 
                                                                  QMetaType.fromType(int.class)));
mapOfIntegers.insert("first", QList.of(1, 2, 3));
```

QtJambi container wrapper classes are fully compatible with Java
containers, for instance, `QList` implements `java.util.List` and `QMap`
implements `java.util.Map`.

All Qt functions with container parameters accept lightweight Java
containers as well, for instance, `QWidget::addActions(QList<QAction*>)`
maps to `QWidget.addActions(java.util.Collection<QAction>)`.

#### Best Practices

QtJambi container wrappers are significantly less performant than lightweight Java
containers because every single access has Qt-Java interoperability and type conversion overhead.

A Java container is based on references to `java.lang.Object` type. 
The generic type parameter is just compile time information. 
On the contrary, a Qt container is a data structure of actual value type (or key-value types)
as given in its constructor. When inserting anything to Qt container in Java, 
the native counterpart of the Java object is copied into the native container.
In case of a type mismatch an exception is thrown:

``` java
// raw type list
QList list = new QList(QObject.class);
list.add("STRING"); // causes IllegalArgumentException
```

All of this makes Qt containers more expensive than Java containers.
However, when using a Java container as Qt method parameter (like in `QWidget.addActions(java.util.Collection<QAction>)`) this effect is reversed.
Here, sending a Qt container is much faster than a Java container because the java container needs to be converted to native container entry by entry.

### QVariant

The generic Qt type `QVariant` is directly mapped to `java.lang.Object` int Java.
However, the Java class `ioqt.core.QVariant` makes type management functionalities available.

## Operator Overloads

Since Java does not allow overloading operators, operator overloads in
Qt are made available as methods with corresponding names in Java. For
instance:

  - `QMatrix4x4::operator+=(const QMatrix4x4&)` →
    `QMatrix4x4.add(QMatrix4x4)`
  - `QPainterPath::operator&=(const QPainterPath&)` →
    `QPainterPath.intersect(QPainterPath)`
  - `QPolygon::operator=(const QPolygon&)` → `QPolygon.assign(QPolygon)`
  - `QBitArray::operator~()` → `QBitArray.inverted()`
  - `QVector4D::operator/=(float)` → `QVector4D.divide(float)`

## Object Life Cycle

A Java object of any Qt type is actually a wrapper for an underlying C++
object. The native C++ object is created immediately when the Java
object is created. The C++ object exists as long as the Java object
exists unless it is deleted by Qt internal mechanisms. The Java object exists
as long as there is no more reference available unless the native side 
does not own a global reference of the object. This is the case whenever
the ownership of an argument is given to Qt as described in Qt API.

If the C++ object is deleted prior to the Java object, the Java object
is *disposed*, i.e. it does no longer provide a native resource. Calling
any method on the object will then throw a `QNoNativeResourcesException`. You
can check if an object is disposed by `isDisposed()`.

You can actively delete the native C++ object by calling `dispose()`.
Usually, you don't have to care about object deletion because the Java
garbage collection cares for it.
However, `QObject` parenthood avoids the garbage collection to delete the child
objects of a parent even if no more references to a child exist in Java.
**You need to manage the life cycle of parented `QObject` instances manually wherever the parent outlasts the child's life time.**
Therfore, use `dispose()` or `disposeLater()`. This is especially required
for `QDialog` because dialogs are usually created with the main window as parent
which avoids deletion even when Java has no more reference.

### Object Deletion

Calling `dispose()` causes a native object to be deleted. Most types are not thread affine, thus, these objects are deleted instantly. Other types are thread affine. 
In this case, the deletion is scheduled in the object's thread. All `QObject`-derived types are thread affine but also some types that are associated to `QObject`s such as `QTextCursor` (it belongs to a `QTextDocument`). 
When calling `dispose()` on `QObject` in another thread the behavior is similar to `disposeLater()`, i.e. a deleteion event is posted and handled by the event loop. 
When calling `dispose()` on an "owned `non-QObject`" object (as `QTextCursor` is) in another thread its deletion also takes place in the thread of the owner (here `QTextDocument`) by posing a deletion event on a deletion handler object.
When calling `dispose()` the Java object is instantly disconnected from its native counterpart even when the effective deletion takes place later through event handler.
When calling `disposeLater()` the Java object stays connected to its native counterpart until the deletion event has been executed by event handler (which always takes place in the future).
Java GC always performs a cleanup similar to `dispose()`, i.e. `QObject`s are always deleted in their associated thread.

### Smart Pointers

  - `QPointer` is a strong reference to any Qt object whereas
    `QPointer.get()` returns `null` as soon as the referenced object is
    disposed.
  - `QWeakPointer` is a weak reference to any Qt object whereas
    `QWeakPointer.get()` returns `null` as soon as the referenced object
    is disposed.
  - `QScopedPointer` and `QScopedArrayPointer` can be used in try-with-resource blocks to dispose
    an object or array of objects when leaving the scope:

``` Java
int dialogCode;
try(QScopedPointer<QDialog> dialogPtr = QScopedPointer.disposing(new QDialog())){
    dialogCode = dialogPtr.get().exec();
}
```

alternative implementation with lambda expression:

``` Java
int dialogCode = QScopedPointer.performAndDispose(dialog->{
        return dialog.exec();
    }, new QDialog());
```

or simpler:

``` Java
int dialogCode = QScopedPointer.performAndDispose(QDialog::exec, new QDialog());
```


### Signal On Dispose

In the very rare case where it is necessary to perform an operation when
a Qt object is about to be disposed you can request the on-dispose
signal and connect to it:

``` Java
QColor color = ...
QtUtilities.getSignalOnDispose(color).connect( ()->{ System.out.println("Color is disposed."); } );
```

Be aware that `disposed` is not identical to `QObject`'s `deleted` signal.
The `deleted` signal is emitted during an object's destructor, i.e. when the native component is deleted.
The `disposed` signal (by `QtUtilities.getSignalOnDispose()`) is emitted when the Java component is detached from its native component.
This can be by deleting the native component or by other reasons where the native object survives the Java wrapper.

### Scope

Use `QScope` to manage the object life time in a try-with-resource block. This is similar to `QScopedPointer` but for many objects.

## QObject and QMetaObject

By subclassing a `QObject` type QtJambi automatically creates the
corresponding `QMetaObject` describing the object's properties, signals,
slots and invokable methods.

### Methods

Basically, all public and non-static methods in a `QObject`-based class are considered to be
invokable by Qt. This additionally applies to non-public void methods. If you want to make other members invokable
(i.e. static and non-public methods as well as constructors) use the `@QtInvokable` annotation.
Likewise, you can avoid a method to be invokable by annotating with `@QtUninvokable`.

```Java
public class Implementor extends QObject{
    // invokable by default:
    public String doSomething(int arg) {
        ...
    }
	
    // not invokable by default:
    private int doSomething(double arg) {
        ...
    }
	
    // avoids being invokable by default:
    @QtUninvokable
    private void doSomething() {
        ...
    }

    // making private supplier invokable:
    @QtInvokable
    private int returnSomething() {
        ...
    }
	
    // making static method invokable:
    @QtInvokable
    public static void doSomethingStatic(int arg) {
        ...
    }
	
    // making constructor invokable:
    @QtInvokable
    public Implementor(){
        super();
    }
}
```

### Signals

The signal-slot mechanism in Java has a different appearance as in C++
as faced below:

#### Declaring

Signals in C++ are methods prefixed by the key word `signals`.

``` c++
// defining signals in C++
signals:
    void stateChanged();
    void textChanged(const QString& text);
    void lengthChanged(int length, QPrivateSignal);

// defining slots
public slots:
    void onStatechanged();
    void onTextChanged(const QString& text);
    void onLengthChanged(int length);
```

In Java, a signal is a *final* member variable of type `SignalN`
with `N`=*number of arguments* (0-9). For private signals use the type
`PrivateSignalN`. The arguments of the signal are given as generic type
arguments. Java does not allow primitive types (i.e. `byte`, `short`,
`int`, `long`, `char`, `float`, `double` and `boolean`) as generic type
arguments. If you want to specify a primitive type as signal argument
use the boxed Java type (one of `Byte`, `Short`, `Integer`, `Long`,
`Character`, `Float`, `Double` and `Boolean`) annotated with
`@QtPrimitiveType`. `@QtPrimitiveType` denies emmitting the signal with
`null` parameter. On Android, type parameter annotations are ignored.

``` java
// defining signals in Java
public final Signal0 stateChanged = new Signal0();
public final Signal1<String> textChanged = new Signal1<>();
public final PrivateSignal1<@QtPrimitiveType Integer> lengthChanged = new PrivateSignal1<>();

// defining slots
void onStatechanged(){}
void onTextChanged(String text){}
void onLengthChanged(int length){}
```

If a signal is not declared `final` `QSignalDeclarationException` is
thrown at runtime.

#### Connecting

Creating signal-slot connections in C++ is done with
`QObject::connect(...)`:

``` c++
// connecting signals in C++
QObject::connect(this, SIGNAL(statechanged()), this, SLOT(onStatechanged()));
QObject::connect(this, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
QObject::connect(this, SIGNAL(lengthChanged(int)), this, SLOT(onLengthChanged(int)));

// connecting signals in C++ with function pointers
QObject::connect(this, &ObjectType::statechanged, this, &ObjectType::onStatechanged);
QObject::connect(this, &ObjectType::textChanged, this, &ObjectType::onTextChanged);
QObject::connect(this, &ObjectType::lengthChanged, this, &ObjectType::onLengthChanged);
```

In Java, you can use the static `connect(...)` method of `QObject` or alternatively the
`connect(...)` method of the signal directly:

``` java
// connecting signals in Java textual
QObject.connect(this, "statechanged()", this, "onStatechanged()");
QObject.connect(this, "textChanged(String)", this, "onTextChanged(String)");
QObject.connect(this, "lengthChanged(int)", this, "onLengthChanged(int)");

// alternatively
this.statechanged.connect(this, "onStatechanged()");
this.textChanged.connect(this, "onTextChanged(String)");
this.lengthChanged.connect(this, "onLengthChanged(int)");

// connecting signals in Java with method references
QObject.connect(this.statechanged, this::onStatechanged);
QObject.connect(this.textChanged, this::onTextChanged);
QObject.connect(this.lengthChanged, this::onLengthChanged);

// alternatively
this.statechanged.connect(this::onStatechanged);
this.textChanged.connect(this::onTextChanged);
this.lengthChanged.connect(this::onLengthChanged);
```

  - `QNoSuchSignalException` is thrown if textually specified signal can
    not be found.
  - `QNoSuchSlotException` is thrown if textually specified slot can not
    be found.
  - `QUninvokableSlotException` is thrown if specified slot is not
    invokable.
  - `QMisfittingSignatureException` is thrown if signal and slot have
    incompatible arguments.
      -   In rare cases, it might be necessary to define value type
        arguments as pointer or reference to make signal and slot
        signatures compatible. Therfore, use the annotations
        `@QtPointerType` and `@QtReferenceType` (ignored on Android).
        Examples: Java method `void whatSize(@QtPointerType QSize size)`
        has the following C++ signature: `void whatSize(QSize* size)`.

Disconnecting signals and slots works analogous with `disconnect()`.

**It is highly recommended to not subclassing slot interfaces.**
When using string-based or method-handle signal slot connections as shown above Qt monitors receiver's life cycle.
If the receiver is deleted all connections are removed. This monitoring is not possible when connecting to custom slot type implementation.

##### Special Characteristics in Android

Android is not able to resolve the corresponding `QMetaMethod` from a method reference.
I.e. by connecting to method reference a lambda object is created and invoked at signal emitting.
Also, the same method reference at different positions will lead to unequal lambda objects.

For example, the following code connects signal `statechanged` to a lambda object calling `this.onStatechanged()`.
But the second line tries to disconect the signal from a second lambda object that has never been connected.

``` java
QObject.connect(this.statechanged, this::onStatechanged);
QObject.disconnect(this.statechanged, this::onStatechanged); //->false in Android
```

A possible solution is to use a variable to store the lamnda object:

``` java
Slot0 slot = this::onStatechanged;
QObject.connect(this.statechanged, slot);
QObject.disconnect(this.statechanged, slot); //->true
```

In any case, textual signal slot connections are resolved to `QMetaMethod`s even in Android:

``` java
QObject.connect(this.statechanged, this, "onStatechanged()");
QObject.disconnect(this.statechanged, this, "onStatechanged()"); //->true in Android
```

##### Code Obfuscation

CAUTION: Code obfuscation breaks the ability to resolve textual signal slot connections. 
If you intend to release your application with obfuscated byte code you should only use 
methodhandle-based or lambda-based connections.

#### Emmitting

Emitting a signal in Java is done by the signal's `emit()` method.
Private methods can only be emitted within their declaring classes:

``` java
// emitting normal signal
this.statechanged.emit();
this.textChanged.emit("new text");

// emitting private signal
emit(this.lengthChanged, 5);
```

### Overloaded Signals

There are a couple of Qt classes providing overloaded signals, for
instance, `QSpinBox` provides two "valueChanged" signals:

``` c++
signals:
    void valueChanged(int);
    void valueChanged(const QString &);
```

In the Java type `QSpinBox` there is only one signal `valueChanged`.
When connecting, it determines the correct signal depending on the slot's arguments:

``` java
// given:
// void onValueChanged(int value)

QSpinBox spinBox = new QSpinBox();
spinBox.valueChanged.connect(this::onValueChanged);
```

Also, the signal provides overloaded emit methods:

``` java
spinBox.valueChanged.emit(1);
spinBox.valueChanged.emit("item");
```

In rare cases where connections are ambiguous or you need the individual signal object of a certain signal method use `overload(...)`:

``` java
spinBox.valueChanged.overload(int.class); // returns Signal1<Integer> for valueChanged(int)
```

### Signals in other Contexts

In contrast to native Qt, QtJambi allows to use the signal-slot
mechanism also in any other class not being subclass of `QObject`.
Therefore, the custom class needs to implement the interfaces
`QtSignalEmitterInterface` and `QInstanceMemberSignals`:

``` java
public class NotifyingList<T> extends ArrayList<T> 
                              implements QtSignalEmitterInterface, QInstanceMemberSignals{
    public final Signal1<T> added = new Signal1<>(this);

    public boolean add(T t){
        if(super.add(t)){
            added.emit(t);
            return true;
        }
        return false;
    }
}
```

If you want to define a static signal use the signal classes from
`QStaticMemberSignals`:

``` java
public final static QStaticMemberSignals.Signal1<String> textChanged = new QStaticMemberSignals.Signal1<>();

public static void changeText(String text){
    textChanged.emit(text);
}
```

If you want to use signals in a local context use the signal classes
from `QDeclarableSignals`:

``` java
public void signalInsideMethod(){
    QDeclarableSignals.Signal1<Integer, String> localSignal = new QDeclarableSignals.Signal1<>(String.class);
    localSignal.connect(...);
    localSignal.emit("test");
}
```

Non-QObject member signals as well as static and local signals do not
use the underlying meta-object system but are based on lightweight Java
implementation.

### Properties

QtJambi automatically detects properties by looking for typical getters
and setters. For instance, if the class has two methods `int getFoo()`
and `void setFoo(int)` the class is considered to have a property called
"foo". 

All features supported by [Qt
properties](https://doc.qt.io/qt-6/properties.html) are also available
in QtJambi:

``` java
@QtPropertyNotify
public final Signal1<String> textChanged = new Signal1<>();

@QtPropertyReader
public final String text(){...}

@QtPropertyWriter
public final void setText(String text){...}

@QtPropertyResetter
public final void resetText(){...}
```

...creates a property "text" with reader, writer, resetter and notify signal.
In case the annotated getter/setter method does not reflect the initended property name
you can use the field `name` of `@QtPropertyReader` and/or `@QtPropertyWriter` to specify the actual property name.

Further annotations reflect the corresponding features of Qt properties:

  - `@QtPropertyMember`
  - `@QtPropertyRequired`
  - `@QtPropertyScriptable`
  - `@QtPropertyStored`
  - `@QtPropertyUser`
  - `@QtPropertyConstant`
  - `@QtPropertyDesignable`
  - `@QtPropertyBindable` (Qt6 only)

Qt6 provides `QProperty` as bindable property member.

In QObject-derived classes, the appearance of `void setFoo(int)` and `int getFoo()` (or `int foo()`) is auto-detected as property `foo`. 
Here, you don't need `QtPropertyReader` and `QtPropertyWriter` annotations. For an existing property "foo" an available method `void resetFoo()`
is considered to be the property's resetter.

Likewise, signal `fooChanged` is auto-detected as corresponding notifier even without `QtPropertyNotify` annotation and
method `bindableFoo()` returning `QBindable` is auto-detected as corresponding bindable even without `QtPropertyBindable` annotation.

In QObject-derived classes, public final fields are considered to be constant (read-only) properties. 
Additionally, a `final QProperty<...> fooProperty` field is automatically considered to be property `foo`. You don't need getter, setter and bindable.
**Caution, if the member property's identifier ends with `Property` this suffix is cut from the property name!**

If your class declares a getter or setter but you don't intend to use it as Qt property you can annotate `@QtPropertyReader(enabled=false)`.

### Dynamic Member Access

The classes defined in Java are fully compatible with Qt's [meta-object
system](https://doc.qt.io/qt-6/metaobjects.html). All Java defined
subclasses of `QObject` provide corresponding `QMetaObject`s giving
access to signals, invokable methods and properties.
`QMetaObject.forType(type)` provides meta-objects for any Java class
even for non-`QObject` types.

By using meta-objects, it is also possible to access native objects
whose classes are not public API.

``` java
QObject internalObject = ...
QMetaObject internalType = internalObject.metaObject();

// dynamically connecting to signal
internalType.findSignal(internalObject, "orientationChanged", Qt.Orientation.class)
            .connect(this::onOrientationChanged);

// dynamically calling method
QMetaMethod changeOrientation = internalType.method("changeOrientation", Qt.Orientation.class);
changeOrientation.invoke(internalObject, Qt.Orientation.Horizontal);

// dynamically casting to interface type
if(internalObject.inherits(QPaintDevice.class)){
    QPaintDevice paintDevice = internalType.cast(internalObject, QPaintDevice.class);
    // this is even possible if (internalObject instanceof QPaintDevice)==false
    // and (QPaintDevice)internalObject leads to ClassCastException
}
```

## Gadgets

Basically, every Java class can be used as gadget, i.e. as meta-programmable type in Qt, so called gadgets.
Gadgets are not QObject-based types with invokable methods and/or properties. In contrast to QObject a gadget class cannot declare native signals.

QtJambi does not auto-detect properties and invokable methods on non-QObject classes.
You explicitely need to specify `@QtPropertyReader/Writer` annotation for every property as well as `@QtInvokable` for every method.
Alternatively, you can enable auto-detection by annotating the gadget class with `@QtAsGadget`.
If you want to use other classes as gadgets, e.g. from third-party library, you may specify `QtUtilities.useAsGadget(class)` 
or `QtUtilities.usePackageContentAsGadgets(package)` at startup.

Pure java objects which are prepared as Qt gadget can also be used in QML.

## Threads

Like Java, [Qt provides extensive thread
support](https://doc.qt.io/qt-6/threads.html). In QtJambi, both
perspectives on threads run in parallel. While originally in Java, every
thread is represented by an instance of `java.lang.Thread`. You can
start a new thread by creating a new `Thread` instance and call
`start()`. Qt provides the `QThread` class for thread management, thus,
by using Qtjambi you have basically the choice:

``` java
// creating a thread with Java originals:
Thread javaThread = new Thread(()->{ ... });
javaThread.start();

// creating a thread with Qt:
QThread qtThread = QThread.create(()->{ ... });
qtThread.start();

// get the corresponding QThread for a Java thread:
QThread javaThreadAsQt = QThread.thread(javaThread);

// get the corresponding Java thread for a QThread:
Thread qtThreadAsJava = qtThread.javaThread();
```

**__Remarks:__**

  - The native method `QThread::wait(...)` has been renamed in Java to `QThread.join(...)`.
  - The Java thread features *uncaught exception handler*, *context class loader*, *thread group*, *daemon thread* and *thread name* are made available in `QThread`.
  - Never call `dispose()` on a running thread!

### Thread Synchronization

For [thread
synchronization](https://doc.qt.io/qt-6/threads-synchronizing.html), Qt
provides a number of classes for different scenarios all avaliable in
Java:

  - `QMutex`,
  - `QReadWriteLock`,
  - `QSemaphore` and
  - `QWaitCondition`

However, Qtjambi does not provide the convenience classes
`QMutexLocker`, `QReadLocker`, `QWriteLocker` and `QSemaphoreReleaser`.
Use try-finally blocks instead:

``` java
QReadWriteLock lock = new QReadWriteLock();

try{
    lock.lockForRead();
}finally{
    lock.unlock();
}
```

### Atomic Memory Operations

QtJambi does not provide the Qt classes for atomic operations like
`QAtomicInteger` and `QAtomicPointer`. Please, use Java built-in atomic
classes.

### Thread Affinity

[QObjects are
thread-affine](https://doc.qt.io/qt-6/threads-qobject.html), i.e every
`QObject` is associated to the `QThread` it was created in (or moved to).
Signals and events of such an object can only run by the associated
thread. Thus, using a `QObject` from outside its own thread may cause a
`QThreadAffinityException` to be thrown.

If you need to use a method of a `QObject` from another thread, use the
meta-object system instead:

``` java
QComboBox comboBox = ...
if(comboBox.thread() != QThread.currentThread())
    QMetaObject.invokeMethod(comboBox::clear, Qt.ConnectionType.BlockingQueuedConnection);
```

You can switch on thread affinity checks with the Java start parameter
`-Dqt.enable.thread.affinity.check=true`. This decreases performance
but leads to exceptions in case of thread affinity breaches. It is
recommended to test your application with enabled thread affinity checks
and to disable these checks in release/productive mode.

## Interfaces

The Qt type system provides many interfaces, i.e. C++ classes with pure
virtual functions that are intended to be implemented in a
multi-inheritance context. For instance, the type `QPaintDevice` is
implemented by the `QObject` subclass `QWidget` as well as by the value
type `QPixmap`. QtJambi provides these Qt interface classes as Java
interfaces. You can use these interfaces in the same degree of freedom
as any other Java interface:

``` java
QRunnable runnable;

// creating annonymous class
runnable = new QRunnable(){
    @Override
    public void run() {}
};

// using lambda expression
runnable = ()->{};

// creating a custom class
class MyRunnable implements QRunnable{
    @Override
    public void run() {}
}
runnable = new MyRunnable();

// implementing the interface as subtype
class MyRunnableWidget extends QWidget implements QRunnable{
    @Override
    public void run() {}
}
runnable = new MyRunnableWidget();
```

In the example code above, the class `MyRunnableWidget` combines a
`QObject` subtype with an interface type. It is completely equivalent to
a custom C++ class:

``` c++
class MyRunnableWidget : public QWidget, public QRunnable{
    Q_OBJECT
public: 
    void run() override {}
};
```

### Interface Constructor Call

Some Qt interfaces provide non-trivial constructors. Since Java
interfaces cannot provide constructors, there is a workaround for
constructing these interface instances:

``` java
public class MyLayoutItem implements QLayoutItem {
    public MyLayoutItem(Qt.Alignment alignment){
        QtUtilities.initializeNativeObject(this, 
                QtArgument.begin(QLayoutItem.class)
                      .add(alignment));
    }
    // implemented interface methods
    // ...
}
```

Here, `QtArgument` creates a stream of arguments subdivided by the
implemented interface types.

``` java
public class MyLayoutItemObject extends QObject implements QLayoutItem {
    public MyLayoutItemObject(QObject parent, Qt.Alignment alignment){
        QtUtilities.initializeNativeObject(this, 
                QtArgument.begin(QObject.class)
                      .add(parent)
                      .begin(QLayoutItem.class)
                      .add(alignment));
    }
    // implemented interface methods
    // ...
}
```

### Protected Interface Methods

In contrast to Qt, Java interfaces do not support protected methods in
interfaces. There are some Qt interfaces providing protected methods to
be used within subtypes. The Javadoc pages of these interfaces provide
suitable code snippets to access the missing protected members.

``` java
class MyGraphicsLayoutItem implements QGraphicsLayoutItem{
    @QtUninvokable
    protected void setGraphicsItem(QGraphicsItem item){
        QGraphicsLayoutItem.MemberAccess.of(this).setGraphicsItem(item);
    }

    @QtUninvokable
    protected void setOwnedByLayout(boolean ownedByLayout){
        QGraphicsLayoutItem.MemberAccess.of(this).setOwnedByLayout(ownedByLayout);
    }

    @QtUninvokable
    protected QSizeF sizeHint(Qt.SizeHint which) {
        return sizeHint(which, new QSizeF());
    }

    @QtUninvokable
    protected QSizeF sizeHint(Qt.SizeHint which, QSizeF constraint){
        return new QSizeF();
    }

    //...
}
```

In case, a protected (or even private) interface method is pure virtual in C++,
`QMissingVirtualOverridingException` is thrown at runtime when the
method is missing in the custom implementation.

### Non-Virtual Interface Methods

In contrast to Qt, methods in Java interfaces are never final and, thus,
can always be implemented by subclasses. Some Qt interfaces provide
non-virtual functions that are represented in the corresponding Java
interface by non-final default methods. If a custom implementation of
the interface overrides such a non-virtual functions,
`QNonVirtualOverridingException` is thrown at runtime.

### Internal-Only Interfaces

Some interfaces are not intended to be subclassed in Java at all, e.g.
the interface `QSurface`. If you implement such an interface,
`QInterfaceCannotBeSubclassedException` is thrown at runtime.

### Pre-Implementations

All interface types provide a nested default implementor class called
`Impl`. Instead of implementing the Java interface, you can extend the
nested implementor class:

``` java
// creating a custom class from pre-implementation
class MyRunnable extends QRunnable.Impl{
    @Override
    public void run() {}
}
QRunnable runnable = new MyRunnable();
```

## Resource System

Qt has a [resource system](https://doc.qt.io/qt-6/resources.html) for
storing icons and other resources in the application's executable or in
libraries. You can use this mechanism also in Java, however, using
resources in *compiled-in manner* is not possible. You should deliver
your rcc resource files along with your Java program or pack it into
your jar files.

Alternatively, you can store your icons (and other resources) directly
in a Java package and access it from classpath:

``` java
QAction newFileAction = new QAction(new QIcon(":com/myapplication/icons/newFile.png"), "New File");
QFile cpResourceFile = new QFile(":com/myapplication/icons/data.dat");
```

If you want to specify an URL referring to classpath resource use `qrc:`:

``` java
QUrl url = new QUrl("qrc:/com/myapplication/icons/data.dat");
```

If you want to use classpath resources in QML you always need to specify it this way.

## Internationalization

[Qt's
internationalization](https://doc.qt.io/qt-6/internationalization.html)
mechanism is completely supported by QtJambi. Simply embed all UI texts
by `tr(...)` as introduced
[here](https://doc.qt.io/qt-6/i18n-source-translation.html).

``` java
QAction newFileAction = new QAction(new QIcon(":com/myapplication/icons/newFile.png"), tr("New File"));
```

Use Qt's `lupdate` tool to extract all UI text from source code.
Therfore, create a \*.pro file listing all Java source code paths as it
is exemplified below:

**files.pro** containing

    SOURCES = com/myapplication/MyApplication.java \
              com/myapplication/MainWindow.java \
              com/myapplication/UserDialog.java

Then, call `lupdate`:

``` powershell
lupdate files.pro -ts com/myapplication/translations/app_de.ts
```

The next step is to open the file
`com/myapplication/translations/app_de.ts` in **Qt Linguist** and
translate the UI texts entry by entry. Thereafter, use `lrelease` to
create the binary file containing the translations:

``` powershell
lrelease com/myapplication/translations/app_de.ts -qm com/myapplication/translations/app_de.qm
```

Install the required translation at runtime by loading the corresponding
qm file from classpath:

``` java
QTranslator translator = new QTranslator();
translator.load(":com/myapplication/translations/app_de.qm");
QCoreApplication.installTranslator(translator);
```

## Generating User Interfaces

Qt allows to create sophisticated user interfaces in graphical manner by using
[Designer](https://doc.qt.io/qt/qtdesigner-manual.html). Designer produces a `*.ui` file 
containing all components and properties of the designed user interface.
There are two ways to use these designed UIs in your QtJambi java application.

* ...by dynamically loading at runtime. Therefore, use the class [`io.qt.widgets.tools.QUiLoader`](https://doc.qt.io/qt/quiloader.html#QUiLoader) from module `qtjambi.uitools`:

``` java
QUiLoader loader = new QUiLoader();
QFile device = new QFile(":com/myapplication/widgets/mainwindow.ui");
device.open(QIODevice.OpenModeFlag.ReadOnly);
QWidget widget = loader.load(device);
device.close();
```

* ...by generating source code.
  Therefore, use the tool **UIC** available in module `qtjambi.uic`.
  Download **qtjambi-uic.jar** from the release of your choice along 
  with the correponding platform-dependent **qtjambi-uic-native-X.jar** and call:

``` shell
java -Djava.library.path=<path to Qt libraries>
     -p qtjambi-6.5.4.jar:qtjambi-uic-6.5.4.jar
     -m qtjambi.uic --output=src --package=com.myapplication.widgets com/myapplication/widgets/mainwindow.ui
```

Alternative way to call it:

``` shell
java -Djava.library.path=<path to Qt libraries>
     -cp qtjambi-6.5.4.jar:qtjambi-uic-6.5.4.jar
     io.qt.uic.Main --output=src --package=com.myapplication.widgets com/myapplication/widgets/mainwindow.ui
```


**QtJambi UIC** produces the widget class in output directory (`-o`) and target package (`-p`) as java source code file.
By specifying `--generator=kotlin` you can generate Kotlin code.

## Platform Specific API

Qt provides a number of classes and functions only available on specific
platforms and/or for specific configurations. QtJambi provides these
specific API components on all platforms and for all configurations.
Thus, QtJambi is source and binary compatible for all cases.

For instance, QtJambi provides the class `QSslConfiguration` and the
function `QNetworkAccessManager.connectToHostEncrypted(...)` wether ssl
is available at compile time or not. However, if ssl is not available at
runtime, `QNetworkAccessManager.connectToHostEncrypted(...)` and likwise
`new QSslConfiguration(...)` throw a `QNoImplementationException`.

## Java and QML

QtJambi makes Java and [QML](https://doc.qt.io/qt-6/qtqml-index.html)
fully interoperable. You can use Java-defined classes in QML and vice
versa.

### Initialization

If your Java program uses QML for creating extended UI objects, it is
recommended to first initialize all Qt modules you intend to use in Qt,
e.g.:

``` java
QtUtilities.initializePackage("io.qt.network");
QtUtilities.initializePackage("io.qt.quick");
```

Also, it is necessary to load all Qt libraries not available as QtJambi
module if you intend to use them in QML, e.g.:

``` java
QtUtilities.loadQtLibrary("QuickShapes");
QtUtilities.loadQtLibrary("QuickTemplates2");
QtUtilities.loadQtLibrary("QuickControls2");
QtUtilities.loadQtLibrary("QuickParticles");
```

Now, you can load qml code, for instance, by using `QQuickView`:

``` java
QQuickView view = new QQuickView();
view.setSource(new QUrl("qrc:com/myapplication/qml/Main.qml"));
view.show();
```

### Integrating QML and Java

[In analogy to C++](https://doc.qt.io/qt-6/qtqml-cppintegration-topic.html) 
you can make Java classes available to QML. Therefore, register the Java class
as QML type:

``` java
QtQml.qmlRegisterType(Message.class, "com.mycompany.messaging", 1, 5, "Message");
```

There are two requirements for Java classes to run with QML:

1.  The Java class has to subclass `QObject` or a subclass of `QObject`.
2.  The Java class has to provide a declarative constructor as
    exemplified below:

``` java
public class Message extends QObject{
    private final String author;
    private final QDateTime creationDate;

    private Message(QDeclarativeConstructor dc) throws IllegalAccessException {
        super(dc);
    }
    
    public String author() { return author; }
    public void setAuthor(String author) { this.author = author; }
    public QDateTime creationDate() { return creationDate; }
    public void setCreationDate(QDateTime creationDate) { this.creationDate = creationDate; }
}
```

The `QDeclarativeConstructor` parameter is an internal constructor
marker and needs to be passed through to the super constructor. It is
not allowed to call the declarative constructor of any `QObject` class
from inside Java.

This allows you to use the Java-defined type in QML:

``` qml
import com.mycompany.messaging 1.5

Message {
    author: "Amelie"
    creationDate: new Date()
}
```

The `QtQml` class also provides methods for registering singleton types
or instances, uncreatable types, interface types, extended types,
attached properties and so on. Refer to
[QML C++ integration](https://doc.qt.io/qt-6/qtqml-cppintegration-definetypes.html)
to read more about how to use custom types in QML.

Since Java does not support preprocessor macros, there is no automatic
type registration as it is enabled for C++ projects by `CONFIG += qmltypes`.
You need to register all Java classes you want to use in QML
manual via `QtQml.qmlRegister...`. Alternatively, you can use the class
`QmlTypes` in the Qtjambi QML utilities and prepare entire packages to
be exported to QML.

``` java
QmlTypes.registerPackage("com.mycompany.messaging", 1);
```

You could also specify the package's version and qml classes by annotating the
**package-info.java** with `QmlImport`:

``` java
@io.qt.qml.util.QmlImport(majorVersion=1, classes={Message.class})
package com.mycompany.messaging;
```

In this case, register the package without specifying a version:

``` java
QmlTypes.registerPackage("com.mycompany.messaging");
```

By registering an entire package, all Java classes annotated as QML type
are registered. Additionally, all \*.qml files located in the registered
package are registered as QML type. Annotating as QML type works similar
to C++ but instead of preprocessor macros use corresponding annotations:

  - [`QML_ELEMENT`](https://doc.qt.io/qt-6/qqmlengine.html#QML_ELEMENT)
    → `@QmlElement`
  - [`QML_NAMED_ELEMENT`](https://doc.qt.io/qt-6/qqmlengine.html#QML_NAMED_ELEMENT)
    → `@QmlNamedElement(name)`
  - [`QML_ANONYMOUS`](https://doc.qt.io/qt-6/qqmlengine.html#QML_ANONYMOUS)
    → `@QmlAnonymous`
  - [`QML_INTERFACE`](https://doc.qt.io/qt-6/qqmlengine.html#QML_INTERFACE)
    → `@QmlInterface`
  - [`QML_UNCREATABLE`](https://doc.qt.io/qt-6/qqmlengine.html#QML_UNCREATABLE)
    → `@QmlUncreatable(reason)`
  - [`QML_SINGLETON`](https://doc.qt.io/qt-6/qqmlengine.html#QML_SINGLETON)
    → `@QmlSingleton`
  - [`QML_ADDED_IN_MINOR_VERSION`](https://doc.qt.io/qt-6/qqmlengine.html#QML_ADDED_IN_MINOR_VERSION)
    → `@QmlAddedInMinorVersion(N)`

Example:

``` java
package com.mycompany.messaging;

import io.qt.*;
import io.qt.core.*;
import io.qt.qml.*;
import io.qt.qml.util.*;

@QmlElement
@QmlAddedInMinorVersion(5)
public class Message extends QObject{
    // constructor, properties, getters and setters see above
}
```

### Object Ownership

When transfering `QObject`s from Java to QML be aware that QML engine might change the object's ownership as
[introduced here](https://doc.qt.io/qt-6/qtqml-cppintegration-data.html#data-ownership). In Qt's terminology 
"JavaScriptOwnership" means an object is owned by QML/JavaScript code. Do not mix up JavaScript with Java.
*JavaScript* is the runtime-interpreted script language used in QML when defining functions and bindings.
On the contrary, *Java* is a compiled programming language using the same type space and API as C++.
Thus, when using Qt API in Java "CppOwnership" means an object owned by Java code.

Be aware that object's ownership is transferred to QML when it is returned by a Java method (and jas no parent).
In this case, QML might delete the object although still used in Java. It is highly recommended to only submit
Java created `QObject`s through properties to avoid this situation.

### Java QML Modules

Finally, you can bundle your custom Java QML classes into a JAR file and
provide it as QML module. Therfore, QtJambi provides the **jarimport**
plugin to be found in the `utilities` folder of the platform binaries.

Create a jar file containing the Java package to be provided for QML
import, e.g. `com.mycompany.messaging`. Then, create a directory path in
the qml import location that matches your package subdirectories, e.g.
`<QTPREFIX>/qml/com/mycompany/messaging` and place the jar file in it.
Additionally, copy the **jarimport** plugin library to the package
directory.

Now, create a file called `qmldir` with following content:

``` text
module com.mycompany.messaging
plugin jarimport
```

The directory substructure should look like this:

``` text
qml
 |  com
     |  mycompany
         |  messaging
             |  com-mycompany-messaging.jar
             |  jarimport.dll
             |  qtdir
```

### Pure Java objects in QML

Since pure java objects can be used as Qt gadget they can also be used in QML/JavaScript.
There are basically two different types of Java-defined gadgets: cloneable values and unique entities.
A Java class represents a cloneable value if it has a standard constructor and a `clone()` method
returning a copy of the instance. Alternatively, instead of `clone()` method the class can 
implement the interface `java.lang.Cloneable`. By convention, the `equals(Object)` function returns `true` 
for each clone and the `hashCode()` of clones returns the same value. All Java classes not meeting this
characteristic are types for unique entities.

Unique entities can only be created in Java. If a unique entity is transferred to QML for instance by a method call
the JavaScript variable stores a reference to the Java object. Actually, the reference is represented by a wrapper, thus, 
you cannot successively compare to `null` in JavaScript. therefore, you need to write an invokable Java method and do the null check there.

```QML
Item{
	Component.onCompleted: {
		// expecting a Java-defined singleton subclassing QObject...
		var object = singleton.supplyJavaGadget();
		// object now contains a reference to the Java object.
		// The Java object has getText and setText methods.
		object.text = "Hello World!"
		
		// null test never succeeds:
		if(object==null)...
		
		// instead do the null check in Java:
		if(singleton.checkForNull(object))...
	}
}
```

In contrast to unique entities instances of cloneable value types are created in QML.
Whenever they are assigned to another variable or used as function parameter a new clone of the object is created.

Cloneable types have to be registered as QML type with lower case typename:

```Java
qmlRegisterType(ValueType.class, "com.example.program", 1, 0, "myGadget");
```

Such a type can then be used in QML:

```QML
import com.example.program

QtObject{
    property myGadget gadget
}
```

An object of type `ValueType` is created when the QML object is loaded and assigned to the proeprty variable `gadget`.
Optionally, a cloneable value type class can define a constructor taking one `QJSValue` argument.
If this is the case you can specify arguments for value creation in QML:

```QML
import com.example.program

QtObject{
    property myGadget gadget: {"text": "Hello World!"}
}
```

## Qt Plugins

You can implement custom Qt plugins in Java. These plugins can be either
realized as application internal implementations or as jar library to be
loaded automatically on demand.

### Registering Custom Plugins

Use the method `QPluginLoader.qRegisterStaticPluginFunction(...)` to
register a plugin class or instance. The class inherits `QObject` and
needs to implement an interface (or class) known as Qt plugin, for
instance:

``` java
public class CustomImageIOPlugin extends QImageIOPlugin {
    @io.qt.QtUninvokable
    public QImageIOHandler create(QIODevice device, QByteArray format){
        return new CustomImageIOHandler(device, format);
    }
}
```

Finally, register the plugin implementation anywhere in your
application:

``` java
QPluginLoader.qRegisterStaticPluginFunction(CustomImageIOPlugin.class, Map.of("Keys", List.of("custom")));
```

### Custom Plugin Libraries

If you want to provide a custom plugin as jar library you need to
provide a platform-dependent loader library along with the jar file.
Therefore, use the *QtJambi plugin deployer tool* to prepare the
loader library. Download **qtjambi-deployer.jar** from the release
of your choice along with the correponding platform-dependent **qtjambi-deployer-native-X.jar**.
Call the plugin deployer as shown below. Make sure the library path points to the *Qt*
and *QtJambi* libraries:

``` shell
java -Djava.library.path=<path to Qt libraries>
     -p qtjambi-6.5.4.jar:qtjambi-deployer-6.5.4.jar
     -m qtjambi.deployer plugin
     --class-name=my.company.CustomImageIOPlugin
     --class-path=my-company-library.jar
     --dir=<output directory>
     --meta-data=metadata.json
```

Alternative way to call it:

``` shell
java -Djava.library.path=<path to Qt libraries>
     -cp qtjambi-6.5.4.jar:qtjambi-deployer-6.5.4.jar
     io.qt.qtjambi.deployer.Main plugin
     --class-name=my.company.CustomImageIOPlugin
     --class-path=my-company-library.jar
     --dir=<output directory>
     --meta-data=metadata.json
```

The **metadata.json** file contains the keys of the plugin and
additional meta data. Example:

``` json
{
    "Keys": ["custom"]
}
```

QtJambi plugin deployer tool saves the prepared library and the jar file
in the specified output directory.

#### Generating Source Code

Alternatively, deployer can generate source code for compiling the plugin library.
This is especially necessary on macOS (arm64).

``` shell
java -Djava.library.path=<path to Qt libraries>
     -p qtjambi-6.5.4.jar:qtjambi-deployer-6.5.4.jar
     -m qtjambi.deployer plugin
     --class-name=my.company.CustomImageIOPlugin
     --class-path=my-company-library.jar
     --dir=<output directory>
     --meta-data=metadata.json
     --source
```

Now, output directory contains a source code project for the plugin library.
Call `qmake` and `make` to build the library.

### JDBC Plugin

QtJambi provides a JDBC plugin for using Java SQL capabilities in Qt.

``` java
QSqlDatabase db = QSqlDatabase.addDatabase("QJDBC");
db.setDatabaseName("jdbc:sqltech:192.168.178.155:4444");
QSqlQuery query = new QSqlQuery(db);
query.prepare("SELECT * FROM qtjambi");
query.exec();
```
