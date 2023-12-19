package io.qt.autotests;

import java.io.Serializable;

import io.qt.QtInvokable;
import io.qt.core.QBuffer;
import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QDataStream;
import io.qt.core.QDir;
import io.qt.core.QFile;
import io.qt.core.QIODevice;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.QRect;
import io.qt.core.QRectF;
import io.qt.core.QTextStream;
import io.qt.core.QTimer;
import io.qt.core.QUrl;
import io.qt.core.Qt;
import io.qt.gui.QColor;
import io.qt.remoteobjects.QRemoteObjectHost;
import io.qt.widgets.QApplication;

public class QtRemoteObjectsPong {
	
	public static final class NonQtType implements Serializable, Cloneable{
		private static final long serialVersionUID = 1L;
		int i;
		boolean b;
		double d;
		String s;
		
		public int getI() {
			return i;
		}
		public void setI(int i) {
			this.i = i;
		}
		public boolean isB() {
			return b;
		}
		public void setB(boolean b) {
			this.b = b;
		}
		public double getD() {
			return d;
		}
		public void setD(double d) {
			this.d = d;
		}
		public String getS() {
			return s;
		}
		public void setS(String s) {
			this.s = s;
		}
		@Override
		public String toString() {
			return "NonQtType [i=" + i + ", b=" + b + ", d=" + d + ", s=" + s + "]";
		}
	}
	
	public static class Pong extends QObject{
		public final Signal0 ping = new Signal0();
		public final Signal0 pong = new Signal0();
		
		{
			ping.connect(pong);
			pong.connect(()->{
				System.out.println("pongSignal emitted");
			});
			ping.connect(()->{
				System.out.println("pingSignal emitted");
			});
		}
		
		public void emitPing(){
			System.out.println("Pong.emitPing()");
			ping.emit();
		}
		
		public void emitPong(){
			System.out.println("Pong.emitPong()");
			pong.emit();
		}
		
		@QtInvokable
		public int getInt() {
			return 5;
		}
		
		@QtInvokable
		public QRect toRect(int left, int top, int width, int height) {
			return new QRect(left, top, width, height);
		}
		
		@QtInvokable
		public QByteArray fromRectF(QRectF rect) {
			QByteArray result = new QByteArray();
			QBuffer buffer = new QBuffer(result);
			buffer.open(QIODevice.OpenModeFlag.WriteOnly);
			QDataStream stream = new QDataStream(buffer);
			stream.writeDouble(rect.left());
			stream.writeDouble(rect.top());
			stream.writeDouble(rect.width());
			stream.writeDouble(rect.height());
			buffer.close();
			return result;
		}
		
		@QtInvokable
		public NonQtType getNonQtType() {
			NonQtType value = new NonQtType();
			value.setI(5);
			value.setD(2.5);
			value.setS("TEST");
			return value;
		}
		
		@QtInvokable
		public int[] getIntArray() {
			return new int[]{1,2,3,4,5};
		}
		
		@QtInvokable
		public QColor getColor() {
			return new QColor(Qt.GlobalColor.magenta);
		}
		
		@QtInvokable
		public void quit() {
			QCoreApplication.quit();
		}
	}

	public static void main(String... args) {
		System.out.println("initialize QtRemoteObjectsPong...");
		QCoreApplication.initialize(args);
		{
			System.out.println("Writing PID "+QCoreApplication.applicationPid()+" to file "+new QDir(System.getProperty("user.dir")).absoluteFilePath("pid"));
			new QDir(System.getProperty("user.dir")).mkpath(".");
			QFile libFile = new QFile(new QDir(System.getProperty("user.dir")).absoluteFilePath("pid"));
			if(libFile.open(QIODevice.OpenModeFlag.WriteOnly)) {
				QTextStream s = new QTextStream(libFile);
				s.append(""+QCoreApplication.applicationPid());
				s.dispose();
				libFile.close();
			}
			libFile.dispose();
		}
		QMetaType.qRegisterMetaType(NonQtType.class);
		Pong pong = new Pong();
		pong.setObjectName("pong");
		QRemoteObjectHost host = new QRemoteObjectHost(new QUrl("local:ropong"));
		if(host.enableRemoting(pong)) {
			if (args.length > 1) {
				System.out.println("QtRemoteObjectsPong write hello-file...");
				QFile file = new QFile(QApplication.arguments().get(1));
				if (file.open(QIODevice.OpenModeFlag.WriteOnly)) {
					file.write(new QByteArray(QApplication.arguments().get(2)));
					file.close();
				}else {
					System.out.println("unable to write to " + QApplication.arguments().get(1));
				}
				file.dispose();
			}
			System.out.println("QtRemoteObjectsPong waiting for connection...");
			QTimer.singleShot(100000, QCoreApplication::quit);
			Thread thread = new Thread(()->{
				try {
					byte[] b = new byte[1024];
					int length = System.in.read(b);
					if(new String(b, 0, length).startsWith("quit")) {
						System.out.println("quit QtRemoteObjectsPong");
						QCoreApplication.quit();
					}
				} catch (Exception e) {
				}
			});
			thread.start();
			try {
				QCoreApplication.exec();
			}finally {
				thread.interrupt();
			}
			System.out.println("QtRemoteObjectsPong shutting down...");
		}else {
			if (args.length > 1) {
				System.out.println("QtRemoteObjectsPong write hello-file...");
				QFile file = new QFile(QApplication.arguments().get(1));
				if (file.open(QIODevice.OpenModeFlag.WriteOnly)) {
					file.write(new QByteArray(QApplication.arguments().get(2)));
					file.close();
				}else {
					System.out.println("unable to write to " + QApplication.arguments().get(1));
				}
				file.dispose();
			}
			System.out.println("unable to start remoting: "+host.lastError());
		}
		host.dispose();
		pong.dispose();
		QCoreApplication.shutdown();
	}
}
