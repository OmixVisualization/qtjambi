/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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
package io.qt;

/**
 * This exception is thrown when a certain value cannot be resolved as Qt enum.
 */
public class QNoSuchEnumValueException extends RuntimeException {
    private static final long serialVersionUID = 1L;

    public QNoSuchEnumValueException(String name) {
        super(name);
    }
    
    public QNoSuchEnumValueException(int value) {
        super(String.valueOf(value));
    }

	public QNoSuchEnumValueException(long value) {
        super(String.valueOf(value));
    }
	
	public QNoSuchEnumValueException(short value) {
        super(String.valueOf(value));
    }
	
	public QNoSuchEnumValueException(byte value) {
        super(String.valueOf(value));
    }

    public QNoSuchEnumValueException(int value, String name) {
    	super(String.format("%1$s(%2$s)", name, String.valueOf(value)));
    }

	public QNoSuchEnumValueException(long value, String name) {
		super(String.format("%1$s(%2$s)", name, String.valueOf(value)));
    }
	
	public QNoSuchEnumValueException(short value, String name) {
		super(String.format("%1$s(%2$s)", name, String.valueOf(value)));
    }
	
	public QNoSuchEnumValueException(byte value, String name) {
        super(String.format("%1$s(%2$s)", name, String.valueOf(value)));
    }

    public QNoSuchEnumValueException(int value, Throwable t) {
        super(String.valueOf(value), t);
    }

	public QNoSuchEnumValueException(long value, Throwable t) {
        super(String.valueOf(value), t);
    }
	
	public QNoSuchEnumValueException(short value, Throwable t) {
        super(String.valueOf(value), t);
    }
	
	public QNoSuchEnumValueException(byte value, Throwable t) {
        super(String.valueOf(value), t);
    }

    public QNoSuchEnumValueException(int value, String name, Throwable t) {
    	super(String.format("%1$s(%2$s)", name, String.valueOf(value)), t);
    }

	public QNoSuchEnumValueException(long value, String name, Throwable t) {
		super(String.format("%1$s(%2$s)", name, String.valueOf(value)), t);
    }
	
	public QNoSuchEnumValueException(short value, String name, Throwable t) {
		super(String.format("%1$s(%2$s)", name, String.valueOf(value)), t);
    }
	
	public QNoSuchEnumValueException(byte value, String name, Throwable t) {
        super(String.format("%1$s(%2$s)", name, String.valueOf(value)), t);
    }
}
