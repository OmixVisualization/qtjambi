This is QTJambi maven stuff.

  ** For quickstart as qtjamvbi developer, go to "INSTALLATION" section...
  ** For quickstart as user see usage.txt


It consists of 2 main sections:

1. Stuff used to mavenize QtJambi libraries

	It consists of directory misc and mavenize_qtjambi script.
	"misc" directory contains pom template files used to mavenize
	qtjambi-VERSION.jar and qtjambi-PLATFORM-VERSION.jar files.


2. QtJambiMavenPlugin

	It handles .jui->.java conversion and translations.

	Project is multi-module maven project, containing following modules":

	- plugin
  		This is qtjambi maven plugin code. This plugin uses ( depending on platform )
		resources-* artifact, which contains required binaries such as juic, lupdate and others.
		You must use this plugin in your maven app to handle .jui and .ts files.

	- resources-*
  		These modules are simple jar modules, which contains qtjambi binary resources
  		for each plaform ( juic, lupdate, lrelease... ). 
  		Splitting it into modules should save space and download time when building
  		qtjambi application, because only binaries for current platform are downloaded
  		from maven repo.
		Do not forget to call import_binaries.sh BEFORE you build this project.
  
	- qtjambi
		This module is simple jar module, containing no sources. It only handles
		dependencies on misc qtjambi packages for each platform. This saves time
		to a application developer, because he doesn't need to care about platform.
		Just include groupId=net.sf.qtjambi, artifactId=QtJambi into your project
		and maven will handle platform itself.
		Name of build target is net.sf.qtjambi/QtJambi and this is the only project
		you need to include into your maven app.

	- simpleTestingApp
		This is an example qtjambi application using designer forms and translation
		files. Take a look into the pom.xml how things works and try to run it.


	Limitations:

	- currently based on qtjambi 4.5
	- only win32 and linux64 tested
	- mac not implemented at all
	- it doesn't produce .qm files yet, only .ts


INSTALLATION:

	You don't need to know details about stuff here. It is not depending on current qtjambi
	sources so you don't need to build anything in qtjambi sources.

	Just follow these steps:

	-	download all [linux,windows][32,64] + mac releases into one directory
		( for example ~/qtjambi/ )

	-	unpack all .tar.gz and .zip there

	-	properly set version and path to all releases in env_releases file

	-	run release_all.sh

	If no error occure, all mavenized stuff is now in your local maven repository (~/.m2/repository/net/sf/qtjambi).
	All artifacts from that directory should be published on some public/shared maven repository.

