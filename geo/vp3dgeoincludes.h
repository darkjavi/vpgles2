#ifndef VP3DGEOINCLUDES_H
#define VP3DGEOINCLUDES_H

#include "vp3dgeoobj.h"
#include "vp3dgeoaxis.h"
#include "vp3dgeoprimitive.h"
#include "vp3dgeocamera.h"

#include "vp3dgeodataobj.h"
#include "vp3dgeograph.h"

class vp3DGeoTools
{
public:
    static void delete_ojb( vp3DGeoObj* obj)
    {
        if (obj->type() == vp3DGeoObj::TypeAxis)
        {
            vp3DGeoAxis* aux = (vp3DGeoAxis*) obj;
            delete aux;

        } else if (obj->type() == vp3DGeoObj::TypeGraph)
        {
            vp3DGeoGraph* aux = (vp3DGeoGraph*) obj;
            delete aux;
        }
        else if (obj->type() == vp3DGeoObj::TypePrimitive)
        {
            vp3DGeoPrimitive* aux = (vp3DGeoPrimitive*) obj;
            delete aux;
        }
        else
        {// si no sabemos de que tipo es lo borramos con el generico
            qDebug() << "[WARN]vp3DGeoTools::delete_obj-> No se de que tipo es este objeto:" << obj->type();
            delete obj;
        }
    }
};

#endif // VP3DGEOINCLUDES_H
