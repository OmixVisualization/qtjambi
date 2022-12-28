/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#ifndef ENUMS_H
#define ENUMS_H

#include <QtCore/QtGlobal>

enum TheGlobalEnum {
    Zero,
    One,
    Two,
    Four = 4
};

enum TheGlobalShortEnum : short {
    Five,
    Six,
    Seven,
    Eight = 8
};

enum TheGlobalFlag {
    None = 0x0000,
    Flag1 = 0x0001,
    Flag2 = 0x0002,
    Flag3 = 0x0004
};

Q_DECLARE_FLAGS(TheGlobalFlags, TheGlobalFlag);

enum TheGlobalIntegerEnum {
    A = 0x0001,
    B = 0x0002,
    C = 0x0004,
    D = 0x0008
};

//#if defined(Q_COMPILER_CLASS_ENUM)
enum class TheGlobalEnumClass {
	Entry1,
	Entry2,
	Entry3,
	Entry4,
	Repeat = TheGlobalEnumClass::Entry2
};

enum struct TheGlobalCharEnumStruct : char {
	Entry1 = 'a',
	Entry2 = 'b',
	Entry3 = 'c',
	Entry4 = 'd'
};

enum class TheGlobalShortEnumClass : unsigned short {
	Entry1 = 567,
	Entry2 = 1357,
	Entry3 = 8723,
	Entry4 = 1111
};

enum struct TheGlobalLongEnumStruct : qlonglong {
	Entry1 = -5000000000000L,
	Entry2 = 8000000000000L,
	Entry3 = -12000000000000L,
	Entry4 = 89000000000000L
};

enum class TheGlobalLongIntegerEnumClass : qlonglong {
	Entry1 = -5000000000000L,
	Entry2 = 8000000000000L,
	Entry3 = -12000000000000L,
	Entry4 = 89000000000000L
};

enum class TheExtensibleGlobalLongEnumClass : unsigned long long {
	Entry1 = 1000000000000L,
	Entry2 = 2000000000000L,
	Entry3 = 3000000000000L,
	Entry4 = 4000000000000L,
	User = 9000000000000L
};
//#endif

namespace EnumNamespace{
	enum NSFlag {
		None = 0x0000,
		Flag1 = 0x0001,
		Flag2 = 0x0002,
		Flag3 = 0x0004,
		Flag4 = 0x0008
	};
	Q_DECLARE_FLAGS(NSFlags, NSFlag);

//#if defined(Q_COMPILER_CLASS_ENUM)
	enum class NSEnumClass : int{
		Entry1 = 5,
		Entry2 = 8,
		Entry3 = 12,
		Entry4 = 89,
		Repeat1 = Entry2,
		Repeat2 = NSEnumClass::Entry3,
		Repeat3 = EnumNamespace::NSEnumClass::Entry4
	};

	enum class NSClassFlag {
		None = 0x0000,
		Flag1 = 0x0001,
		Flag2 = 0x0002,
		Flag3 = 0x0004
	};
	Q_DECLARE_FLAGS(NSClassFlags, NSClassFlag);
//#endif
}

class GlobalEnumTest{
	public:
	enum ScopedEnum{
		E1,
		E2,
		E3
	};
	enum class ScopedEnumClass{
		E1,
		E2,
		E3
	};
	
	static TheGlobalEnum getGlobalEnum(){return Two;}
	static TheGlobalShortEnum getGlobalShortEnum(){return Seven;}
	static TheGlobalFlag getGlobalFlag(){return Flag3;}
	static TheGlobalFlags getGlobalFlags(){return TheGlobalFlags(Flag2|Flag3);}
	static TheGlobalIntegerEnum getGlobalIntegerEnum(){return B;}
	static EnumNamespace::NSFlag getNSFlag(){return EnumNamespace::Flag2;}
	static EnumNamespace::NSFlags getNSFlags(){return EnumNamespace::NSFlags(EnumNamespace::Flag2|EnumNamespace::Flag4);}
//#if defined(Q_COMPILER_CLASS_ENUM)
	static EnumNamespace::NSEnumClass getNSEnumClass(){return EnumNamespace::NSEnumClass::Entry4;}
	static EnumNamespace::NSClassFlag getNSClassFlag(){return EnumNamespace::NSClassFlag::Flag2;}
	static EnumNamespace::NSClassFlags getNSClassFlags(){return EnumNamespace::NSClassFlags((int)EnumNamespace::NSClassFlag::Flag2 | (int)EnumNamespace::NSClassFlag::Flag1 | (int)EnumNamespace::NSClassFlag::Flag3);}
	static TheGlobalEnumClass getGlobalEnumClass(){return TheGlobalEnumClass::Entry2;}
	static TheGlobalCharEnumStruct getGlobalCharEnumStruct(){return TheGlobalCharEnumStruct::Entry2;}
	static TheGlobalLongEnumStruct getGlobalLongEnumStruct(){return TheGlobalLongEnumStruct::Entry2;}
	static TheGlobalShortEnumClass getGlobalShortEnumClass(){return TheGlobalShortEnumClass::Entry3;}
	static TheExtensibleGlobalLongEnumClass getExtensibleGlobalLongEnumClass(){return TheExtensibleGlobalLongEnumClass::Entry3;}
	static TheGlobalLongIntegerEnumClass getGlobalLongIntegerEnumClass(){return TheGlobalLongIntegerEnumClass::Entry3;}
	static TheExtensibleGlobalLongEnumClass getExtendedExtensibleGlobalLongEnumClass(){return (TheExtensibleGlobalLongEnumClass)9900000000000L;}
//#endif
	static bool testGlobalEnum(TheGlobalEnum e = Two){return e == Two;}
	static bool testGlobalShortEnum(TheGlobalShortEnum e = Six){return e == Six;}
	static bool testGlobalFlag(TheGlobalFlag e = Flag3){return e == Flag3;}
	static bool testGlobalFlags(TheGlobalFlags e = TheGlobalFlags(Flag2|Flag3)){return e == TheGlobalFlags(Flag2|Flag3);}
	static bool testGlobalIntegerEnum(TheGlobalIntegerEnum e = B){return e == B;}
	static bool testGlobalLongIntegerEnumClass(TheGlobalLongIntegerEnumClass e = TheGlobalLongIntegerEnumClass::Entry3){return e == TheGlobalLongIntegerEnumClass::Entry3;}
	static bool testExtensibleGlobalLongEnumClass(TheExtensibleGlobalLongEnumClass e = TheExtensibleGlobalLongEnumClass::Entry3){return e == TheExtensibleGlobalLongEnumClass::Entry3;}
	static bool testNSFlag(EnumNamespace::NSFlag e = EnumNamespace::Flag1){return e == EnumNamespace::Flag1;}
	static bool testNSFlags(EnumNamespace::NSFlags e = EnumNamespace::NSFlags(EnumNamespace::Flag1|EnumNamespace::Flag3)){return e == EnumNamespace::NSFlags(EnumNamespace::Flag1|EnumNamespace::Flag3);}
//#if defined(Q_COMPILER_CLASS_ENUM)
	static bool testNSEnumClass(EnumNamespace::NSEnumClass e = EnumNamespace::NSEnumClass::Entry2){return e == EnumNamespace::NSEnumClass::Entry2;}
	static bool testNSClassFlag(EnumNamespace::NSClassFlag e = EnumNamespace::NSClassFlag::Flag1){return e == EnumNamespace::NSClassFlag::Flag1;}
	static bool testNSClassFlags(EnumNamespace::NSClassFlags e = EnumNamespace::NSClassFlags((int)EnumNamespace::NSClassFlag::Flag1 | (int)EnumNamespace::NSClassFlag::Flag2)){return e == EnumNamespace::NSClassFlags( (int)EnumNamespace::NSClassFlag::Flag1 | (int)EnumNamespace::NSClassFlag::Flag2);}
	static bool testGlobalEnumClass(TheGlobalEnumClass e = TheGlobalEnumClass::Entry2){return e == TheGlobalEnumClass::Entry2;}
	static bool testGlobalCharEnumStruct(TheGlobalCharEnumStruct e = TheGlobalCharEnumStruct::Entry2){return e == TheGlobalCharEnumStruct::Entry2;}
	static bool testGlobalLongEnumStruct(TheGlobalLongEnumStruct e = TheGlobalLongEnumStruct::Entry2){return e == TheGlobalLongEnumStruct::Entry2;}
	static bool testGlobalShortEnumClass(TheGlobalShortEnumClass e = TheGlobalShortEnumClass::Entry4){return e == TheGlobalShortEnumClass::Entry4;}
	static bool testScopedEnumClass(ScopedEnumClass e = ScopedEnumClass::E3){return e == ScopedEnumClass::E3;}
	static bool testScopedEnum(ScopedEnum e = ScopedEnum::E3){return e == ScopedEnum::E3;}
//#endif
};

#endif
