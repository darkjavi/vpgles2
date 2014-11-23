#include "vptcamera.h"

vpTCamera::vpTCamera(QObject *parent) :
    QThread(parent)
{
    m_cam_target = 0;
    m_cam_modulus = 20;
    m_cam_pos  = QVector3D(1000,500,1300);
    m_cam_alpha_rotation = ALPHA_OFFSET;
    m_cam_beta_rotation  = BETA_OFFSET;
    m_cam_up            = QVector3D(0,1,0);
    m_cam_fov           = 30;
    m_cam_nearPlane     = 0.1;
    m_cam_farPlane      = 10000;
    m_cam_aspectRatio   = 800.0/600.0;
    set_movemode_free();
    m_shutter_mode      = CamShutterModeEvents;
    m_shutter_maxfps    = CamShutterDefaultMaxFPS;
    m_shutter_time      = 1000/m_shutter_maxfps*1.1;
    m_shutter_update_needed = false;
    m_shutter.start(m_shutter_time);
    connect(&m_shutter,SIGNAL(timeout()),this,SLOT(shutter()));

    m_lastXCursor = 0;
    m_lastYCursor = 0;

    m_cam_representation = new vp3DGeoCamera(this);
}

vpTCamera::~vpTCamera()
{
    //de momento vamos a dejar que sea la escena la que borre la camara
    //delete m_cam_representation;
}

void vpTCamera::set_aspectRadio(qreal n_aspectRatio)
{
    m_cam_aspectRatio = n_aspectRatio;
    update_modelview();
}

void vpTCamera::set_movemode_free()
{
    m_cam_move_mode = CamMoveModeFree;
    m_cam_move_stepsize = 1;
    m_cam_target = 0;
}

void vpTCamera::set_movemode_arround(vp3DGeoObj* object)
{
    m_cam_move_mode = CamMoveModeArroundObj;
    m_cam_move_stepsize = 1;
    focus_object(object);
    m_cam_target = object;
}

void vpTCamera::set_movemode_along(vp3DGeoObj* object, QVector3D face)
{
    m_cam_move_mode = CamMoveModeAlongObj;
    m_cam_move_stepsize = 1;
    focus_object(object,face);
    m_cam_target = object;
    m_cam_target_face = face;
}

//-----<movimiento de la camara>--------------------------
void vpTCamera::translate(qreal right, qreal up, qreal forward)
{
    if ( forward != 0 )
    {
        qreal distance = QVector3D(right,up,forward).length();
        qreal auxZ = (distance*cos(m_cam_alpha_rotation)*sin(m_cam_beta_rotation)) ;
        qreal auxX = (distance*sin(m_cam_alpha_rotation)*sin(m_cam_beta_rotation));
        qreal auxY = (distance*cos(m_cam_beta_rotation));

        QVector3D displacement(auxX,auxY,auxZ);

        if (forward > 0)
        {
            set_position(m_cam_pos + displacement);
        } else {
            set_position (m_cam_pos - displacement);
        }
    }

    if ( right != 0 )
    {
        QVector3D displacement(right,0,0);
        set_position(m_cam_pos + displacement);
    }
    if ( up != 0 )
    {
        QVector3D displacement(0,up,0);
        set_position(m_cam_pos + displacement);
    }
}

void vpTCamera::set_position(QVector3D n_pos)
{
    m_cam_pos = n_pos;
    update_modelview();
}

void vpTCamera::rotate(qreal rel_alpha, qreal rel_beta,bool already_RAD)
{
    qreal n_alpha;
    qreal n_beta;
    if (already_RAD)
    {
        n_alpha = rel_alpha;
        n_beta  = rel_beta;
    } else {
        n_alpha = DEG2RAD*rel_alpha;
        n_beta = DEG2RAD*rel_beta;
    }
    n_alpha += m_cam_alpha_rotation;
    n_beta  += m_cam_beta_rotation;
    set_rotation(n_alpha,n_beta,true);
}

void vpTCamera::set_rotation(qreal abs_alpha, qreal abs_beta,bool already_RAD)
{
    qreal n_alpha;
    qreal n_beta;

    if (already_RAD)
    {
        n_alpha = abs_alpha;
        n_beta  = abs_beta;
    } else {
        n_alpha = DEG2RAD*abs_alpha;
        n_beta = DEG2RAD*abs_beta;
    }

    if ( n_beta > M_PI )
    {
        qreal offset = n_beta - M_PI;
        n_beta = (M_PI*(-1))+offset;
    }
    else if ( n_beta < M_PI*(-1) )
    {
        qreal offset = n_beta - (M_PI*-1);
        n_beta = M_PI + offset ;
    }

    if ( ( n_beta < 0 && m_cam_up.y()> 0 ) ||
       (   n_beta > 0 && m_cam_up.y() < 0 ) )
    {
        invert_camera_up();
    }

    if( n_alpha > M_PI )
    {
        qreal offset = n_alpha - M_PI;
        n_alpha = (M_PI*(-1))+offset;

    }
    else if( n_alpha < M_PI*(-1) )
    {
        qreal offset = n_alpha - (M_PI*-1);
        n_alpha = M_PI + offset ;
    }

    m_cam_alpha_rotation = n_alpha;
    m_cam_beta_rotation  = n_beta;
    qDebug() << "[INFO]vpTCamera::set_rotation:" << m_cam_alpha_rotation<<m_cam_beta_rotation ;
    update_modelview();
}

void vpTCamera::rotate_arround_obj(vp3DGeoObj* obj,qreal rel_alpha, qreal rel_beta, bool already_RAD)
{
    qreal n_alpha;
    qreal n_beta;
    if (already_RAD)
    {
        n_alpha = rel_alpha;
        n_beta  = rel_beta;
    } else {
        n_alpha = DEG2RAD*rel_alpha;
        n_beta = DEG2RAD*rel_beta;
    }


    qreal modulus2obj = QVector3D(obj->get_center() - m_cam_pos).length();

    qreal obj_beta = calc_beta(obj->get_center(),m_cam_pos);
    qreal obj_alpha = calc_alpha(obj->get_center(),m_cam_pos);

    //qDebug() << "[INFO]vpTCamera::rotate_arround_obj-> alpha/beta objeto->camara:" << obj_beta << obj_alpha;
    obj_beta  += n_beta;
    obj_alpha += n_alpha;
/*
    if ( obj_beta > M_PI )
    {
        qreal offset = obj_beta - M_PI;
        obj_beta = (M_PI*(-1))+offset;
    }
    else if ( obj_beta < M_PI*(-1) )
    {
        qreal offset = obj_beta - (M_PI*-1);
        obj_beta = M_PI + offset ;
    }

    if( obj_alpha > M_PI )
    {
        qreal offset = obj_alpha - M_PI;
        obj_alpha = (M_PI*(-1))+offset;

    }
    else if( obj_alpha < M_PI*(-1) )
    {
        qreal offset = obj_alpha - (M_PI*-1);
        obj_alpha = M_PI + offset ;
    }
*/
    qreal nX = modulus2obj*sin(obj_alpha)*sin(obj_beta)+obj->get_center().x();
    qreal nY = modulus2obj*cos(obj_beta)+obj->get_center().y();
    qreal nZ = modulus2obj*cos(obj_alpha)*sin(obj_beta)+obj->get_center().z();
    m_cam_pos = QVector3D(nX,nY,nZ);
    //look_at(obj->get_center());
}

void vpTCamera::focus_object(vp3DGeoObj* obj, QVector3D face)
{
    m_cam_target = obj;
    m_cam_target_face = face;
    qreal size_to_fit = 0;
    if( face.x() != 0 )
    {
        size_to_fit = obj->get_size().y();
        if ( obj->get_size().z() > size_to_fit )
            size_to_fit = obj->get_size().z();
    }
    else if( face.y() != 0 )
    {
        size_to_fit = obj->get_size().x();
        if ( obj->get_size().z() > size_to_fit )
            size_to_fit = obj->get_size().z();
    }
    else if( face.z() != 0 )
    {
        size_to_fit = obj->get_size().y();
        if ( obj->get_size().x() > size_to_fit )
            size_to_fit = obj->get_size().x();
    }
    qreal distance = framing_distance(size_to_fit);
    QVector3D offset(distance,distance,distance);
    offset *= face;
    m_cam_pos = obj->get_center()+offset;
    look_at(obj->get_center());
}

qreal vpTCamera::framing_distance(qreal obj_size)
{
    //qreal distance = (obj_size)/tan(m_cam_fov);
    qreal distance = obj_size * 0.1;
    return distance;
}

void vpTCamera::look_at(QVector3D target)
{
    //qDebug() << "[INFO]vpTCamera::look_at:" << target;
    m_cam_modulus = QVector3D(target - m_cam_pos).length();
    //qreal n_beta  =  acos( (target.y()- m_cam_pos.y()) / m_cam_modulus );
    //qreal n_alpha = acos( (target.z() - m_cam_pos.z())/(m_cam_modulus*sin(n_beta)) );

    //qreal n_beta  = acos( (( target.y() - m_cam_pos.y() ) / m_cam_modulus) );
    //qreal n_alpha = asin( (( target.x() - m_cam_pos.x()) / (m_cam_modulus*sin(n_beta))) );

    qreal beta = cam_beta_to(target);
    qreal alpha = cam_alpha_to(target);
    //qreal n_beta = calc_beta(m_cam_pos,target);
    //qreal n_alpha = calc_alpha(m_cam_pos,target);
    qDebug() << "[INFO]vpTCamera::look_at-> alpha/beta camara->objeto" << alpha << beta;
    //qDebug() <<target.y()- m_cam_pos.y() << target.z() - m_cam_pos.z()<< n_alpha;
    //set_rotation(n_alpha,n_beta,true);
    rotate(alpha,beta,true);
}

//-----</movimiento de la camara>--------------------------

QVector3D vpTCamera::cam_looking()
{
    qreal auxZ = m_cam_modulus*cos(m_cam_alpha_rotation)*sin(m_cam_beta_rotation)+m_cam_pos.z();
    qreal auxX = m_cam_modulus*sin(m_cam_alpha_rotation)*sin(m_cam_beta_rotation)+m_cam_pos.x();
    qreal auxY = m_cam_modulus*cos(m_cam_beta_rotation)+m_cam_pos.y();
    return QVector3D(auxX,auxY,auxZ);
}

void vpTCamera::update_modelview()
{
    //info(true);
    m_modelview = QMatrix4x4();
    m_modelview.perspective(m_cam_fov,m_cam_aspectRatio,m_cam_nearPlane,m_cam_farPlane);
    m_modelview.lookAt(m_cam_pos,cam_looking(),m_cam_up);
    m_cam_representation->update();
    if( m_shutter_mode == CamShutterModeEvents )
    {
        update();
    }
}

void vpTCamera::invert_camera_up()
{
    m_cam_up = QVector3D(m_cam_up.x()*-1,m_cam_up.y()*-1,m_cam_up.z()*-1);
}

void vpTCamera::move_cam_free(QVector3D displacement)
{
    rotate(displacement.x(),displacement.y());
    translate(0,0,displacement.z());
}

void vpTCamera::move_cam_arroundobj(QVector3D displacement)
{
    rotate_arround_obj(m_cam_target,displacement.x(),displacement.y());
    translate(0,0,displacement.z());
    look_at(m_cam_target->get_center());
}

void vpTCamera::move_cam_alongobj(QVector3D displacement)
{
    translate(displacement.x(),displacement.y(),displacement.z());
}

QString vpTCamera::info(bool to_screen)
{
    QString infostr = "CAM-INFO:\n";
    infostr.append("[eX:" + QString::number(m_cam_pos.x(),'f',2) +
                   " eY:" + QString::number(m_cam_pos.y(),'f',2) +
                   " eZ:" + QString::number(m_cam_pos.z(),'f',2) + "]") ;

    infostr.append(" A:" + QString::number(m_cam_alpha_rotation,'f',2) +
                   " B:" + QString::number(m_cam_beta_rotation,'f',2) + "\n");

    infostr.append("[cX:" + QString::number(cam_looking().x(),'f',2) +
                   " cY:" + QString::number(cam_looking().y(),'f',2) +
                   " cZ:" + QString::number(cam_looking().z(),'f',2) + "]") ;

    infostr.append(" M:" + QString::number(m_cam_modulus,'f',2) );

    if (to_screen)
    {
        qDebug() << infostr;
    }
    return infostr;
}

//**********************************
//SLOTS
void vpTCamera::move_cam(QVector3D displacement)
{
    if ( m_cam_move_mode == CamMoveModeFree )
    {
        move_cam_free(displacement);
    }
    else if ( m_cam_move_mode == CamMoveModeAlongObj )
    {
        move_cam_alongobj(displacement);
    }
    else if ( m_cam_move_mode == CamMoveModeArroundObj )
    {
        move_cam_arroundobj(displacement);
    }
}

void vpTCamera::set_shuttler_time(uint msecs)
{
    uint max_framems = 1000/m_shutter_maxfps;
    if( msecs*1.1 < max_framems  )
    {// limitamos los frames a maxfps
        m_shutter_time = max_framems;
    }
    else
    {// si no podemos llegar a maxfps ajustamos el tiempo de refresco a lo que tardo el ultimo frame + 10%
        m_shutter_time = msecs*1.1;
    }
}

void vpTCamera::shutter()
{
    m_shutter.stop();
    if( m_shutter_mode == CamShutterModeAuto )
    {
        m_shutter.start(m_shutter_time);
        emit camera_updated();
    }
    else if ( m_shutter_mode == CamShutterModeEvents )
    {
        if (m_shutter_update_needed)
        {
            m_shutter.start(m_shutter_time);
            m_shutter_update_needed = false;
            emit camera_updated();
        }
    }
}

void vpTCamera::update()
{
    m_shutter_update_needed = true;
    if (!m_shutter.isActive())
    {
        shutter();
    }
}
//**************************************
//Manejo de teclado/raton:
void vpTCamera::keyPressEvent      (QKeyEvent   *event)
{
    switch (event->key())
    {

    }
}

void vpTCamera::mousePressEvent    (QMouseEvent *event)
{
    m_lastXCursor = event->pos().x();
    m_lastYCursor = event->pos().y();
    switch(event->buttons())
    {
        case Qt::LeftButton:
        {
            qDebug() << "left mouse button pressed" << endl;
            if (event->type() == event->MouseButtonDblClick)
            {
                qDebug()<<"Doble click!!";
            }
        }
        break;
        case Qt::RightButton:
        {
            qDebug() << "right mouse button pressed" << endl;
        }
        break;
        default:
        event->ignore();//?
    }
}

void vpTCamera::mouseMoveEvent     (QMouseEvent *event)
{
    int actualXCursor = event->pos().x();
    int actualYCursor = event->pos().y();
    if ( ( m_lastXCursor == 0 ) && ( m_lastXCursor == 0 ))
    {
        m_lastXCursor = actualXCursor, m_lastYCursor = actualYCursor;
    }
    if (event->buttons()==Qt::LeftButton)
    {
        actualXCursor = event->pos().x();
        actualYCursor = event->pos().y();
        double x_displacement =(double) -(actualXCursor - m_lastXCursor);
        double y_displacement =(double) (actualYCursor - m_lastYCursor);

        move_cam(QVector3D(x_displacement,y_displacement,0));
    }
    m_lastXCursor = actualXCursor;
    m_lastYCursor = actualYCursor;
}

void vpTCamera::wheelEvent         (QWheelEvent *event)
{
    move_cam(QVector3D(0,0,event->delta()/12));
}


//aux
qreal vpTCamera::calc_beta(QVector3D center,QVector3D point)
{
    qreal mod = QVector3D(point - center).length();
    qreal beta  = acos( ( point.y() - center.y() ) / mod );
    return beta;

}

qreal vpTCamera::calc_abs_alpha(QVector3D center, QVector3D point)
{
    QVector3D beta_cero(center + QVector3D(0,0,10));
    QVector3D a(point - center);
    QVector3D b(beta_cero - center);
    qreal abs_beta = acos(QVector3D::dotProduct(a,b) / (a.length()*b.length()));

    QVector3D c(QVector3D::crossProduct(a,b));
    //qDebug()<<c;
    //qreal det = (a.x()*b.y()*c.z()) + (a.y()*b.z()*c.x()) + (a.z()*b.x()*c.y()) - (a.z()*b.y()*c.x() + a.y()*b.x()*c.z() + a.x()*b.z()*c.y() );
    //qDebug()<< det;
    if (c.y() > 0)
        abs_beta *= -1;


    return abs_beta;

}

qreal vpTCamera::cam_alpha_to(QVector3D dest)
{
    QVector2D a( QVector2D(dest.x(),dest.z()) -
                 QVector2D(m_cam_pos.x(),m_cam_pos.z()));

    QVector2D b( QVector2D(cam_looking().x(),cam_looking().z()) -
                 QVector2D(m_cam_pos.x(),m_cam_pos.z()));

    //QVector3D a(dest - m_cam_pos);
    //QVector3D b(cam_looking() - m_cam_pos);
    qreal alpha_to = acos(QVector2D::dotProduct(a,b) / (a.length()*b.length()));
    //QVector2D c(QVector2D::crossProduct(a,b));
    //if (c.y() > 0)
    //    alpha_to *= -1;
    return alpha_to;
}

qreal vpTCamera::cam_beta_to(QVector3D dest)
{
    QVector2D a( QVector2D(dest.y(),dest.z()) -
                 QVector2D(m_cam_pos.y(),m_cam_pos.z()));

    QVector2D b( QVector2D(cam_looking().y(),cam_looking().z()) -
                 QVector2D(m_cam_pos.y(),m_cam_pos.z()));

    qreal beta_to = acos(QVector2D::dotProduct(a,b) / (a.length()*b.length()));
    return beta_to;// -m_cam_beta_rotation;
}

qreal vpTCamera::calc_alpha(QVector3D center,QVector3D point)
{
    qreal mod = QVector3D(point - center).length();
    qreal beta = calc_beta(center,point);
    qreal alpha = asin( (point.x() - center.x()) / (mod*sin(beta)) );

    return alpha;
}

QVector3D vpTCamera::espherical_rotation(QVector3D center, QVector3D point, qreal alpha, qreal beta)
{
    qreal modulus = QVector3D(point-center).length();
    qreal auxZ = modulus*cos(alpha)*sin(beta)+center.z();
    qreal auxX = modulus*sin(alpha)*sin(beta)+center.x();
    qreal auxY = modulus*cos(beta)+center.y();
    return QVector3D(auxX,auxY,auxZ);

/*
//este codigo rota p alrededor de a
QVector3D p(0.0, 0.0, 20.0);
QVector3D a(0.0, 0.0, 10.0);
QMatrix4x4 m;

p-=a;
m.rotate(180, 0.0, 1.0, 0.0);
p*=m;
p+=a;
*/
}
