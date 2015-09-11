#!/bin/bash

source env_releases

for i in `find . -name pom.in`; do
	cat $i | sed s/%QTJAMBI_VERSION%/${QTJAMBI_VERSION}/g | sed s/%QTJAMBI_PLUGIN_VERSION%/${QTJAMBI_PLUGIN_VERSION}/g > `dirname $i`/pom.xml
	
done

