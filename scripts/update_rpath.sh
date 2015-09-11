#!/bin/bash

function patchBinary {
    name=$1
    rpath=$2

    echo chrpath --replace \'$rpath\' $name
    chrpath --replace "$rpath" $name
}

function patchLibraries 
{
    for lib in $(find . -name "*.so" -o -name "*.so.4")
    do
	if [ $(expr "$lib" : "./plugins") -gt 0 ]; then
	    patchBinary $lib '$ORIGIN/../../lib'
	elif [ $(expr "$lib" : "./lib") -gt 0 ]; then
	    patchBinary $lib '$ORIGIN/../lib'
	fi
    done
}

function patchApplications {
    for app in $(ls bin)
    do
	appName=bin/$app
	if [ -x $appName ] && [ ! -d $appName ]; then
	    patchBinary $appName '$ORIGIN/../lib'
	fi
    done

}

patchLibraries
patchApplications

