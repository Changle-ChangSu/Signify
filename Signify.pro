QT += core gui
QT += multimedia
QT += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


# Connect Winsock Library
LIBS += -lWs2_32

# Include path of folders
INCLUDEPATH += $$PWD/include/utils
INCLUDEPATH += $$PWD/include/core
INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/src/utils
INCLUDEPATH += $$PWD/src/core



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/core/cannonball.cpp \
    src/utils/datareceiver.cpp \
    src/core/enemyword.cpp \
    src/core/gamewidget.cpp \
    src/core/gamewindowlabel.cpp \
    src/core/homepage.cpp \
    src/core/mainwindow.cpp\
    src/core/recordwidget.cpp \
    src/core/studywidget.cpp \
    src/utils/tcpserver.cpp \
    src/core/welcome.cpp \
    src/main.cpp \


HEADERS += \
    include/core/cannonball.h \
    include/utils/datareceiver.h \
    include/core/enemyword.h \
    include/core/gamewidget.h \
    include/core/gamewindowlabel.h \
    include/core/homepage.h \
    include/core/mainwindow.h \
    include/core/recordwidget.h \
    include/core/studywidget.h \
    include/utils/tcpserver.h \
    include/core/welcome.h

FORMS += \
    resources/mainwindow.ui

TRANSLATIONS += \
    resources/Signify_en_SG.ts

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    # icons.qrc \
    resources/fonts.qrc \
    resources/images.qrc \
    resources/sounds.qrc

# DISTFILES += \
#     singleType.wav \
#     type.jpg \
#     type.wav \
#     wordBoom.wav \
#     attack.mp3 \
#     crossbow.wav

# RC_ICONS = resources/icon/images/signify.ico
