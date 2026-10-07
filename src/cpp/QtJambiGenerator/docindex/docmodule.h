#ifndef DOCMODULE_H
#define DOCMODULE_H

#include "docelement.h"

class DocModule : public DocElement
{
public:
    explicit DocModule(QObject *parent = nullptr);
};

#endif // DOCMODULE_H
