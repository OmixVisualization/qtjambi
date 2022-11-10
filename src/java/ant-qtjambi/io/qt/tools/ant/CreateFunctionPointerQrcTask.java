package io.qt.tools.ant;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

public class CreateFunctionPointerQrcTask extends Task {
	
	@Override
	public void execute() throws BuildException {
		PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
		int qtMajorVersion = Integer.parseInt(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_MAJOR));
		String fileName = "functionpointers.qrc";
		if("qtjambi".equals(module)) {
			String libFormat = "%1$s.dll";
			switch(OSInfo.crossOS()) {
			case Windows:
				libFormat = "%1$s.dll";
				break;
			case MacOS:
				libFormat = "lib%1$s.dylib";
				break;
			case Linux:
				libFormat = "lib%1$s.so";
				break;
			case Android:
				if(qtMajorVersion<6) {
					libFormat = "lib%1$s_armeabi-v7a.so";
					write(new java.io.File(new java.io.File(new java.io.File(new java.io.File(dir), "qtjambi"), "release-armeabi-v7a"), fileName), libFormat, qtMajorVersion);
					write(new java.io.File(new java.io.File(new java.io.File(new java.io.File(dir), "qtjambi"), "debug-armeabi-v7a"), fileName), libFormat, qtMajorVersion);
					libFormat = "lib%1$s_arm64-v8a.so";
					write(new java.io.File(new java.io.File(new java.io.File(new java.io.File(dir), "qtjambi"), "release-arm64-v8a"), fileName), libFormat, qtMajorVersion);
					write(new java.io.File(new java.io.File(new java.io.File(new java.io.File(dir), "qtjambi"), "debug-arm64-v8a"), fileName), libFormat, qtMajorVersion);
					libFormat = "lib%1$s_x86.so";
					write(new java.io.File(new java.io.File(new java.io.File(new java.io.File(dir), "qtjambi"), "release-x86"), fileName), libFormat, qtMajorVersion);
					write(new java.io.File(new java.io.File(new java.io.File(new java.io.File(dir), "qtjambi"), "debug-x86"), fileName), libFormat, qtMajorVersion);
					libFormat = "lib%1$s_x86_64.so";
					write(new java.io.File(new java.io.File(new java.io.File(new java.io.File(dir), "qtjambi"), "release-x86_64"), fileName), libFormat, qtMajorVersion);
					write(new java.io.File(new java.io.File(new java.io.File(new java.io.File(dir), "qtjambi"), "debug-x86_64"), fileName), libFormat, qtMajorVersion);
					return;
				}else {
					switch(OSInfo.crossOSArchName()) {
		        	case OSInfo.K_ANDROID_ARM32:
		        		libFormat = "lib%1$s_armeabi-v7a.so"; break;
		        	case OSInfo.K_ANDROID_ARM64:
		        		libFormat = "lib%1$s_arm64-v8a.so"; break;
		        	case OSInfo.K_ANDROID_X86:
		        		libFormat = "lib%1$s_x86.so"; break;
		        	case OSInfo.K_ANDROID_X64:
		        		libFormat = "lib%1$s_x86_64.so"; break;
		        	}
					break;
				}
			default:
				return;
			}
			write(new java.io.File(new java.io.File(new java.io.File(new java.io.File(dir), "qtjambi"), "release"), fileName), libFormat, qtMajorVersion);
			write(new java.io.File(new java.io.File(new java.io.File(new java.io.File(dir), "qtjambi"), "debug"), fileName), libFormat, qtMajorVersion);
		}
	}
	
	private void write(java.io.File file, String libFormat, int qtMajorVersion) {
		file.getParentFile().mkdirs();
		try(FileOutputStream fos = new FileOutputStream(file);
				PrintWriter stream = new PrintWriter(fos)){
			stream.println("<RCC>");
			stream.println("    <qresource prefix=\"/io/qt/qtjambi/functionpointers\">");
//			stream.append("        <file alias=\"void(QObject*,QMetaObject::Call,int,void**)\">").append(String.format(libFormat, "StaticMetaCallFunction"));
//			stream.println("</file>");
			stream.append("        <file alias=\"GenericFunction\">").append(String.format(libFormat, "GenericFunction"));
			stream.println("</file>");
			stream.append("        <file alias=\"Generic1Function\">").append(String.format(libFormat, "Generic1Function"));
			stream.println("</file>");
			stream.append("        <file alias=\"Generic2Function\">").append(String.format(libFormat, "Generic2Function"));
			stream.println("</file>");
			stream.append("        <file alias=\"Generic3Function\">").append(String.format(libFormat, "Generic3Function"));
			stream.println("</file>");
			stream.append("        <file alias=\"Generic4Function\">").append(String.format(libFormat, "Generic4Function"));
			stream.println("</file>");
			stream.append("        <file alias=\"Generic5Function\">").append(String.format(libFormat, "Generic5Function"));
			stream.println("</file>");
			stream.append("        <file alias=\"Generic6Function\">").append(String.format(libFormat, "Generic6Function"));
			stream.println("</file>");
			stream.append("        <file alias=\"double(double)\">").append(String.format(libFormat, "DoubleToDoubleFunction"));
			stream.println("</file>");
			stream.append("        <file alias=\"float(float)\">").append(String.format(libFormat, "FloatToFloatFunction"));
			stream.println("</file>");
			if(qtMajorVersion>=6) {
				stream.append("        <file alias=\"QPluginMetaData()\">").append(String.format(libFormat, "PluginMetaDataFunction"));
				stream.println("</file>");
				stream.append("        <file alias=\"QMetaType()\">").append(String.format(libFormat, "MetaTypeFunction"));
				stream.println("</file>");
				stream.append("        <file alias=\"QVariant(QJSValue const&amp;)\">").append(String.format(libFormat, "QmlTypeCreatorFunction"));
				stream.println("</file>");
			}else {
				stream.append("        <file alias=\"QtMetaTypePrivate::VariantData(const void*const*, int, unsigned int)\">").append(String.format(libFormat, "VariantDataFunction"));
				stream.println("</file>");
			}
			stream.println("    </qresource>");
			stream.println("</RCC>");
		} catch (IOException e) {
			throw new BuildException(e);
		}
	}
	
	private String dir;
	private String module;

	public String getDir() {
		return dir;
	}

	public void setDir(String path) {
		this.dir = path;
	}

	public String getModule() {
		return module;
	}

	public void setModule(String module) {
		this.module = module;
	}
}
