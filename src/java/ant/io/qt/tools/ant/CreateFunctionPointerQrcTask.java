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
		String fileName = "functionpointers-%1$s.qrc";
		if("qtjambi".equals(module)) {
			String dlibFormat = "%1$sd.dll";
			String libFormat = "%1$s.dll";
			switch(OSInfo.crossOS()) {
			case Windows:
				dlibFormat = "%1$sd.dll";
				libFormat = "%1$s.dll";
				break;
			case IOS:
//				dlibFormat = "lib%1$s_debug.dylib";
//				libFormat = "lib%1$s.dylib";
//				break;
			case MacOS:
				dlibFormat = "lib%1$s_debug.dylib";
				libFormat = "lib%1$s.dylib";
				break;
			case Linux:
				dlibFormat = "lib%1$s_debug.so";
				libFormat = "lib%1$s.so";
				break;
			case Android:
				if(qtMajorVersion<6) {
					libFormat = "lib%1$s_armeabi-v7a.so";
					dlibFormat = "lib%1$s_debug_armeabi-v7a.so";
					write(new java.io.File(new java.io.File(new java.io.File(dir), "QtJambi"), String.format(fileName, "release-armeabi-v7a")), libFormat, qtMajorVersion);
					write(new java.io.File(new java.io.File(new java.io.File(dir), "QtJambi"), String.format(fileName, "debug-armeabi-v7a")), dlibFormat, qtMajorVersion);
					libFormat = "lib%1$s_arm64-v8a.so";
					dlibFormat = "lib%1$s_debug_arm64-v8a.so";
					write(new java.io.File(new java.io.File(new java.io.File(dir), "QtJambi"), String.format(fileName, "release-arm64-v8a")), libFormat, qtMajorVersion);
					write(new java.io.File(new java.io.File(new java.io.File(dir), "QtJambi"), String.format(fileName, "debug-arm64-v8a")), dlibFormat, qtMajorVersion);
					libFormat = "lib%1$s_x86.so";
					dlibFormat = "lib%1$s_debug_x86.so";
					write(new java.io.File(new java.io.File(new java.io.File(dir), "QtJambi"), String.format(fileName, "release-x86")), libFormat, qtMajorVersion);
					write(new java.io.File(new java.io.File(new java.io.File(dir), "QtJambi"), String.format(fileName, "debug-x86")), dlibFormat, qtMajorVersion);
					libFormat = "lib%1$s_x86_64.so";
					dlibFormat = "lib%1$s_debug_x86_64.so";
					write(new java.io.File(new java.io.File(new java.io.File(dir), "QtJambi"), String.format(fileName, "release-x86_64")), libFormat, qtMajorVersion);
					write(new java.io.File(new java.io.File(new java.io.File(dir), "QtJambi"), String.format(fileName, "debug-x86_64")), dlibFormat, qtMajorVersion);
					return;
				}else {
					switch(OSInfo.crossOSArchName()) {
		        	case OSInfo.K_ANDROID_ARM32:
		        		dlibFormat = "lib%1$s_debug_armeabi-v7a.so"; 
		        		libFormat = "lib%1$s_armeabi-v7a.so"; break;
		        	case OSInfo.K_ANDROID_ARM64:
		        		dlibFormat = "lib%1$s_debug_arm64-v8a.so"; 
		        		libFormat = "lib%1$s_arm64-v8a.so"; break;
		        	case OSInfo.K_ANDROID_X86:
		        		dlibFormat = "lib%1$s_debug_x86.so"; 
		        		libFormat = "lib%1$s_x86.so"; break;
		        	case OSInfo.K_ANDROID_X64:
		        		dlibFormat = "lib%1$s_debug_x86_64.so"; 
		        		libFormat = "lib%1$s_x86_64.so"; break;
		        	}
					break;
				}
			default:
				return;
			}
			write(new java.io.File(new java.io.File(new java.io.File(dir), "QtJambi"), String.format(fileName, "debug")), dlibFormat, qtMajorVersion);
			write(new java.io.File(new java.io.File(new java.io.File(dir), "QtJambi"), String.format(fileName, "release")), libFormat, qtMajorVersion);
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
			stream.append("        <file alias=\"GenericFunction\">../lib/").append(String.format(libFormat, "GenericFunction"));
			stream.println("</file>");
			stream.append("        <file alias=\"Generic1Function\">../lib/").append(String.format(libFormat, "Generic1Function"));
			stream.println("</file>");
			stream.append("        <file alias=\"Generic2Function\">../lib/").append(String.format(libFormat, "Generic2Function"));
			stream.println("</file>");
			stream.append("        <file alias=\"Generic3Function\">../lib/").append(String.format(libFormat, "Generic3Function"));
			stream.println("</file>");
			stream.append("        <file alias=\"Generic4Function\">../lib/").append(String.format(libFormat, "Generic4Function"));
			stream.println("</file>");
			stream.append("        <file alias=\"Generic5Function\">../lib/").append(String.format(libFormat, "Generic5Function"));
			stream.println("</file>");
			stream.append("        <file alias=\"Generic6Function\">../lib/").append(String.format(libFormat, "Generic6Function"));
			stream.println("</file>");
			stream.append("        <file alias=\"double(double)\">../lib/").append(String.format(libFormat, "DoubleToDoubleFunction"));
			stream.println("</file>");
			stream.append("        <file alias=\"float(float)\">../lib/").append(String.format(libFormat, "FloatToFloatFunction"));
			stream.println("</file>");
			if(qtMajorVersion>=6) {
				stream.append("        <file alias=\"QPluginMetaData()\">../lib/").append(String.format(libFormat, "PluginMetaDataFunction"));
				stream.println("</file>");
				stream.append("        <file alias=\"QMetaType()\">../lib/").append(String.format(libFormat, "MetaTypeFunction"));
				stream.println("</file>");
				stream.append("        <file alias=\"QVariant(QJSValue const&amp;)\">../lib/").append(String.format(libFormat, "QmlTypeCreatorFunction"));
				stream.println("</file>");
				stream.append("        <file alias=\"QUntypedPropertyBinding(QUntypedPropertyData const*,QPropertyBindingSourceLocation const&amp;)\">../lib/").append(String.format(libFormat, "MakeBindingFunction"));
				stream.println("</file>");
				stream.append("        <file alias=\"bool(QMetaType,QUntypedPropertyData*,QtPrivate::QPropertyBindingFunction)\">../lib/").append(String.format(libFormat, "PropertyBindingWrapperFunction"));
				stream.println("</file>");
				stream.append("        <file alias=\"QUntypedPropertyBinding(QUntypedPropertyData*,QUntypedPropertyBinding const&amp;)\">../lib/").append(String.format(libFormat, "BindingSetterFunction"));
				stream.println("</file>");
			}else {
				stream.append("        <file alias=\"QtMetaTypePrivate::VariantData(void* const*,int,unsigned int)\">../lib/").append(String.format(libFormat, "VariantDataFunction"));
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
