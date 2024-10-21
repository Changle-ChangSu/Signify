QT       += core gui
QT += multimedia
QT += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# # 设置 ONNX Runtime 安装路径
# ONNX_RUNTIME_PATH = D:/onnxruntime
# # 包含 ONNX Runtime 头文件
# INCLUDEPATH += $${ONNX_RUNTIME_PATH}/include
# # 链接 ONNX Runtime 库
# LIBS += -L$${ONNX_RUNTIME_PATH}/lib \
#         -lonnxruntime



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camaradetection.cpp \
    cannonball.cpp \
    enemyword.cpp \
    gamewidget.cpp \
    gamewindowlabel.cpp \
    homepage.cpp \
    mainwindow.cpp\
    recordwidget.cpp \
    studywidget.cpp \
    welcome.cpp \
    main.cpp \


HEADERS += \
    cannonball.h \
    enemyword.h \
    gamewidget.h \
    gamewindowlabel.h \
    homepage.h \
    mainwindow.h \
    recordwidget.h \
    studywidget.h \
    welcome.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Signify_en_SG.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    font.qrc \
    icons.qrc \
    images.qrc \
    sounds.qrc

DISTFILES += \
    singleType.wav \
    type.jpg \
    type.wav \
    wordBoom.wav \
    attack.mp3 \
    crossbow.wav
