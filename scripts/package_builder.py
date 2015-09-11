#!/usr/bin/python

# TODO:
#  - Add qtjambi.jnlp generation to signWebstarJars
#  - Whoever thought libqtdesigner.so was a good name for a plugin to be loaded by designer
#    is a fucking retard...

import datetime
import os
import re
import shutil
import socket
import string
import sys
import time
import traceback
import types

import pkgutil


# initialize the socket callback interface so we have it
# available..
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.bind((socket.gethostname(), pkgutil.PORT_CREATOR))
serversocket.listen(16)

host_linux64 = "tirionvm-linux64.troll.no"
host_linux32 = "tirionvm-linux32.troll.no"
host_win64   = "tirionvm-win64.troll.no"
host_win32   = "packy-win32-jambiclone.troll.no"
host_mac     = "alqualonde.troll.no"

class Options:
    def __init__(self):
        if pkgutil.isWindows():
            self.packageRoot = "c:/tmp/qtjambi-builder"
        else:
            self.packageRoot = "/tmp/qtjambi-builder"
        self.eclipseVersion = "1.2.2"
        self.p4User = "qt"
        self.p4Client = "qt-builder"
        self.binaryPackageCount = 0
        self.packageExtraName = ""

        self.buildPreview = False;

        self.buildMac = True
        self.buildWindows = True
        self.buildLinux = True
        self.build32 = True
        self.build64 = True
        self.buildEval = True
        self.buildGpl = False
        self.buildLgpl = True
        self.buildCommercial = True
        self.buildBinary = True
        self.buildSource = True
        self.resetKeystore = True
        self.buildWebstart = True

        self.copiedWebstartBinaries = False;


options = Options()


class Package:
    def __init__(self, platform, arch, license):
        self.done = False
        self.success = False
        self.license = license
        self.platform = platform
        self.arch = arch
        self.binary = False
        self.removeDirs = [
            "ant",
            "autotestlib",
            "org/qtjambi/autotests",
            "org/qtjambi/bcc",
            "org/qtjambi/benchmarks",
            "org/qtjambi/extensions/awt",
            "org/qtjambi/extensions/jfc",
            "org/qtjambi/extensions/jython",
            "org/qtjambi/manualtests",
            "org/qtjambi/tests",
            "cpp",
            "dist",
            "doc/config",
            "doc/src",
            "eclipse",
            "ide",
            "launcher_launcher",
            "libbenchmark",
            "scripts",
            "tools",
            "whitepaper"
            ]
        self.removeFiles = [
            "build_autotests.xml"
            ]
        self.removePatterns = [
            re.compile("CRT"),
            re.compile("Makefile$"),
            re.compile("Makefile.Debug$"),
            re.compile("Makefile.Release$"),
            re.compile("\\.a$"),
            re.compile("\\.class$"),
            re.compile("\\.debug$"),
            re.compile("\\.exp$"),
            re.compile("\\.ilk$"),
            re.compile("\\.lib$"),
            re.compile("\\.log$"),
            re.compile("\\.manifest$"),
            re.compile("\\.o$"),
            re.compile("\\.obj$"),
            re.compile("\\.pch$"),
            re.compile("\\.pdb$"),
            re.compile("\\[\\/]debug$"),
            re.compile("\\[\\/]release$"),
            re.compile("\\_debuglib\\."),
            re.compile("com_trolltech.*\\.lib$"),
            re.compile("task(.bat)?$"),
            ]

        self.mkdirs = []

        self.copyFiles = [
            # text files for main directory...
            "dist/readme.html",
            "dist/install.html",
            "dist/changes-" + options.qtJambiVersion
            ]
        self.licenseHeader = pkgutil.readLicenseHeader(self.license, options.startDir)

        if self.license == pkgutil.LICENSE_COMMERCIAL:
            self.setCommercial()
        elif self.license == pkgutil.LICENSE_EVAL:
            self.setEval()
        elif self.license == pkgutil.LICENSE_GPL:
            self.setGpl()
        elif self.license == pkgutil.LICENSE_PREVIEW:
            self.setPreview()
        elif self.license == pkgutil.LICENSE_LGPL:
            self.setLgpl()
        else:
            raise "bad license type:" + self.license

    def setBinary(self):
        self.binary = True
        self.removeFiles.append("build_generator_example.xml")
        self.removePatterns.extend([
            re.compile("ant-qtjambi.jar"),
            re.compile("build.xml$"),
            re.compile("task.bat$"),
            re.compile("task.sh$"),
            re.compile("java.pro")
            ])
        self.removeDirs.extend([
            "common",
            "org/qtjambi/qt",
            "designer-integration",
            "juic",
            "generator",
            "generator_example",
            "qtjambi",
            "qtjambi_core",
            "qtjambi_designer",
            "qtjambi_generator",
            "qtjambi_gui",
            "qtjambi_network",
            "qtjambi_opengl",
            "qtjambi_phonon",
            "qtjambi_sql",
            "qtjambi_svg",
            "qtjambi_webkit",
            "qtjambi_xml",
            "qtjambi_xmlpatterns",
            "org/qtjambi/tools/generator",
            "org/qtjambi/tools/designer"
            ])


    def setMacBinary(self):
        self.setBinary()
        self.copyFiles.append("dist/mac/qtjambi.sh")
        self.copyFiles.append("dist/mac/designer.sh")
        self.compiler = "gcc"
        self.make = "make"
        self.platformJarName = "qtjambi-macosx-gcc-" + options.qtJambiVersion + ".jar"
        self.removeFiles.append("set_qtjambi_env.bat")
        self.removePatterns.append(re.compile(".*\\.1\\.(\\d\\.)*(jnilib|dylib)$"))
        if self.license == pkgutil.LICENSE_EVAL:
            self.copyFiles.append("dist/mac/binpatch")
            self.removeDirs.append("Demos.app")

    def setWinBinary(self):
        self.setBinary()
        self.copyFiles.append("dist/win/designer.bat")
        if self.arch == pkgutil.ARCH_64:
            self.copyFiles.append(["dist/win/qtjambi64.exe", "qtjambi.exe"])
            self.platformJarName = "qtjambi-win64-msvc2005x64-" + options.qtJambiVersion + ".jar"
        else:
            self.copyFiles.append("dist/win/qtjambi.exe")
            self.copyFiles.append(["dist/win/msvcr71.dll", "bin"])
            self.platformJarName = "qtjambi-win32-msvc2005-" + options.qtJambiVersion + ".jar"

        if self.arch == pkgutil.ARCH_64:
            self.compiler = "msvc2005_x64"
        else:
            self.compiler = "msvc2005"
        self.make = "nmake"
        self.removeFiles.append("set_qtjambi_env.sh")
        self.removeDirs.append("Demos.app")

    def setLinuxBinary(self):
        self.setBinary()
        self.copyFiles.append("dist/linux/designer.sh")
        self.copyFiles.append("dist/linux/qtjambi.sh")
        self.compiler = "gcc"
        self.make = "make"
        if self.arch == pkgutil.ARCH_64:
            self.platformJarName = "qtjambi-linux64-gcc-" + options.qtJambiVersion + ".jar"
        else:
            self.platformJarName = "qtjambi-linux32-gcc-" + options.qtJambiVersion + ".jar"
        self.removeFiles.append("set_qtjambi_env.bat")
        self.removeDirs.append("Demos.app")
        self.removePatterns.append(re.compile(".*\\.so\\.1(\\.\\d)*$"));
        # self.removePatterns.append(re.compile("libqtdesigner\\.so\\.4(\\.\\d)*$"));
        if self.license == pkgutil.LICENSE_EVAL:
            self.copyFiles.append("dist/linux/binpatch")

    def setSource(self):
        self.binary = False
        self.mkdirs.extend(["qtjambi_designer/private",
                            "designer-integration/language/private",
                            "include"
                            ]
                           )

        uicPrefix = options.qtDir + "/src/tools/uic/"
        designerPrefix = options.qtDir + "/tools/designer/src/lib/"
        self.copyFiles.extend([
            # Designer Files
            [designerPrefix + "sdk/abstractintrospection_p.h",
             "designer-integration/language/private/abstractintrospection_p.h"],
            [designerPrefix + "uilib/ui4_p.h",
             "designer-integration/language/private/ui4_p.h"],
            [designerPrefix + "shared/qdesigner_utils_p.h",
             "qtjambi_designer/private/qdesigner_utils_p.h"],
            [designerPrefix + "shared/shared_global_p.h",
             "qtjambi_designer/private/shared_global_p.h"],

            # uic files
            [uicPrefix + "customwidgetsinfo.cpp", "juic"],
            [uicPrefix + "customwidgetsinfo.h", "juic"],
            [uicPrefix + "databaseinfo.cpp", "juic"],
            [uicPrefix + "databaseinfo.h", "juic"],
            [uicPrefix + "driver.cpp", "juic"],
            [uicPrefix + "driver.h", "juic"],
            [uicPrefix + "globaldefs.h", "juic"],
            [uicPrefix + "option.h", "juic"],
            [uicPrefix + "treewalker.cpp", "juic"],
            [uicPrefix + "treewalker.h", "juic"],
            [uicPrefix + "ui4.cpp", "juic"],
            [uicPrefix + "ui4.h", "juic"],
            [uicPrefix + "uic.cpp", "juic"],
            [uicPrefix + "uic.h", "juic"],
            [uicPrefix + "uic.pri", "juic"],
            [uicPrefix + "utils.h", "juic"],
            [uicPrefix + "validator.cpp", "juic"],
            [uicPrefix + "validator.h", "juic"],

            # other files...
            ["qtjambi/qtjambi_core.h", "include"],
            ["qtjambi/qtjambi_cache.h", "include"],
            ["qtjambi/qtjambi_global.h", "include"],
            ["qtjambi/qtjambilink.h", "include"]
            ])
        
        self.removeDirs.append("Qt Jambi Demos.app");
        if self.platform == pkgutil.PLATFORM_WINDOWS:
            self.removeFiles.append("set_qtjambi_env.sh")
        else:
            self.removeFiles.append("set_qtjambi_env.bat")


    def setCommercial(self):
        self.copyFiles.append("dist/LICENSE")

    def setLgpl(self):
        self.copyFiles.append("dist/LICENSE.LGPL")
        self.copyFiles.append("dist/LGPL_EXCEPTION.TXT")
        self.copyFiles.append("dist/LICENSE.GPL3");

    def setGpl(self):
        self.copyFiles.append("dist/LICENSE.GPL")
        self.copyFiles.append("dist/GPL_EXCEPTION.TXT");
        self.copyFiles.append("dist/GPL_EXCEPTION_ADDENDUM.TXT");

    def setEval(self):
        self.copyFiles.append("dist/LICENSE.EVAL")
        self.copyFiles.append("dist/readme_eval.html");
        if self.arch == pkgutil.PLATFORM_MAC:
            self.removeDirs.append("Demos.app")
        if self.arch == pkgutil.PLATFORM_WINDOWS:
            self.removeFiles.append("set_qtambi_env.bat");
        else:
            self.removeFiles.append("set_qtjambi_env.sh");

    def setPreview(self):
        self.copyFiles.append("dist/LICENSE.PREVIEW")

    def name(self):
        if self.binary:
            arch = self.arch
            if self.arch == pkgutil.ARCH_UNIVERSAL:
                arch = ""
            return "qtjambi-" + self.platform + arch + "-" + self.license + "-" + options.qtJambiVersion;
        else:
            return "qtjambi-src-%s-%s" % (self.license, options.qtJambiVersion)

    def writeLog(self, list, subname):
        logName = os.path.join(options.packageRoot, ".%s.%s" % (self.name(), subname))
        pkgutil.debug("   - log into: " + logName)
        log = open(logName, "w")
        log.write("\n".join(list))
        log.close()

    def hasEclipse(self):
        if self.binary and not self.license == pkgutil.LICENSE_EVAL:
            if self.platform == pkgutil.PLATFORM_LINUX:
                return True
            if self.platform == pkgutil.PLATFORM_WINDOWS and self.arch == pkgutil.ARCH_32:
                return True
        return False

packages = []



# Sets up all the various packages to be built into the global
# variable "packages"
def setupPackages():

    if options.buildBinary:
        if options.buildWindows:
            if options.build64:
                if options.buildCommercial:
                    winMoney64 = Package(pkgutil.PLATFORM_WINDOWS, pkgutil.ARCH_64, pkgutil.LICENSE_COMMERCIAL)
                    winMoney64.setWinBinary()
                    winMoney64.buildServer = host_win64;
                    packages.append(winMoney64)
                if options.buildGpl:
                    winFree64 = Package(pkgutil.PLATFORM_WINDOWS, pkgutil.ARCH_64, pkgutil.LICENSE_GPL)
                    winFree64.setWinBinary()
                    winFree64.buildServer = host_win64
                    packages.append(winFree64)
                if options.buildLgpl:
                    winSuperFree64 = Package(pkgutil.PLATFORM_WINDOWS, pkgutil.ARCH_64, pkgutil.LICENSE_LGPL)
                    winSuperFree64.setWinBinary()
                    winSuperFree64.buildServer = host_win64
                    packages.append(winSuperFree64)
                if options.buildEval:
                    winEval64 = Package(pkgutil.PLATFORM_WINDOWS, pkgutil.ARCH_64, pkgutil.LICENSE_EVAL)
                    winEval64.setWinBinary()
                    winEval64.buildServer = host_win64
                    packages.append(winEval64)
                if options.buildPreview:
                    winPreview64 = Package(pkgutil.PLATFORM_WINDOWS, pkgutil.ARCH_64, pkgutil.LICENSE_PREVIEW)
                    winPreview64.setWinBinary()
                    winPreview64.buildServer = host_win64
                    packages.append(winPreview64)
            if options.build32:
                if options.buildCommercial:
                    winMoney32 = Package(pkgutil.PLATFORM_WINDOWS, pkgutil.ARCH_32, pkgutil.LICENSE_COMMERCIAL)
                    winMoney32.setWinBinary()
                    winMoney32.buildServer = host_win32
                    packages.append(winMoney32)
                if options.buildGpl:
                    winFree32 = Package(pkgutil.PLATFORM_WINDOWS, pkgutil.ARCH_32, pkgutil.LICENSE_GPL)
                    winFree32.setWinBinary()
                    winFree32.buildServer = host_win32
                    packages.append(winFree32)
                if options.buildLgpl:
                    winSuperFree32 = Package(pkgutil.PLATFORM_WINDOWS, pkgutil.ARCH_32, pkgutil.LICENSE_LGPL)
                    winSuperFree32.setWinBinary()
                    winSuperFree32.buildServer = host_win32
                    packages.append(winSuperFree32)
                if options.buildEval:
                    winEval32 = Package(pkgutil.PLATFORM_WINDOWS, pkgutil.ARCH_32, pkgutil.LICENSE_EVAL)
                    winEval32.setWinBinary()
                    winEval32.buildServer = host_win32
                    packages.append(winEval32)
                if options.buildPreview:
                    winPreview32 = Package(pkgutil.PLATFORM_WINDOWS, pkgutil.ARCH_32, pkgutil.LICENSE_PREVIEW)
                    winPreview32.setWinBinary()
                    winPreview32.buildServer = host_win32
                    packages.append(winPreview32)

        if options.buildMac:
            if options.buildCommercial:
                macMoney = Package(pkgutil.PLATFORM_MAC, pkgutil.ARCH_UNIVERSAL, pkgutil.LICENSE_COMMERCIAL)
                macMoney.setMacBinary()
                macMoney.buildServer = host_mac
                packages.append(macMoney)
            if options.buildGpl:
                macFree = Package(pkgutil.PLATFORM_MAC, pkgutil.ARCH_UNIVERSAL, pkgutil.LICENSE_GPL)
                macFree.setMacBinary()
                macFree.buildServer = host_mac
                packages.append(macFree)
            if options.buildLgpl:
                macSuperFree = Package(pkgutil.PLATFORM_MAC, pkgutil.ARCH_UNIVERSAL, pkgutil.LICENSE_LGPL)
                macSuperFree.setMacBinary()
                macSuperFree.buildServer = host_mac
                packages.append(macSuperFree)
            if options.buildEval:
                macEval = Package(pkgutil.PLATFORM_MAC, pkgutil.ARCH_UNIVERSAL, pkgutil.LICENSE_EVAL)
                macEval.setMacBinary()
                macEval.buildServer = host_mac
                packages.append(macEval)
            if options.buildPreview:
                macPreview = Package(pkgutil.PLATFORM_MAC, pkgutil.ARCH_UNIVERSAL, pkgutil.LICENSE_PREVIEW)
                macPreview.setMacBinary()
                macPreview.buildServer = host_mac
                packages.append(macPreview)

        if options.buildLinux:
            if options.build64:
                if options.buildCommercial:
                    linuxMoney64 = Package(pkgutil.PLATFORM_LINUX, pkgutil.ARCH_64, pkgutil.LICENSE_COMMERCIAL)
                    linuxMoney64.setLinuxBinary()
                    linuxMoney64.buildServer = host_linux64
                    packages.append(linuxMoney64)
                if options.buildGpl:
                    linuxFree64 = Package(pkgutil.PLATFORM_LINUX, pkgutil.ARCH_64, pkgutil.LICENSE_GPL)
                    linuxFree64.setLinuxBinary()
                    linuxFree64.buildServer = host_linux64
                    packages.append(linuxFree64)
                if options.buildLgpl:
                    linuxSuperFree64 = Package(pkgutil.PLATFORM_LINUX, pkgutil.ARCH_64, pkgutil.LICENSE_LGPL)
                    linuxSuperFree64.setLinuxBinary()
                    linuxSuperFree64.buildServer = host_linux64
                    packages.append(linuxSuperFree64)
                if options.buildEval:
                    linuxEval64 = Package(pkgutil.PLATFORM_LINUX, pkgutil.ARCH_64, pkgutil.LICENSE_EVAL)
                    linuxEval64.setLinuxBinary()
                    linuxEval64.buildServer = host_linux64
                    packages.append(linuxEval64)
                if options.buildPreview:
                    linuxPreview64 = Package(pkgutil.PLATFORM_LINUX, pkgutil.ARCH_64, pkgutil.LICENSE_PREVIEW)
                    linuxPreview64.setLinuxBinary()
                    linuxPreview64.buildServer = host_linux64
                    packages.append(linuxPreview64)
            if options.build32:
                if options.buildCommercial:
                    linuxMoney32 = Package(pkgutil.PLATFORM_LINUX, pkgutil.ARCH_32, pkgutil.LICENSE_COMMERCIAL)
                    linuxMoney32.setLinuxBinary()
                    linuxMoney32.buildServer = host_linux32
                    packages.append(linuxMoney32)
                if options.buildGpl:
                    linuxFree32 = Package(pkgutil.PLATFORM_LINUX, pkgutil.ARCH_32, pkgutil.LICENSE_GPL)
                    linuxFree32.setLinuxBinary()
                    linuxFree32.buildServer = host_linux32
                    packages.append(linuxFree32)
                if options.buildLgpl:
                    linuxSuperFree32 = Package(pkgutil.PLATFORM_LINUX, pkgutil.ARCH_32, pkgutil.LICENSE_LGPL)
                    linuxSuperFree32.setLinuxBinary()
                    linuxSuperFree32.buildServer = host_linux32
                    packages.append(linuxSuperFree32)                    
                if options.buildEval:
                    linuxEval32 = Package(pkgutil.PLATFORM_LINUX, pkgutil.ARCH_32, pkgutil.LICENSE_EVAL)
                    linuxEval32.setLinuxBinary()
                    linuxEval32.buildServer = host_linux32
                    packages.append(linuxEval32)
                if options.buildPreview:
                    linuxPreview32 = Package(pkgutil.PLATFORM_LINUX, pkgutil.ARCH_32, pkgutil.LICENSE_PREVIEW)
                    linuxPreview32.setLinuxBinary()
                    linuxPreview32.buildServer = host_linux32
                    packages.append(linuxPreview32)


    # randomize the packages a bit to spread the load better...
    i = 0
    while i < len(packages):
        fr = i
        to = (i * 3 + 1) % len(packages)
        tmp = packages[to]
        packages[to] = packages[fr]
        packages[fr] = tmp
        i = i + 1

    if options.buildSource:
        if options.buildLgpl:
            if options.buildWindows:
                winSrcLgpl = Package(pkgutil.PLATFORM_WINDOWS, None, pkgutil.LICENSE_LGPL)
                winSrcLgpl.setSource()
                packages.append(winSrcLgpl)
            if options.buildLinux:
                linuxSrcLgpl = Package(pkgutil.PLATFORM_LINUX, None, pkgutil.LICENSE_LGPL)
                linuxSrcLgpl.setSource()
                packages.append(linuxSrcLgpl)            
        if options.buildGpl:
            if options.buildWindows:
                winSrcGpl = Package(pkgutil.PLATFORM_WINDOWS, None, pkgutil.LICENSE_GPL)
                winSrcGpl.setSource()
                packages.append(winSrcGpl)
            if options.buildLinux:
                linuxSrcGpl = Package(pkgutil.PLATFORM_LINUX, None, pkgutil.LICENSE_GPL)
                linuxSrcGpl.setSource()
                packages.append(linuxSrcGpl)
        if options.buildCommercial:
            if options.buildWindows:
                winSrcMoney = Package(pkgutil.PLATFORM_WINDOWS, None, pkgutil.LICENSE_COMMERCIAL)
                winSrcMoney.setSource()
                packages.append(winSrcMoney)
            if options.buildLinux:
                linuxSrcMoney = Package(pkgutil.PLATFORM_LINUX, None, pkgutil.LICENSE_COMMERCIAL)
                linuxSrcMoney.setSource()
                packages.append(linuxSrcMoney)
        if options.buildPreview:
            if options.buildWindows:
                winSrcPreview = Package(pkgutil.PLATFORM_WINDOWS, None, pkgutil.LICENSE_PREVIEW)
                winSrcPreview.setSource()
                packages.append(winSrcPreview)
            if options.buildLinux:
                linuxSrcPreview = Package(pkgutil.PLATFORM_LINUX, None, pkgutil.LICENSE_PREVIEW)
                linuxSrcPreview.setSource()
                packages.append(linuxSrcPreview)

    # set some extra properties that depend on the config above...
    for package in packages:
        package.packageDir = options.packageRoot + "/" + package.name()



# Sets up the client spec and performs a complete checkout of the
# tree...
def prepareSourceTree():

    # remove and recreat dir and cd into it...
    if os.path.isdir(options.packageRoot):
        shutil.rmtree(options.packageRoot)
    os.makedirs(options.packageRoot)
    os.chdir(options.packageRoot)

    # set up the perforce client...
    tmpFile = open("p4spec.tmp", "w")
    tmpFile.write("Root: %s\n" % (options.packageRoot))
    tmpFile.write("Owner: %s\n" % options.p4User)
    tmpFile.write("Client: %s\n" % options.p4Client)
    tmpFile.write("View:\n")
    tmpFile.write("        //depot/qtjambi/%s/...  //qt-builder/qtjambi/...\n" % options.qtJambiVersion)
    tmpFile.write("        //depot/qt/%s/src/tools/uic/...  //qt-builder/qt/src/tools/uic/...\n" % options.qtVersion)
    tmpFile.write("        //depot/qt/%s/tools/designer/src/lib/...  //qt-builder/qt/tools/designer/src/lib/...\n" % options.qtVersion)
    tmpFile.write("        //depot/eclipse/qtjambi-4.5/...  //qt-builder/qtjambi/eclipse/qtjambi-4.5/...\n")
    tmpFile.write("        //depot/ide/main/shared/designerintegrationv2/...  //qt-builder/qtjambi/ide/main/shared/designerintegrationv2/...\n")
    tmpFile.write("        //depot/ide/main/shared/namespace_global.h  //qt-builder/qtjambi/ide/main/shared/namespace_global.h\n")
    tmpFile.close()
    os.system("p4 -u %s -c %s client -i < p4spec.tmp" % (options.p4User, options.p4Client) )
    os.remove("p4spec.tmp")

    # sync p4 client spec into subdirectory...
    pkgutil.debug(" - syncing p4...")
    os.system("p4 -u %s -c %s sync -f //%s/... > .p4sync.buildlog" % (options.p4User, options.p4Client, options.p4Client))
    os.system("chmod -R a+wX .")


def packageSourcePackage(package):
    pkgutil.debug("packaging source package: %s..." % package.name())

    os.chdir(options.packageRoot)

    if os.path.isdir(package.name()):
        shutil.rmtree(package.name())

    shutil.copytree("qtjambi", package.name())

    postProcessPackage(package)


# Creates the build script (.bat or .sh), zips up the file and sends it off to the
# build server
def packageAndSend(package):
    pkgutil.debug("packaging and sending: %s..." % package.name())

    os.chdir(options.packageRoot)

    if os.path.isdir("tmptree"):
        shutil.rmtree("tmptree")

    shutil.copytree("qtjambi", "tmptree");

    qtEdition = "qt-" + package.license;
    if package.license == pkgutil.LICENSE_PREVIEW:
        qtEdition = "qt-commercial"

    pkgutil.debug(" - creating task script")
    arch = "x86";
    if arch == pkgutil.ARCH_64:
        arch = "x86_64"
    if package.platform == pkgutil.PLATFORM_WINDOWS:
        buildFile = open("tmptree/task.bat", "w")
        buildFile.write("call qt_pkg_setup %s %s\n" % (package.compiler, "c:\\tmp\\qtjambi-package-builder\\" + qtEdition))

        # build eclipse on 32-bit windows...
        if package.hasEclipse():
#            if package.license == pkgutil.LICENSE_EVAL:
#                buildFile.write("call qt_pkg_setup %s %s\n" % (package.compiler, "c:\\tmp\\qtjambi-package-builder\\qt-commercial"))
            buildFile.write("cd scripts\n")
            buildFile.write("call build_eclipse.bat %%cd%%\\..\\eclipse\\qtjambi-4.5 %s %s\n" % (options.eclipseVersion, arch))
            buildFile.write("cd ..\n")
#            if package.license == pkgutil.LICENSE_EVAL:
#                buildFile.write("call qt_pkg_setup %s %s\n" % (package.compiler, "c:\\tmp\\qtjambi-package-builder\\" + qtEdition))
            
        buildFile.write("call ant\n")
        buildFile.write('if "%ERRORLEVEL%" == "0" ' + package.make + ' clean\n')
        buildFile.write("copy %QTDIR%\\bin\\designer.exe bin\n")
        buildFile.write("copy %QTDIR%\\bin\\lrelease.exe bin\n")
        buildFile.write("copy %QTDIR%\\bin\\lupdate.exe bin\n")
        buildFile.write("copy %QTDIR%\\bin\\linguist.exe bin\n")
        buildFile.write("copy %QTDIR%\\bin\\QtDesigner4.dll bin\n")
        buildFile.write("copy %QTDIR%\\bin\\QtDesignerComponents4.dll bin\n")
        buildFile.write("copy %QTDIR%\\bin\\QtScript4.dll bin\n")

    else:
        buildFile = open("tmptree/task.sh", "w")

        qtLocation = "/tmp/qtjambi-package-builder/" + qtEdition
        
        buildFile.write(". qt_pkg_setup %s %s\n" % (package.compiler, qtLocation))
        buildFile.write("ant\n")
        buildFile.write(package.make + " clean \n")
        
        if package.hasEclipse():
            # a little trick needed to bypass that eclipse tools use
            # gui which will pop up eval dialogs on the build
            # servers...
#            if package.license == pkgutil.LICENSE_EVAL: 
#                buildFile.write(". qt_pkg_setup %s %s\n" % (package.compiler, "/tmp/qtjambi-package-builder/qt-commercial"))            
            buildFile.write("cd scripts\n")
            buildFile.write("bash ./build_eclipse.sh $PWD/../eclipse/qtjambi-4.5 %s %s\n" % (options.eclipseVersion, arch))
            buildFile.write("cd ..\n")
            buildFile.write("cp -v lib/libqtdesigner.so lib/libqtdesignerplugin.so\n")
            buildFile.write("rm -v lib/libqtdesigner.*\n")
#            if package.license == pkgutil.LICENSE_EVAL: 
#                buildFile.write(". qt_pkg_setup %s %s\n" % (package.compiler, qtLocation))

        if package.platform == pkgutil.PLATFORM_LINUX:
            buildFile.write("cp -v $QTDIR/bin/designer bin\n")
            buildFile.write("cp -v $QTDIR/bin/lrelease bin\n")
            buildFile.write("cp -v $QTDIR/bin/lupdate bin\n")
            buildFile.write("cp -v $QTDIR/bin/linguist bin\n")
            buildFile.write("jar -xf qtjambi-linux*.jar\n")
            buildFile.write("rm -rf META-INF\n");
            buildFile.write("cp -v $QTDIR/lib/libQtDesigner.so.4 lib\n")
            buildFile.write("cp -v $QTDIR/lib/libQtDesignerComponents.so.4 lib\n")
            buildFile.write("cp -v $QTDIR/lib/libQtScript.so.4 lib\n")
            buildFile.write("chmod 755 scripts/update_rpath.sh\n");
            buildFile.write("./scripts/update_rpath.sh\n");
        else:
            buildFile.write("cp -vR $QTDIR/bin/Designer.app bin\n")
            buildFile.write("cp -v $QTDIR/bin/lrelease bin\n")
            buildFile.write("cp -v $QTDIR/bin/lupdate bin\n")
            buildFile.write("cp -vR $QTDIR/bin/Linguist.app bin\n")
            buildFile.write("jar -xf qtjambi-mac*.jar\n");
            buildFile.write("rm -rf META-INF\n");
            buildFile.write("cp -v $QTDIR/lib/libQtDesigner.4.dylib lib\n")
            buildFile.write("cp -v $QTDIR/lib/libQtDesignerComponents.4.dylib lib\n")
            buildFile.write("cp -v $QTDIR/lib/libQtScript.4.dylib lib\n")
            buildFile.write("chmod 755 scripts/update_installname.sh\n");
            buildFile.write("./scripts/update_installname.sh\n");

    buildFile.close()

    pkgutil.debug(" - expanding macroes prior to sending...");
    pkgutil.expandMacroes("tmptree", package.licenseHeader)

    zipFile = os.path.join(options.packageRoot, "tmp.zip")
    pkgutil.debug(" - compressing...")
    pkgutil.compress(zipFile, os.path.join(options.packageRoot, "tmptree"))
    pkgutil.debug(" - sending %s to host: %s.." % (package.name(), package.buildServer))
    pkgutil.sendDataFileToHost(package.buildServer, pkgutil.PORT_SERVER, zipFile)



# performs the post-compilation processing of the package
def postProcessPackage(package):
    pkgutil.debug("Post process package " + package.name())
    os.chdir(package.packageDir)

    if os.path.isfile("FATAL.ERROR"):
        print "\nFATAL ERROR on package %s\n" % (package.name())
        return

    if package.hasEclipse():
        doEclipse(package)
    logFile = package.packageDir + "/.task.log"
    if os.path.isfile(logFile):
        shutil.copy(logFile, options.packageRoot + "/." + package.name() + ".tasklog");

    pkgutil.debug(" - creating directories...")
    for mkdir in package.mkdirs:
        os.makedirs(mkdir)

    pkgutil.debug(" - copying files around...")
    copyFiles(package)

    pkgutil.debug(" - deleting files and directories...")
    removeFiles(package)

    # Patch uic.pri since this does not include all necessary files
    if not package.binary:
        pkgutil.debug(" - patching uic.pri")
        tmpFile = open("juic/uic.pri", "a")
        tmpFile.write("\nSOURCES += uic.cpp\n")
        tmpFile.write("HEADERS += uic.h\n")
        tmpFile.close()
    
    if package.binary:
        # move platform jar to startdir for webstart, take the examples and classes from windows
        if package.license == pkgutil.LICENSE_LGPL:
            shutil.copy(package.platformJarName, options.startDir)
            if package.platform == pkgutil.PLATFORM_WINDOWS:
                shutil.copy("qtjambi-%s.jar" % (options.qtJambiVersion), options.startDir);
                shutil.copy("qtjambi-examples-%s.jar" % (options.qtJambiVersion), options.startDir);
        
        # unjar docs into doc directory...
        pkgutil.debug(" - doing docs...")
        os.makedirs("doc/html")
        os.chdir("doc/html")
        os.system("jar -xf %s/javadoc-%s.jar" % (options.startDir, options.qtJambiVersion) )
        os.chdir(package.packageDir)

        # unpack the platform .jar to get the correct binary content into
        # the package...
        pkgutil.debug(" - doing native libraries...")
        os.system("jar -xf %s" % package.platformJarName)
        shutil.rmtree("%s/META-INF" % package.packageDir)
        os.remove("qtjambi-deployment.xml")

        if package.license == pkgutil.LICENSE_EVAL:
            # Eval packages cannot use platform jar's as these cannot
            # be patched with eval key...
            os.remove(package.platformJarName)

        if not package.platform == pkgutil.PLATFORM_WINDOWS:
            os.system("chmod a+rx designer.sh qtjambi.sh")
            os.system("chmod -R a+rw .")
            if package.license == pkgutil.LICENSE_EVAL:
                os.system("chmod a+rx binpatch")

        if package.platform == pkgutil.PLATFORM_LINUX:
            os.chdir("lib")
            os.system("ln -s libqtjambi.so libqtjambi.so.1")
            os.chdir(package.packageDir)

        if package.platform == pkgutil.PLATFORM_MAC:
            os.chdir("lib")
            os.system("ln -s libqtjambi.jnilib libqtjambi.1.jnilib")
            os.chdir(package.packageDir)
            if not package.license == pkgutil.LICENSE_EVAL:
                os.system("chmod a+x Demos.app/Contents/MacOS/JavaApplicationStub")

        if package.platform == pkgutil.PLATFORM_WINDOWS:
            shutil.copytree("plugins/imageformats/Microsoft.VC80.CRT", "plugins/designer/Microsoft.VC80.CRT");

    pkgutil.expandMacroes(package.packageDir, package.licenseHeader)
 
    bundle(package)

    package.success = True



# Goes into the eclipse folder, removes all directories and creates a zip file of the
# remaining .jar files there..
def doEclipse(package):
    os.chdir("eclipse")

    shutil.copy("../dist/eclipse/LICENSE.QT_JAMBI_ECLIPSE_INTEGRATION", "LICENSE")

    for name in os.listdir("qtjambi-4.5"):
        fullName = os.path.join("qtjambi-4.5", name)
        if os.path.isdir(fullName):
            shutil.rmtree(fullName)
    shutil.move("qtjambi-4.5", "plugins")

    if package.platform == pkgutil.PLATFORM_WINDOWS:
        os.system("zip -rq %s/qtjambi-eclipse-integration-%s%s-%s.zip ." % (options.startDir, package.platform, package.arch, options.qtJambiVersion))
    else:
        os.system("tar -czf %s/qtjambi-eclipse-integration-%s%s-%s.tar.gz --owner=0 --group=0 ." % (options.startDir, package.platform, package.arch, options.qtJambiVersion))
        

    os.chdir(package.packageDir)
    if package.platform == pkgutil.PLATFORM_LINUX:
        shutil.move("lib/libqtdesignerplugin.so", "lib/libqtdesigner.so");


# Zips or tars the final content of the package into a bundle in the
# users root directory...
def bundle(package):
    os.chdir(options.packageRoot)
    if package.platform == pkgutil.PLATFORM_WINDOWS:
        if package.binary:
            os.system("chmod -R a+x %s" % package.name())
        os.system("zip -rq %s/%s%s.zip %s" % (options.startDir, package.name(), options.packageExtraName, package.name()))
    else:
        if package.binary:
            os.system("chmod a+x %s/designer.sh" % package.name())
            os.system("chmod a+x %s/qtjambi.sh" % package.name())
            os.system("chmod -R a+x %s/bin" % package.name())
        os.system("tar -czf %s/%s%s.tar.gz --owner=0 --group=0 %s" % (options.startDir, package.name(), options.packageExtraName, package.name()))



# Moves the package content around, such as copying the license files
# from dist etc. This is mostly specified the variable moveFiles in
# the package object.
def copyFiles(package):
    copylog = []
    for m in package.copyFiles:
        if isinstance(m, types.ListType):
            (source, target) = m;
            shutil.copy(source, target);
            copylog.append("%s -> %s" % (source, target))
        else:
            shutil.copy(m, package.packageDir)
            copylog.append("%s -> root" % m)
    package.writeLog(copylog, "copylog");



# Removing all the unwanted content... The package contains two variables,
# removeDirs and removeFiles which are used to kill content. removeDirs is removed
# recursivly and brutally. In addition to the predefined content, we search for a number
# of regexp patterns and remove that content too.
def removeFiles(package):
    for (root, dirs, files) in os.walk(package.packageDir, False):
        for pattern in package.removePatterns:
            for relfile in files:
                file = os.path.join(root, relfile)
                if pattern.search(file):
                    package.removeFiles.append(file)
            for reldir in dirs:
                dir = os.path.join(root, reldir)
                if pattern.search(dir):
                    package.removeDirs.append(dir)
                    
    rmlist = [];
    for fileToRemove in package.removeFiles:
        try:
            if os.path.isfile(fileToRemove):
                os.remove(fileToRemove)
                rmlist.append("remove file: " + fileToRemove);
        except:
            pkgutil.debug("Failed to delete file: " + fileToRemove)

    for dirToRemove in package.removeDirs:
        try:
            if os.path.isdir(dirToRemove):
                shutil.rmtree(dirToRemove)
                rmlist.append("remove dir: " + dirToRemove)
        except:
            pkgutil.debug("Failed to delete directory: " + dirToRemove)

    package.writeLog(rmlist, "removelog")



def displayStatus():
    print "********************** Server status: ( + = ok, - = not ok, blank = waiting)"
    for pkg in packages:
        if not pkg.binary:
            continue
        status = "   ";
        if pkg.done:
            if pkg.success:
                status = " + "
            else:
                status = " - "
        print "    %s: %s %s" % (status, string.ljust(pkg.buildServer, 30), string.ljust(pkg.name(), 25))
    print "********************"



def waitForResponse():
    packagesRemaining = options.binaryPackageCount
    pkgutil.debug("Waiting for build server responses...")

    while packagesRemaining:
        displayStatus()
        (sock, (host, port)) = serversocket.accept()
        pkgutil.debug(" - got response from %s:%d" % (host, port))
        match = False
        for pkg in packages:
            if pkg.binary and socket.gethostbyname(pkg.buildServer) == host and not pkg.done:
                pkg.done = True
                pkg.dataFile = options.packageRoot + "/" + pkg.name() + ".zip"
                pkgutil.getDataFile(sock, pkg.dataFile)
                pkgutil.debug(" - uncompressing to %s" % (pkg.packageDir))
                pkgutil.uncompress(pkg.dataFile, pkg.packageDir);
                try:
                    postProcessPackage(pkg)
                except:
                    traceback.print_exc()
                packagesRemaining = packagesRemaining - 1
                match = True
                break
        if not match:
            print "   - unknown host... %s" % host
    displayStatus()



def signWebstartJars():
    print " - Setting up webstart jars..."

    if options.resetKeystore:
        keystore = os.path.expanduser("~") + "/.keystore";
        print "   - resetting keystore... %s" % keystore
        try:
            os.remove(keystore)
        except:
            print "     - did not delete keystore..."
        keystoreInput = "qqqqqq\nTrolltech Developer\nDevelopment\nTrolltech ASA\nOslo\nOslo\nNO\nyes\n\n"
        inputFile = open("keystore.tmp", "w")
        inputFile.write(keystoreInput)
        inputFile.close()
        os.system("keytool -genkey -alias trolltech < keystore.tmp")
        os.remove("keystore.tmp")

    for pkg in packages:
        if pkg.binary and pkg.license == pkgutil.LICENSE_GPL and not pkg.arch == pkgutil.ARCH_64:
            print "   - signing %s" % (pkg.platformJarName)
            os.system("jarsigner -storepass qqqqqq -keypass qqqqqq %s/%s trolltech" % (options.startDir, pkg.platformJarName))
            if pkg.platform == pkgutil.PLATFORM_WINDOWS:
                print "   - doing examples and classes too..."
                os.system("jarsigner -storepass qqqqqq -keypass qqqqqq %s/qtjambi-%s.jar trolltech" % (options.startDir, options.qtJambiVersion))
                os.system("jarsigner -storepass qqqqqq -keypass qqqqqq %s/qtjambi-examples-%s.jar trolltech" % (options.startDir, options.qtJambiVersion))

    print "   - all jars signed..."



# The main function, parses cmd line arguments and starts the pacakge
# building process...
def main():
    for i in range(0, len(sys.argv)):
        arg = sys.argv[i];
        if arg == "--qt-version":
            options.qtVersion = sys.argv[i+1]
        elif arg == "--package-root":
            options.packageRoot = sys.argv[i+1]
        elif arg == "--package-extra-name":
            options.packageExtraName = sys.argv[i+1]
        elif arg == "--qt-jambi-version":
            options.qtJambiVersion = sys.argv[i+1]
        elif arg == "--no-mac":
            options.buildMac = False
        elif arg == "--no-win":
            options.buildWindows = False
        elif arg == "--no-linux":
            options.buildLinux = False
        elif arg == "--no-eval":
            options.buildEval = False
        elif arg == "--gpl":
            options.buildGpl = True
        elif arg == "--no-lgpl":
            options.buildLgpl = False
        elif arg == "--no-commercial":
            options.buildCommercial = False
        elif arg == "--no-32bit":
            options.build32 = False
        elif arg == "--no-64bit":
            options.build64 = False
        elif arg == "--no-source":
            options.buildSource = False
        elif arg == "--no-binary":
            options.buildBinary = False
        elif arg == "--no-webstart":
            options.buildWebstart = False
        elif arg == "--no-keystore-reset":
            options.resetKeystore = False;
        elif arg == "--verbose":
            pkgutil.VERBOSE = 1
        elif arg == "--preview":
            options.buildPreview = True

    if options.buildPreview:
        options.buildGpl = False
        options.buildLgpl = False
        options.buildEval = False
        options.buildCommercial = False

    options.startDir = os.getcwd()
    options.qtDir = "%s/qt" % options.packageRoot

    pkgutil.debug("Options:")
    print "  - Qt Version: " + options.qtVersion
    print "  - Qt Directory: " + options.qtDir
    print "  - Package Root: " + options.packageRoot
    print "  - Qt Jambi Version: " + options.qtJambiVersion
    print "  - P4 User: " + options.p4User
    print "  - P4 Client: " + options.p4Client
    print "  - Package Extra Name: " + options.packageExtraName
    print "  - buildMac: %s" % options.buildMac
    print "  - buildWindows: %s" % options.buildWindows
    print "  - buildLinux: %s" % options.buildLinux
    print "  - buildEval: %s" % options.buildEval
    print "  - buildGpl: %s" % options.buildGpl
    print "  - buildLgpl: %s" % options.buildLgpl
    print "  - buildCommercial: %s" % options.buildCommercial
    print "  - build32: %s" % options.build32
    print "  - build64: %s" % options.build64
    print "  - buildBinary: %s" % options.buildBinary
    print "  - buildSource: %s" % options.buildSource
    print "  - package for webstart: %s" % options.buildWebstart
    print "  - reset keystore: %s" % options.resetKeystore
    print "  - Preview Packages: %s" % options.buildPreview

    pkgutil.debug("preparing source tree...")
    prepareSourceTree()

    pkgutil.debug("configuring packages...");
    setupPackages()
    pkgutil.debug(" - %d packages in total..." % len(packages))


    # Package and send all packages, finish off by closing all sockets
    # to make sure they are properly closed...
    for package in packages:
        if package.binary:
            packageAndSend(package)
            options.binaryPackageCount = options.binaryPackageCount + 1

    for package in packages:
        if not package.binary:
            packageSourcePackage(package)

    if options.binaryPackageCount:
        waitForResponse()

    if options.buildBinary and options.buildGpl and options.buildWebstart:
        signWebstartJars();

if __name__ == "__main__":
    main()
