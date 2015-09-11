#!/bin/bash

source env_releases
source functions

bash set_version.sh || die

# mavenize qtjambi libraries
bash mavenize_qtjambi.sh || die

# release qtjambi maven plugin
bash import_binaries.sh || die
mvn ${MAVEN_ACTION} || die
bash clean_binaries.sh || die

# prepare maven repo for upload
rm -rf tmp
mkdir -p tmp/maven2/net/sf/qtjambi/
cp -a ~/.m2/repository/net/sf/qtjambi/* tmp/maven2/net/sf/qtjambi/ || die
echo "Options +Indexes" > tmp/maven2/.htaccess
cp usage.txt tmp/maven2
cd tmp

# upload maven repo to sf.net
read -p "sf.net username: " uname
scp -r maven2 ${uname},qtjambi@web.sourceforge.net://home/groups/q/qt/qtjambi/htdocs

# repo is at http://qtjambi.sourceforge.net/maven2/
