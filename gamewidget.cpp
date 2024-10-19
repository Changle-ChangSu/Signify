#include "gamewidget.h"  // 头文件
// #include "homepage.h"
#include <QGraphicsDropShadowEffect>

// parent 可以指定当前窗口的父窗口，这样可以保持窗口层次结构一致。如果 parent 为 nullptr，表示这是一个独立的窗口。
GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
{
    layout = new QGridLayout(this);  // 创建了一个 QGridLayout 布局管理器，用于将控件按网格排列。this 表示该布局属于当前窗口 (GameWidget)。

    for(int i=0; i<8; i++) {
        keyPressFlag[i] = false;
    }  // 此处假定有 8 个键的状态需要追踪，也就是支持同时按下八个按键

    lblGameWindow = new GameWindowLabel(this);  // 用于显示游戏画面或游戏界面。它被传递了 this，表示它的父窗口是 GameWidget。

    // 难度等级选择器
    spinMaxLvl = new QSpinBox(this);
    spinMaxLvl->setRange(1, 10);
    spinMaxLvl->setAlignment(Qt::AlignCenter);
    spinMaxLvl->setPrefix("Choose Level: ");
    spinMaxLvl->setFixedHeight(30);
    spinMaxLvl->setFocusPolicy(Qt::NoFocus);  // 用户无法通过键盘焦点操作它
    spinMaxLvl->setStyleSheet(
        "QSpinBox {"
        "    background-color: #f0f0f0;"  // 背景颜色
        // "    color: #737B89;"             // 字体颜色
        // "    border: 1px solid #c0c0c0;"  // 可选：边框颜色
        // "    border-radius: 0px;"         // 可选：圆角
        "    border-left-width: 2px;"
        "    border-right-width: 1px;"
        "    border-bottom-width: 2px;"
        "    border-style: solid;"
        "}"
        // "QSpinBox::up-button, QSpinBox::down-button {"
        // // "    background-color: #c0c0c0;"  // 设置上下按钮的背景颜色
        // // "    color: white;"
        // "}"
    );

    // 读取设置文件中的最大关卡值
    QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
    spinMaxLvl->setValue(settings.value("MaxLvl", 1).toInt());

    // 保存新的最大关卡值
    connect(spinMaxLvl, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value) {
        QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
        settings.setValue("MaxLvl", value);
    });

    // 正确率统计模块
    lblStatistics = new QLabel("Accuracy: 0% ");
    lblStatistics->setFixedHeight(30);
    lblStatistics->setAlignment(Qt::AlignCenter);
    lblStatistics->setStyleSheet(
        "QLabel {"
        "    border-bottom-width: 2px;"
        "    border-style: solid;"
        "}"
    );

    // 返回键设置（已经废弃，不注释防止奇妙报错）
    backbutton = new QPushButton("", this);
    backbutton->setStyleSheet("background-color: transparent;""border-radius: 0px;");
    // backbutton->hide();
    // 连接返回按钮的点击信号到槽函数
    connect(backbutton, &QPushButton::clicked, this, &GameWidget::goBack);

    pauseButton = new QPushButton("Pause/Resume", this);
    pauseButton->setFixedHeight(30);
    // pauseButton->setAlignment(Qt::AlignCenter);
    pauseButton->setStyleSheet(
        "QPushButton {"
        "    border-right: 2px solid #000000;"  // 右侧边框
        "    border-bottom: 2px solid #000000;" // 底部边框
        "    border-top: 0px;"                  // 移除顶部边框
        "    border-left: 1px solid #000000;;"
        "    padding: 5px;"                     // 保持内容与边框的间距
        "}"
        "QPushButton:hover {"
        "    background-color: #e5f1fb;"        // 当鼠标悬停时改变背景色
        "}"
        "QPushButton:pressed {"
        "    background-color: #e0e0e0;"        // 当按钮被按下时改变背景色
        "}"
    );
    connect(pauseButton, &QPushButton::clicked, this, &GameWidget::togglePause);
    connect(lblGameWindow, &GameWindowLabel::gameStatusChanged, this, &GameWidget::onGameStatusChanged);

    // 初始状态检查
    pauseButton->setEnabled(lblGameWindow->begin);

    layout->addWidget(lblGameWindow, 0, 0, 1, 3);
    layout->addWidget(spinMaxLvl, 1, 0);
    layout->addWidget(lblStatistics, 1, 1);
    layout->addWidget(pauseButton, 1, 2);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    setFixedSize(810, 720);
    setWindowTitle("Signify");

}

int GameWidget::getMaxLvl(void) {
    return spinMaxLvl->value();
}

// 启动游戏相关，将废弃
void GameWidget::keyReleaseEvent(QKeyEvent *ev) {
    if(lblGameWindow->isPaused) {
        // qDebug() << ev->key() << lblGameWindow->isPaused;
        return;
    }

    if(lblGameWindow->isRunning() == false) {
        switch(ev->key()) {
        // case Qt::Key_Space:
        //     connect(pauseButton, &QPushButton::clicked, this, &GameWidget::togglePause);
        case Qt::Key_A:
            keyPressFlag[0] = false;
            break;
        case Qt::Key_S:
            keyPressFlag[1] = false;
            break;
        case Qt::Key_D:
            keyPressFlag[2] = false;
            break;
        case Qt::Key_F:
            keyPressFlag[3] = false;
            break;
        case Qt::Key_J:
            keyPressFlag[0 + 4] = false;
            break;
        case Qt::Key_K:
            keyPressFlag[1 + 4] = false;
            break;
        case Qt::Key_L:
            keyPressFlag[2 + 4] = false;
            break;
        case Qt::Key_Semicolon:
            keyPressFlag[3 + 4] = false;
            break;
        default:
            // qDebug() << ev->key();
            break;
        }
    }
}

// 键盘事件
void GameWidget::keyPressEvent(QKeyEvent *ev) {
    if(lblGameWindow->isPaused) {
        // qDebug() << ev->key() << lblGameWindow->isPaused;
        return;
    }
    if(lblGameWindow->isRunning() == false) {
        switch(ev->key()) {
        case Qt::Key_A:
            keyPressFlag[0] = true;
            break;
        case Qt::Key_S:
            keyPressFlag[1] = true;
            break;
        case Qt::Key_D:
            keyPressFlag[2] = true;
            break;
        case Qt::Key_F:
            keyPressFlag[3] = true;
            break;
        case Qt::Key_J:
            keyPressFlag[0 + 4] = true;
            break;
        case Qt::Key_K:
            keyPressFlag[1 + 4] = true;
            break;
        case Qt::Key_L:
            keyPressFlag[2 + 4] = true;
            break;
        case Qt::Key_Semicolon:
            keyPressFlag[3 + 4] = true;
            break;
        default:
            // qDebug() << ev->key();
            break;
        }

        // if((keyPressFlag[0] && keyPressFlag[1] && keyPressFlag[2] && keyPressFlag[3]) ||
        //         (keyPressFlag[0+4] && keyPressFlag[1+4] && keyPressFlag[2+4] && keyPressFlag[3+4])) {
        //     lblGameWindow->setRunning(true);
        // }
    } else {
        if(ev->isAutoRepeat())
            return;
        lblGameWindow->getKey(ev->text());
    }
}


// 统计信息实时修改
void GameWidget::updateStatistic(int rightCnt, int wrongCnt) {
    // qDebug() << rightCnt << wrongCnt;
    if(rightCnt == 0 && wrongCnt == 0)
    {
        lblStatistics->setText("Accuracy: 0% (0 / 0)");
    }
    else
    {
        lblStatistics->setText(QString().asprintf("Accuracy: %.1f%% ( %d / %d )", 100.0 * rightCnt / (rightCnt + wrongCnt), rightCnt, rightCnt + wrongCnt));
    }
}

// void GameWidget::goBack()
// {
//     // 实例化HomePage，并显示
//     HomePage *homePage = new HomePage();
//     // 显示主页并隐藏当前窗口
//     homePage->show();
//     this->hide();
// }

void GameWidget::goBack() // 已废弃
{
    // 发送返回信号
    emit backClicked();
}

void GameWidget::onGameStatusChanged(bool isStarted) {
    pauseButton->setEnabled(isStarted);  // 根据游戏状态启用或禁用 pauseButton
}


GameWidget::~GameWidget()
{
}

