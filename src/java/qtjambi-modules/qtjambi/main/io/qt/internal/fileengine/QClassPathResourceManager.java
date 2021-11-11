/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
package io.qt.internal.fileengine;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Enumeration;
import java.util.List;
import java.util.zip.ZipException;

import io.qt.core.QDir;

public final class QClassPathResourceManager {
	private QClassPathResourceManager() {
	}

    private static class URLAlias {
        URLAlias(URL url, File file) {
			super();
			this.url = url;
			this.file = file;
		}
		final URL url;
		final File file;
    }

    final static String FileNameDelim = "#";
    final static String FileNameIndicator = "classpath";
    final static String FileNamePrefix1 = FileNameIndicator + ":";
    final static String FileNamePrefix2 = "/:" + FileNameIndicator + ":";
	private static JarCache cache;
    private static String currentDirectory;

	public static void addSearchPath(String path) {
		initialize();
		URL url = makeUrl(path);
		if (url != null) {
			cache.addPath(url);
		}
	}

	public static void removeSearchPath(String path) {
		initialize();
		URL url = makeUrl(path);
		if (url != null) {
			cache.removePath(url);
		}
	}

	static synchronized void initialize() {
		if (cache == null) {
			cache = new JarCache();

			List<URL> cpUrls = new ArrayList<URL>();

			try {
				// FIXME: QtJambi should not mix and match the method of obtaining the current
				// ClassLoader
				// all use this class or all use Thread.
				ClassLoader loader = Thread.currentThread().getContextClassLoader();
				if (loader == null)
					loader = QClassPathResourceManager.class.getClassLoader();

				Enumeration<URL> urls = loader.getResources("META-INF/MANIFEST.MF");
				while (urls.hasMoreElements()) {
					URL url = urls.nextElement();
					if ("jar".equals(url.getProtocol())) {
						try {
							String f = url.getPath();
							int bang = f.indexOf("!");
							if (bang >= 0)
								f = f.substring(0, bang);

							if (f.trim().length() > 0) {
								URL fileUrl = new URL(f);
								cache.addPath(fileUrl);
								cpUrls.add(fileUrl);
							}
						} catch (Exception e) {
							java.util.logging.Logger.getLogger("io.qt.internal.fileengine")
									.log(java.util.logging.Level.SEVERE, "", e);
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
			int k = 0;
			for (String p : paths) {
				if (p.trim().length() > 0) {
					k++; // count all paths, invalid and valid

					URL url = makeUrl(p);
					boolean match = false;

					if(url!=null){
						cache.addPath(url);
					}else{
						continue;
					}

					MyJarFile myJarFile2 = null;
					MyJarFile myJarFile1 = null;
					try {
						myJarFile2 = resolveUrlToMyJarFile(url);
						if (myJarFile2 != null) {
							for (URL otherURL : cpUrls) {
								myJarFile1 = resolveUrlToMyJarFile(otherURL);
								if (myJarFile1 != null) {
									File file1 = new File(myJarFile1.getName());
									File file2 = new File(myJarFile2.getName());
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
						if (myJarFile2 != null) {
							myJarFile2.put();
							myJarFile2 = null;
						}
						if (myJarFile1 != null) {
							myJarFile1.put();
							myJarFile1 = null;
						}
					}

					if (!match)
						cache.addPath(url);
				}
			}

			// If there are no paths set in java.class.path, we do what Java does and
			// add the current directory; at least ask Java what the current directory
			// is and not Qt.
			if (k == 0) {
				// FIXME: Use JVM cwd notion
				try {
					cache.addPath(new URL("file:" + QDir.currentPath()));
				} catch (MalformedURLException e) {
					java.util.logging.Logger.getLogger("io.qt.internal.fileengine")
					.log(java.util.logging.Level.SEVERE, "", e);
				}
			}
		}
	}

	static URL makeUrl(String path) {
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
							// FIXME: ../../foo/bar
							// Special case for current directory
							String tmpPath = resolveCurrentDirectory();
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
							String tmpPath = resolveCurrentDirectory();
							if (tmpPath != null) {
								// FIXME: This is better resolved later via path canonicalization (to fix the
								// ./././foo case)
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
						// If skipTryAsis==true then we found an absolute path (or converted
						// what is there already to an absolute path)
						boolean prefix = true;
						// Eek... temporary hack, some users of this method makeUrl() seem to be
						// already passing a valid URL as input. This is one point to fix in a
						// later review. For now be blacklist known prefixes from being treated
						// as relative paths. FIXME
						if (path.startsWith("file:") || path.startsWith("jar:") || path.startsWith("http:")
								|| path.startsWith("https:"))
							prefix = false;
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
							String tmpPath = resolveCurrentDirectory();
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
						// Validate the URL we build is well-formed
						// FIXME: file://
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
	
			InputStream inStream = null;
			try {
				URLAlias urlAlias = checkNeedWorkaround(result);
				if (urlAlias.file != null) { // Due to workaround
					if (urlAlias.file.isFile()) // skip dirs
						inStream = new FileInputStream(urlAlias.file);
				} else if(!urlAlias.url.toString().endsWith("/")){
					URLConnection urlConn = urlAlias.url.openConnection();
					inStream = urlConn.getInputStream();
				}
			} catch (Exception e) {
				java.util.logging.Logger.getLogger("io.qt.internal.fileengine").log(java.util.logging.Level.SEVERE, ""+result, e);
			} finally {
				if (inStream != null) {
					try {
						inStream.close();
					} catch (IOException eat) {
					}
				}
			}
		}
		return result;
	}
	
	static Collection<String> pathToJarFiles(String entry) {
		initialize();
		return cache.pathToJarFiles(entry);
	}
	
	static Collection<String> classPathDirs(){
		initialize();
		return cache.classPathDirs();
	}

    static MyJarFile resolveUrlToMyJarFile(URL url) throws IOException, ZipException {
        MyJarFile myJarFile = null;
        URLAlias urlAlias = checkNeedWorkaround(url);
        try {
            if(urlAlias.file != null) {  // Due to workaround
                if(urlAlias.file.isFile()) // skip dirs
                    myJarFile = new MyJarFile(urlAlias.file);
            } else {
                 myJarFile = new MyJarFile(urlAlias.url);
            }
        } catch(ZipException e) {
            // This often fails with "java.util.zip.ZipException: error in opening zip file" but never discloses the filename
            throw new ZipException(e.getMessage() + ": " + url);
        }
        return myJarFile;
    }
    


    static URLAlias checkNeedWorkaround(URL url) {
        final String SEPARATOR = "!/";
        File file = null;
        // Sun/Oracle bugid#7050028  "IllegalStateException: zip file closed" from JarURLConnection
        // Workarounds 1) Do not turn off URLConnection caches.
        //             2) Use file:/x.jar URLs rather than jar:file:x.jar!/ URLs for URLClassLoader
        if("jar".equals(url.getProtocol())) {
            // Replace "^jar:file:" with "file:" only if it also ends with "!/"
            final String PREFIX_JAR_FILE = "jar:file:";
            String externalForm = url.toExternalForm();
            if(externalForm.startsWith(PREFIX_JAR_FILE)) {
                // Don't use URL#toExternalForm() as this results in conversions in path to URL encoded
                //  such as s/ /%20/ this will break java.io.File API usage on the resulting path.
                String path = url.getPath();  // this has "file:///" prefix already and "!/" suffix
                int index = externalForm.indexOf(SEPARATOR);
                int externalFormLength = externalForm.length();
                // check the first "!/" is also the last and at the end of the string
                if(index == externalFormLength - 2) {
                    // Ok we implement the workaround (start at 4 as we need to keep the "file:"
                    //externalForm = externalForm.substring(4, externalFormLength - SEPARATOR.length());
                    // this approach caused %20 to appear which broke things later on.
                    String urlString = stripUrlFileColonPrefix(path, path);
                    urlString = stripSuffix(urlString, SEPARATOR, urlString);
                    file = new File(urlString);
                    try {
                        url = new URL(path);
                    } catch(MalformedURLException eat) {
                        url = null;
                    }
                }
            }
        } else if("file".equals(url.getProtocol())) {
            String path = url.getPath();
            String urlString = stripUrlFileColonPrefix(path, path);
            urlString = stripSuffix(urlString, SEPARATOR, urlString);
            file = new File(urlString);
        }
        return new URLAlias(url, file);
    }

    private static synchronized String resolveCurrentDirectory() {
        String tmpCurrentDirectory = currentDirectory;
        if(tmpCurrentDirectory != null)
            return tmpCurrentDirectory;

        tmpCurrentDirectory = currentDirectory;
        // retest
        if(tmpCurrentDirectory != null)
            return tmpCurrentDirectory;

        // FIXME: Maybe user.dir should be priority here ?

        File fileCurDir = new File(".");
        if(fileCurDir.isDirectory()) {
            // getParentFile() does not do the trick for us (it will/can be null in this circumstance)
            //  this method ensures it does not end with "/." or "\\." which is unwanted
            tmpCurrentDirectory = fileCurDir.getAbsolutePath();
            String removeDelimAndDot = File.separator + ".";  // "/." on unix, "\\." on windows
            if(tmpCurrentDirectory.endsWith(removeDelimAndDot))
                tmpCurrentDirectory = tmpCurrentDirectory.substring(0, tmpCurrentDirectory.length() - removeDelimAndDot.length());
        } else {
            tmpCurrentDirectory = System.getProperty("user.dir");
        }

        currentDirectory = tmpCurrentDirectory;
        return tmpCurrentDirectory;
    }

    static synchronized void setCurrentDirectory(String newCurrentDirectory) {
        currentDirectory = newCurrentDirectory;
    }

    private static String stripSuffix(String s, String suffix, String defaultValue) {
        if(s.endsWith(suffix)) {
            int endIndex = s.length() - suffix.length();
            return s.substring(0, endIndex);
        }
        return defaultValue;
    }

    private static String stripUrlFileColonPrefix(String urlString, String defaultValue) {
        final int len = urlString.length();
        final String PREFIX_FILE = "file:";
        if(urlString.startsWith(PREFIX_FILE)) {  // efficiency?  f**K that for now
            int skipCount = PREFIX_FILE.length();
            while(len > skipCount) {  // remove multiple "/" prefixes
                char c = urlString.charAt(skipCount);
                if(c != '/') {
                    if(skipCount > 0)
                        skipCount--;  // put at least one back
                    break;
                }
                skipCount++;
            }
            if(skipCount > 0)
                urlString = urlString.substring(skipCount);
            return urlString;
        }
        return defaultValue;
    }
}
