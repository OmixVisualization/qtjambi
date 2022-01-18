package io.qt.autotests;

import java.io.Serializable;
import java.util.Arrays;

import io.qt.QtInvokable;
import io.qt.core.QBuffer;
import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QDataStream;
import io.qt.core.QFile;
import io.qt.core.QIODevice;
import io.qt.core.QObject;
import io.qt.core.QRect;
import io.qt.core.QRectF;
import io.qt.core.QThread;
import io.qt.core.QTimer;
import io.qt.core.Qt;
import io.qt.dbus.*;
import io.qt.gui.QColor;
import io.qt.widgets.QApplication;

public class QtDBusPong {
	
	public static final class NonQtType implements Serializable{
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
	}

	static String PONG_SERVICE = "dbus.pong";
	static String PONG_INTERFACE_NAME = "dbus.Pong";

	static int i = 0;
	
	public static void registerTypes() {
		QDBusMetaType.registerDBusMetaType(QColor.class);
	}

	public static void main(String... args) {
		System.out.println("QtDBusPong get session bus...");
		QDBusConnection sb = QDBusConnection.sessionBus();
		try {
			if (!sb.isConnected())
				throw new RuntimeException("Cannot connect to the D-Bus session bus: "+sb.lastError().message());
			System.out.println("QtDBusPong initializing...");
			QCoreApplication.initialize(args);
			try {
				System.out.println("QtDBusPong register service...");
				if (!sb.registerService(PONG_SERVICE))
					throw new RuntimeException(sb.lastError().message());
				registerTypes();
				@SuppressWarnings("unused")
				QObject pong = new QObject() {
					public String pingPong(String arg) {
						System.out.println("QtDBusPong received: " + arg);
						return "Pong#" + (i++);
					}
					
					public QRect toRect(int left, int top, int width, int height) {
						return new QRect(left, top, width, height);
					}
					
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
					
					private int[] intArray = new int[]{1,2,3,4,5};
					
					@QtInvokable
					public int[] getIntArray() {
						System.out.println("getIntArray() = "+Arrays.toString(intArray));
						return intArray;
					}
					
					@QtInvokable
					public void setIntArray(int[] intArray) {
						System.out.println("setIntArray("+Arrays.toString(intArray)+")");
						this.intArray = intArray;
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
					public QColor getColor() {
						return new QColor(Qt.GlobalColor.magenta);
					}
					
					public String callMultiArgs(NonQtType value/*, int i, double d, QColor color*/) {
						return "multi args: "+value/*+", "+i+", "+d+", "+color*/+")";
					}
					
					@QtInvokable
					public void quit() {
						QCoreApplication.quit();
					}
				};
				System.out.println("QtDBusPong register object...");
				sb.registerObject("/", PONG_INTERFACE_NAME, pong, QDBusConnection.RegisterOption.ExportAllSlots);
				if (args.length > 1) {
					System.out.println("QtDBusPong write hello-file...");
					QFile file = new QFile(QApplication.arguments().get(1));
					if (file.open(QIODevice.OpenModeFlag.WriteOnly)) {
						file.write(new QByteArray(QApplication.arguments().get(2)));
						file.close();
					}
					file.dispose();
				}
				System.out.println("QtDBusPong waiting...");
				QTimer.singleShot(100000, QCoreApplication::quit);
				Thread thread = new Thread(()->{
					try {
						byte[] b = new byte[1024];
						int length = System.in.read(b);
						if(new String(b, 0, length).startsWith("quit")) {
							QCoreApplication.quit();
						}
					} catch (Exception e) {
					}
				});
				thread.start();
				try{
					QCoreApplication.exec();
				}finally {
					thread.interrupt();
				}
				System.out.println("QtDBusPong shutting down...");
			}finally {
				QCoreApplication.shutdown();
			}
		} catch (Exception e) {
			e.printStackTrace(System.out);
			throw e;
		}finally {
			sb.dispose();
		}
	}

}
