#!/bin/sh

javac -cp ../target/marble-linux32-gcc-4.6.3.jar:$JAMBIDIR/qtjambi-4.6.3.jar TestJavaMarble.java
java -cp ../target/marble-linux32-gcc-4.6.3.jar:$JAMBIDIR/qtjambi-4.6.3.jar:. TestJavaMarble
