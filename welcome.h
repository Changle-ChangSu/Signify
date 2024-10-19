#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton> // 添加 QPushButton 头文件
#include <QCamera> // 添加 QCamera 头文件
#include <QVideoWidget> // 添加 QVideoWidget 头文件
#include <QMediaCaptureSession> // 添加 QMediaCaptureSession 头文件
#include <QSpacerItem>

class Welcome : public QWidget {
    Q_OBJECT

public:
    explicit Welcome(QWidget *parent = nullptr);

private:
    QLabel *lblWelcome; // 用于显示欢迎文本
    QCamera *camera;
    QMediaCaptureSession captureSession;
    QVideoWidget *videoWidget;
    void openCameraWindow(); // 添加打开摄像头的槽函数
};

#endif // WELCOME_H
