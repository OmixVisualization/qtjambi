import datetime
import os
import platform
import re
import socket
import zipfile

VERBOSE = 1
PORT_SERVER = 8184
PORT_CREATOR = 8185

ARCH_32 = "32"
ARCH_64 = "64"
ARCH_UNIVERSAL = "universal"

PLATFORM_WINDOWS = "win"
PLATFORM_LINUX = "linux"
PLATFORM_MAC = "mac"

LICENSE_LGPL = "lgpl"
LICENSE_GPL = "gpl"
LICENSE_EVAL = "eval"
LICENSE_COMMERCIAL = "commercial"
LICENSE_PREVIEW = "preview"

CMD_RESET = "R";
CMD_NEWPKG = "N";

# Debugs out a string if the global variable "VERBOSE" is set
#  - 0: str: The string to debug out...
def debug(str):
    if VERBOSE:
        print str



# Compresses a directory into a zipfile
#  - 0: zipFile: The name of the output file...
#  - 1: zipRoot: The directory to zip down
def compress(zipFile, zipRoot):
    def zipHelper(unused, dir, fileList):
        for file in fileList:
            absFile = (dir + "/" + file)[len(zipRoot) + 1:]
            if os.path.isfile(absFile):
                zip.write(absFile);
    os.chdir(zipRoot);
    zip = zipfile.ZipFile(zipFile, "w", zipfile.ZIP_DEFLATED);
    os.path.walk(zipRoot, zipHelper, 'somenull')
    zip.close()



# Decompresses a zipfile to a certain directory
#  - 0: zipFile: The name of the zipfile to compress
#  - 1: rootDir: The directory in which to stuff the output..
def uncompress(zipFile, rootDir):

    if os.path.isfile(rootDir):
        raise "   - uncompress: rootdir " + rootDir + " exists and is a file!"
    elif not os.path.isdir(rootDir):
        os.makedirs(rootDir, 0777)

    file = zipfile.ZipFile(zipFile);
    for name in file.namelist():
        absPath = os.path.join(rootDir, "/".join(os.path.split(name)[: -1]))
        if not os.path.isdir(absPath):
            os.makedirs(absPath)

        outName = os.path.join(rootDir, name)
        outfile = open(outName, 'wb')
        outfile.write(file.read(name))
        outfile.close()

    file.close()



# Opens a connection to hostName and sends the file specified by
# dataFile to that machine... The port number used is 8184 (ascii dec
# codes for 'Q', 'T')
#  - 0: hostName: The host name of the target machine.
#  - 1: dataFile: the file to transfer...
def sendDataFile(socket, dataFile):
    file = open(dataFile, "rb")
    debug("   - sendDataFile: transfering %s..." % dataFile)
    block = file.read(4096)
    total = len(block)
    while len(block) > 0:
        socket.send(block);
        block = file.read(4096)
        total = total + len(block)
    file.close()
    debug("   - sendDataFile: transfer of file %s complete, total=%d..." % (dataFile, total))



# Opens a connection to hostName and sends the file specified by
# dataFile to that machine... The port number used is 8184 (ascii dec
# codes for 'Q', 'T')
#  - 0: hostName: The host name of the target machine.
#  - 1: port: the port of the target machine, preferably PORT_SERVER or PORT_CREATOR
#  - 2: dataFile: the file to transfer...
def sendDataFileToHost(hostName, port, dataFile):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    address = (hostName, port)
    debug("   - sendDataFile: connecting to: %s:%d" % address)
    s.connect(address)
    sendDataFile(s, dataFile)
    debug("   - sendDataFile: closed connection...")
    s.close()




# Gets a binary file from the 'socket' and writes it to 'dataFile'
#  - 0: socket: The socket
#  - 1: dataFile: the binary file to write..
def getDataFile(socket, dataFile):
    debug("   - getDataFile: receiving  %s..." % dataFile)
    file = open(dataFile, "wb")
    data = socket.recv(4096)
    total = len(data)
    while len(data) > 0:
        file.write(data);
        data = socket.recv(4096);
        total = total + len(data)
    file.close();
    debug("   - getDataFile: transfer of file %s complete, total=%d..." % (dataFile, total))


# Recursively deletes the directory specified with root
#  - 0: root: The root directory.
def rmdirs(root):
    for (dir, dirs, names) in os.walk(root, False):
        for name in names:
            os.remove(os.path.join(dir, name))
        os.rmdir(dir)



# Returns true if the script is running on mac os x
def isMac():
    return platform.system().find("Darwin") >= 0;



# Returns true if the script is running on windows
def isWindows():
    s = platform.system()
    return s.find("Windows") >= 0 or s.find("CYGWIN") >= 0 or s.find("Microsoft") >= 0



# Returns true if the script is running on linux
def isLinux():
    return platform.system().find("Linux") >= 0;



# Reads the license header from /dist/license_....txt and returns it
def readLicenseHeader(license, startDir):
    if license == LICENSE_GPL:
        name = "gpl_header.txt"
    elif license == LICENSE_EVAL:
        name = "eval_header.txt"
    elif license == LICENSE_COMMERCIAL:
        name = "commercial_header.txt"
    elif license == LICENSE_PREVIEW:
        name = "preview_header.txt"
    elif license == LICENSE_LGPL:
        name = "lgpl_header.txt"
    name = "%s/../dist/%s" % (startDir, name)
    file = open(name, "r")
    content = file.read()
    file.close()
    return content



# Locates all text files and expands the $LICENSE$ macroes and similar
# located in them
# 0: dir: The directory to perform the expansion
# 1: header: The content to replace $LICENSE$ tags
def expandMacroes(dir, header):
    thisYear = "%d" % datetime.date.today().year
    patterns = [
        [ re.compile("\\$THISYEAR\\$"), thisYear ],
        [ re.compile("\\$TROLLTECH\\$"), "Nokia" ],
        [ re.compile("\\$PRODUCT\\$"), "Qt Jambi" ],
        [ re.compile("\\$LICENSE\\$"), header ],
        [ re.compile("\\$TROLLTECH_DUAL_LICENSE\\$"), header ],
        [ re.compile("\\$CPP_LICENSE\\$"), header ],
        [ re.compile("\\$JAVA_LICENSE\\$"), header ]
        ]
    
    extensions = [
        re.compile("\\.cpp$"),
        re.compile("\\.h$"),
        re.compile("\\.java"),
        re.compile("\\.html"),
        re.compile("\\.ui"),
        re.compile("LICENSE")
        ]
    for (root, dirs, files) in os.walk(dir):
        for relfile in files:
            file = os.path.join(root, relfile)
            replace = False
            for ext in extensions:
                if ext.search(file):
                    replace = True
            if replace:
                handle = open(file, "r")
                content = handle.read()
                handle.close()
                check = False
                for (regex, replacement) in patterns:
                    content = re.sub(regex, replacement, content)
                os.chmod(file, 0755)
                handle = open(file, "w")
                handle.write(content)
                handle.close()



