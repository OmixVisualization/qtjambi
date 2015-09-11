/****************************************************************************
**
** Copyright (C) 2013-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
** (in parts)
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef STACKELEMENT_H_
#define STACKELEMENT_H_

class CustomFunction;
class TemplateEntry;
class TemplateInstance;
class TypeEntry;

class StackElement {
    public:
        enum ElementType {
            None = 0x0,

            // Type tags (0x1, ... , 0xff)
            ObjectTypeEntry      = 0x1,
            ValueTypeEntry       = 0x2,
            InterfaceTypeEntry   = 0x3,
            NamespaceTypeEntry   = 0x4,
            ComplexTypeEntryMask = 0xf,

            // Non-complex type tags (0x10, 0x20, ... , 0xf0)
            PrimitiveTypeEntry   = 0x10,
            EnumTypeEntry        = 0x20,
            TypeEntryMask        = 0xff,

            // Simple tags (0x100, 0x200, ... , 0xf00)
            ExtraIncludes               = 0x100,
            Include                     = 0x200,
            ModifyFunction              = 0x300,
            ModifyField                 = 0x400,
            Root                        = 0x500,
            CustomMetaCreator           = 0x600,
            CustomMetaDeleter           = 0x700,
            CustomMetaConstructor       = 0x6000,
            CustomMetaDestructor        = 0x7000,
            ArgumentMap                 = 0x800,
            SuppressedWarning           = 0x900,
            Rejection                   = 0xa00,
            LoadTypesystem              = 0xb00,
            RejectEnumValue             = 0xc00,
            Template                    = 0xd00,
            TemplateInstanceEnum        = 0xe00,
            Replace                     = 0xf00,
            SimpleMask                  = 0xf00,

            // Code snip tags (0x1000, 0x2000, ... , 0xf000)
            InjectCode               = 0x00001000,
            InjectCodeInFunction     = 0x00002000,
            CodeSnipMask             = 0x0000f000,
            CustomTypeFlags          = 0x00003000,

            // Function modifier tags (0x010000, 0x020000, ... , 0xf00000)
            Access                   = 0x00010000,
            Removal                  = 0x00020000,
            Rename                   = 0x00040000,
            ModifyArgument           = 0x00080000,
            InstantiateTemplate      = 0x00100000,
            FunctionModifiers        = 0x00ff0000,

            // Argument modifier tags (0x01000000 ... 0xf0000000)
            ConversionRule           = 0x01000000,
            ReplaceType              = 0x02000000,
            ReplaceDefaultExpression = 0x04000000,
            RemoveArgument           = 0x08000000,
            DefineOwnership          = 0x10000000,
            RemoveDefaultExpression  = 0x20000000,
            NoNullPointers           = 0x40000000,
            ReferenceCount           = 0x80000000,
            ArgumentModifiers        = 0xff000000
        };

        StackElement(StackElement *p) : entry(0), type(None), parent(p) { }

        TypeEntry *entry;
        ElementType type;
        StackElement *parent;

        union {
            TemplateInstance *templateInstance;
            TemplateEntry *templateEntry;
            CustomFunction *customFunction;
        } value;
};

#endif
