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
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

public class FindCompiler {

	private String compilerPathValue;
    private Compiler compiler;
    private boolean verbose = false;
    private final Task task;
    private final PropertyHelper propertyHelper;
    private String compilerDetectionLine;
    private String availableCompilers;

    public enum Compiler {
        MSVC1998("msvc98"),
        MSVC2002("msvc2002"),
        MSVC2003("msvc2003"),
        MSVC2005("msvc2005"),
        MSVC2005_x64("msvc2005x64"),
        MSVC2008("msvc2008"),
        MSVC2008_x64("msvc2008x64"),
        MSVC2010("msvc2010"),
        MSVC2010_x64("msvc2010x64"),
        MSVC2012("msvc2012"),
        MSVC2012_x64("msvc2012x64"),
        MSVC2013("msvc2013"),
        MSVC2013_x64("msvc2013x64"),
        MSVC2015("msvc2015"),
        MSVC2015_x64("msvc2015x64"),
        MSVC2017("msvc2017"),
        MSVC2017_x64("msvc2017x64"),
        MSVC2019_x86("msvc2019x86"),
        MSVC2019_x64("msvc2019x64"),
        MSVC2019_arm64("msvc2019arm64"),
        MSVC2022_x86("msvc2022x86"),
        MSVC2022_x64("msvc2022x64"),
        MSVC2022_arm64("msvc2022arm64"),
        MSVC20XX_x86("msvc20XXx86"),
        MSVC20XX_x64("msvc20XXx64"),
        MSVC20XX_arm64("msvc20XXarb64"),
        MinGW("mingw"),
        MinGW_W64("mingw-w64"),
        OldGCC("gcc3.3"),
        GCC("gcc"),
        SUNCC("suncc"),
        CLANG("clang"),
        Other("unknown");

        Compiler(String n) {
            this.name = n;
        }

        public String toString() { return name; }

        private String name;

        public static Compiler resolve(String name) {
            if(name.equals("msvc98")) return MSVC1998;
            if(name.equals("msvc2002")) return MSVC2002;
            if(name.equals("msvc2003")) return MSVC2003;
            if(name.equals("msvc2005")) return MSVC2005;
            if(name.equals("msvc2005x64")) return MSVC2005_x64;
            if(name.equals("msvc2008")) return MSVC2008;
            if(name.equals("msvc2008x64")) return MSVC2008_x64;
            if(name.equals("msvc2010")) return MSVC2010;
            if(name.equals("msvc2010x64")) return MSVC2010_x64;
            if(name.equals("msvc2012")) return MSVC2012;
            if(name.equals("msvc2012x64")) return MSVC2012_x64;
            if(name.equals("msvc2013")) return MSVC2013;
            if(name.equals("msvc2013x64")) return MSVC2013_x64;
            if(name.equals("msvc2015")) return MSVC2015;
            if(name.equals("msvc2015x64")) return MSVC2015_x64;
            if(name.equals("msvc2017")) return MSVC2017;
            if(name.equals("msvc2017x64")) return MSVC2017_x64;
            if(name.equals("msvc2019x86")) return MSVC2019_x86;
            if(name.equals("msvc2019x64")) return MSVC2019_x64;
            if(name.equals("msvc2019arm64")) return MSVC2019_arm64;
            if(name.equals("msvc2022x86")) return MSVC2022_x86;
            if(name.equals("msvc2022x64")) return MSVC2022_x64;
            if(name.equals("msvc2022arm64")) return MSVC2022_arm64;
            if(name.equals("msvc20XXx86")) return MSVC20XX_x86;
            if(name.equals("msvc20XXx64")) return MSVC20XX_x64;
            if(name.equals("msvc20XXarm64")) return MSVC20XX_arm64;
            if(name.equals("mingw")) return MinGW;
            if(name.equals("mingw-w64")) return MinGW_W64;
            if(name.equals("gcc3.3")) return OldGCC;
            if(name.equals("gcc")) return GCC;
            if(name.equals("suncc")) return SUNCC;
            if(name.equals("clang")) return CLANG;
            return Other;
        }
    }

    public FindCompiler(PropertyHelper propertyHelper, Task task) {
        this.propertyHelper = propertyHelper;
        this.task = task;
    }
    
    void checkCompilerDetails() {
        if(compiler!=null) switch(compiler) {
        case MSVC2005:
        case MSVC2005_x64:
        case MSVC2008:
        case MSVC2008_x64:
        case MSVC2010:
        case MSVC2010_x64:
        case MSVC2012:
        case MSVC2012_x64:
        case MSVC2013:
        case MSVC2013_x64:
        case MSVC2015:
        case MSVC2015_x64:
        case MSVC2017:
        case MSVC2017_x64:
            try {
                String vcdir = System.getenv("VSINSTALLDIR");
                if(vcdir == null) {
                    // It is not strictly _required_ to have VSINSTALLDIR set (i.e. if the user is using
                    //  Windows Platform SDK or VS Express editions to build qtjambi).  As the qtjambi can
                    //  still be built, used and deployed.   The target installed systems can manually
                    //  download and install vcredist.exe to provide the VS runtime needed.
                    System.err.println("WARNING: VSINSTALLDIR not set and building with MSVC toolchain; the resulting build output will not attempt to package Visual C redistributable components.");
                } else {
                    File fileVcDir = new File(vcdir);
                    if(!fileVcDir.isDirectory()) {
                        System.err.println("WARNING: VSINSTALLDIR is set but the path is not found or not a directory; the resulting build output will not attempt to package Visual C redistributable components.");
                        System.err.println("         VSINSTALLDIR=\"" + vcdir + "\"");
                    } else {
                    	if(task instanceof InitializeBuildTask)
                    		((InitializeBuildTask)task).mySetProperty(-1, Constants.VSINSTALLDIR, " (taken from environment)", vcdir, true);

                        String redistDir;
                        if(compiler == Compiler.MSVC2005_x64 || compiler == Compiler.MSVC2008_x64 || compiler == Compiler.MSVC2010_x64)
                            redistDir = Util.pathCanon(new String[] { vcdir, "vc", "redist", "amd64" });
                        else if(compiler == Compiler.MSVC2012_x64 || compiler == Compiler.MSVC2013_x64 || compiler == Compiler.MSVC2015_x64)
                            redistDir = Util.pathCanon(new String[] { vcdir, "vc", "redist", "x64" });
						else if(compiler == Compiler.MSVC2017)
							redistDir = Util.pathCanon(new String[] { vcdir, "vc", "redist", "MSVC", "14.10.25008", "x86" });
						else if(compiler == Compiler.MSVC2017_x64)
							redistDir = Util.pathCanon(new String[] { vcdir, "vc", "redist", "MSVC", "14.10.25008", "x64" });
						else
                            redistDir = Util.pathCanon(new String[] { vcdir, "vc", "redist", "x86" });
                        File fileRedistDir = new File(redistDir);
                        if(!fileRedistDir.isDirectory()) {
                            System.err.println("WARNING: VSINSTALLDIR is set but the path is not found or not a directory; the resulting build output will not attempt to package Visual C redistributable components.");
                            System.err.println("         VSINSTALLDIR=\"" + vcdir + "\" checking for \"" + redistDir + "\"");
                        } else if(task instanceof InitializeBuildTask){
                        	((InitializeBuildTask)task).mySetProperty(-1, Constants.VSREDISTDIR, " (taken from VSINSTALLDIR)", fileRedistDir.getAbsolutePath(), true);
                        }
                    }
                }
            } catch(SecurityException eat) {
            }
            checkCompilerBits();    // This is checking in respect of MSVC compilers
            break;
		default:
			break;
        }
    }

    /*
     * check if trying to mix 32 bit vm with 64 bit compiler and other way around
     */
    void checkCompilerBits() {
        if(OSInfo.os() == OSInfo.OS.Windows) {
            boolean vmx64 = OSInfo.osArchName().equals(OSInfo.K_WIN_X64);
            boolean compiler64 = compiler == Compiler.MSVC2005_x64 || compiler == Compiler.MSVC2008_x64 || compiler == Compiler.MSVC2010_x64 || compiler == Compiler.MSVC2012_x64 || compiler == Compiler.MSVC2013_x64 || compiler == Compiler.MSVC2015_x64 || compiler == Compiler.MSVC2017_x64;
            if(vmx64 != compiler64) {
                // This is allowed and is not an outright build failure, but warn the user.
                if(vmx64)
                    System.err.println("WARNING: You are not building for 64-bit on a 64-bit operating system with MSVC compiler...");
                else
                    System.err.println("WARNING: You are not building for 32-bit on a 32-bit operating system with MSVC compiler...");
                if(compilerDetectionLine != null)
                    System.err.println("         decideCompiler(): " + compilerDetectionLine);
            }
        }
    }

    private void checkWindowsCompilers(boolean isTools) {
        Compiler msvc = testForVisualStudio();
        Compiler mingw_w64 = testForMinGW_W64();

        StringBuffer sb = new StringBuffer();
        if(mingw_w64 != null) {
            if(sb.length() > 0)
                sb.append(", ");
            sb.append(mingw_w64.toString());
        }
        if(msvc != null) {
            if(sb.length() > 0)
               sb.append(", ");
            sb.append(msvc.toString());
        }
        availableCompilers = sb.toString();

        if(msvc != null && mingw_w64 != null) {
            // allows use of QMAKESPEC to select between msvc and mingw-like
            System.out.println("Multiple compilers are available (" + sb.toString() + ")\n"
                               + "Choosing based on environment variable QMAKESPEC");
            String spec = System.getenv("QMAKESPEC");
            if(spec == null) {
	            if(isTools) {
	            	spec = AntUtil.getPropertyAsString(propertyHelper, Constants.TOOLS_QMAKESPEC);
	            }else {
	            	spec = AntUtil.getPropertyAsString(propertyHelper, Constants.QT_SPEC);
	            }
            }
            if(spec == null) {
            	compiler = msvc;
            } else if(msvc != null && spec.contains("msvc")) {
                compiler = msvc;
            } else if(mingw_w64 != null && spec.contains("g++")) {  // first due to regex for "mingw" on mingw detection
                compiler = mingw_w64;
            } else {
                throw new BuildException("Invalid QMAKESPEC variable...");
            }
        } else if(msvc != null) {
            compiler = msvc;
        } else if(mingw_w64 != null) {  // first due to regex for "mingw" on mingw detection
            compiler = mingw_w64;
        } else {
            throw new BuildException("No compiler detected, please make sure " +
                    "MinGW, MinGW-W64 or VisualC++ binaries are available in PATH");
        }
    }

    Compiler decideCompiler() {
        switch(OSInfo.crossOS()) {
        case Windows:
            checkWindowsCompilers(false);
            break;
        case IOS:
        case MacOS:
            compiler = Compiler.CLANG;
            break;
        case Linux:
            compiler = testForGCC();
            if(compiler==null)
            	compiler = testForCLANG();
            break;
        case Android:
        	compiler = testForCLANG();
            break;
		default:
			break;
        }
        
        String compilerPathValue = AntUtil.getPropertyAsString(propertyHelper, "tools.compiler.path");
    	if(compilerPathValue!=null && !compilerPathValue.isEmpty()) {
    		Compiler crossCompiler = compiler;
    		this.compilerPathValue = new File(compilerPathValue).getAbsolutePath();
    		try {
    			switch(OSInfo.os()) {
                case Windows:
                    checkWindowsCompilers(true);
                    break;
                case MacOS:
                    compiler = Compiler.GCC;
                    break;
                case Linux:
                    compiler = testForGCC();
                    if(compiler==null)
                    	compiler = testForCLANG();
                    break;
                case Android:
                	compiler = testForCLANG();
                    break;
        		default:
        			break;
                }
    		}catch(BuildException e) {
    			throw new BuildException("No cross compiler found in PATH="+this.compilerPathValue+": "+e.getMessage(), e);
    		}
    		if(task instanceof InitializeBuildTask) {
	    		if(compiler!=null) {
	    			((InitializeBuildTask)task).mySetProperty(-1, Constants.TOOLS_COMPILER, " (taken from tools.compiler.path)", compiler.toString(), true);
	    		}else if(crossCompiler!=null){
	    			((InitializeBuildTask)task).mySetProperty(-1, Constants.TOOLS_COMPILER, " (taken from qtjambi.compiler)", crossCompiler.toString(), true);
	    		}
    		}
    		compiler = crossCompiler;
        }else if(task instanceof InitializeBuildTask){
        	if(compiler!=null) {
        		((InitializeBuildTask)task).mySetProperty(-1, Constants.TOOLS_COMPILER, " (taken from qtjambi.compiler)", compiler.toString(), true);
        	}
        }

        if(availableCompilers == null && compiler != null)
            availableCompilers = compiler.toString();

        if(verbose) System.out.println("qtjambi.compiler: " + compiler);
        return compiler;
    }

    private Compiler testForGCC() {
        try {
            List<String> cmdAndArgs = new ArrayList<String>();
            String cmd = "gcc";
            if(OSInfo.os()==OSInfo.OS.Windows)
            	cmd += ".exe";
            if(compilerPathValue!=null)
            	cmd = compilerPathValue+File.separator+cmd;
            cmdAndArgs.add(cmd);
            cmdAndArgs.add("-dumpversion");
            String[] sA = Exec.executeCaptureOutput(task, cmdAndArgs, new File("."), propertyHelper.getProject(), compilerPathValue, null, false);
            Util.emitDebugLog(propertyHelper.getProject(), sA);
            if(sA != null && sA.length == 2 && sA[0] != null) {
                if(match(new String[] { sA[0] }, new String[] { "^3\\.3\\." }))  // sA[0] is stdout
                    return Compiler.OldGCC;
            }
            return Compiler.GCC;
        } catch(InterruptedException ex) {
            if(verbose)
                ex.printStackTrace();
            throw new BuildException("Failed to properly execute 'gcc' command");
        } catch(IOException ex) {
            if(verbose)
                ex.printStackTrace();
        }
        return null;
    }

    private Compiler testForCLANG() {
        try {
            List<String> cmdAndArgs = new ArrayList<String>();
            cmdAndArgs.add("clang");
            cmdAndArgs.add("-dumpversion");
            String[] sA = Exec.executeCaptureOutput(task, cmdAndArgs, new File("."), propertyHelper.getProject(), compilerPathValue, null, false);
            Util.emitDebugLog(propertyHelper.getProject(), sA);
            return Compiler.CLANG;
        } catch(InterruptedException ex) {
            if(verbose)
                ex.printStackTrace();
            throw new BuildException("Failed to properly execute 'clang' command");
        } catch(IOException ex) {
            if(verbose)
                ex.printStackTrace();
        }
        return null;
    }

    private boolean match(String[] sA, String[] regexA) {
        if(sA == null || regexA == null)
            return false;
        boolean bf = false;
        Pattern[] patternA = new Pattern[regexA.length];
        for(int i = 0; i < regexA.length; i++)
            patternA[i] = Pattern.compile(regexA[i]);
        for(String s : sA) {
            // regex does appear to still work without needing to ensure not multiline ah but habbit
            s = s.replace('\r', ' ');  
            s = s.replace('\n', ' ');
            StringBuilder sb = new StringBuilder();
            sb.append(">>" + s);
            for(Pattern p : patternA) {
                Matcher matcher = p.matcher(s);
                if(matcher.find()) {
                    sb.append(" matches:" + p.pattern());
                    bf = true;
                    break;
                }
            }
            propertyHelper.getProject().log(sb.toString(), Project.MSG_VERBOSE);
        }
        return bf;
    }

    /**
     * Takes output from gcc and if its platform target corresponds to mingw, returns
     * Compiler.Mingw.
     * @return Compiler.MinGW if successful, null otherwise.
     */
    private Compiler testForMinGW() {
        try {
            List<String> cmdAndArgs = new ArrayList<String>();
            String cmd = "gcc.exe";
            if(compilerPathValue!=null)
            	cmd = compilerPathValue+File.separator+cmd;
            cmdAndArgs.add(cmd);
            cmdAndArgs.add("-v");
            String[] sA = Exec.executeCaptureOutput(task, cmdAndArgs, new File("."), propertyHelper.getProject(), compilerPathValue, null, false);
            Util.emitDebugLog(propertyHelper.getProject(), sA);
            if(sA != null && sA.length == 2 && sA[1] != null) {
                if(match(new String[] { sA[1] }, new String[] { "mingw" }))  // sA[1] is stderr
                    return Compiler.MinGW;
            }
        } catch(InterruptedException ex) {
            if(verbose)
                ex.printStackTrace();
            throw new BuildException("Failed to properly execute from 'gcc' command");
        } catch(IOException ex) {
            if(verbose)
                ex.printStackTrace();
        }
        return null;
    }

    private Compiler testForMinGW_W64() {
        // FIXME: Prepend $CROSS_COMPILE
        String crossCompiler = System.getenv("CROSS_COMPILE");
        String cmd;
        if(crossCompiler != null)
            cmd = crossCompiler + "gcc.exe";
        else
            cmd = "gcc.exe";
        if(compilerPathValue!=null)
        	cmd = compilerPathValue+File.separator+cmd;
        try {
            List<String> cmdAndArgs = new ArrayList<String>();
            cmdAndArgs.add(cmd);
            cmdAndArgs.add("-v");
            String[] sA = Exec.executeCaptureOutput(task, cmdAndArgs, new File("."), propertyHelper.getProject(), compilerPathValue, null, false);
            Util.emitDebugLog(propertyHelper.getProject(), sA);
            if(sA != null && sA.length == 2 && sA[1] != null) {
                if(match(new String[] { sA[1] }, new String[] { "mingw-w64", "mingw64" }))   // sA[1] is stderr
                    return Compiler.MinGW_W64;
            }
        } catch(InterruptedException ex) {
            if(verbose)
                ex.printStackTrace();
            throw new BuildException("Failed to properly execute from '" + cmd + "' command");
        } catch(IOException ex) {
            if(verbose)
                ex.printStackTrace();
        }
        return null;
    }

    private Compiler testForVisualStudio() {
        try {
            List<String> cmdAndArgs = new ArrayList<String>();
            if(compilerPathValue!=null)
            	cmdAndArgs.add(compilerPathValue+File.separator+"cl.exe");
            else
            	cmdAndArgs.add("cl.exe");   // /version ?
            String[] sA = Exec.executeCaptureOutput(task, cmdAndArgs, new File("."), propertyHelper.getProject(), compilerPathValue, null, false);
            Util.emitDebugLog(propertyHelper.getProject(), sA);
            String stderr = null;
            if(sA != null && sA.length == 2 && sA[1] != null)
                stderr = sA[1];
            compilerDetectionLine = stderr;
            if(stderr != null) {
                if(stderr.contains("12.0"))
                    return Compiler.MSVC1998;
                if(stderr.contains("13.00"))
                    return Compiler.MSVC2002;
                if(stderr.contains("13.10"))
                    return Compiler.MSVC2003;
                if(stderr.contains("14.00")) {
                    if(stderr.contains("x64"))
                        return Compiler.MSVC2005_x64;
                    return Compiler.MSVC2005;
                }
                if(stderr.contains("15.00")) {
                    if(stderr.contains("x64"))
                        return Compiler.MSVC2008_x64;
                    return Compiler.MSVC2008;
                }
                if(stderr.contains("16.00")) {
                    if(stderr.contains("x64"))
                        return Compiler.MSVC2010_x64;
                    return Compiler.MSVC2010;
                }
                if(stderr.contains("17.00")) {
                    if(stderr.contains("x64"))
                        return Compiler.MSVC2012_x64;
                    return Compiler.MSVC2012;
                }
                if(stderr.contains("18.00")) {
                    if(stderr.contains("x64"))
                        return Compiler.MSVC2013_x64;
                    return Compiler.MSVC2013;
                }
                if(stderr.contains("19.00")) {
                    if(stderr.contains("x64"))
                        return Compiler.MSVC2015_x64;
                    return Compiler.MSVC2015;
                }
				if(stderr.contains("19.1")) {
                    if(stderr.contains("x64"))
                        return Compiler.MSVC2017_x64;
                    return Compiler.MSVC2017;
                }
				if(stderr.contains("19.2")) {
                    if(stderr.contains("x64"))
                        return Compiler.MSVC2019_x64;
                    if(stderr.contains("ARM64"))
                        return Compiler.MSVC2019_arm64;
                    return Compiler.MSVC2019_x86;
                }
				if(stderr.contains("19.3")) {
                    if(stderr.contains("x64"))
                        return Compiler.MSVC2022_x64;
                    if(stderr.contains("ARM64"))
                        return Compiler.MSVC2022_arm64;
                    return Compiler.MSVC2022_x86;
                }
                if(stderr.contains("x64"))
                    return Compiler.MSVC20XX_x64;
                if(stderr.contains("ARM64"))
                    return Compiler.MSVC20XX_arm64;
                return Compiler.MSVC20XX_x86;
            }
        } catch(InterruptedException ex) {
            if(verbose)
                ex.printStackTrace();
        } catch(IOException ex) {
            if(verbose)
                ex.printStackTrace();
        }
        return null;
    }

    public String getAvailableCompilers() {
        return availableCompilers;
    }
}
