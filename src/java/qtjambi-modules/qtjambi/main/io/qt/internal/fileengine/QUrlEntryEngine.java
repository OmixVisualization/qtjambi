/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.internal.fileengine;

import java.io.IOException;
import java.io.InputStream;
import java.net.URLConnection;
import java.util.logging.Level;
import java.util.logging.Logger;

import io.qt.QtUtilities;
import io.qt.core.QByteArray;
import io.qt.core.QDateTime;
import io.qt.core.QIODevice;

@Deprecated
class QUrlEntryEngine extends io.qt.core.internal.QAbstractFileEngine implements QClassPathEntry{
	private final String classPathEntryFileName;
	private final URLConnection connection;
	private InputStream inputStream;
	
    public QUrlEntryEngine(URLConnection connection, String classPathEntryFileName) {
		super();
		this.connection = connection;
		this.classPathEntryFileName = classPathEntryFileName;
	}

    public String classPathEntryName() {
        return classPathEntryFileName;
    }
    
    @Override
	public boolean caseSensitive() {
		return true;
	}

	@Override
	public boolean close() {
		try {
			if(inputStream!=null){
				QtUtilities.getSignalOnDispose(this).disconnect(inputStream::close);
				inputStream.close();
			}
			return true;
		} catch (Exception e) {
			Logger.getAnonymousLogger().log(Level.SEVERE, "", e);
			return false;
		}
	}
	
	@Override
	public String fileName(FileName file) {
		switch(file){
		case DefaultName:
		case BaseName:
		case PathName:
		case AbsoluteName:
		case AbsolutePathName:
		case LinkName:
		case CanonicalName:
		case CanonicalPathName:
		case BundleName:
		case NFileNames:
			return connection.getURL().getFile();
		default:
			return connection.getURL().getFile();
		}
	}
	
	@Override
	public boolean open(QIODevice.OpenMode openMode) {
		if(openMode.isSet(QIODevice.OpenModeFlag.ReadOnly) && this.inputStream==null){
			try{
				this.inputStream = connection.getInputStream();
				QtUtilities.getSignalOnDispose(this).connect(inputStream::close);
				return true;
			} catch (Exception e) {
				Logger.getAnonymousLogger().log(Level.SEVERE, "", e);
			}
		}
		return false;
	}
	
	@Override
	public long size() {
		return connection.getContentLengthLong();
	}
	
	@Override
	public int read(byte[] data) {
		if(inputStream!=null){
			try {
				return inputStream.read(data);
			} catch (IOException e) {
				Logger.getAnonymousLogger().log(Level.SEVERE, "", e);
			}
		}
		return 0;
	}

	@Override
	public FileFlags fileFlags(FileFlags type) {
		return new FileFlags(FileFlag.ExistsFlag, FileFlag.FileType, FileFlag.ReadGroupPerm,
							 FileFlag.ReadOtherPerm, FileFlag.ReadOwnerPerm, FileFlag.ReadUserPerm);
	}

	@Override
	public QDateTime fileTime(FileTime time) {
		return super.fileTime(time);
	}

	@Override
	public int handle() {
		return super.handle();
	}

	@Override
	public QByteArray id() {
		return super.id();
	}
}
