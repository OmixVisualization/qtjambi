#!/bin/sh

function patchBinary {
    name=$1
    toPrefix=$2

    for i in $(otool -L $name)
    do 
	if [ $(expr "$i" : "libQt") -gt 0 ]; then
	    echo install_name_tool -change $i $toPrefix/$i $name
	    install_name_tool -change $i $toPrefix/$i $name
	fi
	    
    done
}

function patchApplicationBundles {
    for bundle in $(find . -name "*.app") 
    do
	for appName in $(ls $bundle/Contents/MacOS) 
	do
	    name=$bundle/Contents/MacOS/$appName
	    patchBinary $name '@executable_path/../../../../lib'
	done
    done
}

function patchLibraries {
    for lib in $(find . -name "*.dylib")
    do
	if [ $(expr "$lib" : "./plugins") -gt 0 ]; then
	    patchBinary $lib '@loader_path/../../lib'
	elif [ $(expr "$lib" : "./lib") -gt 0 ]; then
	    patchBinary $lib '@loader_path'
	fi
    done
}

function patchApplications {
    for app in $(ls bin)
    do
	appName=bin/$app
	if [ -x $appName ] && [ ! -d $appName ]; then
	    patchBinary $appName '@executable_path/../lib'
	fi
    done

}

patchApplicationBundles
patchLibraries
patchApplications

#patchBinary 'bin/juic' '@executable_path/../lib'
#patchBinary 'bin/lrelease' '@executable_path/../lib'
#patchBinary 'bin/lupdate' '@executable_path/../lib'

