#ifndef DOCPAGE_H
#define DOCPAGE_H

#include "docelement.h"

class DocPage : public DocElement
{
public:
    explicit DocPage(QObject *parent = nullptr);
};

#endif // DOCPAGE_H
