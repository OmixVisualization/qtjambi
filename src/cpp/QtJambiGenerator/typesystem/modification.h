/****************************************************************************
**
** This file is part of QtJambi.
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

#ifndef MODIFICATION_H_
#define MODIFICATION_H_

#include <QHash>
#include "utils.h"
#include "codesnip.h"

namespace TS {

enum class AsArrayType{
    No = 0,
    Yes = 0x001,
    Deref = 0x002,
    VarArgs = 0x004,
    NoOffset = 0x008,
    AddPlainDelegate = 0x010
};
enum class AsBufferType{
    No = 0,
    Yes = 0x001,
    Deref = 0x002,
};
typedef QFlags<AsArrayType> AsArrayTypes;
typedef QFlags<AsBufferType> AsBufferTypes;

enum class ThreadAffinity : uint{
    None =   0x00000000,
    Yes =    0x00000100,
    UI =     0x00000200,
    Pixmap = 0x00000400
};

struct ArgumentModification {
    enum Type{
        Default,
        AdditionalArgument,
        TypeParameter,
    };

    ArgumentModification(Type _type = Type::AdditionalArgument) :
            removed_default_expression(false),
            removed(false),
            no_null_pointers(false),
            reset_after_use(false),
            value_as_pointer(false),
            no_implicit_calls(false),
            type(_type),
            thread_affine(ThreadAffinity::None),
            index(std::numeric_limits<int>::max()),
            useAsArrayType(AsArrayType::No),
            arrayLengthParameter(-1),
            minArrayLength(-1),
            maxArrayLength(-1)
    {}
    ArgumentModification(int idx, Type _type = Type::Default) :
            removed_default_expression(false),
            removed(false),
            no_null_pointers(false),
            reset_after_use(false),
            value_as_pointer(false),
            no_implicit_calls(false),
            type(_type),
            thread_affine(ThreadAffinity::None),
            index(idx),
            useAsArrayType(AsArrayType::No),
            arrayLengthParameter(-1),
            minArrayLength(-1),
            maxArrayLength(-1)
    {}

    //! Should the default expression be removed?
    uint removed_default_expression : 1;
    uint removed : 1;
    uint no_null_pointers : 1;
    uint reset_after_use : 1;
    uint value_as_pointer : 1;
    uint no_implicit_calls : 1;
    Type type;
    ThreadAffinity thread_affine;

    //! The index of this argument
    int index;

    //! Reference count flags for this argument
    QList<ReferenceCount> referenceCounts;

    //! The text given for the new type of the argument
    QString modified_type;
    QString modified_jni_type;
    QString modified_name;

    QString replace_value;

    //! The code to be used to construct a return value when no_null_pointers is true and
    //! the returned value is null. If no_null_pointers is true and this string is
    //! empty, then the base class implementation will be used (or a default construction
    //! if there is no implementation)
    QString null_pointer_default_value;

    //! The text of the new default expression of the argument
    QString replaced_default_expression;

    //! The new definition of ownership for a specific argument
    QHash<TS::Language, OwnershipRule> ownerships;

    //! Different conversion rules
    CodeSnipList conversion_rules;

    AsArrayTypes useAsArrayType;
    AsBufferTypes useAsBufferType;
    int arrayLengthParameter;
    int minArrayLength;
    int maxArrayLength;
    QString arrayLengthExpression;
    QStringList implicitCalls;
    QStringList inhibitedImplicitCalls;
};

struct Modification {
    enum Modifiers {
        Private =               0x00000001,
        Protected =             0x00000002,
        Public =                0x00000004,
        Friendly =              0x00000008,
        AccessModifierMask =    0x0000000f,

        Final =                 0x00000010,
        NonFinal =              0x00000020,
        NativeDeclFinal =       0x00000040,
        FinalMask =             0x000000f0,

        ThreadAffine =          uint(ThreadAffinity::Yes),
        UIThreadAffine =        uint(ThreadAffinity::UI),
        PixmapThreadAffine =    uint(ThreadAffinity::Pixmap),
        AffinityMask =          0x00000f00,

        Readable =              0x00001000,
        Writable =              0x00002000,

        CodeInjection =         0x00004000,
        Rename =                0x00008000,
        Deprecated =            0x00010000,
        ReplaceExpression =     0x00020000,
        VirtualSlot =           0x00040000 | NonFinal,
        AllowAsSlot =           0x00080000,
        PrivateSignal =         0x00100000,
        NoExcept =              0x00200000,
        BlockExcept =           0x00400000,
        RethrowExcept =         0x00800000,
        IsPaintMethod =         0x01000000,
        NoKotlinGetter =        0x02000000,
        ForcedExplicit =        0x04000000,
        ForcedImplicit =        0x08000000,
        NoImplicitArguments =   0x10000000
    };

    Modification() : modifiers(0) { }
    Modification(const Modification&) = default;

    bool isAccessModifier() const { return modifiers & AccessModifierMask; }
    Modifiers accessModifier() const { return Modifiers(modifiers & AccessModifierMask); }
    bool isPrivate() const { return accessModifier() == Private; }
    bool isProtected() const { return accessModifier() == Protected; }
    bool isPublic() const { return accessModifier() == Public; }
    bool isFriendly() const { return accessModifier() == Friendly; }
    bool isFinal() const { return modifiers & Final; }
    bool isDeclaredFinal() const { return modifiers & NativeDeclFinal; }
    bool isNonFinal() const { return modifiers & NonFinal && !(modifiers & NativeDeclFinal); }
    bool isVirtualSlot() const { return (modifiers & VirtualSlot) == VirtualSlot; }
    bool isThreadAffine() const { return (modifiers & ThreadAffine) == ThreadAffine; }
    bool isUIThreadAffine() const { return (modifiers & UIThreadAffine) == UIThreadAffine; }
    bool isPixmapThreadAffine() const { return (modifiers & PixmapThreadAffine) == PixmapThreadAffine; }
    bool isAllowedAsSlot() const { return (modifiers & AllowAsSlot) == AllowAsSlot; }
    bool isPaintMethod() const { return (modifiers & IsPaintMethod) == IsPaintMethod; }
    bool isPrivateSignal() const { return (modifiers & PrivateSignal) == PrivateSignal; }
    bool isForcedExplicit() const { return (modifiers & ForcedExplicit) == ForcedExplicit; }
    bool isForcedImplicit() const { return (modifiers & ForcedImplicit) == ForcedImplicit; }
    bool isNoImplicitArguments() const { return (modifiers & NoImplicitArguments) == NoImplicitArguments; }
    QString accessModifierString() const;

    bool isDeprecated() const { return modifiers & Deprecated; }
    bool isNoExcept() const { return modifiers & NoExcept; }
    bool isBlockExceptions() const { return modifiers & BlockExcept; }
    bool isRethrowExceptions() const { return modifiers & RethrowExcept; }
    bool isNoKotlinGetter() const { return modifiers & NoKotlinGetter; }

    void setRenamedTo(const QString &name) { renamedToName = name; }
    QString renamedTo() const { return renamedToName; }
    bool isRenameModifier() const { return modifiers & Rename; }

    uint modifiers;
    QString renamedToName;
};

struct Delegate{
    QString name;
    bool isDeprecated;
    bool isSelfReturning;
    uint modifiers;
    CodeSnipList snips;

    Modification::Modifiers accessModifier() const { return Modification::Modifiers(modifiers & Modification::AccessModifierMask); }
    bool isPrivate() const { return accessModifier() == Modification::Private; }
    bool isProtected() const { return accessModifier() == Modification::Protected; }
    bool isPublic() const { return accessModifier() == Modification::Public; }
    bool isFriendly() const { return accessModifier() == Modification::Friendly; }
    bool isFinal() const { return modifiers & Modification::Final; }
    bool isNonFinal() const { return modifiers & Modification::NonFinal && !(modifiers & Modification::NativeDeclFinal); }
};

struct AbstractFunctionModification: public Modification {
    AbstractFunctionModification() = default;
    AbstractFunctionModification(const AbstractFunctionModification&) = default;
    bool isCodeInjection() const { return modifiers & CodeInjection; }
    QString ppCondition;
    QString throws;
    QString association;
    CodeSnipList snips;
    QString targetType;
    QString proxyCall;
    QList<ArgumentModification> argument_mods;
    QList<Delegate> delegates;
};

struct Parameter{
    QString type;
    QString name;
    QString extends;
    bool implicit;
};

struct TemplateInstantiation: public AbstractFunctionModification {
    TemplateInstantiation() = default;
    TemplateInstantiation(const TemplateInstantiation&) = default;
    QList<Parameter> arguments;
};

typedef QList<TemplateInstantiation> TemplateInstantiationList;

struct FunctionModification: public AbstractFunctionModification {
    FunctionModification() : AbstractFunctionModification(), removal(TS::NoLanguage) { }
    FunctionModification(const FunctionModification&) = default;
    bool isRemoveModifier() const { return removal != TS::NoLanguage; }
    QString toString() const;
    TS::Language removal;
    QString signature;
    QList<TemplateInstantiation> template_instantiations;
};
typedef QList<FunctionModification> FunctionModificationList;

struct FieldModification: public Modification {
    FieldModification()
        : name(),
          modified_type(),
          modified_jni_type(),
          ownerships(),
          referenceCounts(),
          conversion_rules(),
          no_null_pointers(false)
    {}
    bool isReadable() const { return modifiers & Readable; }
    bool isWritable() const { return modifiers & Writable; }

    QString name;
    //! The text given for the new type of the argument
    QString modified_type;
    QString modified_jni_type;

    //! The new definition of ownership for a specific argument
    QHash<TS::Language, OwnershipRule> ownerships;

    //! Reference count flags for this argument
    QList<ReferenceCount> referenceCounts;

    //! Different conversion rules
    CodeSnipList conversion_rules;
    uint no_null_pointers : 1;
};
typedef QList<FieldModification> FieldModificationList;

}

#endif
