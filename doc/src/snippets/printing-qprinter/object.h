#include <QObject>

class Object : public QObject
{
    Q_OBJECT

public:
    Object(QObject *parent = 0);

public slots:
    void print();
};
