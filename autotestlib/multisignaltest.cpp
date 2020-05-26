#include "multisignaltest.h"

MultiSignalTest::MultiSignalTest(QObject *parent) : QObject(parent)
{

}


void MultiSignalTest::emit_multiSignal1(){ emit multiSignal1(); }
void MultiSignalTest::emit_multiSignal1(int i){ emit multiSignal1(i); }
void MultiSignalTest::emit_multiSignal1(int i, const QString& s){ emit multiSignal1(i,s); }
void MultiSignalTest::emit_multiSignal2(const QString& s){ emit multiSignal2(s); }
void MultiSignalTest::emit_multiSignal2(int i){ emit multiSignal2(i); }
void MultiSignalTest::emit_multiSignal2(bool b){ emit multiSignal2(b); }
void MultiSignalTest::emit_multiSignal3(){ emit multiSignal3(); }
void MultiSignalTest::emit_multiSignal3(int i){ emit multiSignal3(i); }
void MultiSignalTest::emit_multiSignal3(bool b){ emit multiSignal3(b); }
void MultiSignalTest::emit_multiSignal3(const QString& s){ emit multiSignal3(s); }
void MultiSignalTest::emit_multiSignal4(const QString& s){ emit multiSignal4(s); }
void MultiSignalTest::emit_multiSignal4(int i){ emit multiSignal4(i); }
void MultiSignalTest::emit_multiSignal4(bool b){ emit multiSignal4(b); }
void MultiSignalTest::emit_multiSignal4(const QString& s, int i){ emit multiSignal4(s,i); }
void MultiSignalTest::emit_multiSignal4(int i, bool b){ emit multiSignal4(i,b); }
void MultiSignalTest::emit_multiSignal4(bool b, const QString& s){ emit multiSignal4(b,s); }
void MultiSignalTest::emit_multiSignal4(bool b, int i, const QString& s){ emit multiSignal4(b,i,s); }
