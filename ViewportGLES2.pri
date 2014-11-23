#-------------------------------------------------
#
# Incluir este .pri archivo en el .pro del proyecto en el que se quiera user el vp
# include(ruta/ViewportGLES2.pri)
#-------------------------------------------------

QT       += core gui opengl network

INCLUDEPATH += vpgles2 ViewportGLES2 bt ViewportGLES2/data ViewportGLES2/geo ViewportGLES2/gl ViewportGLES2/widgets

include(qserialport/serialport-lib.pri)

HEADERS += \
    bt/bthelper.h \
    bt/btenumerator.h \
    bt/btdevicewidget.h \
    bt/btdevicebrowserdialog.h \
    bt/btdevice.h \
    bt/btrfcommhelper.h \
    bt/btdevicebrowser.h \
    ViewportGLES2/data/vpparsertorque.h \
    ViewportGLES2/data/vpparserserialbluetooth.h \
    ViewportGLES2/data/vpparserserial.h \
    ViewportGLES2/data/vpparsernmea.h \
    ViewportGLES2/data/vpparsermindflex.h \
    ViewportGLES2/data/vpparsercsv.h \
    ViewportGLES2/data/vpparser.h \
    ViewportGLES2/data/vpdatasource.h \
    ViewportGLES2/data/vpdataset.h \
    ViewportGLES2/data/vpdatapool.h \
    ViewportGLES2/widgets/vptxrxwidget.h \
    ViewportGLES2/widgets/vpparserwidget.h \
    ViewportGLES2/widgets/vpdatawidgettable.h \
    ViewportGLES2/widgets/vpdatawidget3dmap.h \
    ViewportGLES2/widgets/vpdatawidget3dgraph.h \
    ViewportGLES2/widgets/vpdatawidget2dmap.h \
    ViewportGLES2/widgets/vpdatawidget2dgraph.h \
    ViewportGLES2/widgets/vpdatawidget.h \
    ViewportGLES2/widgets/vpdatasetwidget.h \
    ViewportGLES2/widgets/vpdatapoolwidget.h \
    ViewportGLES2/widgets/vp2dsimplegraph.h \
    ViewportGLES2/widgets/vpdatasourcewidget.h

SOURCES += \
    bt/bthelper.cpp \
    bt/btenumerator.cpp \
    bt/btdevicewidget.cpp \
    bt/btdevicebrowserdialog.cpp \
    bt/btdevice.cpp \
    bt/btrfcommhelper.cpp \
    bt/btdevicebrowser.cpp \
    ViewportGLES2/data/vpparsertorque.cpp \
    ViewportGLES2/data/vpparserserialbluetooth.cpp \
    ViewportGLES2/data/vpparserserial.cpp \
    ViewportGLES2/data/vpparsernmea.cpp \
    ViewportGLES2/data/vpparsermindflex.cpp \
    ViewportGLES2/data/vpparsercsv.cpp \
    ViewportGLES2/data/vpparser.cpp \
    ViewportGLES2/data/vpdatasource.cpp \
    ViewportGLES2/data/vpdataset.cpp \
    ViewportGLES2/data/vpdatapool.cpp \
    ViewportGLES2/widgets/vptxrxwidget.cpp \
    ViewportGLES2/widgets/vpparserwidget.cpp \
    ViewportGLES2/widgets/vpdatawidgettable.cpp \
    ViewportGLES2/widgets/vpdatawidget3dmap.cpp \
    ViewportGLES2/widgets/vpdatawidget3dgraph.cpp \
    ViewportGLES2/widgets/vpdatawidget2dmap.cpp \
    ViewportGLES2/widgets/vpdatawidget2dgraph.cpp \
    ViewportGLES2/widgets/vpdatawidget.cpp \
    ViewportGLES2/widgets/vpdatasetwidget.cpp \
    ViewportGLES2/widgets/vpdatapoolwidget.cpp \
    ViewportGLES2/widgets/vp2dsimplegraph.cpp \
    ViewportGLES2/widgets/vpdatasourcewidget.cpp


FORMS += \
    bt/btdevicewidget.ui \
    bt/btdevicebrowserdialog.ui \
    bt/btdevicebrowser.ui \
    ViewportGLES2/widgets/vpparserwidget.ui \
    ViewportGLES2/widgets/vpdatawidget.ui \
    ViewportGLES2/widgets/vpdatasetwidget.ui \
    ViewportGLES2/widgets/vpdatapoolwidget.ui \
    ViewportGLES2/widgets/vp2dsimplegraph.ui \
    ViewportGLES2/widgets/vpdatasourcewidget.ui


RESOURCES += \
    ViewportGLES2/icons.qrc \
    ViewportGLES2/gl/shaders.qrc \

OTHER_FILES += \
    ViewportGLES2/gl/v_frag.fsh \
    ViewportGLES2/gl/simpleshader.fsh \
    ViewportGLES2/gl/simpleshader.vsh \
    ViewportGLES2/gl/ShaderVTest.vsh \
    ViewportGLES2/gl/ShaderFTest.fsh
