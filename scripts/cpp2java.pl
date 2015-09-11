#!/usr/bin/perl

import Tie;
use Tie::File;

# We eat input files indiscriminately
die "cpp2java file1 [file2...fileN]\n" if @ARGV < 1;

while ($file = shift @ARGV) {
    tie @array, 'Tie::File', $file
        or print "Failed to tie: " . $file . "\n" and next
            if -e $file;

    for (@array) {
        s/->/\./g;
        s/::/\./g;
        s/\*[a-zA-Z]//g;
        s/foreach/for/g;
        s/bool\s/boolean\s/g;
        s/qreal /double /g;
        s/&\w//g;
        s/const //g;
        s/ const//g;
        s/QString\s/String\s/g;
        s/QStringList/List<String>/g;
    }
}
