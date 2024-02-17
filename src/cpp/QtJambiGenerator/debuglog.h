/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#ifndef DEBUGLOG_H_
#define DEBUGLOG_H_

#define DEBUGLOG_PREAMBLE		0x0001
#define DEBUGLOG_DEFINE			0x0010
#define DEBUGLOG_UNDEF			0x0020
#define DEBUGLOG_DUMP_BEFORE		0x0100
#define DEBUGLOG_DUMP_MIDDLE		0x0200
#define DEBUGLOG_DUMP_AFTER		0x0400
#define DEBUGLOG_INCLUDE_FULL		0x1000
#define DEBUGLOG_INCLUDE_DIRECTIVE	0x2000
#define DEBUGLOG_INCLUDE_ERRORS		0x4000

#define DEBUGLOG_DEFAULTS		(DEBUGLOG_DUMP_MIDDLE|DEBUGLOG_INCLUDE_ERRORS)

#endif // DEBUGLOG_H_
