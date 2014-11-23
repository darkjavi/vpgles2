#ifndef VPTCAMERA_H
#define VPTCAMERA_H

#include <QThread>
#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QVector>
#include <qmath.h>
#include "vp3dgeocamera.h"

#ifndef M_PI
#define M_PI        3.14159265358979323846
#endif
#define DEG2RAD     M_PI/180.0
#define RAD2DEG     180.0/M_PI

#define ALPHA_OFFSET        M_PI
#define BETA_OFFSET         M_PI/2

class vp3DGeoCamera;

class vpTCamera : public QThread
{
    Q_OBJECT
public:
    enum CameraMoveMode
    {
        CamMoveModeFree,
        CamMoveModeArroundObj,
        CamMoveModeAlongObj
    };
    enum CamShutter
    {
        CamShutterModeEvents,
        CamShutterModeAuto,
        CamShutterDefaultMaxFPS = 50
    };

    explicit vpTCamera(QObject *parent = 0);
    ~vpTCamera();

    QMatrix4x4&    modelview(){return m_modelview;}
    vp3DGeoCamera* camera_representation(){return m_cam_representation;}
    QString        info(bool to_screen = false);



//sets
    void set_aspectRadio(qreal n_aspectRatio);
  //modos de control de la camara:
    void set_movemode_free();
    void set_movemode_arround(vp3DGeoObj* object);
    void set_movemode_along(vp3DGeoObj* object, QVector3D face);
  //controles de la camara:
    void translate(qreal right, qreal up, qreal forward);
    void set_position(QVector3D n_pos);
    void rotate(qreal rel_alpha, qreal rel_beta,bool already_RAD = false);
    void set_rotation(qreal abs_alpha, qreal abs_beta,bool already_RAD = false);

    void rotate_arround_obj(vp3DGeoObj* obj,qreal rel_alpha, qreal rel_beta, bool already_RAD = false);

    void  focus_object(vp3DGeoObj* obj, QVector3D face = QVector3D(0,0,1));
    qreal framing_distance(qreal obj_size);// calcula la distancia a la que tiene que estar la camara para encuadrar un objeto de esas dimensiones
    void  look_at(QVector3D target);

    void set_shuttler_mode_events()         {m_shutter_mode = CamShutterModeEvents;}
    void set_shuttler_mode_auto()           {m_shutter_mode = CamShutterModeAuto;}
    void set_shuttler_maxfps(quint8 maxfps) {m_shutter_maxfps = maxfps;}

    void move_cam_free(QVector3D displacement);
    void move_cam_arroundobj(QVector3D displacement);
    void move_cam_alongobj(QVector3D displacement);

    //aux
    qreal calc_beta(QVector3D orig,QVector3D dest);
    qreal calc_alpha(QVector3D orig,QVector3D dest);
    qreal calc_abs_alpha(QVector3D center, QVector3D point);
//gets
    QVector3D get_cam_pos(){return m_cam_pos;}
    QVector3D get_cam_looking(){return cam_looking();}
    uint      get_cam_movemode(){return m_cam_move_mode;}
protected:
    QVector3D   m_cam_pos;
    QVector3D   m_cam_up;
    qreal       m_cam_modulus;
    qreal       m_cam_fov;
    qreal       m_cam_nearPlane;
    qreal       m_cam_farPlane;
    qreal       m_cam_alpha_rotation;
    qreal       m_cam_beta_rotation;

    vp3DGeoCamera* m_cam_representation;
    quint8      m_cam_move_mode;
    qreal       m_cam_move_stepsize;
    vp3DGeoObj *m_cam_target;
    QVector3D   m_cam_target_face;

    qreal       m_cam_aspectRatio;
    QMatrix4x4  m_modelview;

    quint8      m_shutter_mode;
    quint8      m_shutter_time;
    quint8      m_shutter_maxfps;
    bool        m_shutter_update_needed;
    QTimer      m_shutter;

    int m_lastXCursor;
    int m_lastYCursor;
    //
    QVector3D   cam_looking();
    void        update_modelview();
    void        invert_camera_up();

    qreal  cam_alpha_to(QVector3D dest);
    qreal  cam_beta_to(QVector3D dest);

    QVector3D espherical_rotation(QVector3D center, QVector3D point,qreal alpha,qreal beta);
signals:
    void camera_updated();
private slots:
    void shutter();
public slots:
    void move_cam(QVector3D displacement);

    void keyPressEvent      (QKeyEvent *event);
    void mousePressEvent    (QMouseEvent *event);
    void mouseMoveEvent     (QMouseEvent *event);
    void wheelEvent         (QWheelEvent *event );

    void update();
    void set_shuttler_time(uint msecs);
};

#endif // VPTCAMERA_H
