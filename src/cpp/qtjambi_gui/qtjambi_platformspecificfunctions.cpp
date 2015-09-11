/*
 *
 *
 *
 */

#include "qtjambi_platformspecificfunctions.h"

#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include <qtjambi/qtjambi_global.h>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambifunctiontable.h>
#include <qtjambi/qtjambilink.h>


#ifdef Q_OS_LINUX
 void qt_x11_wait_for_window_manager(QWidget *widget);
#endif

/*
 * Class:     org_qtjambi_qt_gui_X11Functions
 * Method:    __qt_x11_wait_for_window_manager
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_qtjambi_qt_gui_X11Functions__1_1qt_1x11_1wait_1for_1window_1manager
#ifdef Q_OS_LINUX
  (JNIEnv *, jclass, jlong w0){
  QWidget*  widget = (QWidget*) qtjambi_from_jlong(w0);
  qt_x11_wait_for_window_manager(widget);
#else
    (JNIEnv *, jclass, jlong){
#endif
  }

/*
 * Class:     org_qtjambi_qt_gui_MacOSXFunctions
 * Method:    __qt_mac_secure_keyboard
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_org_qtjambi_qt_gui_MacOSXFunctions__1_1qt_1mac_1secure_1keyboard
#ifdef Q_OS_MAC
  (JNIEnv *, jclass, jboolean enable){
 void qt_mac_secure_keyboard(bool enable);
 qt_mac_secure_keyboard((bool) enable);
#else
  (JNIEnv *, jclass, jboolean){
#endif
 }

/*
 * Class:     org_qtjambi_qt_gui_MacOSXFunctions
 * Method:    __qt_mac_set_dock_menu
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_qtjambi_qt_gui_MacOSXFunctions__1_1qt_1mac_1set_1dock_1menu
#ifdef Q_OS_MAC
  (JNIEnv *, jclass, jlong w0){
 QMenu*  menu = (QMenu*) qtjambi_from_jlong(w0);
 void qt_mac_set_dock_menu(QMenu *menu);
 qt_mac_set_dock_menu(menu);
#else
  (JNIEnv *, jclass, jlong){
#endif    
  }

/*
 * Class:     org_qtjambi_qt_gui_MacOSXFunctions
 * Method:    __qt_mac_set_menubar_icons
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_org_qtjambi_qt_gui_MacOSXFunctions__1_1qt_1mac_1set_1menubar_1icons
#ifdef Q_OS_MAC
  (JNIEnv *, jclass, jboolean enable){
 void qt_mac_set_menubar_icons(bool enable);
 qt_mac_set_menubar_icons((bool) enable);
#else
  (JNIEnv *, jclass, jboolean){
#endif    
  }

/*
 * Class:     org_qtjambi_qt_gui_MacOSXFunctions
 * Method:    __qt_mac_set_menubar_merge
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_org_qtjambi_qt_gui_MacOSXFunctions__1_1qt_1mac_1set_1menubar_1merge
#ifdef Q_OS_MAC
  (JNIEnv *, jclass, jboolean enable){
 void qt_mac_set_menubar_merge(bool enable);
 qt_mac_set_menubar_merge((bool) enable);
#else
  (JNIEnv *, jclass, jboolean){
#endif    
  }

/*
 * Class:     org_qtjambi_qt_gui_MacOSXFunctions
 * Method:    __qt_mac_set_native_menubar
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_org_qtjambi_qt_gui_MacOSXFunctions__1_1qt_1mac_1set_1native_1menubar
#ifdef Q_OS_MAC
  (JNIEnv *, jclass, jboolean enable){
 void qt_mac_set_native_menubar(bool enable);
 qt_mac_set_native_menubar((bool) enable);
#else
  (JNIEnv *, jclass, jboolean){
#endif    
  }

/*
 * Class:     org_qtjambi_qt_gui_MacOSXFunctions
 * Method:    __qt_mac_set_press_and_hold_context
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_org_qtjambi_qt_gui_MacOSXFunctions__1_1qt_1mac_1set_1press_1and_1hold_1context
#ifdef Q_OS_MAC
  (JNIEnv *, jclass, jboolean enable){
 void qt_mac_set_press_and_hold_context(bool enable);
 qt_mac_set_press_and_hold_context((bool) enable);
#else
  (JNIEnv *, jclass, jboolean){
#endif    
  }
