/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.tools.ant;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.file.Files;
import java.util.Properties;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

import io.qt.tools.ant.OSInfo.OS;

public abstract class AbstractInitializeTask extends Task {

    private int verboseLevel;

    private boolean configurationViaSetter;
    private String configuration;

    private boolean test;

    protected PropertyHelper propertyHelper;


    public void execute() throws BuildException {
        propertyHelper = PropertyHelper.getPropertyHelper(getProject());
        executeInitialize();
    }

    abstract void executeInitialize() throws BuildException;


    public int getVerboseLevel() {
        return verboseLevel;
    }

    public void setVerbose(String verboseLevelString) {
        if("true".equalsIgnoreCase(verboseLevelString)) {
            verboseLevel = 1;
            return;
        } if("false".equalsIgnoreCase(verboseLevelString)) {
            verboseLevel = 0;
            return;
        }

        Integer i = Integer.valueOf(verboseLevelString);
        this.verboseLevel = i.intValue();
    }


    // package accessible
    protected String mySetProperty(int verboseMode, String attrName, String sourceValue, String newValue, boolean forceNewValue) throws BuildException {
        String currentValue = AntUtil.getPropertyAsString(propertyHelper, attrName);
        if(newValue != null) {
            if(currentValue != null && !currentValue.isEmpty()) {
                String s;
                if(currentValue.equals(newValue))
                    s = " (already set to same value)";
                else
                    s = " (already set; detected as: " + newValue + ")";
                if(sourceValue != null)
                    sourceValue += s;
                else
                    sourceValue = s;
                // Don't error if we don't have to i.e. the two values are the same
                if(forceNewValue && !newValue.equals(currentValue))
                    throw new BuildException("Unable to overwrite property " + attrName + " with value " + newValue + " (current value is: " + currentValue + ")");
            } else {
                if(forceNewValue)
                    AntUtil.setProperty(propertyHelper, attrName, newValue, false);
                else
                    AntUtil.setNewProperty(propertyHelper, attrName, newValue);
                currentValue = newValue;
            }
        } else {
            if(currentValue != null)
                sourceValue = null;  // we don't use newValue in any way, and currentValue exists
        }

        if(sourceValue == null)
            sourceValue = "";

        if((verboseMode == -1 && verboseLevel > 0) || (verboseMode > 0)) {
            String prettyCurrentValue = prettyValue(currentValue);
            getProject().log(this, attrName + ": " + prettyCurrentValue + sourceValue, Project.MSG_INFO);
        }

        return currentValue;
    }

    protected String prettyValue(String s) {
        if(s == null)
            s = "<notset>";
        else if(s.length() == 0)
            s = "<empty-string>";
        return s;
    }

    /**
     * Convert "/tmp/somdir/qt/lib/libQtCore.so.4" into "$QTDIR/lib/libQtCore.so.4"
     * @param s
     * @param qtdir  We expect this to have been validated and have any trailing pathSeparator removed.
     * @return
     */
    protected String prettyPathValue(String s, String qtdir) {
        if(s == null) {
            s = "<notset>";
        } else if(s.length() == 0) {
            s = "<empty-string>";
        } else if(qtdir != null) {
            if(s.startsWith(qtdir))
                return "$QTDIR" + s.substring(qtdir.length());
        }
        return s;
    }


    protected String decideJavaHomeTarget() {
        String sourceValue = null;
        String s = AntUtil.getPropertyAsString(propertyHelper, Constants.JAVA_HOME_TARGET);
        String error = null;
        if(s == null || s.isEmpty()) {
            try {
                s = System.getenv("JAVA_HOME_TARGET");
                if(s != null) {
                	File home = new File(s);
                	try {
						while(Files.isSymbolicLink(home.toPath())) {
							home = Files.readSymbolicLink(home.toPath()).toFile();
						}
					} catch (Exception e) {
					}
            		File includeDir = new File(home, "include");
            		File jni_h = new File(includeDir, "jni.h");
            		if(jni_h.isFile()) {
                    	sourceValue = " (from envvar:JAVA_HOME_TARGET)";
            		}else {
                		File binDir = new File(home, "bin");
                		File javaExe = new File(binDir, OSInfo.os()==OS.Windows ? "java.exe" : "java");
                		if(javaExe.isFile()) {
                			error = "JDK does not provide headers: "+home.getAbsolutePath();
                		}else {
                			error = "Environment variable JAVA_HOME_TARGET does not specify suitable JDK: "+s;
                		}
            			s = null;
            		}
                }
            } catch(SecurityException eat) {
            }
        }
        if(s == null || s.isEmpty()) {
            try {
                s = System.getenv("JAVA_HOME");
                if(s != null) {
                	File home = new File(s);
                	try {
						while(Files.isSymbolicLink(home.toPath())) {
							home = Files.readSymbolicLink(home.toPath()).toFile();
						}
					} catch (Exception e) {
					}
            		File includeDir = new File(home, "include");
            		File jni_h = new File(includeDir, "jni.h");
            		if(jni_h.isFile()) {
            			sourceValue = " (from envvar:JAVA_HOME)";
            			error = null;
            		}else {
                		File binDir = new File(home, "bin");
                		File javaExe = new File(binDir, OSInfo.os()==OS.Windows ? "java.exe" : "java");
                		if(javaExe.isFile()) {
                			error = "JDK does not provide headers: "+home.getAbsolutePath();
                		}else {
                			error = "Environment variable JAVA_HOME does not specify suitable JDK: "+s;
                		}
            			s = null;
            		}
                }
            } catch(SecurityException eat) {
            }
        }
//        System.getProperties().forEach((a,b)->System.out.println(a+"="+b));
        if(s == null || s.isEmpty()) {
        	s = System.getProperty("java.home");
        	if(s != null) {
        		File home = new File(s);
            	try {
					while(Files.isSymbolicLink(home.toPath())) {
						home = Files.readSymbolicLink(home.toPath()).toFile();
					}
				} catch (Exception e) {
				}
        		File includeDir = new File(home, "include");
        		File jni_h = new File(includeDir, "jni.h");
        		if(jni_h.isFile()) {
        			sourceValue = " (from JVM property:java.home)";
        			error = null;
        		}else {
        			s = null;
        		}
        	}
        }
        if(error!=null) {
        	throw new BuildException(error);
        }
        String result = s;
        mySetProperty(-1, Constants.JAVA_HOME_TARGET, sourceValue, result, true);
        String targetJavaVersion = this.getProject().getProperty("target.java.version");
        if(targetJavaVersion==null || targetJavaVersion.isEmpty()) {
	        File releaseFile = new File(new File(result), "release");
			targetJavaVersion = "11";
			String javaVersion = null;
	        if(releaseFile.exists()) {
		        Properties properties = new Properties();
				try(FileInputStream stream = new FileInputStream(releaseFile)){
					properties.load(stream);
				} catch (IOException e) {
					getProject().log("reading java version", e, Project.MSG_ERR);
				}
				javaVersion = properties.getProperty("JAVA_VERSION", "\"11\"");
	        }
			if(javaVersion!=null && !javaVersion.isEmpty()) {
				int offset = 0;
				if(javaVersion.startsWith("\"")) {
					++offset;
				}
                if(javaVersion.substring(offset).startsWith("1.")){
                    targetJavaVersion = "1.";
                    offset += 2;
                }else{
                    targetJavaVersion = "";
                }
                for (; offset < javaVersion.length(); ++offset) {
                    char c = javaVersion.charAt(offset);
                    if(c>='0' && c<='9') {
                        targetJavaVersion += c;
                    }else {
                        break;
                    }
                }
				PropertyHelper.setNewProperty(getProject(), "target.java.version", ""+targetJavaVersion);
				mySetProperty(-1, "target.java.version", " (auto-detected)", targetJavaVersion, true);
			}
			if(!targetJavaVersion.startsWith("1.")) {
				if(Integer.parseInt(targetJavaVersion)>9) {
					mySetProperty(-1, "java.module.based", " (auto-detected)", "true", true);
//					this.getProject().setProperty("java.module.based", "true");
				}
			}
			String minJavaVersion = this.getProject().getProperty("minimum.java.version");
			if(minJavaVersion==null || minJavaVersion.isEmpty()) {
				mySetProperty(-1, "minimum.java.version", " (auto-detected)", minJavaVersion = targetJavaVersion, true);
			}
			String srcJavaVersion = this.getProject().getProperty("source.java.version");
			if(srcJavaVersion==null || srcJavaVersion.isEmpty()) {
				mySetProperty(-1, "source.java.version", " (auto-detected)", minJavaVersion, true);
			}
        }
        String javaHome = AntUtil.getPropertyAsString(propertyHelper, Constants.JAVA_HOME_TARGET);
        String app = OSInfo.os()==OSInfo.OS.Windows ? "%1$s.exe" : "%1$s";
        File executable;
		if((executable = new File(new File(javaHome, "bin"), String.format(app, "java"))).exists()) {
			mySetProperty(-1, "tools.jvm", " (taken from "+Constants.JAVA_HOME_TARGET+")", executable.getAbsolutePath(), true);
		}else {
			mySetProperty(-1, "tools.jvm", " (default)", "java", true);
		}
		if((executable = new File(new File(javaHome, "bin"), String.format(app, "javac"))).exists()) {
			mySetProperty(-1, "tools.javac", " (taken from "+Constants.JAVA_HOME_TARGET+")", executable.getAbsolutePath(), true);
		}else {
			mySetProperty(-1, "tools.javac", " (default)", "javac", true);
		}
		if((executable = new File(new File(javaHome, "bin"), String.format(app, "javadoc"))).exists()) {
			mySetProperty(-1, "tools.javadoc", " (taken from "+Constants.JAVA_HOME_TARGET+")", executable.getAbsolutePath(), true);
		}else {
			mySetProperty(-1, "tools.javadoc", " (default)", "javadoc", true);
		}
        return result;
    }
    
    protected void decideAlternativeJavaHomesTarget() {
    	int qtMajorVersion = 5;
    	int qtMinorVersion = 0;
    	try {
    		qtMajorVersion = Integer.parseInt(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_MAJOR));
		} catch (Exception e) {
		}
    	try {
    		qtMinorVersion = Integer.parseInt(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_MINOR));
		} catch (Exception e) {
		}
        String sourceValue = null;
        String s = AntUtil.getPropertyAsString(propertyHelper, Constants.JAVA8_HOME_TARGET);
        if(s == null || s.isEmpty()) {
            try {
                s = System.getenv("JAVA8_HOME_TARGET");
                if(s != null) {
            		File includeDir = new File(s, "include");
            		File jni_h = new File(includeDir, "jni.h");
            		if(jni_h.isFile()) {
                    	sourceValue = " (from envvar:JAVA8_HOME_TARGET)";
            		}else {
            			s = null;
            		}
                }
            } catch(SecurityException eat) {
            }
        }
        if(s == null || s.isEmpty()) {
            try {
                s = System.getenv("JAVA8_HOME");
                if(s != null) {
            		File includeDir = new File(s, "include");
            		File jni_h = new File(includeDir, "jni.h");
            		if(jni_h.isFile()) {
            			sourceValue = " (from envvar:JAVA8_HOME)";
            		}else {
            			s = null;
            		}
                }
            } catch(SecurityException eat) {
            }
        }
        mySetProperty(-1, Constants.JAVA8_HOME_TARGET, sourceValue, s, true);
        switch(OSInfo.os()) {
        case MacOS: 
        	switch(System.getProperty("os.arch").toLowerCase()) {
        	case "arm64":
        	case "aarch64":
        		if((qtMajorVersion>=6 && qtMinorVersion>=2) || qtMajorVersion>6) {
        			sourceValue = " (from "+Constants.JAVA_HOME_TARGET+")";
        			mySetProperty(-1, Constants.JAVA_ARM64_HOME_TARGET, sourceValue, AntUtil.getPropertyAsString(propertyHelper, Constants.JAVA_HOME_TARGET), true);
        		}
        		s = AntUtil.getPropertyAsString(propertyHelper, Constants.JAVA_X64_HOME_TARGET);
                if(s == null || s.isEmpty()) {
                    try {
                        s = System.getenv("JAVA_X64_HOME_TARGET");
                        if(s != null) {
                    		File includeDir = new File(s, "include");
                    		File jni_h = new File(includeDir, "jni.h");
                    		if(jni_h.isFile()) {
                            	sourceValue = " (from envvar:JAVA_X64_HOME_TARGET)";
                    		}else {
                    			s = null;
                    		}
                        }
                    } catch(SecurityException eat) {
                    }
                }
                if(s == null || s.isEmpty()) {
                    try {
                        s = System.getenv("JAVA_X64_HOME");
                        if(s != null) {
                    		File includeDir = new File(s, "include");
                    		File jni_h = new File(includeDir, "jni.h");
                    		if(jni_h.isFile()) {
                    			sourceValue = " (from envvar:JAVA_X64_HOME)";
                    		}else {
                    			s = null;
                    		}
                        }
                    } catch(SecurityException eat) {
                    }
                }
                mySetProperty(-1, Constants.JAVA_X64_HOME_TARGET, sourceValue, s, true);
                if(qtMajorVersion==5) {
                	if(s!=null) {
                		File jvm;
	        			if((jvm = new File(new File(s, "bin"), "java")).exists()) {
	        				mySetProperty(-1, "tools.jvm", " (taken from "+Constants.JAVA_X64_HOME_TARGET+")", jvm.getAbsolutePath(), true);
	        			}else {
	        				mySetProperty(-1, "tools.jvm", " (default)", "java", true);
	        			}
                	}else {
                		throw new BuildException("Cannot build Qt5 without x64 JVM.");
                	}
                }
        		break;
        	case "x86_64":
        	case "x64":
        	case "amd64":
        		sourceValue = " (from "+Constants.JAVA_HOME_TARGET+")";
                mySetProperty(-1, Constants.JAVA_X64_HOME_TARGET, sourceValue, AntUtil.getPropertyAsString(propertyHelper, Constants.JAVA_HOME_TARGET), true);
                if(qtMajorVersion>=6 && qtMinorVersion>=2) {
	        		s = AntUtil.getPropertyAsString(propertyHelper, Constants.JAVA_ARM64_HOME_TARGET);
	                if(s == null || s.isEmpty()) {
	                    try {
	                        s = System.getenv("JAVA_ARM64_HOME_TARGET");
	                        if(s != null) {
	                    		File includeDir = new File(s, "include");
	                    		File jni_h = new File(includeDir, "jni.h");
	                    		if(jni_h.isFile()) {
	                            	sourceValue = " (from envvar:JAVA_ARM64_HOME_TARGET)";
	                    		}else {
	                    			s = null;
	                    		}
	                        }
	                    } catch(SecurityException eat) {
	                    }
	                }
	                if(s == null || s.isEmpty()) {
	                    try {
	                        s = System.getenv("JAVA_ARM64_HOME");
	                        if(s != null) {
	                    		File includeDir = new File(s, "include");
	                    		File jni_h = new File(includeDir, "jni.h");
	                    		if(jni_h.isFile()) {
	                    			sourceValue = " (from envvar:JAVA_ARM64_HOME)";
	                    		}else {
	                    			s = null;
	                    		}
	                        }
	                    } catch(SecurityException eat) {
	                    }
	                }
	                mySetProperty(-1, Constants.JAVA_ARM64_HOME_TARGET, sourceValue, s, true);
                }
        		break;
        	default:
        		break;            	
        	}
        	break;
        	default: break;
        }
    }

    protected String decideJavaOsarchTarget() {
        String sourceValue = null;
        String s = AntUtil.getPropertyAsString(propertyHelper, Constants.JAVA_OSARCH_TARGET);

        if(s == null || s.isEmpty()) {
            try {
                s = System.getenv("JAVA_OSARCH_TARGET");
                if(s != null)
                    sourceValue = " (from envvar:JAVA_OSARCH_TARGET)";
            } catch(SecurityException eat) {
            }
        }

        if(s == null || s.isEmpty()) {    // auto-detect using what we find
            // This is based on a token observation that the include directory
            //  only had one sub-directory (this is needed for jni_md.h).
            String javaHomeTarget = AntUtil.getPropertyAsString(propertyHelper, Constants.JAVA_HOME_TARGET);
            if(javaHomeTarget != null && !javaHomeTarget.isEmpty()) {
                File includeDir = new File(javaHomeTarget, "include");
                File found = null;
                int foundCount = 0;

                if(includeDir.exists()) {
                    File[] listFiles = includeDir.listFiles();
                    for(File f : listFiles) {
                        if(f.isDirectory()) {
                            foundCount++;
                            found = f;
                        }
                    }
                }

                if(foundCount == 1) {
                    s = found.getName();
                    sourceValue = " (auto-detected)";
                }
            }
        }

        String result = s;
        mySetProperty(-1, Constants.JAVA_OSARCH_TARGET, sourceValue, result, true);
        return result;
    }

    protected String decideJavaOscpu() {
        String sourceValue = null;
        String s = AntUtil.getPropertyAsString(propertyHelper, Constants.JAVA_OSCPU);

        if(s == null || s.isEmpty()) {
            try {
                s = System.getenv("JAVA_OSCPU");
                if(s != null)
                    sourceValue = " (from envvar:JAVA_OSCPU)";
            } catch(SecurityException eat) {
            }
        }
        if(s == null || s.isEmpty()) {
            try {
                s = System.getProperty("os.arch");
                if(s != null)
                    sourceValue = " (detected from JVM property:os.arch)";
            } catch(SecurityException eat) {
            }
        }

        String result = s;
        mySetProperty(-1, Constants.JAVA_OSCPU, sourceValue, result, true);
        return result;
    }

    protected String decideJavaOscpuTarget() {
        String sourceValue = null;
        String s = AntUtil.getPropertyAsString(propertyHelper, Constants.JAVA_OSCPU_TARGET);

        if(s == null || s.isEmpty()) {
            try {
                s = System.getenv("JAVA_OSCPU_TARGET");
                if(s != null)
                    sourceValue = " (from envvar:JAVA_OSCPU_TARGET)";
            } catch(SecurityException eat) {
            }
        }
        if(s == null || s.isEmpty()) {
            s = AntUtil.getPropertyAsString(propertyHelper, Constants.JAVA_OSCPU);
            if(s != null)
                sourceValue = " (inherited from ${" + Constants.JAVA_OSCPU + "})";
        }
        if(s == null || s.isEmpty()) {
            try {
                s = System.getProperty("os.arch");
                if(s != null)
                    sourceValue = " (detected from JVM property:os.arch)";
            } catch(SecurityException eat) {
            }
        }

        String result = s;
        mySetProperty(-1, Constants.JAVA_OSCPU_TARGET, sourceValue, result, true);
        return result;
    }

    /**
     * Decides whether to use debug or release configuration
     *
     * @return string "debug" or "release" according config resolution
     */
    protected String decideConfiguration() {
        String c = getConfiguration();
        if(c == null) {
            // FIXME perform auto-detect (look at Qt SDK kind)
            throw new BuildException("invalid value for 'configuration' it has not been set");
        }
        return c;
    }

    protected void setConfigurationInternal(String configuration) {
        this.configuration = configuration;
    }
    public void setConfiguration(String configuration) {
        if(Constants.CONFIG_DEBUG.equals(configuration) || Constants.CONFIG_RELEASE.equals(configuration) ||
         Constants.CONFIG_TEST.equals(configuration) || Constants.CONFIG_DEBUG_AND_RELEASE.equals(configuration)) {
            this.configurationViaSetter = true;
            setConfigurationInternal(configuration);
            return;
        }
        throw new BuildException("invalid value for 'configuration' of '" + prettyValue(configuration) + "' allowed settings 'debug', 'release', 'test', 'debug_and_release'");
    }
    public String getConfiguration() {
        return configuration;
    }
    protected boolean isConfigurationViaSetter() {
        return configurationViaSetter;
    }
    protected boolean isConfigurationDebug() {
        return Constants.CONFIG_DEBUG.equals(configuration);
    }
    protected boolean isConfigurationTest() {
        return Constants.CONFIG_TEST.equals(configuration);
    }
    protected boolean isConfigurationDebugAndRelease() {
        return Constants.CONFIG_DEBUG_AND_RELEASE.equals(configuration);
    }

    public void setTest(boolean test) {
        this.test = test;
    }
    protected boolean getTest() {
        return test;
    }
}
