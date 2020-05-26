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
        enum ElementType : unsigned long long {
            None = 0x0,

            // Type tags (0x1, ... , 0xff)
            ObjectTypeEntry      = 0x01,
            ValueTypeEntry       = 0x02,
            InterfaceTypeEntry   = 0x03,
            NamespaceTypeEntry   = 0x04,
            IteratorTypeEntry    = 0x08,
            FunctionalTypeEntry  = 0x040,
            TemplateTypeEntry    = 0x080,
            ComplexTypeEntryMask = 0x0cf,

            // Non-complex type tags (0x10, 0x20, ... , 0xf0)
            PrimitiveTypeEntry   = 0x010,
            EnumTypeEntry        = 0x020,
            TypeEntryMask        = 0x0ff,

            // Simple tags (0x100, 0x200, ... , 0xf00)
            ExtraIncludes               = 0x0100,
            Include                     = 0x0200,
            ModifyFunction              = 0x0300,
            ModifyField                 = 0x0400,
            Root                        = 0x0500,
            CustomMetaConstructor       = 0x0600,
            CustomMetaDestructor        = 0x0700,
            ArgumentMap                 = 0x0800,
            SuppressedWarning           = 0x0900,
            Rejection                   = 0x0a00,
            LoadTypesystem              = 0x0b00,
            RejectEnumValue             = 0x0c00,
            Template                    = 0x0d00,
            TemplateInstanceEnum        = 0x0e00,
            Replace                     = 0x0f00,
            RenameEnumValue             = 0x1000,
            SimpleMask                  = 0xff00,

            // Code snip tags (0x0100000, 0x0200000, ... , 0xf00000)
            InjectCode               = 0x00000100000,
            InjectCodeInFunction     = 0x00000200000,
            CodeSnipMask             = 0x00000f00000,
            CustomTypeFlags          = 0x00000300000,
            InterfacePolymorphicId   = 0x00000400000,

            // Function modifier tags (0x001000000, 0x02000000, ... , 0xf0000000)
            Access                   = 0x00001000000,
            Removal                  = 0x00002000000,
            Rename                   = 0x00004000000,
            ModifyArgument           = 0x00008000000,
            InstantiateTemplate      = 0x00010000000,
            FunctionModifiers        = 0x000ff000000,

            // Argument modifier tags (0x00100000000 ... 0xf000000000)
            ConversionRule           = 0x00100000000,
            ReplaceType              = 0x00200000000,
            ReplaceDefaultExpression = 0x00400000000,
            RemoveArgument           = 0x00800000000,
            DefineOwnership          = 0x01000000000,
            RemoveDefaultExpression  = 0x02000000000,
            NoNullPointers           = 0x04000000000,
            ReferenceCount           = 0x08000000000,
            ArrayType                = 0x10000000000,
            BufferType               = 0x20000000000,
            ImportTemplate           = 0x40000000000,
            AddArgument              = 0x80000000000,
            ArgumentModifiers        = 0xfff00000000
        };

        StackElement(StackElement *p) : entry(nullptr), type(None), parent(p) { }

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
