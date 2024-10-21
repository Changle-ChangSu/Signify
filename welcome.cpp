#include "welcome.h"


Welcome::Welcome(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Welcome Page");
    setFixedSize(810, 720); // 设置固定大小

    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter); // 中心对齐
    layout->setSpacing(20); // 设置控件间距

    // // 设置布局背景颜色
    // QWidget *backgroundWidget = new QWidget(this);
    // backgroundWidget->setStyleSheet("background-color: white;");
    // layout->addWidget(backgroundWidget);
    // backgroundWidget->setLayout(layout);


    // 创建欢迎文本
    lblWelcome = new QLabel("Welcome to use Signify！", this);
    lblWelcome->setAlignment(Qt::AlignCenter);
    lblWelcome->setStyleSheet("font-size: 24px;");
    layout->addWidget(lblWelcome);

    // 添加目标说明文本
    QLabel *lblDescription = new QLabel("We aim to teach you how to use the most basic sign language letters, "
                                        "so that you can easily and quickly communicate in sign language with those in need!", this);
    lblDescription->setAlignment(Qt::AlignCenter);
    lblDescription->setWordWrap(true); // 允许文本换行
    lblDescription->setStyleSheet("font-size: 18px;");
    lblDescription->setMinimumWidth(640); // 设置最小宽度
    layout->addWidget(lblDescription);


    // 添加额外的说明文本
    QLabel *lblInstruction = new QLabel("While learning and playing, we need to turn on your camera to determine if you're doing the right sign language.", this);
    lblInstruction->setAlignment(Qt::AlignCenter);
    lblInstruction->setWordWrap(true); // 允许文本换行
    lblInstruction->setStyleSheet("font-size: 18px;");
    lblInstruction->setMinimumWidth(640); // 设置最小宽度
    layout->addWidget(lblInstruction);

    // // 添加垫片
    // layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // 创建底部信息
    QLabel *lblCameraInfo = new QLabel("You can adjust the camera here", this);
    lblCameraInfo->setAlignment(Qt::AlignCenter);
    lblCameraInfo->setStyleSheet("font-size: 18px;"); // 设置字体大小
    layout->addWidget(lblCameraInfo);

    // 创建摄像头按钮
    QPushButton *btnCamera = new QPushButton("Camera", this);
    btnCamera->setFixedSize(200, 50); // 设置按钮大小
    btnCamera->setStyleSheet(
        "QPushButton {"
        "background-color: red;"
        "color: white;"
        "border-radius: 25px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: darkred;"
        "}"
        );
    layout->addWidget(btnCamera, 0, Qt::AlignCenter); // 居中对齐

    // 连接信号
    connect(btnCamera, &QPushButton::clicked, this, &Welcome::openCameraWindow);

    // 设置布局
    setLayout(layout);
}




void Welcome::openCameraWindow()
{
    // 创建一个新窗口，用于显示摄像头
    QWidget *cameraWindow = new QWidget;
    cameraWindow->setWindowTitle("Camera View");
    cameraWindow->setFixedSize(400, 300);

    // 创建视频小部件（用于替代 QCameraViewfinder）
    QVideoWidget *videoWidget = new QVideoWidget(cameraWindow);
    QVBoxLayout *cameraLayout = new QVBoxLayout(cameraWindow);
    cameraLayout->addWidget(videoWidget);

    // 创建摄像头对象
    QCamera *camera = new QCamera(this);

    // 创建媒体捕捉会话并设置摄像头和视频输出
    // QMediaCaptureSession captureSession;
    captureSession.setCamera(camera);
    captureSession.setVideoOutput(videoWidget);

    // 开始捕获视频
    camera->start();

    // 显示摄像头窗口
    cameraWindow->setLayout(cameraLayout);
    cameraWindow->show();

    // 处理窗口关闭事件，关闭窗口时关闭摄像头
    connect(cameraWindow, &QWidget::destroyed, this, [=]() {
        camera->stop();
        delete camera;
    });
}
