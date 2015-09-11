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

package org.qtjambi.qt;

/**
 * FIXME: This is priviledged API move to org.qtjambi.qt.native.internal
 * TODO: Rename this class since it relates to QtJambi not Qt API "QtJambiThreadManager"
 * @exclude
 */
class QThreadManager {

	static{
		QtJambi_LibraryInitializer.init();
	}
	
    private static NativeResourcesReleaseThread nativeResourcesReleaseThread;

    static boolean postShutdownRequest() {
        NativeResourcesReleaseThread tmpThread;
        synchronized(QThreadManager.class) {
            tmpThread = nativeResourcesReleaseThread;
        }
        if(tmpThread == null)
            return false;
        return tmpThread.postShutdownRequest();
    }

    static void interrupt() {
        NativeResourcesReleaseThread tmpThread;
        synchronized(QThreadManager.class) {
            tmpThread = nativeResourcesReleaseThread;
        }
        if(tmpThread == null)
            return;
        tmpThread.interrupt();
    }

    static boolean shutdown(long millis) {
        postShutdownRequest();
        NativeResourcesReleaseThread tmpThread;
        synchronized(QThreadManager.class) {
            tmpThread = nativeResourcesReleaseThread;
        }
        if(tmpThread == null)
            return true;
        try {
            tmpThread.join(millis);
        } catch(InterruptedException e) {
        }
        return tmpThread.isAlive() == false;
    }

    // This is a debugging / unittest aid
    static boolean setSkipIt(boolean skipIt) {
        NativeResourcesReleaseThread tmpThread;
        synchronized(QThreadManager.class) {
            tmpThread = nativeResourcesReleaseThread;
        }
        if(tmpThread == null)
            return true;
        return tmpThread.setSkipIt(skipIt);
    }

    private static class NativeResourcesReleaseThread extends Thread {
        private static final int DEFAULT_SLEEPTIME = 100;
        private static final int MILLIS_PER_MINUTE = 60 * 1000;

        private boolean skipIt;
        private boolean shutdownFlag;
        private int sleepTime = DEFAULT_SLEEPTIME;

        public NativeResourcesReleaseThread() {
            setName("qtjambi-" + NativeResourcesReleaseThread.class.getName());
            setDaemon(true);
        }

        boolean postShutdownRequest() {
            boolean bf;
            synchronized(this) {
                bf = shutdownFlag;
                if(!bf)
                    shutdownFlag = true;
                interrupt();		// wake it up now
            }
            return bf;
        }

        boolean setSkipIt(boolean skipIt) {
            boolean orig;
            synchronized(this) {
                orig = this.skipIt;
                this.skipIt = skipIt;
            }
            return orig;
        }

        @Override
        public void run() {
            while(true) {
                try {
                    sleep(sleepTime);
                } catch(InterruptedException e) {
                    interrupted();        // clears the flag
                }
                boolean doIt = true;
                synchronized(this) {
                    if(shutdownFlag)
                        break;
                    if(skipIt)
                        doIt = false;
                }
                if(doIt) {
                    int releaseCount = releaseNativeResources();
                    sleepTime = (releaseCount > 0) ? DEFAULT_SLEEPTIME : Math.min(sleepTime * 2, MILLIS_PER_MINUTE);
                } else {
                }
            }
            synchronized(QThreadManager.class) {
                nativeResourcesReleaseThread = null;
            }
        }
    }

    public static void initialize() {
        synchronized(QThreadManager.class) {
            if(nativeResourcesReleaseThread != null)
                return;
            nativeResourcesReleaseThread = new NativeResourcesReleaseThread();
        }
        nativeResourcesReleaseThread.start();
    }

    public synchronized static native int releaseNativeResources();
}
