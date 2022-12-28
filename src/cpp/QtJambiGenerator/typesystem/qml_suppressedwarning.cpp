#include "qml_suppressedwarning.h"

SuppressedWarning::SuppressedWarning(QObject *parent)
    : AbstractObject{parent}
{

}

const QString &SuppressedWarning::getText() const
{
    return text;
}

void SuppressedWarning::setText(const QString &newText)
{
    if (text == newText)
        return;
    text = newText;
    emit textChanged();
}
