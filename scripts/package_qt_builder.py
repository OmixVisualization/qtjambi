#!/usr/bin/python

import os
import shutil
import socket
import sys


import pkgutil



# initialize the socket callback interface so we have it
# available..
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.bind((socket.gethostname(), pkgutil.PORT_CREATOR))
serversocket.listen(16)



class Options:
    def __init__(self):
        self.qtLabel = None
        self.qtBranch = None
        if pkgutil.isWindows():
            self.packageRoot = "C:/tmp/qt-build"
        else:
            self.packageRoot = "/tmp/qt-build"
        self.p4User = "qt"
        self.p4Client = "qt-builder"
        self.startDir = os.getcwd()
        self.p4Resync = True

        self.buildMac = True
        self.buildWindows = True
        self.buildLinux = True
        self.build32 = True
        self.build64 = True

options = Options()

lgpl_header = pkgutil.readLicenseHeader(pkgutil.LICENSE_LGPL, options.startDir)
gpl_header = pkgutil.readLicenseHeader(pkgutil.LICENSE_GPL, options.startDir)
commercial_header = pkgutil.readLicenseHeader(pkgutil.LICENSE_COMMERCIAL, options.startDir)


class BuildServer:
    def __init__(self, platform, arch):
        self.arch = arch
        self.platform = platform

servers = []



def setupServers():
    os.chdir(options.packageRoot)

    if options.buildLinux and options.build32:
        linux32 = BuildServer(pkgutil.PLATFORM_LINUX, pkgutil.ARCH_UNIVERSAL)
        linux32.host = "tirionvm-linux32.troll.no"
        linux32.task = options.startDir + "/build_qt_linux.sh"
        servers.append(linux32)

    if options.buildLinux and options.build64:
        linux64 = BuildServer(pkgutil.PLATFORM_LINUX, pkgutil.ARCH_UNIVERSAL)
        linux64.host = "tirionvm-linux64.troll.no"
        linux64.task = options.startDir + "/build_qt_linux.sh"
        servers.append(linux64)

    if options.buildWindows and options.build64:
        win64 = BuildServer(pkgutil.PLATFORM_WINDOWS, pkgutil.ARCH_64)
        win64.host = "tirionvm-win64.troll.no"
        win64.task = options.startDir + "/build_qt_windows.bat"
        win64.compiler = "msvc2005_x64"
        servers.append(win64)

    if options.buildWindows and options.build32:
        win32 = BuildServer(pkgutil.PLATFORM_WINDOWS, pkgutil.ARCH_32)
        win32.host = "packy-win32-jambiclone.troll.no"
        win32.task = options.startDir + "/build_qt_windows.bat"
        win32.compiler = "msvc2005"
        servers.append(win32)

    if options.buildMac:
        mac = BuildServer(pkgutil.PLATFORM_MAC, pkgutil.ARCH_UNIVERSAL)
        mac.host = "alqualonde.troll.no"
        mac.task = options.startDir + "/build_qt_mac.sh"
        servers.append(mac)

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
    tmpFile.write("        //depot/qt/%s/...  //%s/qt/...\n" % (options.qtBranch, options.p4Client))
    tmpFile.write("        -//depot/qt/%s/examples/... //qt-builder/qt/examples/...\n" % options.qtBranch)
    tmpFile.write("        -//depot/qt/%s/tests/... //qt-builder/qt/tests/...\n" % options.qtBranch)
    tmpFile.write("        -//depot/qt/%s/demos/... //qt-builder/qt/demos/...\n" % options.qtBranch)
    tmpFile.write("        -//depot/qt/%s/doc/... //qt-builder/qt/doc/...\n" % options.qtBranch)
    tmpFile.write("        -//depot/qt/%s/tmake/... //qt-builder/qt/tmake/...\n" % options.qtBranch)
    tmpFile.write("        -//depot/qt/%s/translations/... //qt-builder/qt/translations/...\n" % options.qtBranch)
    tmpFile.write("        -//depot/qt/%s/dist/... //qt-builder/qt/dist/...\n" % options.qtBranch)
    tmpFile.close()
    os.system("p4 -u %s -c %s client -i < p4spec.tmp" % (options.p4User, options.p4Client) );
    os.remove("p4spec.tmp")

    # sync p4 client spec into subdirectory...
    label = ""
    if options.qtLabel:
        label = "@" + options.qtLabel
    pkgutil.debug(" - syncing p4...")
    os.system("p4 -u %s -c %s sync -f //%s/... %s > .p4sync.buildlog" % (options.p4User, options.p4Client, options.p4Client, label))
    os.system("chmod -R u+w .")



def packageAndSend(server):
    pkgutil.debug("sending to %s, script=%s..." % (server.host, server.task))

    os.chdir(options.packageRoot)

    if os.path.isdir("tmptree"):
        shutil.rmtree("tmptree")
    os.makedirs("tmptree")

    print " - setting up lgpl subdir..."
    shutil.copytree("qt", "tmptree/lgpl");
    pkgutil.expandMacroes("tmptree/lgpl", lgpl_header)
    

    print " - setting up commercial subdir..."
    shutil.copytree("qt", "tmptree/commercial");
    pkgutil.expandMacroes("tmptree/commercial", commercial_header)


    if server.platform == pkgutil.PLATFORM_WINDOWS:
        shutil.copy(server.task, "tmptree/task.bat")
    else:
        shutil.copy(server.task, "tmptree/task.sh")

    zipFile = os.path.join(options.packageRoot, "tmp.zip")
    pkgutil.debug(" - compressing...")
    pkgutil.compress(zipFile, os.path.join(options.packageRoot, "tmptree"))
    pkgutil.debug(" - sending to host: %s.." % (server.host))
    pkgutil.sendDataFileToHost(server.host, pkgutil.PORT_SERVER, zipFile)



def waitForResponse():
    packagesRemaining = len(servers)
    pkgutil.debug("Waiting for build server responses...")

    while packagesRemaining:
        (sock, (host, port)) = serversocket.accept()
        pkgutil.debug(" - got response from %s:%d" % (host, port))
        match = False
        for server in servers:
            if socket.gethostbyname(server.host) == host:
                dataFile = options.packageRoot + "/" + server.host + ".zip"
                outDir = options.packageRoot + "/" + server.host;
                pkgutil.getDataFile(sock, dataFile)
                pkgutil.debug(" - uncompressing to %s" % outDir)
                pkgutil.uncompress(dataFile, outDir);

                if os.path.isfile(outDir + "/FATAL.ERROR"):
                    print "Build server: %s Failed!!!!" % server.host
                else:
                    print "Build server: %s ok!" % server.host

                match = True

        if match:
            packagesRemaining = packagesRemaining - 1




def main():
    for i in range(0, len(sys.argv)):
        arg = sys.argv[i];
        if arg == "--package-root":
            options.packageRoot = sys.argv[i+1]
        elif arg == "--qt-branch":
            options.qtBranch = sys.argv[i+1]
        elif arg == "--qt-label":
            options.qtLabel = sys.argv[i+1]
        elif arg == "--no-p4sync":
            options.p4Resync = False
        elif arg == "--verbose":
            pkgutil.VERBOSE = 1
        elif arg == "--no-mac":
            options.buildMac = False
        elif arg == "--no-win":
            options.buildWindows = False
        elif arg == "--no-linux":
            options.buildLinux = False
        elif arg == "--no-32bit":
            options.build32 = False
        elif arg == "--no-64bit":
            options.build64 = False

    pkgutil.debug("Options:")
    print "  - Qt Branch: %s" % options.qtBranch
    print "  - Qt Label: %s" % options.qtLabel
    print "  - Package Root: " + options.packageRoot
    print "  - P4 User: " + options.p4User
    print "  - P4 Client: " + options.p4Client
    print "  - P4 Resync: %s" % options.p4Resync
    print "  - buildMac: %s" % options.buildMac
    print "  - buildWindows: %s" % options.buildWindows
    print "  - buildLinux: %s" % options.buildLinux
    print "  - build32: %s" % options.build32
    print "  - build64: %s" % options.build64

    if not options.qtBranch:
        pkgutil.debug("At the very least, you must specify --qt-branch")
        return

    if options.p4Resync:
        pkgutil.debug("preparing source tree...")
        prepareSourceTree()

    pkgutil.debug("setting up packages..")
    setupServers()

    for server in servers:
        packageAndSend(server)

    waitForResponse()




if __name__ == "__main__":
    main()
