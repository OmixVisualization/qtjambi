#!/usr/bin/env ruby

# this script is designed to be run from base directory of qtjambi.

# Depends on Trollop

require "fileutils"
require "trollop"

# TODO: unroll changes related QTDIR and Qt source dir, those must be existent for generation to work
# in first place
# TODO: sanitize paths
class JavadocBuilder

    attr_accessor :classpath_argument, :qtinclude_argument, \
            :qtjambi_version, :qdoc, :header, :qtdir

    def initialize
        @BASE_PATH = File.expand_path "."
        @OUTPUT_DIRECTORY = @BASE_PATH + "/build/javadoc"
        @dochome = "http://qtjambi.sourceforge.net/doc/latest"

        #FileUtils.rm_rf @OUTPUT_DIRECTORY, :secure => true if File.exist? @OUTPUT_DIRECTORY
        if not File.exist? @OUTPUT_DIRECTORY
            Dir.mkdir @OUTPUT_DIRECTORY
        end

        parse_arguments

        build_jambi_doclet
        clean_old_dirs
        generate_qdoc_japi
        generate_jdoc
        run_qdoc
        package_jdoc
        create_doclet_header
        generate_javadoc
    end

private

    def parse_arguments
        opts = Trollop::options do
            version "Qt Jambi Javadoc generator 0.1 ♡ Samu Voutilainen"
            banner <<-EOS
Qt Jambi documentation generator
Generates HTML and Javadoc for Qt Jambi.

Usage:
        scripts/build_javadoc.rb [options]

Options:
EOS

            opt :qtjambi_version, "Version of Qt Jambi"
            opt :classpath, "Classpath arguments for doclet building", :type => String
            opt :qtinclude_argument, "TODO: what for was this?"
            opt :qtdir, "Base directory of Qt source", :type => String
            opt :library_path, "Specifies where libraries used the commands are", :type => String
            #opt :path, "Append stuff before PATH", :type => String
            opt :qdoc3, "Run only qdoc3 task"
            opt :japi,  "Generate japi file"
            opt :jdoc, "Generate jdoc"
        end

        Trollop::die :classpath, "Must be set" if opts[:classpath] == nil
        Trollop::die :qtdir, "Must be set" if opts[:qtdir] == nil

        @classpath_argument = opts[:classpath]
        @qtdir = opts[:qtdir]

        # this wouldn’t work for Windows, use PATH instead for it?
        ENV["LD_LIBRARY_PATH"] = opts[:library_path] if opts[:library_path] != nil

        # TODO: works only for Linux
        #ENV["PATH"] = opts[:path] + ":" + ENV["PATH"] if opts[:path] != nil

        if opts[:qdoc3] == true
            run_qdoc
            exit
        end

        if opts[:japi] == true
            generate_qdoc_japi
            exit
        end

        generate_jdoc if opts[:jdoc] == true
    end


    # TODO: this classpath argument should most likely be generated from somewhere.
    # TODO: this is most likely broken
    def generate_javadoc
        classpath = "tools/jambidoc"
        system "javadoc -classpath #{classpath} -doclet jambidoc.JambiDoclet " + 
               "-header \"#{@header}\" -J-Xmx500m -sourcepath #{@BASE_PATH} org.qtjambi.qt org.qtjambi.qt.core " + 
               "org.qtjambi.qt.gui org.qtjambi.qt.opengl org.qtjambi.qt.sql org.qtjambi.qt.opengl "+
               "org.qtjambi.qt.svg org.qtjambi.qt.network org.qtjambi.qt.xml " +
               "org.qtjambi.qt.designer org.qtjambi.qt.webkit org.qtjambi.qt.phonon"
        if $?.exitstatus != 0
            puts "Failed to run javadoc"
            exit 1
        end

        system 'find . -name "qt jambi.dcf" -exec rm {} \;'
        system 'find . -name "qt jambi.index" -exec rm {} \;'
        system 'find . -name "*.jdoc" -exec rm {} \;'

        system "jar -cf #{@OUTPUT_DIRECTORY}/qtjambi-javadoc-#{@qtjambi_version}.jar doc/html/*"
        if $?.exitstatus != 0
            puts "Failed to run jar -cf doc/*"
            exit 1
        end

        #FileUtils.cp "doc/html/qtjambi-javadoc-$QTJAMBI_VERSION.jar", @OUTPUT_DIRECTORY
    end

    def create_doclet_header
        @header = "<table align='right'><tr><td nowrap><a target='_top' href='#{@dochome}/org/qtjambi/qt/qtjambi-index.html'>Qt Jambi Home</a></td>"
        @header += "$HEADER<td><img src='$DOCHOME/org/qtjambi/qt/images/qt-logo.png' width='32' height='32'></td></tr></table>" 
    end

    def package_jdoc
        system "jar -cf #{@OUTPUT_DIRECTORY}/qtjambi-jdoc-#{@qtjambi_version}.jar doc/html/org/qtjambi/qt/*.jdoc"
        if $?.exitstatus != 0
            puts "Failed to run jar -cf *.jdoc"
            exit 1
        end
    end

    def generate_jdoc
        Dir.chdir "generator" do
            command = "./generator --jdoc-enabled --jdoc-dir ../build/doc/jdoc"
            puts "Running " + command
            system command
            if $?.exitstatus != 0
                puts "Failed to generate jdoc"
                exit 1
            end
        end
    end

    def run_qdoc
        #cd $LOCAL_QDOC/test
        #../qdoc3 qt-for-jambi.qdocconf jambi.qdocconf
        ENV["QTDIR"] = @qtdir
        ENV["QT_SOURCE_TREE"] = @qtdir
        ENV["JAMBI"] = Dir.pwd

        docpath = "#{@BASE_PATH}/doc/qdocconf"
        puts

        Dir.chdir "#{@BASE_PATH}/qdoc3" do
            command = "./qdoc3 #{docpath}/qt-for-jambi.qdocconf #{docpath}/jambi.qdocconf"
            puts "Running " + command
            system command
            if $?.exitstatus != 0
                puts "Failed to run qdoc3"
                exit 1
            end
        end
        puts
    end

    def generate_qdoc_japi
        puts
        Dir.chdir "generator" do 
            #command = "#{@BASE_PATH}/generator --build-qdoc-japi #{@qtinclude_argument} --output-directory=\"#{@OUTPUT_DIRECTORY}\""
            command = "./generator --build-qdoc-japi #{@qtinclude_argument} --output-directory=\"#{@OUTPUT_DIRECTORY}\""
            puts "Running " + command
            system command
            if $?.exitstatus != 0
                puts "Failed to run generator --build-qdoc-japi"
                exit 1
            end
        end
        puts
    end

    def clean_old_dirs
        FileUtils.rm_rf "#{@OUTPUT_DIRECTORY}/html", :secure => true
        Dir.mkdir "#{@OUTPUT_DIRECTORY}/html"
    end

    def build_jambi_doclet
        Dir.chdir "#{@BASE_PATH}/tools/jambidoc" do
            command = "javac -cp #{@classpath_argument} -d \"#{@OUTPUT_DIRECTORY}\" *.java"
            puts "running: #{command}"
            system command
            if $?.exitstatus != 0
                puts "Failed to compile jambidoclet"
                exit 1
            end
        end
    end
    
end

JavadocBuilder.new
