/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <Carbon/Carbon.h>

#include <QtGui/qpa/qplatformnativeinterface.h>
#include "classes_p.h"

bool NSWindowUtils::isOnActiveSpace(QWindow* window) const
{
  if (NSWindow *nswindow = static_cast<NSWindow*>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow(QByteArrayLiteral("nswindow"), window)))
    return [nswindow isOnActiveSpace];
  return false;
}

bool NSWindowUtils::isMainWindow(QWindow* window) const
{
  if (NSWindow *nswindow = static_cast<NSWindow*>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow(QByteArrayLiteral("nswindow"), window)))
    return [nswindow isMainWindow];
  return false;
}

bool NSWindowUtils::canBecomeMain(QWindow* window) const
{
  if (NSWindow *nswindow = static_cast<NSWindow*>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow(QByteArrayLiteral("nswindow"), window)))
    return [nswindow canBecomeMain];
  return false;
}

int NSWindowUtils::windowNumber(QWindow* window) const
{
  if (NSWindow *nswindow = static_cast<NSWindow*>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow(QByteArrayLiteral("nswindow"), window)))
    return [nswindow windowNumber];
  return 0;
}

bool NSWindowUtils::canBecomeVisibleWithoutLogin(QWindow* window) const{
  if (NSWindow *nswindow = static_cast<NSWindow*>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow(QByteArrayLiteral("nswindow"), window)))
    return [nswindow canBecomeVisibleWithoutLogin];
  return false;
}

void NSWindowUtils::setCanBecomeVisibleWithoutLogin(QWindow* window, bool b) const{
  if (NSWindow *nswindow = static_cast<NSWindow*>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow(QByteArrayLiteral("nswindow"), window)))
    nswindow.canBecomeVisibleWithoutLogin = b;
}

void NSWindowUtils::makeMain(QWindow* window) const{
  if (NSWindow *nswindow = static_cast<NSWindow*>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow(QByteArrayLiteral("nswindow"), window)))
    [nswindow makeMain];
}

bool NSWindowUtils::allowsToolTipsWhenApplicationIsInactive(QWindow* window) const{
  if (NSWindow *nswindow = static_cast<NSWindow*>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow(QByteArrayLiteral("nswindow"), window)))
    return [nswindow allowsToolTipsWhenApplicationIsInactive];
  return false;
}

void NSWindowUtils::setAllowsToolTipsWhenApplicationIsInactive(QWindow* window, bool b) const{
  if (NSWindow *nswindow = static_cast<NSWindow*>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow(QByteArrayLiteral("nswindow"), window)))
    nswindow.allowsToolTipsWhenApplicationIsInactive = b;
}

