#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>

class TextEdit : public QTextEdit
{
    Q_OBJECT

public:
    TextEdit(QWidget *parent=0);
    bool canInsertFromMimeData( const QMimeData *source ) const;
    void insertFromMimeData( const QMimeData *source );
};

#endif
