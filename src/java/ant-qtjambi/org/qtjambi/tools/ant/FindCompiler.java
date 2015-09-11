package org.qtjambi.tools.ant;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;
import org.apache.tools.ant.Project;

import org.qtjambi.qt.osinfo.OSInfo;

public class FindCompiler {

    private Compiler compiler;
    private boolean verbose = false;
    private final InitializeBuildTask task;
    private String compilerDetectionLine;
    private String availableCompilers;

    public enum Compiler {
        MSVC1998("msvc98"),
        MSVC2002("msvc2002"),
        MSVC2003("msvc2003"),
        MSVC2005("msvc2005"),
        MSVC2005_64("msvc2005x64"),
        MSVC2008("msvc2008"),
        MSVC2008_64("msvc2008x64"),
        MSVC2010("msvc2010"),
        MSVC2010_64("msvc2010x64"),
        MSVC2012("msvc2012"),
        MSVC2012_64("msvc2012x64"),
        MinGW("mingw"),
        MinGW_W64("mingw-w64"),
        OldGCC("gcc3.3"),
        GCC("gcc"),
        SUNCC("suncc"),
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
            if(name.equals("msvc2005x64")) return MSVC2005_64;
            if(name.equals("msvc2008")) return MSVC2008;
            if(name.equals("msvc2008x64")) return MSVC2008_64;
            if(name.equals("msvc2010")) return MSVC2010;
            if(name.equals("msvc2010x64")) return MSVC2010_64;
            if(name.equals("msvc2012")) return MSVC2012;
            if(name.equals("msvc2012x64")) return MSVC2012_64;
            if(name.equals("mingw")) return MinGW;
            if(name.equals("mingw-w64")) return MinGW_W64;
            if(name.equals("gcc3.3")) return OldGCC;
            if(name.equals("gcc")) return GCC;
            if(name.equals("suncc")) return SUNCC;
            return Other;
        }

        public boolean is64Only() {
            return is64Only(name);
        }
        public static boolean is64Only(String name) {
            Compiler compiler = resolve(name);
            if(compiler == MSVC2005_64)
                return true;
            if(compiler == MSVC2008_64)
                return true;
            if(compiler == MSVC2010_64)
                return true;
            if(compiler == MSVC2012_64)
                return true;
            return false;
        }

        public boolean isCompiler(Compiler compiler) {
            return isCompiler(name, compiler);
        }
        public static boolean isCompiler(String name, Compiler ...compilerA) {
            Compiler thisCompiler = resolve(name);
            for(Compiler c : compilerA) {
                if(thisCompiler == c)
                    return true;
            }
            return false;
        }

        public boolean isMsvcKind() {
            Compiler compiler = resolve(name);
            switch(compiler) {
            case MSVC1998:
            case MSVC2002:
            case MSVC2003:
            case MSVC2005:
            case MSVC2005_64:
            case MSVC2008:
            case MSVC2008_64:
            case MSVC2010:
            case MSVC2010_64:
            case MSVC2012:
            case MSVC2012_64:
                return true;
            }
            return false;
        }
    }

    public FindCompiler(InitializeBuildTask task) {
        this.task = task;
    }
    
    void checkCompilerDetails() {
        switch(compiler) {
        case MSVC2005:
        case MSVC2005_64:
        case MSVC2008:
        case MSVC2008_64:
        case MSVC2010:
        case MSVC2010_64:
        case MSVC2012:
        case MSVC2012_64:
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
                    	task.mySetProperty(-1, Constants.VSINSTALLDIR, " (taken from environment)", vcdir, false);

                        String redistDir;
                        if(compiler == Compiler.MSVC2005_64 || compiler == Compiler.MSVC2008_64 || compiler == Compiler.MSVC2010_64)
                            redistDir = Util.pathCanon(new String[] { vcdir, "vc", "redist", "amd64" });
                        else if(compiler == Compiler.MSVC2012_64)
                            redistDir = Util.pathCanon(new String[] { vcdir, "vc", "redist", "x64" });
                        else
                            redistDir = Util.pathCanon(new String[] { vcdir, "vc", "redist", "x86" });
                        File fileRedistDir = new File(redistDir);
                        if(!fileRedistDir.isDirectory()) {
                            System.err.println("WARNING: VSINSTALLDIR is set but the path is not found or not a directory; the resulting build output will not attempt to package Visual C redistributable components.");
                            System.err.println("         VSINSTALLDIR=\"" + vcdir + "\" checking for \"" + redistDir + "\"");
                        } else {
                        	task.mySetProperty(-1, Constants.VSREDISTDIR, " (taken from VSINSTALLDIR)", fileRedistDir.getAbsolutePath(), false);
                        }
                    }
                }
            } catch(SecurityException eat) {
            }
            checkCompilerBits();    // This is checking in respect of MSVC compilers
            break;
        }
    }

    /*
     * check if trying to mix 32 bit vm with 64 bit compiler and other way around
     */
    void checkCompilerBits() {
        if(OSInfo.os() == OSInfo.OS.Windows) {
            boolean vmx64 = OSInfo.osArchName().contains("64");
            boolean compiler64 = compiler == Compiler.MSVC2005_64 || compiler == Compiler.MSVC2008_64 || compiler == Compiler.MSVC2010_64 || compiler == Compiler.MSVC2012_64;
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

    private void checkWindowsCompilers() {
        Compiler msvc = testForVisualStudio();
        Compiler mingw = testForMinGW();
        Compiler mingw_w64 = testForMinGW_W64();

        StringBuffer sb = new StringBuffer();
        if(mingw_w64 != null) {
            if(sb.length() > 0)
                sb.append(", ");
            sb.append(mingw_w64.toString());
        }
        if(mingw != null) {
            if(sb.length() > 0)
                sb.append(", ");
            sb.append(mingw.toString());
        }
        if(msvc != null) {
            if(sb.length() > 0)
               sb.append(", ");
            sb.append(msvc.toString());
        }
        availableCompilers = sb.toString();

        if(msvc != null && mingw != null && mingw_w64 != null) {
            // allows use of QMAKESPEC to select between msvc and mingw-like
            System.out.println("Multiple compilers are available (" + sb.toString() + ")\n"
                               + "Choosing based on environment variable QMAKESPEC");
            String spec = System.getenv("QMAKESPEC");
            if(spec == null) {
                throw new BuildException("Environment variable QMAKESPEC is not set...");
            } else if(msvc != null && spec.contains("msvc")) {
                compiler = msvc;
            } else if(mingw_w64 != null && spec.contains("g++")) {  // first due to regex for "mingw" on mingw detection
                compiler = mingw_w64;
            } else if(mingw != null && spec.contains("g++")) {
                compiler = mingw;
            } else {
                throw new BuildException("Invalid QMAKESPEC variable...");
            }
        } else if(msvc != null) {
            compiler = msvc;
        } else if(mingw_w64 != null) {  // first due to regex for "mingw" on mingw detection
            compiler = mingw_w64;
        } else if(mingw != null) {
            compiler = mingw;
        } else {
            throw new BuildException("No compiler detected, please make sure " +
                    "MinGW, MinGW-W64 or VisualC++ binaries are available in PATH");
        }
    }

    private void checkSolarisCompiler() {
        String spec = System.getenv("QMAKESPEC");
        if(spec == null) {
            System.out.println("QMAKESPEC environment variable not specified using SunCC compiler");
            compiler = Compiler.SUNCC;
        } else if(spec.contains("cc")) {
            compiler = Compiler.SUNCC;
        } else if(spec.contains("g++")) {
            compiler = Compiler.GCC;
        } else {
            throw new BuildException("Invalid QMAKESPEC variable...");
        }
    }

    Compiler decideCompiler() {
        switch(OSInfo.os()) {
        case Windows:
            checkWindowsCompilers();
            break;
        case MacOS:
            compiler = Compiler.GCC;
            break;
        case Linux:
        case FreeBSD:
            compiler = testForGCC();
            break;
        case Solaris:
            checkSolarisCompiler();
            break;
        }

        if(availableCompilers == null && compiler != null)
            availableCompilers = compiler.toString();

        if(verbose) System.out.println("qtjambi.compiler: " + compiler.toString());
        return compiler;
    }

    private Compiler testForGCC() {
        try {
            List<String> cmdAndArgs = new ArrayList<String>();
            cmdAndArgs.add("gcc");
            cmdAndArgs.add("-dumpversion");
            String[] sA = Exec.executeCaptureOutput(cmdAndArgs, new File("."), task.getProject(), null, null, false);
            Util.emitDebugLog(task.getProject(), sA);
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
            task.getProject().log(sb.toString(), Project.MSG_VERBOSE);
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
            cmdAndArgs.add("gcc");
            cmdAndArgs.add("-v");
            String[] sA = Exec.executeCaptureOutput(cmdAndArgs, new File("."), task.getProject(), null, null, false);
            Util.emitDebugLog(task.getProject(), sA);
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
            cmd = crossCompiler + "gcc";
        else
            cmd = "gcc";
        try {
            List<String> cmdAndArgs = new ArrayList<String>();
            cmdAndArgs.add(cmd);
            cmdAndArgs.add("-v");
            String[] sA = Exec.executeCaptureOutput(cmdAndArgs, new File("."), task.getProject(), null, null, false);
            Util.emitDebugLog(task.getProject(), sA);
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
            cmdAndArgs.add("cl.exe");   // /version ?
            String[] sA = Exec.executeCaptureOutput(cmdAndArgs, new File("."), task.getProject(), null, null, false);
            Util.emitDebugLog(task.getProject(), sA);
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
                        return Compiler.MSVC2005_64;
                    return Compiler.MSVC2005;
                }
                if(stderr.contains("15.00")) {
                    if(stderr.contains("x64"))
                        return Compiler.MSVC2008_64;
                    return Compiler.MSVC2008;
                }
                if(stderr.contains("16.00")) {
                    if(stderr.contains("x64"))
                        return Compiler.MSVC2010_64;
                    return Compiler.MSVC2010;
                }
                if(stderr.contains("17.00")) {
                    if(stderr.contains("x64"))
                        return Compiler.MSVC2012_64;
                    return Compiler.MSVC2012;
                }
                throw new BuildException("Failed to detect Visual Studio version\n  \"" + stderr + "\"");
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
