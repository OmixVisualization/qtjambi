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

package org.qtjambi.bcc;

import java.util.*;
import java.lang.reflect.*;

import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class BCC extends QWidget {

    private QLineEdit leftDirectoryInput = new QLineEdit();
    private QLineEdit rightDirectoryInput = new QLineEdit();
    private QStackedWidget stack;
    private QProgressBar progressBar;

    private static class ProblemsWidget extends QTreeWidget {

        private static class TopLevelItem extends QTreeWidgetItem {
            private enum Degree {
                Critical,
                FeatureAddition
            }

            private String title;

            private TopLevelItem(String title, Degree degree) {
                this.title = title;
                setForeground(0, new QBrush(degree == Degree.Critical ? new QColor(Qt.GlobalColor.darkRed) : new QColor(Qt.GlobalColor.darkGreen)));

                updateTitle();
            }

            public void addSubItem(QTreeWidgetItem item) {
                addChild(item);
                updateTitle();
            }

            private void updateTitle() {
                setText(0, title + " (" + childCount() + ")");
            }

            private void writeToLog(QTextStream stream) {
                stream.writeString(title + "\n");
                int childCount = childCount();
                for (int i=0; i<childCount; ++i) {
                    QTreeWidgetItem child = child(i);
                    stream.writeString("    " + child.text(0) + "\n");
                }
                stream.writeString("\n\n");
            }
        }

        private TopLevelItem missingClasses = new TopLevelItem("Missing classes",TopLevelItem.Degree.Critical);
        private TopLevelItem addedClasses = new TopLevelItem("Added classes", TopLevelItem.Degree.FeatureAddition);
        private TopLevelItem missingMethods = new TopLevelItem("Missing methods", TopLevelItem.Degree.Critical);
        private TopLevelItem missingFields = new TopLevelItem("Missing fields", TopLevelItem.Degree.Critical);
        private TopLevelItem changedMethods = new TopLevelItem("Changed methods", TopLevelItem.Degree.Critical);
        private TopLevelItem changedFields = new TopLevelItem("Changed fields", TopLevelItem.Degree.Critical);
        private TopLevelItem changedClasses = new TopLevelItem("Changed classes", TopLevelItem.Degree.Critical);
        private TopLevelItem addedMethods = new TopLevelItem("Added methods", TopLevelItem.Degree.FeatureAddition);
        private TopLevelItem addedFields = new TopLevelItem("Added fields", TopLevelItem.Degree.FeatureAddition);

        private ProblemsWidget() {
            super();
            header().hide();

            setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOn);

            {
                addTopLevelItem(missingClasses);
                addTopLevelItem(missingMethods);
                addTopLevelItem(missingFields);
                addTopLevelItem(changedMethods);
                addTopLevelItem(changedFields);
                addTopLevelItem(changedClasses);
                addTopLevelItem(addedClasses);
                addTopLevelItem(addedMethods);
                addTopLevelItem(addedFields);
            }

        }

        private void writeLog() {
            String logFileName = QFileDialog.getSaveFileName(this);
            if (logFileName.length() == 0)
                return;

            QFile file = new QFile(logFileName);
            if (!file.open(QIODevice.OpenModeFlag.WriteOnly))
                return;

            QTextStream stream = new QTextStream(file);

            int topLevelItemCount = topLevelItemCount();
            for (int i=0; i<topLevelItemCount; ++i) {
                QTreeWidgetItem item = topLevelItem(i);
                if (item instanceof TopLevelItem)
                    ((TopLevelItem) item).writeToLog(stream);
            }

            file.close();

        }

    }


    private void selectJarFile(QWidget w) {
        String selected = QFileDialog.getOpenFileName(this, "Open jar file", null, new QFileDialog.Filter("*.jar"));
        if (selected.length() != 0) {
            if (w instanceof QLineEdit) {
                QAbstractFileEngine.addSearchPathForResourceEngine(selected);
                ((QLineEdit) w).setText("classpath:" + selected + "#/");
            }
        }

    }

    private void selectDirectory(QWidget w) {
        String selected = QFileDialog.getExistingDirectory();
        if (selected.length() != 0) {
            if (w instanceof QLineEdit) {
                ((QLineEdit) w).setText(selected);
            }
        }
    }

    private static class WorkaroundClassLoader extends ClassLoader {

        private String path;
        private WorkaroundClassLoader(String path) {
            super(ClassLoader.getSystemClassLoader());
            this.path = path;
        }

        private Hashtable<String, Class<?>> classes = new Hashtable<String, Class<?>>();

        @Override
        public Class<?> findClass(String name) throws ClassNotFoundException {
            Class<?> cls = loadDataAndDefineClass(name);
            if (cls != null)
                return cls;
            else
                return super.findClass(name);
        }


        public Class<?> loadDataAndDefineClass(String name) {
            try {
                if (classes.containsKey(name))
                    return classes.get(name);

                byte[] b = loadClassData(name);
                Class<?> returned;
                if (b != null)
                    returned = defineClass(name.replaceAll("/", "."), b, 0, b.length);
                else
                    return null;

                classes.put(name, returned);
                return returned;
            } catch (Throwable e) {
                e.printStackTrace();
                return null;
            }
        }

        @Override
        public Class<?> loadClass(String name) throws ClassNotFoundException {
            return loadClass(name, false);
        }

        @Override
        protected Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException {
            Class<?> cls = loadDataAndDefineClass(name);
            if (cls != null)
                return cls;
            else
                return super.loadClass(name, resolve);
        }

        private byte[] loadClassData(String name) {
            QFileInfo info = new QFileInfo(path + "/" + name.replaceAll("\\.", "/") + ".class");
            return loadClassData(info, name);
        }

        private byte[] loadClassData(QFileInfo info, String className) {
            if (!info.exists())
                return null;

            QFile file = new QFile(info.absoluteFilePath());
            if (file.open(QIODevice.OpenModeFlag.ReadOnly)) {
                return file.readAll().toByteArray();
            } else {
                return null;
            }
        }

        private Class<?> loadClassFromPath(QFileInfo info, String className) {
            try {
                if (classes.containsKey(className))
                    return classes.get(className);
                byte bytes[] = loadClassData(info, className);
                return defineClass(className, bytes, 0, bytes.length);
            } catch (Throwable e) {
                e.printStackTrace();
            }

            return null;
        }

    }


    private void handleDirectoryRecursively(WorkaroundClassLoader classLoader, String path, String packageName, Hashtable<String,Class<?>> classes) {

        QDir dir = new QDir(path);
        List<QFileInfo> entryInfoList = dir.entryInfoList(QDir.Filter.createQFlags(QDir.Filter.AllEntries, QDir.Filter.NoDotAndDotDot));

        for (QFileInfo entryInfo : entryInfoList) {
            if (entryInfo.isDir())
                handleDirectoryRecursively(classLoader, entryInfo.absoluteFilePath(), packageName + (packageName.length() == 0 ? "" : ".") + entryInfo.baseName(), classes);
            else if (entryInfo.completeSuffix().equals("class")) {
                Class<?> clazz = classLoader.loadClassFromPath(entryInfo, packageName + (packageName.length() ==  0 ? "" : ".") + entryInfo.baseName());
                if (clazz != null)
                    classes.put(clazz.getName(), clazz);
                else
                    System.err.println("Failed to load class file: " + packageName + (packageName.length() == 0 ? "" : ".") + entryInfo.baseName());
            }
        }
    }

    private Hashtable<String,Class<?>> handleDirectory(String path) {

        Hashtable<String,Class<?>> classes = new Hashtable<String,Class<?>>();

        handleDirectoryRecursively(new WorkaroundClassLoader(path), path, "", classes);

        return classes;
    }

    QThread thread;
    private void scan() {
        stack.setCurrentIndex(1);

        final String leftDir = leftDirectoryInput.text();
        final String rightDir = rightDirectoryInput.text();

        packagesCheckVal = packagesCheck.isChecked();
        packagesCheck.setDisabled(true);
        thread = new QThread(new Runnable() {

            public void run() {
                Hashtable<String,Class<?>> leftHandler = handleDirectory(leftDir);
                Hashtable<String,Class<?>> rightHandler = handleDirectory(rightDir);

                if (leftHandler == null || rightHandler == null)
                    return ;

                compare(leftHandler, rightHandler);

                QApplication.invokeLater(new Runnable() {
                    public void run() {
                        stack.setCurrentIndex(0);
                        packagesCheck.setDisabled(false);
                    }
                });

            }
        });

        thread.setDaemon(true);
        thread.start();
    }

    private boolean isAbstract(int modifiers) {
        return (modifiers & Modifier.ABSTRACT) == Modifier.ABSTRACT;
    }

    private boolean isFinal(int modifiers) {
        return (modifiers & Modifier.FINAL) == Modifier.FINAL;
    }

    private boolean isInterface(int modifiers) {
        return (modifiers & Modifier.INTERFACE) == Modifier.INTERFACE;
    }

    private boolean isNative(int modifiers) {
        return (modifiers & Modifier.NATIVE) == Modifier.NATIVE;
    }

    private boolean isPrivate(int modifiers) {
        return (modifiers & Modifier.PRIVATE) == Modifier.PRIVATE;
    }

    private boolean isProtected(int modifiers) {
        return (modifiers & Modifier.PROTECTED) == Modifier.PROTECTED;
    }

    private boolean isPublic(int modifiers) {
        return (modifiers & Modifier.PUBLIC) == Modifier.PUBLIC;
    }

    private boolean isStatic(int modifiers) {
        return (modifiers & Modifier.STATIC) == Modifier.STATIC;
    }

    private boolean isStrict(int modifiers) {
        return (modifiers & Modifier.STRICT) == Modifier.STRICT;
    }

    private boolean isSynchronized(int modifiers) {
        return (modifiers & Modifier.SYNCHRONIZED) == Modifier.SYNCHRONIZED;
    }

    private boolean isTransient(int modifiers) {
        return (modifiers & Modifier.TRANSIENT) == Modifier.TRANSIENT;
    }

    private boolean isVolatile(int modifiers) {
        return (modifiers & Modifier.VOLATILE) == Modifier.VOLATILE;
    }

    private Field findField(Class<?> cls, Field field) {
        Field fields[] = cls.getDeclaredFields();
        for (Field otherField : fields) {
            if (otherField.getName().equals(field.getName()))
                return otherField;
        }

        return null;
    }

    private Constructor<?> findConstructor(Class<?> cls, Constructor<?> constructor) {
        Constructor<?> constructors[] = cls.getConstructors();
        for (Constructor<?> otherConstructor : constructors) {
            if (otherConstructor.toString().equals(constructor.toString()))
                return otherConstructor;
        }

        return null;
    }

    private Method findMethod(Class<?> cls, Method method) {
        Method methods[] = cls.getDeclaredMethods();

        Method returned = null;
        for (Method otherMethod : methods) {
            if (otherMethod.getName().equals(method.getName())) {
                Class<?> parameterTypes[] = otherMethod.getParameterTypes();
                Class<?> otherParameterTypes[] = method.getParameterTypes();


                if (parameterTypes.length == otherParameterTypes.length) {

                    boolean match = true;
                    for (int i=0; i<parameterTypes.length; ++i) {
                        Class<?> parameterType = parameterTypes[i];

                        if (!parameterType.toString().equals(otherParameterTypes[i].toString())) {
                            match = false;
                            break;
                        }
                    }

                    // Prefer the best match (needed for fromNativePointer() functions)
                    if (match) {
                        if (returned == null) returned = otherMethod;
                        else {
                            if (otherMethod.getReturnType().toString().equals(method.getReturnType().toString()))
                                returned = otherMethod;
                        }
                    }
                }
            }
        }

        return returned;
    }

    private String modifierString(int modifiers) {
        return (isAbstract(modifiers) ? "abstract " : "")
             + (isFinal(modifiers) ? "final " : "")
             + (isInterface(modifiers) ? "interface " : "")
             + (isNative(modifiers) ? "native " : "")
             + (isPrivate(modifiers) ? "private " : "")
             + (isProtected(modifiers) ? "protected " : "")
             + (isPublic(modifiers) ? "public " : "")
             + (isStatic(modifiers) ? "static " : "")
             + (isStrict(modifiers) ? "strict " : "")
             + (isSynchronized(modifiers) ? "synchronized " : "")
             + (isTransient(modifiers) ? "transient " : "")
             + (isVolatile(modifiers) ? "volatile " : "");
    }

    private void compareClasses(final Class<?> oldClass, final Class<?> newClass) {
        try {
            int oldModifiers = oldClass.getModifiers();
            int newModifiers = newClass.getModifiers();
            if (oldModifiers != newModifiers) {
                final String oldModifierString = modifierString(oldModifiers);
                final String newModifierString = modifierString(newModifiers);

                QApplication.invokeLater(new Runnable() {
                    public void run() {
                        QTreeWidgetItem item = new QTreeWidgetItem();
                        item.setText(0, "Modifiers for class '" + oldClass.getName() + "' from '" + oldModifierString + "' to '" + newModifierString + "'");
                        problemsWidget.changedClasses.addSubItem(item);
                    }
                });
            }

            Method oldMethods[] = oldClass.getDeclaredMethods();
            for (final Method oldMethod : oldMethods) {
                oldModifiers = oldMethod.getModifiers();
                if (!isPublic(oldModifiers) && !isProtected(oldModifiers))
                    continue;

                Method newMethod = findMethod(newClass, oldMethod);
                if (newMethod == null) {
                    QApplication.invokeLater(new Runnable() {
                        public void run() {
                            QTreeWidgetItem item = new QTreeWidgetItem();
                            item.setText(0, "'" + oldMethod.toString());
                            problemsWidget.missingMethods.addSubItem(item);
                        }
                    });
                } else {
                    newModifiers = newMethod.getModifiers();
                    if (oldModifiers != newModifiers) {
                        final String oldModifierString = modifierString(oldModifiers);
                        final String newModifierString = modifierString(newModifiers);

                        QApplication.invokeLater(new Runnable() {
                           public void run() {
                              QTreeWidgetItem item = new QTreeWidgetItem();
                              item.setText(0, "Modifiers for '" + oldMethod.toString() + "' changed from '" + oldModifierString + "' to '" + newModifierString + "'");
                              problemsWidget.changedMethods.addSubItem(item);
                           }
                        });
                    }

                    final Class<?> oldReturnType = oldMethod.getReturnType();
                    final Class<?> newReturnType = newMethod.getReturnType();
                    if (!oldReturnType.toString().equals(newReturnType.toString())) {
                        QApplication.invokeLater(new Runnable() {
                            public void run() {
                               QTreeWidgetItem item = new QTreeWidgetItem();
                               item.setText(0, "Return type for '" + oldMethod.toString() + "' changed from '" + oldReturnType.getName() + "' to '" + newReturnType.getName());
                               problemsWidget.changedMethods.addSubItem(item);
                            }
                         });
                    }
                }
            }

            Method newMethods[] = newClass.getDeclaredMethods();
            for (final Method newMethod : newMethods) {
                newModifiers = newMethod.getModifiers();
                if (!isPublic(newModifiers) && !isProtected(newModifiers))
                    continue;

                Method oldMethod = findMethod(oldClass, newMethod);
                if (oldMethod == null) {
                    QApplication.invokeLater(new Runnable() {
                        public void run() {
                            QTreeWidgetItem item = new QTreeWidgetItem();
                            item.setText(0, "'" + newMethod.toString());
                            problemsWidget.addedMethods.addSubItem(item);
                        }
                    });
                }
            }

            Constructor<?> oldConstructors[] = oldClass.getConstructors();
            for (final Constructor<?> oldConstructor : oldConstructors) {
                oldModifiers = oldConstructor.getModifiers();
                if (!isPublic(oldModifiers) && !isProtected(oldModifiers))
                    continue;

                Constructor<?> newConstructor = findConstructor(newClass, oldConstructor);
                if (newConstructor == null) {
                    QApplication.invokeLater(new Runnable() {
                        public void run() {
                            QTreeWidgetItem item = new QTreeWidgetItem();
                            item.setText(0, "'" + oldConstructor.toString());
                            problemsWidget.missingMethods.addSubItem(item);
                        }
                    });
                } else {
                    newModifiers = newConstructor.getModifiers();
                    if (oldModifiers != newModifiers) {
                        final String oldModifierString = modifierString(oldModifiers);
                        final String newModifierString = modifierString(newModifiers);

                        QApplication.invokeLater(new Runnable() {
                           public void run() {
                              QTreeWidgetItem item = new QTreeWidgetItem();
                              item.setText(0, "Modifiers for '" + oldConstructor.toString() + "' changed from '" + oldModifierString + "' to '" + newModifierString + "'");
                              problemsWidget.changedMethods.addSubItem(item);
                           }
                        });
                    }
                }
            }

            Constructor<?> newConstructors[] = newClass.getConstructors();
            for (final Constructor<?> newConstructor : newConstructors) {
                newModifiers = newConstructor.getModifiers();
                if (!isPublic(newModifiers) && !isProtected(newModifiers))
                    continue;

                Constructor<?> oldConstructor = findConstructor(oldClass, newConstructor);
                if (oldConstructor == null) {
                    QApplication.invokeLater(new Runnable() {
                        public void run() {
                            QTreeWidgetItem item = new QTreeWidgetItem();
                            item.setText(0, "'" + newConstructor.toString() + "'");
                            problemsWidget.addedMethods.addSubItem(item);
                        }
                    });
                }
            }

            Field oldFields[] = oldClass.getDeclaredFields();
            for (final Field oldField : oldFields) {
                oldModifiers = oldField.getModifiers();
                if (!isPublic(oldModifiers) && !isProtected(oldModifiers))
                    continue;

                Field newField = findField(newClass, oldField);
                if (newField == null) {
                    QApplication.invokeLater(new Runnable() {
                        public void run() {
                            QTreeWidgetItem item = new QTreeWidgetItem();
                            item.setText(0, "'" + oldField.toString());
                            problemsWidget.missingFields.addSubItem(item);
                        }
                    });
                } else {
                    newModifiers = newField.getModifiers();
                    if (newModifiers != oldModifiers) {
                        final String oldModifierString = modifierString(oldModifiers);
                        final String newModifierString = modifierString(newModifiers);

                        QApplication.invokeLater(new Runnable() {
                            public void run() {
                               QTreeWidgetItem item = new QTreeWidgetItem();
                               item.setText(0, "Modifiers for '" + oldField.toString() + "' changed from '" + oldModifierString + "' to '" + newModifierString + "'");
                               problemsWidget.changedFields.addSubItem(item);
                            }
                         });
                    }

                    final Class<?> oldType = oldField.getType();
                    final Class<?> newType = newField.getType();
                    if (!oldType.toString().equals(newType.toString())) {
                        QApplication.invokeLater(new Runnable() {
                            public void run() {
                               QTreeWidgetItem item = new QTreeWidgetItem();
                               item.setText(0, "Type for '" + oldField.toString() + "' changed from '" + oldType.getName() + "' to '" + newType.getName());
                               problemsWidget.changedFields.addSubItem(item);
                            }
                         });

                    }
                }
            }

            Field newFields[] = newClass.getDeclaredFields();
            for (final Field newField : newFields) {
                newModifiers = newField.getModifiers();
                if (!isPublic(newModifiers) && !isProtected(newModifiers))
                    continue;

                Field oldField = findField(oldClass, newField);
                if (oldField == null) {
                    QApplication.invokeLater(new Runnable() {
                        public void run() {
                            QTreeWidgetItem item = new QTreeWidgetItem();
                            item.setText(0, "'" + newField.toString());
                            problemsWidget.addedFields.addSubItem(item);
                        }
                    });
                }
            }
        } catch (Throwable e) {
            e.printStackTrace();
        }
    }

    private String packageName(String className) {
        int pos = className.lastIndexOf(".");
        if (pos < 0)
            return "(default)";
        else
            return className.substring(0,pos).replaceAll("/", ".");
    }

    private void compare(final Hashtable<String, Class<?>> oldClasses, final Hashtable<String, Class<?>> newClasses) {
        QApplication.invokeLater(new Runnable() {
            public void run() {
                progressBar.setMinimum(0);
                progressBar.setMaximum(oldClasses.keySet().size() + newClasses.keySet().size());
                progressBar.setValue(0);
            }
        });

        final Hashtable<String,Boolean> disabledPackages = new Hashtable<String,Boolean>();
        if (packagesCheckVal) {
            QApplication.invokeAndWait(new Runnable() {
                public void run() {
                    QDialog dialog = new QDialog(BCC.this);
                    dialog.setWindowTitle("In which packages do you want to look?");

                    QVBoxLayout layout = new QVBoxLayout(dialog);

                    QListWidget list = new QListWidget();
                    Set<String> keys = new HashSet<String>(oldClasses.keySet());
                    keys.addAll(newClasses.keySet());
                    for (String key : keys) {
                        String packageName = packageName(key);

                        if (!disabledPackages.containsKey(packageName)) {
                            disabledPackages.put(packageName, false);

                            QListWidgetItem item = new QListWidgetItem(packageName);
                            item.setFlags(Qt.ItemFlag.ItemIsUserCheckable, Qt.ItemFlag.ItemIsEnabled);
                            item.setCheckState(Qt.CheckState.Checked);

                            list.addItem(item);
                        }
                    }
                    layout.addWidget(list);

                    QDialogButtonBox box = new QDialogButtonBox(QDialogButtonBox.StandardButton.createQFlags(QDialogButtonBox.StandardButton.Ok, QDialogButtonBox.StandardButton.Cancel));
                    layout.addWidget(box);

                    box.accepted.connect(dialog, "accept()");
                    box.rejected.connect(dialog, "reject()");

                    if (dialog.exec() == QDialog.DialogCode.Accepted.value()) {
                        int childCount = list.count();
                        for (int i=0; i<childCount; ++i) {
                            QListWidgetItem item = list.item(i);
                            if (item.checkState() != Qt.CheckState.Checked)
                                disabledPackages.put(item.text(), true);
                        }
                    }
                }
            });
        }

        Set<String> keys = oldClasses.keySet();
        for (final String key : keys) {
            String packageName = packageName(key);
            if (disabledPackages.containsKey(packageName) && disabledPackages.get(packageName))
                continue;

            Class<?> oldClass = oldClasses.get(key);
            Class<?> newClass = newClasses.containsKey(key) ? newClasses.get(key) : null;

            // Only public classes are interesting
            if ((oldClass.getModifiers() & Modifier.PUBLIC) == 0)
                continue;

            if (newClass == null) {
                QApplication.invokeLater(new Runnable() {

                    public void run() {
                        QTreeWidgetItem item = new QTreeWidgetItem();
                        item.setText(0, key);
                        problemsWidget.missingClasses.addSubItem(item);
                    }

                });
            } else {
                compareClasses(oldClass, newClass);
            }

            QApplication.invokeLater(new Runnable() {
                public void run() {
                    progressBar.setValue(progressBar.value() + 1);
                }
            });
        }

        keys = newClasses.keySet();
        for (final String key : keys) {
            String packageName = packageName(key);
            if (disabledPackages.containsKey(packageName) && disabledPackages.get(packageName))
                continue;

            Class<?> oldClass = oldClasses.containsKey(key) ? oldClasses.get(key) : null;
            Class<?> newClass = newClasses.get(key);

            // Only public classes are interesting
            if ((newClass.getModifiers() & Modifier.PUBLIC) == 0)
                continue;

            if (oldClass == null) {
                QApplication.invokeLater(new Runnable() {

                    public void run() {
                        QTreeWidgetItem item = new QTreeWidgetItem();
                        item.setText(0, key);
                        problemsWidget.addedClasses.addSubItem(item);
                    }

                });
            }

            QApplication.invokeLater(new Runnable() {
                public void run() {
                    progressBar.setValue(progressBar.value() + 1);
                }
            });
        }
    }

    private ProblemsWidget problemsWidget;
    private QCheckBox packagesCheck;
    private boolean packagesCheckVal = false;

    private BCC() {
        super();

        setWindowTitle("Binary Compatibility Clobbering");

        QGridLayout layout = new QGridLayout(this);
        QGuiSignalMapper jarFileMapper = new QGuiSignalMapper(this);
        QGuiSignalMapper directoryMapper = new QGuiSignalMapper(this);

        {
            QGroupBox leftSide = new QGroupBox("Old root of classes");

            QGridLayout leftLayout = new QGridLayout(leftSide);
            leftLayout.addWidget(leftDirectoryInput, 0, 0, 1, 1);

            QPushButton selectJarFile = new QPushButton("Select Jar File");
            jarFileMapper.setMapping(selectJarFile, leftDirectoryInput);
            jarFileMapper.mappedQWidget.connect(this, "selectJarFile(QWidget)");
            selectJarFile.clicked.connect(jarFileMapper, "map()");
            leftLayout.addWidget(selectJarFile, 0, 1, 1, 1);

            QPushButton selectDirectory = new QPushButton("Select directory");
            directoryMapper.setMapping(selectDirectory, leftDirectoryInput);
            directoryMapper.mappedQWidget.connect(this, "selectDirectory(QWidget)");
            selectDirectory.clicked.connect(directoryMapper, "map()");
            leftLayout.addWidget(selectDirectory, 0, 2, 1, 1);

            layout.addWidget(leftSide, 0, 0, 1, 1);
        }

        problemsWidget = new ProblemsWidget();
        layout.addWidget(problemsWidget, 1, 0, 1, 2);

        QPushButton scanButton = new QPushButton("Scan");
        progressBar = new QProgressBar();
        progressBar.setMinimum(0);
        progressBar.setMaximum(0);
        progressBar.setTextVisible(false);

        stack = new QStackedWidget();
        stack.addWidget(scanButton);
        stack.addWidget(progressBar);

        QSizePolicy policy = stack.sizePolicy();
        policy.setVerticalPolicy(QSizePolicy.Policy.Maximum);
        stack.setSizePolicy(policy);

        layout.addWidget(stack, 2, 0, 1, 2);

        QPushButton logButton = new QPushButton("Save to log");
        layout.addWidget(logButton, 3, 0, 1, 1);

        logButton.clicked.connect(problemsWidget, "writeLog()");
        scanButton.clicked.connect(this, "scan()");

        packagesCheck = new QCheckBox("Select packages manually");
        layout.addWidget(packagesCheck, 3, 1, 1, 1);

        {
            QGroupBox rightSide = new QGroupBox("New root of classes");

            QGridLayout rightLayout = new QGridLayout(rightSide);
            rightLayout.addWidget(rightDirectoryInput, 0, 0, 1, 1);

            QPushButton selectJarFile = new QPushButton("Select Jar File");
            jarFileMapper.setMapping(selectJarFile, rightDirectoryInput);
            selectJarFile.clicked.connect(jarFileMapper, "map()");
            rightLayout.addWidget(selectJarFile, 0, 1, 1, 1);

            QPushButton selectDirectory = new QPushButton("Select directory");
            directoryMapper.setMapping(selectDirectory, rightDirectoryInput);
            selectDirectory.clicked.connect(directoryMapper, "map()");
            rightLayout.addWidget(selectDirectory, 0, 2, 1, 1);

            layout.addWidget(rightSide, 0, 1, 1, 1);
        }

    }

    public static void main(String args[]) {
        QApplication.initialize(args);

        BCC bcc = new BCC();
        bcc.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

}
