#!/bin/sh

javac -cp ../jambi/target/test-linux32-gcc-4.6.3.jar:$JAMBIDIR/qtjambi-4.6.3.jar TestJava.java
java -cp ../jambi/target/test-linux32-gcc-4.6.3.jar:$JAMBIDIR/qtjambi-4.6.3.jar:. TestJava
