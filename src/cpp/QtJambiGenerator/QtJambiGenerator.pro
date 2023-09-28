TEMPLATE = subdirs

SUBDIRS = library executable

library.file = library.pro
executable.file = executable.pro
executable.depends = library

