#ifndef VP3DGEOCAMERA_H
#define VP3DGEOCAMERA_H

#include "vp3dgeoobj.h"
#include "vptcamera.h"

class vpTCamera;

class vp3DGeoCamera : public vp3DGeoObj
{
public:
    vp3DGeoCamera(vpTCamera* camera);
    ulong draw2D(QPainter* p);
    QVector3D get_center();
protected:
    bool m_draw_fov;
    vpTCamera* m_camera;
    void generate_geometry();
public slots:
    void update();
};

#endif // VP3DGEOCAMERA_H
