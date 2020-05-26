/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef FLAGSANDENUMTEST_H
#define FLAGSANDENUMTEST_H

#include <QtCore/QtCore>

class HiddenObject : public QObject
{
    Q_OBJECT
public:
    HiddenObject(QObject *parent = nullptr);
    ~HiddenObject();

    enum HiddenEnum{
        None = 0, Entry1 = 1, Entry2 = 2, Entry3 = 4, Entry4 = 8
    };
    Q_DECLARE_FLAGS(HiddenFlags, HiddenEnum)
    Q_FLAG(HiddenFlags)

    enum class HiddenEnumClass{
        None = 0, Entry1 = 1, Entry2 = 2, Entry3 = 4, Entry4 = 8
    };
    Q_ENUM(HiddenEnumClass)

    Q_PROPERTY(HiddenFlags hiddenFlags READ hiddenFlags WRITE setHiddenFlags NOTIFY hiddenFlagsChanged)
    Q_PROPERTY(HiddenEnumClass hiddenEnum READ hiddenEnum WRITE setHiddenEnum NOTIFY hiddenEnumChanged)

    Q_INVOKABLE HiddenObject::HiddenEnumClass hiddenEnum() const;
    Q_INVOKABLE void setHiddenEnum(HiddenObject::HiddenEnumClass hiddenEnumClass);
    Q_INVOKABLE HiddenObject::HiddenFlags hiddenFlags() const;
    Q_INVOKABLE void setHiddenFlags(HiddenObject::HiddenFlags hiddenFlags);

Q_SIGNALS:
    void hiddenEnumChanged(HiddenObject::HiddenEnumClass hiddenEnumClass);
    void hiddenFlagsChanged(HiddenObject::HiddenFlags hiddenFlags);
private:
    HiddenFlags m_hiddenFlags;
    HiddenEnumClass m_hiddenEnumClass;
private:
    Q_DISABLE_COPY(HiddenObject)
};

Q_DECLARE_METATYPE(HiddenObject*)
Q_DECLARE_METATYPE(HiddenObject::HiddenFlags)
Q_DECLARE_METATYPE(HiddenObject::HiddenEnumClass)
Q_DECLARE_OPERATORS_FOR_FLAGS(HiddenObject::HiddenFlags)

class FlagsAndEnumTest : public QObject
{
    Q_OBJECT
public:
    explicit FlagsAndEnumTest(QObject *parent = nullptr);

    virtual void processResult(int result);
	
	virtual void setAttributes(std::initializer_list<Qt::WidgetAttribute> attributes);
    virtual void setInts(std::initializer_list<int> iList);
	
	const QList<Qt::WidgetAttribute>& getAttributes();
    const QList<int>& getInts();

    static QObject* createHiddenObject(QObject* parent);
    
public slots:
    void testSlot1(Qt::AlignmentFlag o);
    void testSlot2(Qt::Orientation o);
    void testSlot3(Qt::Alignment o);
    void testSlot4(std::initializer_list<Qt::WidgetAttribute> o);
    void testSlot5(std::initializer_list<int> o);

signals:
    void testSignal1(Qt::AlignmentFlag);
    void testSignal2(Qt::Orientation);
    void testSignal3(Qt::Alignment);
	void testSignal4(std::initializer_list<Qt::WidgetAttribute>);
    void testSignal5(std::initializer_list<int>);
	
private:
	QList<Qt::WidgetAttribute> m_attributes;
    QList<int> m_ints;
};

#endif // FLAGSANDENUMTEST_H
