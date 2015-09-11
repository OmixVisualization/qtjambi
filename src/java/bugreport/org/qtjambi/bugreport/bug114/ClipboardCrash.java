// mkdir build/bugreport
// javac -g -d build/bugreport  -cp qtjambi-4.7.4.jar -source 1.5 -target 1.5  src/java/bugreport/org/qtjambi/bugreport/bug114/ClipboardCrash.java
// jar -cvf $PWD/bug114.jar -C src/java/bugreport .
// jar -uvf $PWD/bug114.jar -C build/bugreport .
// java -Dorg.qtjambi.qt.debug=true -cp qtjambi-4.7.4.jar:build/qtjambi-linux64-gcc-4.7.4.jar:bug114.jar org/qtjambi/bugreport/bug114/ClipboardCrash

package org.qtjambi.bugreport.bug114;

import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;

import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.QThread;
import org.qtjambi.qt.gui.QKeyEvent;
import org.qtjambi.qt.gui.QApplication;
import org.qtjambi.qt.gui.QClipboard;
import org.qtjambi.qt.gui.QMainWindow;

public class ClipboardCrash //extends QMainWindow
{
        QMainWindow mainWindow;

	public static void main(String[] args)
	{
		QApplication.initialize(args);
	        ClipboardCrash clipboardCrash = new ClipboardCrash(args);
	        clipboardCrash = null;
                QApplication.shutdown();
	}

        private void setupClipboard() {
                // We use a method to cause the reference to "clipboard" to be destroyed
                /*QClipboard clipboard =*/ QApplication.clipboard().dataChanged.connect(this, "grabClipboard()");
        }

	public ClipboardCrash(String[] args)
	{
//		QApplication.initialize(args);

                setupClipboard();

                mainWindow = new QMainWindow() {
                    @Override
                    protected void keyPressEvent(final QKeyEvent event) {
                        int key = event.key();
                        System.out.println("keyPressEvent() event.key()="+key);
                        if(key == Qt.Key.Key_G.value()) {
                            System.out.println("keyPressEvent(): GC");
                            System.gc();
                        }
                        if(key == Qt.Key.Key_T.value()) {
                            System.out.println("keyPressEvent(): THREAD");
                            runQThread();
                        }
                        if(key == Qt.Key.Key_Q.value()) {
                            System.out.println("keyPressEvent(): QUIT");
                            QApplication.clipboard().dataChanged.disconnect();
                            QApplication.quit();
                        }
                    }
                };
                mainWindow.show();

                QApplication.execStatic();
		mainWindow = null;

		// This test case crashes when QApplication is destroyed while QMainWindow exists
//                QApplication.shutdown();
	}

        private void grabClipboard() throws Exception {
                System.out.println("Clipboard updated");
                Thread.sleep(100);

                Toolkit toolkit = Toolkit.getDefaultToolkit();
                Clipboard clipboard = toolkit.getSystemClipboard();
                Transferable t = clipboard.getContents(null);

                if(t.isDataFlavorSupported(DataFlavor.stringFlavor)) {
                    System.out.println("\t text");
                }

                if(t.isDataFlavorSupported(DataFlavor.imageFlavor)) {
                    System.out.println("\t image");
                }

                if(t.isDataFlavorSupported(DataFlavor.javaFileListFlavor)) {
                    System.out.println("\t file");
                }
	}

        // Sarogate heavy weight thing to cause memory re-use
	private void runQThread()
	{
		TestRunnable testRunnable = new TestRunnable();

		QThread qThread = new QThread(testRunnable);
		
		qThread.start();
	}

	private class TestRunnable implements Runnable
	{
		public TestRunnable()
		{
		}

		public void run()
		{
			System.out.println("TestRunnable run() start");
			
			System.out.println("TestRunnable run() finished");
		}
	}
}
