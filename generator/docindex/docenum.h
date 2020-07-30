#ifndef DOCENUM_H
#define DOCENUM_H

#include "docelement.h"

class DocEnum: public DocElement
{
    Q_OBJECT
public:
    DocEnum(QObject* parent);
};

#endif // DOCENUM_H
