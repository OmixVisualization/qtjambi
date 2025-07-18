/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * <p>This annotation marks a parameter to not accept <code>null</code>.</p>
 * <p>A Qt method with <code>StrictNonNull</code>-annotated return type never returns <code>null</code>.</p>
 * <p>When giving <code>null</code> to a <code>StrictNonNull</code>-annotated method argument a <code>NullPointerException</code> is thrown.</p>
 */
@SuppressWarnings("deprecation")
@Retention(RetentionPolicy.CLASS)
@Target(ElementType.TYPE_USE)
@javax.annotation.Nonnull(when=javax.annotation.meta.When.ALWAYS)
@javax.annotation.meta.TypeQualifierNickname
@kotlin.annotations.jvm.UnderMigration(status = kotlin.annotations.jvm.MigrationStatus.STRICT)
public @interface StrictNonNull {
}
