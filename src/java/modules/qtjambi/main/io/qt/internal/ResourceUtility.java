/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.net.URLClassLoader;
import java.net.URLConnection;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.nio.file.Files;
import java.nio.file.attribute.FileTime;
import java.util.Collection;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Set;
import java.util.TreeSet;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.zip.ZipEntry;
import java.util.zip.ZipException;

import io.qt.NativeAccess;
import io.qt.core.QDir;
import io.qt.core.QLocale;

/**
 * @hidden
 */
final class ResourceUtility {
	
	private ResourceUtility() {
	}
	
	private static final int ExcludeFiles =          0x000004;
	private static final int ExcludeDirs =           0x000008;
	
	private static final Thread cleanupThread;
	private final static JarResourceFactoryInterface factory;
	static {
		cleanupThread = new Thread(JarResource::release);
		cleanupThread.setName("QtJambiResourceReleaserThread");
		cleanupThread.setDaemon(true);
		cleanupThread.start();
		
		if(Boolean.getBoolean("io.qt.acknowledge-resources")) {
			factory = new RecognizableJarResourceFactory();
		}else {
			factory = new JarResourceFactory();
		}
	}
	
	private interface JarResourceFactoryInterface{
		JarResource create(File fileToJarFile) throws IOException;
        JarResource create(URL urlToJarFile) throws IOException;
	}
	
	private static class JarResourceFactory implements JarResourceFactoryInterface{
		@Override
		public JarResource create(File fileToJarFile) throws IOException {
			return new JarResource(fileToJarFile);
		}
		@Override
		public JarResource create(URL urlToJarFile) throws IOException {
			return new JarResource(urlToJarFile);
		}
	}
	
	private static class RecognizableJarResourceFactory implements JarResourceFactoryInterface{
		@Override
		public JarResource create(File fileToJarFile) throws IOException {
			return new RecognizableJarResource(fileToJarFile);
		}
		@Override
		public JarResource create(URL urlToJarFile) throws IOException {
			return new RecognizableJarResource(urlToJarFile);
		}
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

    static void addSearchPath(URL url) {
    	if (url != null) {
			addCachePath(url, false);
    	}
    }

	static void addSearchPath(String path) {
		URL url = resolveUrlFromPath(path);
		if (url != null) {
			addCachePath(url, false);
		}
	}

	static void removeSearchPath(String path) {
		URL url = resolveUrlFromPath(path);
		if (url != null) {
			removeCachePath(url);
		}
	}
	
	static void initialize(String qtJambiConfFile) {
		ensureHandler(qtJambiConfFile);
		if(!Boolean.getBoolean("io.qt.no-classpath-to-resourcepath")) {
			Set<URL> cpUrls = new HashSet<>();
			try {
				for(URI uri : RetroHelper.moduleLocations()) {
					try{
						URL url = uri.toURL();
						if(!"jrt".equals(url.getProtocol()) && !cpUrls.contains(url)) {
							addCachePath(url, true);
							cpUrls.add(url);
						}
					} catch (Exception e) {
						java.util.logging.Logger.getLogger("io.qt.internal.fileengine").log(java.util.logging.Level.SEVERE, "", e);
					}
				}
				for(ClassLoader loader : RetroHelper.classLoaders()) {
					if(loader instanceof URLClassLoader) {
						for(URL url : ((URLClassLoader) loader).getURLs()) {
							if(!cpUrls.contains(url)) {
								addCachePath(url, true);
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
								if("jar".equals(url.getProtocol())) {
									urlPath = url.getPath();
									end = "!/META-INF/MANIFEST.MF";
								}else {
									urlPath = url.toString();
									end = "/META-INF/MANIFEST.MF";
									if(urlPath.endsWith(end))
										end = "META-INF/MANIFEST.MF";
								}
								if(urlPath.endsWith(end)) {
									String metainfURL = url.toString();
									metainfURL = metainfURL.substring(0, metainfURL.length()-11);
									urlPath = urlPath.substring(0, urlPath.length() - end.length());
									URL fileUrl = CoreUtility.createURL(urlPath);
									if(!"jar".equals(url.getProtocol()) && fileUrl.getPath().isEmpty()) {
										fileUrl = CoreUtility.createURL(urlPath+"/");
									}
									if(!cpUrls.contains(fileUrl)) {
										addCachePath(fileUrl, true);
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
			for (String path : paths) {
				path = path.trim();
				if (!path.isEmpty()) {
					URL url = resolveUrlFromPath(path);
					if(url!=null && !cpUrls.contains(url))
						addCachePath(url, true);
				}
			}
			
			if (cpUrls.isEmpty()) {
				try {
					addCachePath(new File(System.getProperty("user.dir")).toURI().toURL(), true);
				} catch (MalformedURLException e) {
					java.util.logging.Logger.getLogger("io.qt.internal.fileengine")
					.log(java.util.logging.Level.SEVERE, "", e);
				}
			}
		}
		switch(LibraryUtility.operatingSystem) {
		case Android:
			String androidBundlePath = androidBundlePath();
			if(androidBundlePath!=null) {
				URL url = resolveUrlFromPath("file:"+androidBundlePath);
				if (url != null) {
					addCachePath(url, true);
				}
			}
			break;
			default: break;
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
					result = CoreUtility.createURL(path);
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
								CoreUtility.createURL(protocol+"://");
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
						result = CoreUtility.createURL(newTmpPath);
					} catch (Exception e) {
					}
				}
			}
	
			try {
				UrlOrFile urlOrFile = checkURL(result);
				if (urlOrFile.file != null) { // Due to workaround
					if (!urlOrFile.file.exists())
						result = null;
				} else if(!urlOrFile.url.toString().endsWith("/") && !urlOrFile.url.getPath().isEmpty()){
					URLConnection urlConn = urlOrFile.url.openConnection();
					if(urlConn!=null && urlConn.getDoInput()) {
						try(InputStream inStream = urlConn.getInputStream()){}
					}else{
						result = null;
					}
				}
			} catch(java.io.FileNotFoundException | java.net.UnknownServiceException e) {
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
	private static void cleanupOnShutdown() {
		jarResources.clear();
	}

	@NativeAccess
	private static JarResource resolveFileToJarResource(String file) throws IOException, ZipException {
        JarResource myJarFile = jarResources.get(file);
        if(myJarFile!=null)
        	myJarFile.ensureRef();
        return myJarFile;
    }


    private static UrlOrFile checkURL(URL url) {
        File file = null;
        if("jar".equals(url.getProtocol())) {
        	String path = url.getPath();
            if(path.endsWith("!/")) {
                try {
                    url = CoreUtility.createURL(path.substring(0, path.length() - 2));
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

    private static class JarResource{
        private URLConnection urlConnection;  // do we need to keep this around ?
        private File fileToJarFile;    // we store this object type to differentiate between URLs and direct File IO.
        private URL urlToJarFile;  // we save this to allow for close/reopen based on just this handle
        private JarFile jarFile;
        private int refCount;
        private long time;
        private final Map<String,Boolean> knownDirectoryPaths = Collections.synchronizedMap(new HashMap<>());
        private static final LinkedList<JarResource> referencesResources = new LinkedList<>();
        
        static final int resourceCacheTime = 1000 * 60 * 3; // keeping cached jars open for 3 minutes
        
        static void release() {
        	try {
        		while(true) {
        			JarResource resource;
        			synchronized(referencesResources) {
						if(referencesResources.isEmpty())
							referencesResources.wait();
						resource = referencesResources.poll();
					}
        			if(resource!=null) {
        				long t1, t2;
        				synchronized(resource) {
        					t1 = resource.time;
        					long delta = System.currentTimeMillis() - t1;
        					if(delta<resourceCacheTime)
        						resource.wait(resourceCacheTime-delta);
        					t2 = resource.time;
        				}
        				if(t1==t2) {
        					resource.deref();
        				}else {
        					synchronized(referencesResources) {
        		            	referencesResources.push(resource);
        		            }
        				}
        			}
        		}
			} catch (InterruptedException e) {
			}
        }

        JarResource(File fileToJarFile) throws IOException {
            this.fileToJarFile = fileToJarFile;
            openInternal();
            time = System.currentTimeMillis();
            synchronized(referencesResources) {
            	referencesResources.push(this);
            	referencesResources.notifyAll();
            }
        }

        JarResource(URL urlToJarFile) throws IOException {
            this.urlToJarFile = urlToJarFile;
            openInternal();
            time = System.currentTimeMillis();
            synchronized(referencesResources) {
            	referencesResources.push(this);
            	referencesResources.notifyAll();
            }
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
        final void ref() {
            synchronized(referencesResources) {
                ++refCount;
            }
        }

        // This method must cause a double increment on the reopen() case
        // Returns the previous refCount, so 0 means we just reopened, non-zero means we did get()
        @NativeAccess
        final int ensureRef() throws IOException {
            int oldRefCount;
            synchronized (referencesResources) {
                oldRefCount = refCount;
                if(refCount <= 0) {
                	if(jarFile != null)
                        throw new IOException("jarFile already open");
                    openInternal();
                    referencesResources.push(this);
                	referencesResources.notifyAll();
                }
                ref();
            }
            synchronized (this) {
            	time = System.currentTimeMillis();
            	this.notifyAll();
			}
            return oldRefCount;
        }

        // This method may never throw an exception
        @NativeAccess
        final void deref() {
            JarFile closeJarFile = null;
            synchronized(referencesResources) {
            	if(refCount == 0)
            		return;
                --refCount;
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

        @NativeAccess
        final String getName() {
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
        			try(InputStream stream = getInputStream(aliasEntry)){
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
        	if(entry==null) {
        		entry = jarFile.getJarEntry(name+"/");
        	}
        	if(entry==null && isDirectory(name)) {
        		if(!name.endsWith("/"))
        			name += "/";
        		entry = new JarEntry(name);
        	}
            return entry;
        }

        @NativeAccess
        InputStream getInputStream(ZipEntry ze) throws IOException {
            return jarFile.getInputStream(ze);
        }
        
        @NativeAccess
        private ReadableByteChannel getChannel(ZipEntry ze) throws IOException{
        	return Channels.newChannel(getInputStream(ze));
        }
        
        @NativeAccess
        private void entryList(List<String> result, int _filters, Collection<String> filterNames, String mentryName, boolean isIteratorFlags){
            if (!mentryName.endsWith("/") && mentryName.length() > 0)
                mentryName = mentryName + "/";

            Enumeration<JarEntry> entries = entries();

            HashSet<String> used = new HashSet<String>();
//            QDir.Filters filters = new QDir.Filters(_filters);
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
                    isDir = checkIsDirectory(entry);
                }

                if(!isIteratorFlags) {
	                if ((_filters & QDir.Filter.Readable.value())==0)
	                    continue ;
	
	                if ((_filters & QDir.Filter.Dirs.value())==0 && isDir)
	                    continue ;
	
	                if ((_filters & QDir.Filter.Files.value())==0 && !isDir)
	                    continue ;
	
	                if (filterNames.size() > 0) {
	                    if ((!isDir || (_filters & QDir.Filter.AllDirs.value())==0)
	                        && (!QDir.match(filterNames, entryName.substring(mentryName.length())))) {
	                        continue;
	                    }
	                }
                }else {
                	if ((_filters & ExcludeDirs)==ExcludeDirs && isDir)
	                    continue ;
	
	                if ((_filters & ExcludeFiles)==ExcludeFiles && !isDir)
	                    continue ;
	                if (filterNames.size() > 0) {
	                    if (!QDir.match(filterNames, entryName.substring(mentryName.length()))) {
	                        continue;
	                    }
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
        private long fileTime(ZipEntry ze, boolean creationTime, boolean lastAccessTime, boolean lastModified) throws IOException {
        	FileTime fileTime = null;
        	if(ze!=null) {
				if(creationTime)
					fileTime = ze.getCreationTime();
				else if(lastAccessTime)
					fileTime = ze.getLastAccessTime();
				else if(lastModified)
					fileTime = ze.getLastModifiedTime();
				else
					fileTime = ze.getCreationTime();
        	}else if(fileToJarFile!=null){
				fileTime = Files.getLastModifiedTime(fileToJarFile.toPath());
        	}
        	if(fileTime!=null) {
        		return fileTime.toMillis();
        	}else {
            	long tm = -1;
        		if(ze!=null){
            		tm = ze.getTime();
            	}else if(fileToJarFile!=null){
            		tm = fileToJarFile.lastModified();
            	}
	            return tm;
        	}
        }
    	
    	@NativeAccess
    	private boolean isDirectory(String _fileName) {
    		return knownDirectoryPaths.computeIfAbsent(_fileName, fileName->{
	    		boolean isDirectory = false;
	    		String fileNameWithSlash = fileName + "/";
	            Enumeration<JarEntry> entries = entries();
	            while (entries.hasMoreElements()) {
	                JarEntry entry = entries.nextElement();
	                String entryName = entry.getName();
	                if (entryName.startsWith(fileNameWithSlash)) {
	                    isDirectory = true;
	                    break;
	                }
	            }
	            return isDirectory;
    		});
    	}

    	@NativeAccess
    	boolean checkIsDirectory(JarEntry fileInJar) {
    		boolean isDirectory = fileInJar.isDirectory();
            if (!isDirectory)
            	isDirectory = isDirectory(fileInJar.getName());
            return isDirectory;
        }
    }
    
    private static final class RecognizableJarResource extends JarResource{

		RecognizableJarResource(URL urlToJarFile) throws IOException {
			super(urlToJarFile);
		}

		RecognizableJarResource(File fileToJarFile) throws IOException {
			super(fileToJarFile);
		}
		
		@Override
		JarEntry getJarEntry(String name) {
			JarEntry entry = super.getJarEntry(name);
			if(entry!=null) {
				try {
					ResourceUtility.class.getClassLoader().getResource(entry.getName());
				} catch (Exception e) {
				}
			}
			return entry;
		}
		
		@Override
		InputStream getInputStream(ZipEntry entry) throws IOException {
			if(entry!=null) {
				try {
					ResourceUtility.class.getClassLoader().getResource(entry.getName());
				} catch (Exception e) {
				}
			}
            return super.getInputStream(entry);
        }
    	
		@Override
		Enumeration<JarEntry> entries() {
            Enumeration<JarEntry> entries = super.entries();
            while (entries.hasMoreElements()) {
                JarEntry entry = entries.nextElement();
    			if(entry!=null) {
    				try {
    					ResourceUtility.class.getClassLoader().getResource(entry.getName());
    				} catch (Exception e) {
    				}
    			}
            }
            return super.entries();
        }
    }
    
    private final static Map<String,JarResource> jarResources = Collections.synchronizedMap(new HashMap<>());
    
	private static void addCachePath(URL url, boolean atInitialization) {
		JarResource resource;
		String resourceName;
        try {
            // 
            if(url.getProtocol().equals("file")) {
                File file = new File(url.toURI());
                if(file.isDirectory()) {
                	if(!atInitialization || !Boolean.getBoolean("io.qt.no-directories-to-resourcepath"))
                		addClassPath(file.getAbsolutePath(), true);
                    return;
                } else if(file.isFile()) {
                	resourceName = file.getPath();
                    if(File.separatorChar=='\\')
                    	resourceName = resourceName.replace(File.separatorChar, '/');
                    resource = jarResources.computeIfAbsent(resourceName, p->{
                    	try {
                    		return factory.create(file);
                        } catch(IOException eat) {
                        	return null;
                        }
                    });
                    if(atInitialization) {
                    	if(resource.getJarEntry(LibraryUtility.DEPLOY_XML)!=null
                    			|| resource.getJarEntry("META-INF/qtjambi-utilities.xml")!=null) {
                    		jarResources.remove(resourceName);
                    		resource.deref();
                    		return;
                    	}
                    }
                }else {
                	return;
                }
            }else if(url.toString().endsWith("/") || url.getPath().isEmpty()) {
            	addClassPath(url.toString(), false);
                return;
            }else{
                try {
                    resource = factory.create(url);
                    resourceName = resource.getName();
                    if(File.separatorChar=='\\')
                    	resourceName = resourceName.replace(File.separatorChar, '/');
                    JarResource tmpResource = resource;
                    resource = jarResources.computeIfAbsent(resourceName, p->tmpResource);
                    if(resource!=tmpResource) {
                    	tmpResource.deref();
                    }
                    if(atInitialization) {
                    	if(resource.getJarEntry(LibraryUtility.DEPLOY_XML)!=null
                    			|| resource.getJarEntry("META-INF/qtjambi-utilities.xml")!=null) {
                    		jarResources.remove(resourceName);
                    		resource.deref();
                    		return;
                    	}
                    }
                } catch(ZipException e) {
                    throw new ZipException(e.getMessage() + ": " + url);
                } catch(IOException eat) {
                	resource = null;
                	resourceName = null;
                	return;
                }
            }

            Set<String> directoryPaths = new TreeSet<>();
            try {
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
	                    if (directoryPaths.contains(dirName))
	                        break;
	                    directoryPaths.add(dirName);
	                    int slashPos = dirName.lastIndexOf("/");
	                    if (slashPos > 0)
	                        dirName = dirName.substring(0, slashPos);
	                    else
	                        dirName = null;
	                }
	
	                if (isToplevelFile) {
	                    if (!directoryPaths.contains("")) {
	                        directoryPaths.add("");
	                    }
	                }
	            }
	
	            // Add root dir for all jar files (even empty ones)
	            if (!directoryPaths.contains("")) {
	                // Add root dir for all jar files (even empty ones)
	                directoryPaths.add("");
	            }
            }finally {
            	insertJarFileResources(directoryPaths, resourceName);
            }
        } catch (Exception e) {
        }
	}

	private static void removeCachePath(URL url) {
        try {
            if(url.getProtocol().equals("file")) {
                File fileDir = new File(url.toURI());
            	String resourceName = fileDir.getAbsolutePath();
                if(File.separatorChar=='\\')
                	resourceName = resourceName.replace(File.separatorChar, '/');
            	removeResource(resourceName);
            	JarResource resource = jarResources.remove(resourceName);
            	if(resource!=null)
            		resource.deref();
            }else if(url.toString().endsWith("/") || url.getPath().isEmpty()) {
            	removeResource(url.toString());
            }
        } catch (Exception e) {
        	removeResource(url.toString());
        }
	}
    
	private static native void ensureHandler(String qtJambiConfFile);
    private static native void insertJarFileResources(Collection<String> directoryPaths, String jarFileName);
    private static native void removeResource(String path);
    private static native void addClassPath(String path, boolean isDirectory);
    private static native String androidBundlePath();
}
