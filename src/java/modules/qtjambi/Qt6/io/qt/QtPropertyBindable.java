/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

import io.qt.core.*;

/**
 * <p>QtPropertyBindable annotates a method as being a <code>QBindable</code> getter for
 * a <code>QProperty</code>-based property.</p>
 * <p>You don't need to use <code>@QtPropertyBindable</code> annotation if the method meets
 * following signature and naming convention:</p>
 * <ol>
 * <li>returns <code>QUntypedBindable</code> or a subtype of <code>QUntypedBindable</code></li>
 * <li>no parameters</li>
 * <li>method name is "<code>bindableProp</code>" for an available property "<code>prop</code>"
 * whereas "<code>prop</code>" can be any name</li>
 * </ol>
 * @see QUntypedBindable
 * @see QMetaProperty#isBindable()
 * @see QMetaProperty#bindable(QObject)
 */
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.METHOD})
public @interface QtPropertyBindable {
    /** Returns true if the property is enabled; otherwise, false. */
    boolean enabled() default true;
    /** Returns the name of the property. */
    String name() default "";
}
