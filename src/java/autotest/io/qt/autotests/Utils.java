/****************************************************************************
**
** Copyright (C) 2011 Darryl L. Miles.  All rights reserved.
** Copyright (C) 2011 D L Miles Consulting Ltd.  All rights reserved.
**
** This file is part of Qt Jambi.
**
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
** 
** In addition, as a special exception, the copyright holders grant you
** certain additional rights. These rights are described in the Nokia Qt
** LGPL Exception version 1.0, included in the file LGPL_EXCEPTION.txt in
** this package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.autotests;

public abstract class Utils {

    ////////////////////////////////////////////////////////////////////////////

    private static Object[] flipUseMemoryValue;
    private static int flipUseMemorySize = 4096;
    private static void flipUseMemory() {
        synchronized(Utils.class) {
            int i;
            if(flipUseMemoryValue == null) {
                flipUseMemoryValue = new Object[flipUseMemorySize];
                for(i = 0; i < flipUseMemoryValue.length; i++) {
                    if((i % 4) == 0)
                        flipUseMemoryValue[i] = Long.valueOf(flipUseMemorySize ^ 0xc4) | (i & 0x13);
                    else if((i % 4) == 1)
                        flipUseMemoryValue[i] = Integer.valueOf(flipUseMemorySize ^ 0xc4) | (i & 0x13);
                    else if((i % 4) == 2)
                        flipUseMemoryValue[i] = new byte[(i & 0xf)];
                    else
                        flipUseMemoryValue[i] = new Object();
                }
                flipUseMemorySize += 1024;
                if(flipUseMemorySize > 20480)
                    flipUseMemorySize = 4096;
            } else {
                for(i = 0; i < flipUseMemoryValue.length; i++) {
                    flipUseMemoryValue[i] = null;	// they say it doesn't really help
                }
                flipUseMemoryValue = null;
            }
        }
    }

    public enum GC_MODE {
        SIMPLE,
        AGGRESIVE
    };

    public static void gc(final GC_MODE mode) {
        if(mode != GC_MODE.SIMPLE) {
            flipUseMemory();
            System.gc();
            try {
				Thread.sleep(50);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
            flipUseMemory();
            System.gc();
            try {
				Thread.sleep(50);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
            flipUseMemory();
        }
        System.gc();
        try {
			Thread.sleep(50);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
    }

    public static void gc() {
        gc(GC_MODE.SIMPLE);
    }

    public static void gc(final GC_MODE mode, final int c) {
        int i;
        for(i = 0; i < c; i++) {
            gc(mode);
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    private static Integer debugLevel;
    private static Integer DEFAULT_DEBUG_LEVEL = Integer.valueOf(1);

    public static int getDebugLevel() {
        Integer tmpDebugLevel = debugLevel;
        if(tmpDebugLevel != null)
            return tmpDebugLevel.intValue();

        synchronized(Utils.class) {
            tmpDebugLevel = debugLevel;
            if(tmpDebugLevel != null)
                return tmpDebugLevel.intValue();

            try {
                final String PROP_NAME = "io.qt.debug.level";
                String s = System.getProperty(PROP_NAME);
                if(s != null) {
                    tmpDebugLevel = Integer.valueOf(s);
                    if(tmpDebugLevel < 0)
                        throw new NumberFormatException("number is negative");
                    if(tmpDebugLevel.intValue() > 1)
                        System.out.println("-D" + PROP_NAME + "=" + tmpDebugLevel + "; is set");
                } else {
                    tmpDebugLevel = DEFAULT_DEBUG_LEVEL;
                }
            } catch(NumberFormatException e) {
                e.printStackTrace();
                tmpDebugLevel = DEFAULT_DEBUG_LEVEL;
            }
            debugLevel = tmpDebugLevel;
        }
        return tmpDebugLevel.intValue();
    }

    public static boolean isDebugLevel(int minimumLevel) {
        int debugLevel = getDebugLevel();
        if(debugLevel >= minimumLevel)
            return true;
        return false;
    }

    public static void println(int level, String msg, Object... args) {
        if(isDebugLevel(level) == false)
            return;

        String s;
        if(args != null && args.length > 0) {
            s = String.format(msg, args);
        } else {
            s = msg;
        }
        System.out.println(s);
        System.out.flush();
    }

    ////////////////////////////////////////////////////////////////////////////
}
