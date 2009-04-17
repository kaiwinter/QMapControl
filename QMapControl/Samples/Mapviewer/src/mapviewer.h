#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include <QtGui>
#include "../../../qmapcontrol.h"
using namespace qmapcontrol;
class Mapviewer : public QMainWindow
{
    Q_OBJECT

public:
    Mapviewer(QWidget *parent = 0);

    ~Mapviewer();

private:
    MapControl* mc;
    MapAdapter* mapadapter;
    Layer* mainlayer;

    void addZoomButtons();

protected:
    virtual void resizeEvent ( QResizeEvent * event );
};

#endif
