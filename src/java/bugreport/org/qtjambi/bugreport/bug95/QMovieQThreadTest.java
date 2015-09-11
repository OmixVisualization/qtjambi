// mkdir build/bugreport
// javac -g -d build/bugreport  -cp qtjambi-4.7.4.jar -source 1.5 -target 1.5  src/java/bugreport/org/qtjambi/bugreport/bug95/QMovieQThreadTest.java
// jar -cvf $PWD/bug95.jar -C src/java/bugreport .
// jar -uvf $PWD/bug95.jar -C build/bugreport .
// java -Dorg.qtjambi.qt.debug=true -cp qtjambi-4.7.4.jar:build/qtjambi-linux64-gcc-4.7.4.jar:bug95.jar org/qtjambi/bugreport/bug95/QMovieQThreadTest

package org.qtjambi.bugreport.bug95;

import org.qtjambi.qt.QThread;
import org.qtjambi.qt.core.QBuffer;
import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QDataStream;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.gui.QApplication;
import org.qtjambi.qt.gui.QLabel;
import org.qtjambi.qt.gui.QMainWindow;
import org.qtjambi.qt.gui.QMovie;
import org.qtjambi.qt.gui.QPushButton;
import org.qtjambi.qt.gui.QVBoxLayout;
import org.qtjambi.qt.gui.QWidget;

public class QMovieQThreadTest extends QMainWindow
{
	private QMovie busyMovie;
	private QIODevice qIODevice;		// hard-ref
	
	public static void main(String[] args)
	{
		QApplication.initialize(args);

		new QMovieQThreadTest().show();

		QApplication.execStatic();
		QApplication.shutdown();
	}

	public QMovieQThreadTest()
	{
		super();
		
		System.out.println(QMovie.supportedFormats());
		
		QVBoxLayout mainLayout = new QVBoxLayout();
		
		QLabel busyLabel = new QLabel();
		/*
		 * loading movie directly from a QFile results in JRE fatal error:
		 * 	QFSFileEngine::open: No file name specified
		 *  QFile::seek: IODevice is not open
		 *	QFSFileEngine::open: No file name specified
		 *  QFSFileEngine::open: No file name specified
		 *  QFSFileEngine::open: No file name specified
		 *  QFSFileEngine::open: No file name specified
		 *  QFSFileEngine::open: No file name specified
		 *  
		 *  SIGSEGV (0xb) at pc=0x00007f36c29474e4, pid=18198, tid=139874917857552
		*/
		
		// neither way of specifying the file fixes the fatal error:
		//QFile file = new QFile("src/java/org/qtjambi/bugreport/bug95/busy.gif");
		//QFile file = new QFile("classpath:org/qtjambi/bugreport/bug95/busy.gif");
		
		/*
		System.out.println("File exists: "+file.exists());
		  
		file.open(new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadOnly));
		
		busyLabel.setMovie(new QMovie(file));
		*/
		
		// busyMovie = new QMovie(getIODevice("classpath:org/qtjambi/bugreport/bug95/busy.gif"));
		qIODevice = getIODevice("classpath:org/qtjambi/bugreport/bug95/busy.gif");
		busyMovie = new QMovie(qIODevice);

                // This commenting out this line below causes no crash, due this due to no hard-reference to
                //  qIODevice being retained by Java.
                qIODevice = null;

		busyLabel.setMovie(busyMovie);
		
		System.out.println("Movie is valid: "+busyMovie.isValid());
		
		busyMovie.start();

		QPushButton btn = new QPushButton("Start Movie");
		btn.pressed.connect(busyMovie, "start()");
		mainLayout.addWidget(btn);
		
		btn = new QPushButton("Stop Movie");
		btn.pressed.connect(busyMovie, "stop()");
		mainLayout.addWidget(btn);
		
		btn = new QPushButton("Run QThread");
		btn.pressed.connect(this, "runQThread()");
		mainLayout.addWidget(btn);

		
		mainLayout.addWidget(busyLabel);
		
		QWidget w = new QWidget();
		w.setLayout(mainLayout);
		setCentralWidget(w);
	}
	
	private void runQThread()
	{
		TestRunnable testRunnable = new TestRunnable();

		QThread qThread = new QThread(testRunnable);
		
		qThread.start();
	}
	
	
	private static QIODevice getIODevice(String resourceName)
	{
		QFile f = new QFile(resourceName);
		
		f.open(new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadOnly));
		
		byte[] bytes = new byte[(int)f.size()];
		
		QDataStream s = new QDataStream(f);
		
		s.readBytes(bytes);
		
		f.close();
		
		QBuffer qb = new QBuffer(new QByteArray(bytes));
		
		qb.open(QIODevice.OpenModeFlag.ReadOnly);

		return qb;
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
