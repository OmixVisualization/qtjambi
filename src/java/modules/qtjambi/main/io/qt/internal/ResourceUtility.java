/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
package io.qt.internal;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.JarURLConnection;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.net.URL;
import java.net.URLClassLoader;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collection;
import java.util.Collections;
import java.util.Date;
import java.util.Enumeration;
import java.util.GregorianCalendar;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Set;
import java.util.TreeSet;
import java.util.function.Function;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.zip.ZipEntry;
import java.util.zip.ZipException;

import io.qt.NativeAccess;
import io.qt.core.QDate;
import io.qt.core.QDateTime;
import io.qt.core.QDir;
import io.qt.core.QLocale;
import io.qt.core.QTime;

final class ResourceUtility {
	
	static {
		QtJambi_LibraryUtilities.initialize();
	}
	
	private ResourceUtility() {
	}

    private static class UrlOrFile {
        UrlOrFile(URL url, File file) {
			super();
			this.url = url;
			this.file = file;
		}
		final URL url;
		final File file;
    }

	private final static JarCache cache = new JarCache();
    
    static void addSearchPath(URL url) {
    	if (url != null) {
			cache.addPath(url);
    	}
    }

	static void addSearchPath(String path) {
		URL url = resolveUrlFromPath(path);
		if (url != null) {
			cache.addPath(url);
		}
	}

	static void removeSearchPath(String path) {
		URL url = resolveUrlFromPath(path);
		if (url != null) {
			cache.removePath(url);
		}
	}
	
	@NativeAccess
	private static void initialize() {
		List<URL> cpUrls = new ArrayList<>();
		try {
			for(ClassLoader loader : RetroHelper.classLoaders()) {
				if(loader instanceof URLClassLoader) {
					for(URL url : ((URLClassLoader) loader).getURLs()) {
						if(!cpUrls.contains(url)) {
							cache.addPath(url);
							cpUrls.add(url);				
						}
					}
				}else {
					Enumeration<URL> urls = loader.getResources("META-INF/MANIFEST.MF");
					while (urls.hasMoreElements()) {
						URL url = urls.nextElement();
						try {
							String end;
							String urlPath;
							if("file".equals(url.getProtocol())) {
								urlPath = url.toString();
								end = "/META-INF/MANIFEST.MF";
							}else {
								urlPath = url.getPath();
								end = "!/META-INF/MANIFEST.MF";
							}
							if(urlPath.endsWith(end)) {
								urlPath = urlPath.substring(0, urlPath.length() - end.length());
								URL fileUrl = new URL(urlPath);
								if(!cpUrls.contains(fileUrl)) {
									cache.addPath(fileUrl);
									cpUrls.add(fileUrl);
								}
							}
						} catch (Throwable e) {
							java.util.logging.Logger.getLogger("io.qt.internal.fileengine")
									.log(java.util.logging.Level.SEVERE, "", e);
						}
					}
				}
			}
		} catch (Exception e) {
			java.util.logging.Logger.getLogger("io.qt.internal.fileengine").log(java.util.logging.Level.SEVERE, "",
					e);
		}

		String javaClassPath = System.getProperty("java.class.path");
		if (javaClassPath == null)
			javaClassPath = ""; // gets ignored below
		String javaModulePath = System.getProperty("jdk.module.path");
		if (javaModulePath != null)
			javaClassPath = javaModulePath + File.pathSeparator + javaClassPath; // gets ignored below
		String paths[] = javaClassPath.split("\\" + File.pathSeparator);

		// Only add the .jar files that are not already added...
		int counter = 0;
		for (String path : paths) {
			path = path.trim();
			if (!path.isEmpty()) {
				counter++; // count all paths, invalid and valid

				URL url = resolveUrlFromPath(path);
				boolean match = false;

				if(url!=null){
					cache.addPath(url);
					if(url.toString().endsWith("/"))
						continue;
				}else{
					continue;
				}

				JarResource resource2 = null;
				JarResource resource1 = null;
				try {
					resource2 = resolveUrlToJarResource(url);
					if (resource2 != null) {
						for (URL otherURL : cpUrls) {
							if(otherURL.toString().endsWith("/"))
								continue;
							resource1 = resolveUrlToJarResource(otherURL);
							if (resource1 != null) {
								File file1 = new File(resource1.getName());
								File file2 = new File(resource2.getName());
								if (file1.getCanonicalPath().equals(file2.getCanonicalPath())) {
									match = true;
									break;
								}
							}
						}
					}
				} catch (Exception e) { // This should probably just be IOException
					java.util.logging.Logger.getLogger("io.qt.internal.fileengine")
							.log(java.util.logging.Level.SEVERE, "", e); // this has been so useful in finding many
																			// bugs/issues
				} finally {
					if (resource2 != null) {
						resource2.put();
						resource2 = null;
					}
					if (resource1 != null) {
						resource1.put();
						resource1 = null;
					}
				}

				if (!match)
					cache.addPath(url);
			}
		}
		
		if (counter == 0) {
			try {
				cache.addPath(new File(System.getProperty("user.dir")).toURI().toURL());
			} catch (MalformedURLException e) {
				java.util.logging.Logger.getLogger("io.qt.internal.fileengine")
				.log(java.util.logging.Level.SEVERE, "", e);
			}
		}
	}

	@NativeAccess
	private static URL resolveUrlFromPath(String path) {
		URL result = null;
		if (path != null) {
			File file = new File(path);
			if(file.exists()) {
				try {
					result = file.toURI().toURL();
				} catch (Exception e) {
				}
			}
			if(result==null) {
				try {
					result = new URL(path);
				} catch (Exception e) {
				}
			}
			if(result==null) {
				final int pathLength = path.length();
		
				boolean skipTryAsis = false; // attempt to not use exceptions for common situations
				if (pathLength > 0) {
					char firstChar = path.charAt(0);
					// Both a "/" and "\\" are illegal characters in the scheme/protocol.
					if (firstChar == File.separatorChar) {
						skipTryAsis = true;
					} else if (pathLength == 1) {
						if (firstChar == '.') {
							String tmpPath = System.getProperty("user.dir");
							if (tmpPath != null) {
								path = tmpPath;
								skipTryAsis = true;
							}
						}
						// ELSE it is a relative path and will be picked up below
					} else if (pathLength > 1) {
						char secondChar = path.charAt(1);
						if (firstChar == '.' && secondChar == File.separatorChar) {
							// ./foo/bar case
							String tmpPath = System.getProperty("user.dir");
							if (tmpPath != null) {
								path = tmpPath + File.separatorChar + path.substring(2);
								skipTryAsis = true;
							}
						} else if (pathLength > 2) {
							// Windows "C:\\..." for which "\\" is incorrect for URLs
							char thirdChar = path.charAt(2);
							if ((firstChar >= 'A' && firstChar <= 'Z') || (firstChar >= 'a' && firstChar <= 'z')) {
								// We don't check for '/' since that might be a real URL "a://host:port/path?qs"
								// and would be invalid for windows using java.io.File API anyway.
								if (secondChar == ':' && thirdChar == '\\')
									skipTryAsis = true;
								if (secondChar == ':' && thirdChar == '/') // "C:/dir1/dir2/file.dat" is seen when processing
																			// paths from QFileInfo
									skipTryAsis = true;
							} else if (pathLength > 3) {
								// Eclipse "/C:/..."
								char fourthChar = path.charAt(3);
								if (firstChar == '/' && (secondChar >= 'A' && secondChar <= 'Z')
										|| (secondChar >= 'a' && secondChar <= 'z')) {
									if (thirdChar == ':' && fourthChar == '/')
										skipTryAsis = true; // we prefix it with file:// below
								}
							}
						}
					}
					if (skipTryAsis == false) {
						boolean prefix = !(path.startsWith("file:") 
											|| path.startsWith("jar:") 
											|| path.startsWith("http:")
											|| path.startsWith("https:"));
						int colon = path.indexOf(':');
						if(colon>0) {
							String protocol = path.substring(0, colon);
							try {
								new URL(protocol+"://");
								prefix = false;
							} catch (MalformedURLException e) {
							}
						}
						if (prefix) {
							String tmpPath = System.getProperty("user.dir");
							if (tmpPath != null) {
								String _path = tmpPath + File.separatorChar + path;
								if(new File(_path).exists()) {
									path = _path;
									skipTryAsis = true;
								}
							}
						}
					}
				}
		
				if (result == null) {
					try {
						String xPath = path.replace('\\', '/');
						String xPrefix;
						if (path.length() > 0 && xPath.charAt(0) != '/')
							xPrefix = "file:///";
						else
							xPrefix = "file://";
						String newTmpPath = xPrefix;
						if (File.separatorChar == '\\')
							newTmpPath += path.replace('\\', '/'); // windows
						else
							newTmpPath += path;
						result = new URL(newTmpPath);
					} catch (Exception e) {
					}
				}
			}
	
			try {
				UrlOrFile urlOrFile = checkURL(result);
				if (urlOrFile.file != null) { // Due to workaround
					if (!urlOrFile.file.exists())
						result = null;
				} else if(!urlOrFile.url.toString().endsWith("/")){
					URLConnection urlConn = urlOrFile.url.openConnection();
					try(InputStream inStream = urlConn.getInputStream()){}
				}
			} catch(java.io.FileNotFoundException e) {
				result = null;
			} catch (Throwable e) {
				if(!result.getProtocol().equals("jrt") && !result.getProtocol().equals("file"))
					java.util.logging.Logger.getLogger("io.qt.internal.fileengine").log(java.util.logging.Level.SEVERE, ""+result, e);
				result = null;
			}
		}
		return result;
	}
	
	@NativeAccess
	private static Collection<String> pathToJarFiles(String entry) {
		return cache.pathToJarFiles(entry);
	}
	
	@NativeAccess
	private static boolean isDirectory(JarResource myJarFile, String fileName) {
		boolean isDirectory = false;
        JarEntry fileInJar = myJarFile.getJarEntry(fileName);

        // If the entry exists in the given file, look it up and
        // check if its a dir or not
        if (fileInJar != null) {
            isDirectory = fileInJar.isDirectory();
            if (!isDirectory) {
                boolean tmpIsDirectory = checkIsDirectory(myJarFile, fileInJar);
                isDirectory = tmpIsDirectory;
            } else {
            }
        }

        if (!isDirectory) {
            // Otherwise, look if the directory exists in the
            // cache...
        	Collection<String> pathToJarFiles = pathToJarFiles(fileName);
            String jarFileName = myJarFile.getName();
            if (pathToJarFiles != null) {
                for (String thisPathToJar : pathToJarFiles) {
                    if (thisPathToJar.equals(jarFileName)) {
                        isDirectory = true;
                        break;
                    }
                }
            }

            // Nasty fallback... Iterate through the .jar file and try to check if
            // fileName is the prefix (hence directory) of any of the entries...
            if (!isDirectory) {
                String fileNameWithSlash = fileName + "/";
                Enumeration<JarEntry> entries = myJarFile.entries();
                while (entries.hasMoreElements()) {
                    JarEntry entry = entries.nextElement();
                    String entryName = entry.getName();
                    if (entryName.startsWith(fileNameWithSlash)) {
                        isDirectory = true;
                        break;
                    }
                }
            }
        }
        return isDirectory;
	}

    /**
     * The JarEntry.isDirectory() method in Java returns false
     * even for directories, so we need this extra check
     * which tries to read a byte from the entry in order
     * to trigger an exception when the entry is a directory.
     */
	static boolean checkIsDirectory(JarResource myJarFile, JarEntry fileInJar) {
        InputStream inStream = null;
        try {
            // CHECKME is this hack/trick/kludge maybe somewhat problematic
            //  for connection handler based JarFile handles ?
            inStream = myJarFile.getInputStream(fileInJar);
            if(inStream == null)
                return true;	// avoid NPE
            inStream.read();
        } catch(IOException e) {
            return true;
        } finally {
            if(inStream != null) {
                try {
                    inStream.close();
                } catch(IOException eat) {
                }
                inStream = null;
            }
        }

        return false;
    }

	@NativeAccess
	private static JarResource resolveUrlToJarResource(URL url) throws IOException, ZipException {
        JarResource myJarFile = null;
        UrlOrFile urlOrFile = checkURL(url);
        try {
            if(urlOrFile.file != null) {  // Due to workaround
                if(urlOrFile.file.isFile()) // skip dirs
                    myJarFile = new JarResource(urlOrFile.file);
            } else if(!"jrt".equals(urlOrFile.url.getProtocol())){
                 myJarFile = new JarResource(urlOrFile.url);
            }
        } catch(ZipException e) {
            // This often fails with "java.util.zip.ZipException: error in opening zip file" but never discloses the filename
            throw new ZipException(e.getMessage() + ": " + url);
        }
        return myJarFile;
    }
    


    private static UrlOrFile checkURL(URL url) {
        File file = null;
        if("jar".equals(url.getProtocol())) {
        	String path = url.getPath();
            if(path.endsWith("!/")) {
                try {
                    url = new URL(path.substring(0, path.length() - 2));
                    return checkURL(url);
                } catch(MalformedURLException eat) {
                    url = null;
                }
            }
        } else if("file".equals(url.getProtocol())) {
            try {
				file = new File(url.toURI());
			} catch (URISyntaxException e) {
			}
        }
        return new UrlOrFile(url, file);
    }

    private static final class JarResource{
        private URLConnection urlConnection;  // do we need to keep this around ?
        private File fileToJarFile;    // we store this object type to differentiate between URLs and direct File IO.
        private URL urlToJarFile;  // we save this to allow for close/reopen based on just this handle
        private JarFile jarFile;
        private int refCount;

        JarResource(File fileToJarFile) throws IOException {
            this.fileToJarFile = fileToJarFile;
            openInternal();
        }

        JarResource(URL urlToJarFile) throws IOException {
            this.urlToJarFile = urlToJarFile;
            openInternal();
        }

        private void openInternal() throws IOException {
            if(fileToJarFile != null) {  // Direct File I/O Jar file
                jarFile = new JarFile(fileToJarFile);
            } else {
                urlConnection = urlToJarFile.openConnection();
                if(urlConnection instanceof JarURLConnection) {
                    JarURLConnection jarUrlConnection = (JarURLConnection) urlConnection;
                    jarFile = jarUrlConnection.getJarFile();
                }else {
                    IOException thr = new IOException("not a JarURLConnection: " + urlConnection.getClass().getName()+" for URL: "+urlToJarFile);
                    urlConnection = null;  // we only keep handle when we have active Jar open
                    throw thr;
                }
            }
            refCount = 1;
        }

        // This method may never throw an exception
        @NativeAccess
        void get() {
            synchronized(this) {
                refCount++;
            }
        }

        // This method must cause a double increment on the reopen() case
        // Returns the previous refCount, so 0 means we just reopened, non-zero means we did get()
        @NativeAccess
        int getOrReopen() throws IOException {
            int oldRefCount;
            synchronized (this) {
                oldRefCount = refCount;
                if(refCount <= 0)
                    reopen();
                get();
            }
            return oldRefCount;
        }

        // This method may never throw an exception
        @NativeAccess
        void put() {
            JarFile closeJarFile = null;
            synchronized(this) {
                refCount--;
                if(refCount == 0) {
                    closeJarFile = jarFile;
                    jarFile = null;
                }
            }
            if(closeJarFile != null) {
                try {
                    closeJarFile.close();
                } catch(IOException eat) {
                }
                if(urlConnection != null) {
                    urlConnection = null;
                }
            }
        }

        void reopen() throws IOException {
            if(jarFile != null)
                throw new IOException("jarFile already open");
            openInternal();
        }

        @NativeAccess
        String getName() {
            return jarFile.getName();
        }

        Enumeration<JarEntry> entries() {
            return jarFile.entries();
        }

        @NativeAccess
        JarEntry getJarEntry(String name) {
        	JarEntry entry = jarFile.getJarEntry(name);
        	if(entry==null) {
        		JarEntry aliasEntry = jarFile.getJarEntry(name+".alias");
        		if(aliasEntry!=null) {
        			try(InputStream stream = jarFile.getInputStream(aliasEntry)){
        				Properties aliasProperties = new Properties();
        				aliasProperties.load(stream);
        				QLocale locale = new QLocale();
        				String localeName = locale.name();
        				String aliasName = aliasProperties.getProperty(localeName);
        				if(aliasName==null && !locale.bcp47Name().equals(localeName)) {
        					aliasName = aliasProperties.getProperty(locale.bcp47Name());
        				}
        				if(aliasName==null) {
        					String[] split = localeName.split("_");
        					if(split.length>1)
        						aliasName = aliasProperties.getProperty(split[0]);
        				}
        				if(aliasName==null) {
        					aliasName = aliasProperties.getProperty("default");
        				}
        				if(aliasName==null) {
        					aliasName = aliasProperties.getProperty("");
        				}
        				if(aliasName!=null) {
        					int idx = name.lastIndexOf('/');
        					if(idx>0) {
        						String path = name.substring(0, idx+1);
        						entry = jarFile.getJarEntry(path + aliasName);
        					}else{
        						entry = jarFile.getJarEntry(aliasName);
        					}
        				}
        			} catch (Throwable ign) {
					}
        		}
        	}
            return entry;
        }

        @NativeAccess
        InputStream getInputStream(ZipEntry ze) throws IOException {
            return jarFile.getInputStream(ze);
        }
        
        @NativeAccess
        private void entryList(List<String> result, QDir.Filters filters, Collection<String> filterNames, String mentryName){
            if (!mentryName.endsWith("/") && mentryName.length() > 0)
                mentryName = mentryName + "/";

            Enumeration<JarEntry> entries = entries();

            HashSet<String> used = new HashSet<String>();
            while (entries.hasMoreElements()) {
                JarEntry entry = entries.nextElement();

                String entryName = entry.getName();

                // Must be inside this directory
                if (entryName.length() <= mentryName.length() || !mentryName.equals(entryName.substring(0, mentryName.length())) || mentryName.equals(entryName))
                    continue;

                // Only one level
                boolean isDir;
                int pos = entryName.indexOf("/", mentryName.length());
                if (pos > 0) {
                    entryName = entryName.substring(0, pos);
                    isDir = true;
                } else {
                    isDir = entry.isDirectory();
                    if (!isDir)
                        isDir = ResourceUtility.checkIsDirectory(this, entry);
                }

                if (!filters.testFlag(QDir.Filter.Readable))
                    continue ;

                if (!filters.testFlag(QDir.Filter.Dirs) && isDir)
                    continue ;

                if (!filters.testFlag(QDir.Filter.Files) && !isDir)
                    continue ;

                if (filterNames.size() > 0) {
                    if ((!isDir || !filters.testFlag(QDir.Filter.AllDirs))
                        && (!QDir.match(filterNames, entryName.substring(mentryName.length())))) {
                        continue;
                    }
                }

                if (entryName.endsWith("/") && entryName.length() > 1)
                    entryName = entryName.substring(0, entryName.length() - 1);

                entryName = entryName.substring(mentryName.length());

                if (!used.contains(entryName)) {
                    used.add(entryName);
                    result.add(entryName);
                }
            }
        }
        
        @NativeAccess
        private static QDateTime fileTime(ZipEntry ze) {
        	long tm = ze.getTime();
            if (tm == -1)
                return new QDateTime();  // the current time

            Calendar calendar = new GregorianCalendar();
            calendar.setTime(new Date(tm));

            return new QDateTime(new QDate(calendar.get(Calendar.YEAR),
                                           calendar.get(Calendar.MONTH) + 1,
                                           calendar.get(Calendar.DAY_OF_MONTH)),
                                 new QTime(calendar.get(Calendar.HOUR_OF_DAY),
                                           calendar.get(Calendar.MINUTE),
                                           calendar.get(Calendar.SECOND),
                                           calendar.get(Calendar.MILLISECOND)));
        }
    }
    
    @NativeAccess
    private static Collection<String> classPathDirs() {
    	return cache.classPathDirs();
    }
    
    @NativeAccess
    private static void clear() {
    	synchronized(cache.cache) {
    		cache.cache.clear();
    	}
    	synchronized(cache.classPathDirs) {
    			cache.classPathDirs.clear();
    	}
    }
    
    private final static class JarCache {
    	
    	private static final Function<String, Set<String>> newSetFactory = s -> new HashSet<>();

        Collection<String> pathToJarFiles(String entry) {
        	synchronized(cache) {
	        	Set<String> result = cache.get(entry);
	            return result==null? Collections.emptyList() : new HashSet<>(result);
        	}
        }

        Collection<String> classPathDirs() {
        	synchronized(classPathDirs) {
        		return new HashSet<>(classPathDirs);
        	}
        }

        private final Map<String, Set<String>> cache = new HashMap<>();
        private final Set<String> classPathDirs = new HashSet<>();

    	void addPath(URL url) {
    		JarResource resource = null;
            try {
                // 
                if(url.getProtocol().equals("file")) {
                    File fileDir = new File(url.toURI());
                    if(fileDir.isDirectory()) {
                    	synchronized(classPathDirs) {
                    		classPathDirs.add(fileDir.getAbsolutePath());
                    	}
                        return;
                    } else if(fileDir.isFile()) {
                        try {
                            resource = new JarResource(fileDir);
                        } catch(IOException eat) {
                        }
                    }else {
                    	return;
                    }
                }else if(url.toString().endsWith("/")) {
                	synchronized(classPathDirs) {
                		classPathDirs.add(url.toString());
                	}
                    return;
                }
                if(resource == null) {
                    try {
                        resource = new JarResource(url);
                    } catch(ZipException e) {
                        throw new ZipException(e.getMessage() + ": " + url);
                    }
                }

                String resourceName = resource.getName();

                Set<String> seenSet = new TreeSet<>();
                Enumeration<JarEntry> entries = resource.entries();
                
                while (entries.hasMoreElements()) {
                    JarEntry entry = entries.nextElement();

                    String dirName = null;
                    boolean isToplevelFile = false;

                    String entryName = entry.getName();

                    // Remove potentially initial '/'
                    while (entryName.startsWith("/"))
                        entryName = entryName.substring(1);

                    if (entry.isDirectory()) {
                        if (entryName.endsWith("/"))
                            dirName = entryName.substring(0, entryName.length() - 1);  // canonicalize
                        else
                            dirName = entryName;
                    } else {
                        int slashPos = entryName.lastIndexOf("/");
                        if (slashPos > 0)
                            dirName = entryName.substring(0, slashPos);  // isolate directory part
                        else
                            isToplevelFile = true;  // dirName will be null; there is no directory part
                    }


                    // Add all parent directories "foo/bar/dir1/dir2", "foo/bar/dir1", "foo/bar", "foo"
                    while (dirName != null) {
                        // optimization: if we saw the long nested path (then we already processed its parents as well)
                        if (seenSet.contains(dirName))
                            break;
                        seenSet.add(dirName);
                        synchronized(cache) {
                        	cache.computeIfAbsent(dirName, newSetFactory).add(resourceName);
                        }
                        int slashPos = dirName.lastIndexOf("/");
                        if (slashPos > 0)
                            dirName = dirName.substring(0, slashPos);
                        else
                            dirName = null;
                    }

                    if (isToplevelFile) {
                        if (!seenSet.contains("")) {
                            seenSet.add("");
                            synchronized(cache) {
                            	cache.computeIfAbsent("", newSetFactory).add(resourceName);
                            }
                        }
                    }
                }

                // Add root dir for all jar files (even empty ones)
                if (!seenSet.contains("")) {
                    seenSet.add("");
                    // Add root dir for all jar files (even empty ones)
                    synchronized(cache) {
                    	cache.computeIfAbsent("", newSetFactory).add(resourceName);
                    }
                }
            } catch (Exception e) {
            } finally {
                if (resource != null) {
                    resource.put();
                    resource = null;
                }
            }
    	}

    	void removePath(URL url) {
    		JarResource resource = null;
            try {
                // 
                if(url.getProtocol().equals("file")) {
                    File fileDir = new File(url.toURI());
                    if(fileDir.isDirectory()) {
                    	removeImpl(fileDir.getAbsolutePath());
                        return;
                    } else if(fileDir.isFile()) {
                        try {
                            resource = new JarResource(fileDir);
                        } catch(IOException eat) {
                        }
                    }else {
                    	return;
                    }
                }else if(url.toString().endsWith("/")) {
                	removeImpl(url.toString());
                    return;
                }
                if(resource == null) {
                    try {
                        resource = new JarResource(url);
                    } catch(ZipException e) {
                    }
                }
                if(resource != null) {
                	removeImpl(resource.getName());
                }else {
                	removeImpl(url.toString());
                }
            } catch (Exception e) {
            	removeImpl(url.toString());
            } finally {
                if (resource != null) {
                    resource.put();
                    resource = null;
                }
            }
    	}
    	
    	private void removeImpl(String jarFileName) {
    		synchronized(cache) {
	    		for(String key : new ArrayList<>(cache.keySet())) {
	    			Set<String> entries = cache.get(key);
	    			if(entries!=null) {
	    				entries.remove(jarFileName);
	    				if(entries.isEmpty()) {
	    					cache.remove(key);
	    				}
	    			}else {
	    				cache.remove(key);
	    			}
	    		}
    		}
    	}
    }
}
