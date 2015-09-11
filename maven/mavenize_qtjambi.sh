#!/bin/bash -x

source env_releases
source functions


# prepare poms
rm -rf tmp
mkdir -p tmp
for platform in linux32 linux64 win32 win64 mac32; do
	cat misc/qtjambi-base.pom | sed s/%PLATFORM%/$platform/ | sed s/%QTJAMBI_VERSION%/$QTJAMBI_VERSION/ > tmp/qtjambi-base-$platform.pom || die
	cat misc/qtjambi-platform.pom | sed s/%PLATFORM%/$platform/  | sed s/%QTJAMBI_VERSION%/$QTJAMBI_VERSION/ > tmp/qtjambi-$platform.pom || die
done


# release qt libs
cd tmp
mvn ${MAVEN_ACTION}:${MAVEN_SUBACTION} -Dfile=${QTJAMBI_LINUX32_RELEASE}/qtjambi-${QTJAMBI_VERSION}.jar -DpomFile=qtjambi-base-linux32.pom $MAVEN_REPO || die
mvn ${MAVEN_ACTION}:${MAVEN_SUBACTION} -Dfile=${QTJAMBI_LINUX32_RELEASE}/qtjambi-linux32-gcc-${QTJAMBI_VERSION}.jar -DpomFile=qtjambi-linux32.pom $MAVEN_REPO || die

mvn ${MAVEN_ACTION}:${MAVEN_SUBACTION} -Dfile=${QTJAMBI_LINUX64_RELEASE}/qtjambi-${QTJAMBI_VERSION}.jar -DpomFile=qtjambi-base-linux64.pom $MAVEN_REPO || die
mvn ${MAVEN_ACTION}:${MAVEN_SUBACTION} -Dfile=${QTJAMBI_LINUX64_RELEASE}/qtjambi-linux64-gcc-${QTJAMBI_VERSION}.jar -DpomFile=qtjambi-linux64.pom $MAVEN_REPO || die

mvn ${MAVEN_ACTION}:${MAVEN_SUBACTION} -Dfile=${QTJAMBI_WIN32_RELEASE}/qtjambi-${QTJAMBI_VERSION}.jar -DpomFile=qtjambi-base-win32.pom $MAVEN_REPO || die
mvn ${MAVEN_ACTION}:${MAVEN_SUBACTION} -Dfile=${QTJAMBI_WIN32_RELEASE}/qtjambi-win32-msvc2005-${QTJAMBI_VERSION}.jar -DpomFile=qtjambi-win32.pom $MAVEN_REPO || die


mvn ${MAVEN_ACTION}:${MAVEN_SUBACTION} -Dfile=${QTJAMBI_WIN64_RELEASE}/qtjambi-${QTJAMBI_VERSION}.jar -DpomFile=qtjambi-base-win64.pom $MAVEN_REPO || die
mvn ${MAVEN_ACTION}:${MAVEN_SUBACTION} -Dfile=${QTJAMBI_WIN64_RELEASE}/qtjambi-win64-msvc2005x64-${QTJAMBI_VERSION}.jar -DpomFile=qtjambi-win64.pom $MAVEN_REPO || die

mvn ${MAVEN_ACTION}:${MAVEN_SUBACTION} -Dfile=${QTJAMBI_MAC32_RELEASE}/qtjambi-${QTJAMBI_VERSION}.jar -DpomFile=qtjambi-base-mac32.pom $MAVEN_REPO || die
mvn ${MAVEN_ACTION}:${MAVEN_SUBACTION} -Dfile=${QTJAMBI_MAC32_RELEASE}/qtjambi-macosx-gcc-${QTJAMBI_VERSION}.jar -DpomFile=qtjambi-mac32.pom $MAVEN_REPO || die

cd ..
rm -rf tmp
