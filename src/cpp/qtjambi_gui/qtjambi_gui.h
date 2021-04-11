#ifndef QTJAMBI_GUI_H
#define QTJAMBI_GUI_H

#include <QtGui/QPainter>
#include <QtGui/QPaintDevice>

class PaintDeviceAccess : public QPaintDevice
{
public:
    inline QPainter* getSharedPainter() const { return sharedPainter(); }
};

inline void qtjambi_finalize_paint_device(QPaintDevice* device){
    PaintDeviceAccess* __this = reinterpret_cast<PaintDeviceAccess*>(device);
    if(QPainter* painter = __this->getSharedPainter()){
        if(painter->isActive()){
            painter->end();
        }
    }
}

#endif // QTJAMBI_GUI_H
