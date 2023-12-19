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

#ifndef INJECTEDCODE_H
#define INJECTEDCODE_H

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#include <QtCore/QtCore>
//#include <QtGui/private/qaccessible2_p.h>
#include <QtGui/QtGui>
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif
#ifndef QTJAMBI_NO_SQL
#include <QtSql/QtSql>
#endif
#ifndef QTJAMBI_NO_XML
#include <QtXml/QtXml>
#endif
#ifndef QTJAMBI_NO_NETWORK
#include <QtNetwork/QtNetwork>
#endif
#include <QtJambi/QtJambiAPI>
#include <QtJambi/javaapi.h>

class SpinBoxHandler
{
public:
    void tryValidate(QAbstractSpinBox *spinBox, const QString &input, int i)
    {
        my_returned_string = input;
        my_returned_pos = i;
        my_returned_state = spinBox->validate(my_returned_string, my_returned_pos);
    }

    void tryFixup(QAbstractSpinBox *spinBox, const QString &input)
    {
        my_returned_string = input;
        spinBox->fixup(my_returned_string);
    }

public:
    QString my_returned_string;
    int my_returned_pos;
    QValidator::State my_returned_state;
};

class SpinBoxSubclass: public QAbstractSpinBox
{
public:
    virtual QValidator::State validate(QString &input, int &pos) const
    {
        this->my_received_pos = pos;
        this->my_received_string = input;
        pos += 13;
        input = "The silence of that " + input.left(5) + "less sleep";

        return QValidator::Intermediate;
    }

    virtual void fixup(QString &input) const
    {
        this->my_received_string = input;
        input = "And " + input.left(4) + " art dead";
    }

public:
    mutable QString my_received_string;
    mutable int my_received_pos;
};

#ifndef QTJAMBI_NO_WIDGETS
class GraphicsSceneSubclass: public QGraphicsScene
{
public:
    void drawItems(QPainter *painter, int numItems, QGraphicsItem *items[], const QStyleOptionGraphicsItem options[], QWidget *widget);

     int numItems;
     QStyleOptionGraphicsItem firstStyleOption;
     QStyleOptionGraphicsItem secondStyleOption;
     QGraphicsItem *firstItem;
     QGraphicsItem *secondItem;
     int firstStyleOptionType;
     int secondStyleOptionType;
     int firstStyleOptionVersion;
     int secondStyleOptionVersion;
     QRectF firstBoundingRect;
     QRectF secondBoundingRect;
};
#endif

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
class TextCodecSubclass: public QTextCodec
{


public:
    QString callToUnicode(const QByteArray &ba, ConverterState *state) {
        return convertToUnicode(ba.data(), ba.size(), state);
    }

    QByteArray callFromUnicode(const QString &str, ConverterState *state) {
        return convertFromUnicode(str.data(), str.length(), state);
    }

    mutable ConverterState *receivedState;

protected:
    TextCodecSubclass() : QTextCodec() {}
    QString convertToUnicode(const char *data, int size, ConverterState *state) const {
        receivedState = state;

        QString returned;
        for (int i=0; i<size; ++i)
            returned += (data[i]-3);

        return returned;
    }

    QByteArray convertFromUnicode(const QChar *data, int size, ConverterState *state) const {
        receivedState = state;

        QByteArray returned;
        for (int i=0; i<size; ++i)
            returned += QChar(data[i].unicode()+3);

        return returned;
    }
};
#endif

class IODeviceSubclass: public QIODevice
{
public:
    ~IODeviceSubclass() {
        if(buffer) {
            delete[] buffer;
            buffer = nullptr;
        }
    }

    IODeviceSubclass(int buffer_length) {
        buffer = new char[buffer_length];
        this->buffer_length = buffer_length;
    }

    qint64 callReadData() {
        return readData(buffer, buffer_length);
    }

    qint64 callWriteData(const QByteArray &data) {
        return writeData(data.data(), data.size());
    }

    qint64 callReadLineData() {
        return readLineData(buffer, buffer_length);
    }

protected:
    qint64 readData(char *data, qint64 maxSize) {
        Q_UNUSED(maxSize);
        const char *inp = "I am a boy";
        const size_t len = strlen(inp);
        for (size_t i=0; i<len; ++i)
            data[i] = inp[i];

        return len;
    }

    qint64 writeData(const char *data, qint64 maxSize) {
        delete[] buffer;
        buffer = new char[maxSize];
        for (qint64 i=0; i<maxSize; ++i)
            buffer[i] = data[i];

        return maxSize;
    }

    qint64 readLineData(char *data, qint64 maxSize) {
        Q_UNUSED(maxSize)
        const char *inp = "Confucius say: Don't go outside with wet hair";
        const size_t len = strlen(inp);
        for (size_t i=0; i<len; ++i)
            data[i] = inp[i];

        return len;
    }

public:
    char *buffer;
    int buffer_length;

};

class PictureSubclass: public QPicture
{
public:
    void callSetData(const QByteArray &byteArray) {
        setData(byteArray.data(), byteArray.size());
    }
};

#ifndef QTJAMBI_NO_WIDGETS
class GraphicsItemSubclass: public QGraphicsItem
{
public:
    void callPaint(QPainter *painter, const QStyleOptionGraphicsItem &option, QWidget *w)
    {
        paint(painter, &option, w);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        this->widget = widget;
        this->painter = painter;
        this->option = *option;

        painter->fillRect(0, 0, 50, 50, Qt::red);
    }

    QPainter *painter;
    QStyleOptionGraphicsItem option;
    QWidget *widget;
};
#endif

class AccessibleInterfaceSubclass: public QAccessibleInterface
{
public:
    int callNavigate(QAccessible::RelationFlag relation, int entry) {
        return navigate(relation, entry, &this->target);
    }

    virtual int navigate(QAccessible::RelationFlag relation, int entry, QAccessibleInterface **target) const {
        if (relation == 0x00000001) {
            *target = const_cast<AccessibleInterfaceSubclass *>(this);
            return entry;
        } else {
            *target = 0;
            return -1;
        }
    }

    QAccessibleInterface *target;
};

class SomeQObject: public QObject
{
    Q_OBJECT
public slots:
    void getAnUrl(const QUrl &url) {
        this->url = url;
    }

    void actionTriggered() {
        emit myActionTriggered();
    }

signals:
    void myActionTriggered();

public:
    QUrl url;
};

class ValidatorSubclass: public QValidator {
public:
    ValidatorSubclass() : QValidator(0) {}

    QString callFixup(const QString &input) {
        QString blah = input;
        fixup(blah);
        return blah;
    }

    State callValidate(QString *input, int *pos) {
        return validate(*input, *pos);
    }

    void fixup(QString &input) const {
        inputString = input;

        input = "somePrefix" + input;
    }

    State validate(QString &input, int &pos) const {
        inputString = input;
        inputPos = pos;

        input += "somePostfix";
        pos = inputString.length();

        if (inputString == "intermediate")
            return Intermediate;
        else if (inputString == "acceptable")
            return Acceptable;
        else
            return Invalid;
    }

    mutable QString inputString;
    mutable int inputPos;
};

class ImageIOHandlerSubclass: public QImageIOHandler
{
public:
    bool callRead(QImage *image);

    bool read(QImage *image) override;
};

#ifndef QTJAMBI_NO_SQL
class SqlTableModelSubclass: public QSqlTableModel
{
    Q_OBJECT
public:
    void emitBeforeInsert() {
        emit beforeInsert(myRecord);
    }

    void connectBeforeInsert() {
        connect(this, SIGNAL(beforeInsert(QSqlRecord &)), this, SLOT(receiveBeforeInsert(QSqlRecord &)));
    }

    QSqlRecord myRecord;

public slots:
    void receiveBeforeInsert(QSqlRecord &rec) {
        rec.append(QSqlField("cppInt", QVariant::Int));
        rec.setValue("cppInt", 1234);
    }

};
#endif /* QTJAMBI_NO_SQL */

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#ifndef QTJAMBI_NO_XML
class XmlReaderSubclass: public QXmlReader
{
public:
    bool callFeature(const QString &name) {
        return feature(name, &myOk);
    }

    bool feature(const QString &name, bool *) const
    {
        myName = name;
        return (name == "true");
    }

    bool myOk;
    mutable QString myName;
};

class XmlEntityResolverSubclass: public QXmlEntityResolver
{
public:
    bool resolveEntity(const QString &publicId, const QString &systemId, QXmlInputSource *&ret)
    {
        if (publicId == "c++") {
            ret = new QXmlInputSource;
            ret->setData(QString::fromLatin1("Made in C++"));
        }

        return (systemId != "error");
    }

    QXmlInputSource *callResolveEntity(const QString &publicId, const QString &systemId)
    {
        QXmlInputSource *ptr = 0;
        bool error = !resolveEntity(publicId, systemId, ptr);

        if (error && ptr != 0)
            ptr->setData(ptr->data() + QString::fromLatin1(" with error"));

        return ptr;
    }
};
#endif // QTJAMBI_NO_XML
#endif

class AccessibleTextInterfaceSubclass: public QAccessibleTextInterface
{
public:
    virtual void selection(int selectionIndex, int *startOffset, int *endOffset) const;
    virtual QString attributes(int offset, int *startOffset, int *endOffset) const;
    virtual QString textBeforeOffset(int offset, QAccessible::TextBoundaryType boundaryType,
                                     int *startOffset, int *endOffset) const;
    virtual QString textAfterOffset(int offset, QAccessible::TextBoundaryType boundaryType,
                                    int *startOffset, int *endOffset) const;
    virtual QString textAtOffset(int offset, QAccessible::TextBoundaryType boundaryType,
                                 int *startOffset, int *endOffset) const;

    static void callSelection(AccessibleTextInterfaceSubclass *obj, int selectionIndex, int *startOffset, int *endOffset);
    static QString callAttributes(AccessibleTextInterfaceSubclass *obj, int offset, int *startOffset, int *endOffset);
    static QString callTextBeforeOffset(AccessibleTextInterfaceSubclass *obj, int offset, QAccessible::TextBoundaryType boundaryType,
                                     int *startOffset, int *endOffset);
    static QString callTextAfterOffset(AccessibleTextInterfaceSubclass *obj, int offset, QAccessible::TextBoundaryType boundaryType,
                                    int *startOffset, int *endOffset);
    static QString callTextAtOffset(AccessibleTextInterfaceSubclass *obj, int offset, QAccessible::TextBoundaryType boundaryType,
                                 int *startOffset, int *endOffset);
};

#ifndef QTJAMBI_NO_NETWORK
class AbstractSocketSubclass: public QAbstractSocket
{
    Q_OBJECT
public:
    AbstractSocketSubclass(QAbstractSocket::SocketType socketType, QObject *parent) : QAbstractSocket(socketType, parent) { }

    void connectProxyAuthenticationRequired(QAbstractSocket *socket);
    void emitProxyAuthenticationRequired(QAbstractSocket *socket, const QNetworkProxy &proxy, QAuthenticator *authenticator);

    inline void emitSignalAccessor(const QNetworkProxy &proxy, QAuthenticator *authenticator)
    {
        proxyAuthenticationRequired(proxy, authenticator);
    }

private slots:
    void aSlot(const QNetworkProxy &proxy, QAuthenticator *authenticator);
public:
    static QList<QString> emitAuthenticationRequired(QNetworkAccessManager* accessManager, QNetworkReply* reply){
        QList<QString> result;
        QAuthenticator authenticator;
        result << authenticator.user();
        result << authenticator.password();
        emit accessManager->authenticationRequired(reply, &authenticator);
        result << authenticator.user();
        result << authenticator.password();
        return result;
    }
};
#endif // QTJAMBI_NO_NETWORK

class SenderQObject: public QObject
{
    Q_OBJECT
public:
    void emitSignal() {
        emit mappedSignal("foo", 0xf00);
    }

    void connect(QSignalMapper *mapper) {
        QObject::connect(this, SIGNAL(mappedSignal(const QString &, int)), mapper, SLOT(map()));
    }

signals:
    void mappedSignal(const QString &, int);
};

#ifndef QTJAMBI_NO_WIDGETS
class StyledItemDelegateSubclass: public QStyledItemDelegate
{
public:
    void initStyleOptionInStyledDelegate(QStyleOptionViewItem *styleOption)
    {
        initStyleOption(styleOption, QModelIndex());
    }
};

class GraphicsWidgetSubclass: public QGraphicsWidget
{
protected:
    void initStyleOption(QStyleOption *option) const
    {
        if (QStyleOptionGroupBox *box = qstyleoption_cast<QStyleOptionGroupBox *>(option)) {
            box->midLineWidth = 123;
        }
    }

public:
    static int callInitStyleOption(GraphicsWidgetSubclass *widget)
    {
        QStyleOptionGroupBox box;
        widget->initStyleOption(&box);

        return box.lineWidth + box.midLineWidth;
    }
};
#endif

#ifndef QTJAMBI_GENERATOR_RUNNING

namespace Java{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
namespace QtXml{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QXmlEntityResolver$ResolvedEntity,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_FIELD(error)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(inputSource))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QXmlNamespaceSupport$ProcessedName,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QXmlNamespaceSupport$SplitName,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
}
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

#ifndef QTJAMBI_NO_WIDGETS
namespace QtWidgets{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QGraphicsItem$BlockedByModalPanelInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
}
#endif //def QTJAMBI_NO_WIDGETS

}

#endif // QTJAMBI_GENERATOR_RUNNING

#endif
