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
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.nio.file.Files;
import java.nio.file.attribute.FileTime;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Map;
import java.util.Properties;
import java.util.Set;
import java.util.TreeMap;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.logging.Logger;
import java.util.zip.ZipEntry;
import java.util.zip.ZipException;

import io.qt.NativeAccess;
import io.qt.core.QLocale;

/**
 * @hidden
 */
final class ResourceUtility {
	
	private static final Logger logger = Logger.getLogger("io.qt.internal.resources");
	
	private static final int resourceCacheTime = Integer.getInteger("io.qt.resource-cache-timeout", 1000 * 60 * 3); // keeping cached jars open for 3 minutes
	private static final boolean noDirectoriesToResourcePath = Boolean.getBoolean("io.qt.no-directories-to-resourcepath");
	private static final boolean metaInfoToResources = Boolean.getBoolean("io.qt.allow-meta-inf-as-resource");
	private static final boolean allowClassFilesAsResources = Boolean.getBoolean("io.qt.allow-classfiles-as-resource");
	private static final JarResourceFactoryInterface factory;
	static {
		if(Boolean.getBoolean("io.qt.acknowledge-resources")) {
			factory = new RecognizableJarResourceFactory();
		}else {
			factory = new JarResourceFactory();
		}
	}
	
	private ResourceUtility() {
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
		Set<URL> cpUrls = new HashSet<>();
		final String resourceFiles = System.getProperty("io.qt.resourcepath");
		if((resourceFiles==null || resourceFiles.trim().isEmpty()) && !Boolean.getBoolean("io.qt.no-classpath-to-resourcepath")) {
			try {
				for(URI uri : RetroHelper.moduleLocations()) {
					try{
						URL url = uri.toURL();
						if(!"jrt".equals(url.getProtocol()) && !cpUrls.contains(url)) {
							addCachePath(url, true);
							cpUrls.add(url);
						}
					} catch (Exception e) {
						logger.log(java.util.logging.Level.SEVERE, "", e);
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
								logger.log(java.util.logging.Level.SEVERE, "", e);
							}
						}
					}
				}
			} catch (Exception e) {
				logger.log(java.util.logging.Level.SEVERE, "", e);
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
					logger
					.log(java.util.logging.Level.SEVERE, "", e);
				}
			}
		}
		if(resourceFiles!=null) {
			for(String path : resourceFiles.split(File.pathSeparator)) {
				URL url = resolveUrlFromPath(path);
				if(url!=null && !cpUrls.contains(url))
					addCachePath(url, true);
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

    private static UrlOrFile checkURL(URL url) {
        File file = null;
        if("jar".equals(url.getProtocol())) {
        	String path = url.getPath();
            if(path.endsWith("!/")) {
                try {
                    url = CoreUtility.createURL(path.substring(0, path.length() - 2));
                    return checkURL(url);
                } catch(Exception eat) {
                    url = null;
                }
            }
        } else if("file".equals(url.getProtocol())) {
            try {
				file = new File(url.toURI());
			} catch (Exception e) {
				try {
					file = new File(url.toString().substring(5));
				} catch (Exception e2) {
				}
			}
        }
        return new UrlOrFile(url, file);
    }

    private static class JarResource{
        private File fileToJarFile;    // we store this object type to differentiate between URLs and direct File IO.
        private URL urlToJarFile;  // we save this to allow for close/reopen based on just this handle
        private JarFile jarFile;
        private int refCount;
        private long time;
        private static final LinkedList<JarResource> referencesResources = new LinkedList<>();
        
		private static final Thread cleanupThread;
		static {
			if(resourceCacheTime>0) {
				cleanupThread = new Thread(JarResource::release);
				cleanupThread.setName("QtJambiResourceReleaserThread");
				cleanupThread.setDaemon(true);
				cleanupThread.start();
			}else {
				cleanupThread = null;
			}
		}
        
        private static void release() {
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
            refCount = 1;
            if(cleanupThread!=null) {
	            time = System.currentTimeMillis();
	            synchronized(referencesResources) {
	            	referencesResources.push(this);
	            	referencesResources.notifyAll();
	            }
            }
        }

        JarResource(URL urlToJarFile) throws IOException {
            this.urlToJarFile = urlToJarFile;
            openInternal();
            refCount = 1;
            if(cleanupThread!=null) {
	            time = System.currentTimeMillis();
	            synchronized(referencesResources) {
	            	referencesResources.push(this);
	            	referencesResources.notifyAll();
	            }
            }
        }

        private void openInternal() throws IOException {
            if(fileToJarFile != null) {  // Direct File I/O Jar file
            	logger.log(java.util.logging.Level.FINEST, ()->"opening resource "+fileToJarFile);
                jarFile = new JarFile(fileToJarFile);
            } else {
            	logger.log(java.util.logging.Level.FINEST, ()->"opening resource "+urlToJarFile);
            	URLConnection urlConnection = urlToJarFile.openConnection();
                if(urlConnection instanceof JarURLConnection) {
                    JarURLConnection jarUrlConnection = (JarURLConnection) urlConnection;
                    jarFile = jarUrlConnection.getJarFile();
                }else {
                    IOException thr = new IOException("not a JarURLConnection: " + urlConnection.getClass().getName()+" for URL: "+urlToJarFile);
                    throw thr;
                }
            }
        }

        // This method must cause a double increment on the reopen() case
        // Returns the previous refCount, so 0 means we just reopened, non-zero means we did get()
        private void ref() throws IOException {
            int oldRefCount;
        	boolean pushAgain = false;
        	synchronized (this) {
            	oldRefCount = refCount;
                if(oldRefCount <= 0) {
                	if(jarFile != null)
                        throw new IOException("jarFile already open");
                    openInternal();
                    refCount = 2;
                    pushAgain = true;
                }else {
                	++refCount;
                }
        	}
        	if(cleanupThread!=null) {
	        	if(pushAgain) {
	        		synchronized (referencesResources) {
	        			referencesResources.push(this);
	                	referencesResources.notifyAll();
	        		}
	        	}
	        	synchronized (this) {
	            	time = System.currentTimeMillis();
	            	this.notifyAll();
				}
        	}
        }

        // This method may never throw an exception
        final void deref() {
            JarFile closeJarFile;
            synchronized(this) {
            	if(refCount == 0)
            		return;
                --refCount;
                if(refCount == 0) {
                    closeJarFile = jarFile;
                    jarFile = null;
                }else {
                	closeJarFile = null;
                }
            }
            if(closeJarFile != null) {
                try {
                	logger.log(java.util.logging.Level.FINEST, ()->"closing resource "+closeJarFile.getName());
                    closeJarFile.close();
                } catch(IOException eat) {
                }
            }
        }

        protected JarEntry getJarEntry(String name) {
        	return jarFile.getJarEntry(name);
        }

        private InputStream getInputStream(ZipEntry ze) throws IOException {
            return jarFile.getInputStream(ze);
        }
        
        final String resolveAlias(String entryName, long[] properties) throws IOException {
        	ref();
        	try {
        		JarEntry entry = getJarEntry(entryName+".alias");
        		if(entry!=null) {
        			try(InputStream stream = getInputStream(entry)){
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
        					int idx = entryName.lastIndexOf('/');
        					if(idx>0) {
        						String path = entryName.substring(0, idx+1);
        						entry = getJarEntry(path + aliasName);
        					}else{
        						entry = getJarEntry(aliasName);
        					}
        					if(entry!=null) {
        						if(properties.length>0)
        							properties[0] = entry.getSize();
        						if(properties.length>=4) {
        							FileTime time = Files.getLastModifiedTime(fileToJarFile.toPath());
        							long lastModified = time==null ? fileToJarFile.lastModified() : time.toMillis();
            						FileTime creationTime = entry.getCreationTime();
            	            		FileTime lastAccessTime = entry.getLastAccessTime();
            	            		FileTime lastModifiedTime = entry.getLastModifiedTime();
            	            		properties[1] = creationTime==null ? lastModified : creationTime.toMillis();
            	            		properties[2] = lastAccessTime==null ? lastModified : lastAccessTime.toMillis();
            	            		properties[3] = lastModifiedTime==null ? lastModified : lastModifiedTime.toMillis();
        						}
        						return entry.getName();
        					}
        				}
        			} catch (Throwable ign) {
					}
        		}
        	}finally {
        		deref();
        	}
        	return null;
        }
        
        final ReadableByteChannel getChannel(String name) throws IOException{
        	ref();
        	JarEntry entry = getJarEntry(name);
        	if(entry!=null) {
            	ReadableByteChannel channel = Channels.newChannel(getInputStream(entry));
            	return new ReadableByteChannel() {
    				@Override
    				public boolean isOpen() {
    					return channel.isOpen();
    				}
    				@Override
    				public void close() throws IOException {
    					try{
    						channel.close();
    					}finally {
    						deref();
    					}
    				}
    				@Override
    				public int read(ByteBuffer dst) throws IOException {
    					return channel.read(dst);
    				}
    			};
            }else {
            	deref();
        	}
            return null;
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
		protected JarEntry getJarEntry(String name) {
			JarEntry entry = super.getJarEntry(name);
			if(entry!=null) {
				try {
					ResourceUtility.class.getClassLoader().getResource(entry.getName());
				} catch (Exception e) {
				}
			}
			return entry;
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
                	if(!atInitialization || !noDirectoriesToResourcePath)
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
                    	if(resource.jarFile.getJarEntry(LibraryUtility.DEPLOY_XML)!=null
                    			|| resource.jarFile.getJarEntry("META-INF/qtjambi-utilities.xml")!=null) {
                    		JarResource tmp = jarResources.remove(resourceName);
                    		if(tmp!=null)
                    			tmp.deref();
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
                    resourceName = resource.jarFile.getName();
                    if(File.separatorChar=='\\')
                    	resourceName = resourceName.replace(File.separatorChar, '/');
                    JarResource tmpResource = resource;
                    resource = jarResources.computeIfAbsent(resourceName, p->tmpResource);
                    if(resource!=tmpResource) {
                    	tmpResource.deref();
                    }
                    if(atInitialization) {
                    	if(resource.jarFile.getJarEntry(LibraryUtility.DEPLOY_XML)!=null
                    			|| resource.jarFile.getJarEntry("META-INF/qtjambi-utilities.xml")!=null) {
                    		JarResource tmp = jarResources.remove(resourceName);
                    		if(tmp!=null)
                    			tmp.deref();
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
            
            final Map<String,long[]> entryPaths = new TreeMap<>();
            final Map<String,long[]> emptyDirectoryPaths = new TreeMap<>();
            final Map<String,long[]> emptyMetaInfDirectoryPaths = new TreeMap<>();
            final Map<String,long[]> metaInfEntryPaths = new TreeMap<>();
            FileTime time = Files.getLastModifiedTime(resource.fileToJarFile.toPath());
            long lastModified = time==null ? resource.fileToJarFile.lastModified() : time.toMillis();
            try {
            	emptyDirectoryPaths.put("", new long[] {Long.valueOf(-1), lastModified, lastModified, lastModified});
            	emptyMetaInfDirectoryPaths.put("", new long[] {Long.valueOf(-1), lastModified, lastModified, lastModified});
	            Enumeration<JarEntry> entries = resource.jarFile.entries();
	            while (entries.hasMoreElements()) {
	            	JarEntry entry = entries.nextElement();
	            	if(entry.isDirectory()) {
		            	String dirName = entry.getName();
		            	while (dirName.startsWith("/"))
		            		dirName = dirName.substring(1);
	            		if (dirName.endsWith("/"))
	                        dirName = dirName.substring(0, dirName.length() - 1);  // canonicalize
	            		FileTime creationTime = entry.getCreationTime();
	            		FileTime lastAccessTime = entry.getLastAccessTime();
	            		FileTime lastModifiedTime = entry.getLastModifiedTime();
	            		if(dirName.equals("META-INF") || dirName.startsWith("META-INF/"))
	                    	emptyMetaInfDirectoryPaths.put(dirName, new long[] {
	                    			Long.valueOf(-1), 
	                    			creationTime==null ? lastModified : creationTime.toMillis(), 
                					lastAccessTime==null ? lastModified : lastAccessTime.toMillis(), 
        							lastModifiedTime==null ? lastModified : lastModifiedTime.toMillis()});
	                    else
	                    	emptyDirectoryPaths.put(dirName, new long[] {
	                    			Long.valueOf(-1), 
	                    			creationTime==null ? lastModified : creationTime.toMillis(), 
                					lastAccessTime==null ? lastModified : lastAccessTime.toMillis(), 
        							lastModifiedTime==null ? lastModified : lastModifiedTime.toMillis()});
	            	}
	            }
	            entries = resource.jarFile.entries();
	            while (entries.hasMoreElements()) {
	            	JarEntry entry = entries.nextElement();
	            	if(!entry.isDirectory() && entry.getSize()>=0) {
		            	String entryName = entry.getName();
		            	while (entryName.startsWith("/"))
		                    entryName = entryName.substring(1);
		            	if(!allowClassFilesAsResources && entry.getName().endsWith(".class")) {
		            		continue;
		            	}
		            	boolean isAlias = entryName.endsWith(".alias");
		            	if(isAlias)
		            		entryName = entryName.substring(0, entryName.length()-6);
		            	FileTime creationTime = entry.getCreationTime();
	            		FileTime lastAccessTime = entry.getLastAccessTime();
	            		FileTime lastModifiedTime = entry.getLastModifiedTime();
		            	if(entryName.startsWith("META-INF/")) {
		            		metaInfEntryPaths.put(entryName, new long[] {isAlias ? -2 : entry.getSize(), 
	                    			creationTime==null ? lastModified : creationTime.toMillis(), 
                					lastAccessTime==null ? lastModified : lastAccessTime.toMillis(), 
        							lastModifiedTime==null ? lastModified : lastModifiedTime.toMillis()});
		            		String dirName = entryName;
		            		int slashPos = dirName.lastIndexOf("/");
		            		while(slashPos > 0){
		                    	dirName = dirName.substring(0, slashPos);
		                    	metaInfEntryPaths.put(dirName, new long[] {Long.valueOf(-1), lastModified, lastModified, lastModified});
		                    	emptyMetaInfDirectoryPaths.remove(dirName);
		                    	slashPos = dirName.lastIndexOf("/");
		            		}
		            		emptyMetaInfDirectoryPaths.remove("");
		            	}else {
		            		entryPaths.put(entryName, new long[] {isAlias ? -2 : entry.getSize(), 
	                    			creationTime==null ? lastModified : creationTime.toMillis(), 
                					lastAccessTime==null ? lastModified : lastAccessTime.toMillis(), 
        							lastModifiedTime==null ? lastModified : lastModifiedTime.toMillis()});
		            		String dirName = entryName;
		            		int slashPos = dirName.lastIndexOf("/");
		            		while(slashPos > 0){
		                    	dirName = dirName.substring(0, slashPos);
		                    	entryPaths.put(dirName, new long[] {Long.valueOf(-1), lastModified, lastModified, lastModified});
		                    	emptyDirectoryPaths.remove(dirName);
		                    	slashPos = dirName.lastIndexOf("/");
		            		}
	                    	emptyDirectoryPaths.remove("");
		            	}
	            	}
	            }
            }finally {
            	if(!emptyDirectoryPaths.isEmpty()) {
            		entryPaths.putAll(emptyDirectoryPaths);
            	}
            	if(!metaInfEntryPaths.isEmpty() && metaInfoToResources) {
            		entryPaths.putAll(metaInfEntryPaths);
            	}
            	if(!emptyMetaInfDirectoryPaths.isEmpty() && metaInfoToResources) {
        			entryPaths.putAll(emptyMetaInfDirectoryPaths);
            	}
            	insertJarFileResources(entryPaths, resourceName);
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
    private static native void insertJarFileResources(Map<String,long[]> entries, String jarFileName);
    private static native void removeResource(String path);
    private static native void addClassPath(String path, boolean isDirectory);
    private static native String androidBundlePath();
	
	@NativeAccess
	private static void cleanupOnShutdown() {
		jarResources.clear();
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
					logger.log(java.util.logging.Level.SEVERE, ""+result, e);
				result = null;
			}
		}
		return result;
	}
	
	@NativeAccess
	private static ReadableByteChannel openChannel(String file, String entryName) throws IOException {
		JarResource myJarFile = jarResources.get(file);
        if(myJarFile!=null) {
        	return myJarFile.getChannel(entryName);
        }
        return null;
	}
	
	@NativeAccess
	private static String resolveAlias(String file, String entryName, long[] properties) throws IOException {
		JarResource myJarFile = jarResources.get(file);
        if(myJarFile!=null) {
        	return myJarFile.resolveAlias(entryName, properties);
        }
        return null;
	}
}
