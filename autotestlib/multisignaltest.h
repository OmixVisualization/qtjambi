#ifndef MULTISIGNALTEST_H
#define MULTISIGNALTEST_H

#ifndef QT_JAMBI_RUN
#include <qtjambi/qtjambi_global.h>
#endif
#include <QObject>

class MultiSignalTest : public QObject
{
    Q_OBJECT
public:
    explicit MultiSignalTest(QObject *parent = nullptr);

signals:
    void multiSignal1();
    void multiSignal1(int i);
    void multiSignal1(int i, const QString& s);
    void multiSignal1(double d, QPrivateSignal);
    void multiSignal2(const QString& s);
    void multiSignal2(int i);
    void multiSignal2(bool b);
    void multiSignal3(int i);
    void multiSignal3(bool b);
    void multiSignal3(const QString& s = "TEST");
    void multiSignal4(const QString& s, int i = 147);
    void multiSignal4(int i, bool b = true);
    void multiSignal4(bool b, const QString& s = "TEST");
    void multiSignal4(bool b, int i, const QString& s = "TEST");
public slots:

public:
    void emit_multiSignal1();
    void emit_multiSignal1(int i);
    void emit_multiSignal1(int i, const QString& s);
    void emit_multiSignal1(double d);
    void emit_multiSignal2(const QString& s);
    void emit_multiSignal2(int i);
    void emit_multiSignal2(bool b);
    void emit_multiSignal3();
    void emit_multiSignal3(int i);
    void emit_multiSignal3(bool b);
    void emit_multiSignal3(const QString& s);
    void emit_multiSignal4(const QString& s);
    void emit_multiSignal4(int i);
    void emit_multiSignal4(bool b);
    void emit_multiSignal4(const QString& s, int i);
    void emit_multiSignal4(int i, bool b);
    void emit_multiSignal4(bool b, const QString& s);
    void emit_multiSignal4(bool b, int i, const QString& s);
};

#endif // MULTISIGNALTEST_H
