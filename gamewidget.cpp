#include "gamewidget.h"  // 头文件
#include "homepage.h"

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
    spinMaxLvl->setRange(1, 50);
    spinMaxLvl->setAlignment(Qt::AlignCenter);
    spinMaxLvl->setPrefix("Level: ");
    spinMaxLvl->setFixedHeight(30);
    spinMaxLvl->setFocusPolicy(Qt::NoFocus);  // 用户无法通过键盘焦点操作它

    // 记忆用户之前选取的最高难度等级
    QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
    spinMaxLvl->setValue(settings.value("MaxLvl", 1).toInt());

    connect(spinMaxLvl, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value) {
        QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
        settings.setValue("MaxLvl", value);
    });

    // 正确率统计模块
    lblStatistics = new QLabel("Accuracy: 0% (0 / 0)");
    lblStatistics->setFixedHeight(30);
    lblStatistics->setAlignment(Qt::AlignCenter);
    lblStatistics->setStyleSheet("background-color:white");

    // 返回键设置
    backbutton = new QPushButton("Back to Home", this);
    // 连接返回按钮的点击信号到槽函数
    connect(backbutton, &QPushButton::clicked, this, &GameWidget::goBack);

    layout->addWidget(lblGameWindow, 0, 0, 1, 3);
    layout->addWidget(spinMaxLvl, 1, 0);
    layout->addWidget(lblStatistics, 1, 1);
    layout->addWidget(backbutton, 1, 2);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    setFixedSize(800, 800);
    setWindowTitle("Signify - Game Mode");
}

int GameWidget::getMaxLvl(void) {
    return spinMaxLvl->value();
}

void GameWidget::keyReleaseEvent(QKeyEvent *ev) {
    switch(ev->key()) {
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
        //qDebug() << ev->key();
        break;
    }
}

void GameWidget::keyPressEvent(QKeyEvent *ev) {
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
            //qDebug() << ev->key();
            break;
        }

        if((keyPressFlag[0] && keyPressFlag[1] && keyPressFlag[2] && keyPressFlag[3]) ||
                (keyPressFlag[0+4] && keyPressFlag[1+4] && keyPressFlag[2+4] && keyPressFlag[3+4])) {
            lblGameWindow->setRunning(true);
        }
    } else {
        if(ev->isAutoRepeat())
            return;
        lblGameWindow->getKey(ev->text());
    }
}

void GameWidget::updateStatistic(int rightCnt, int wrongCnt) {
    qDebug() << rightCnt << wrongCnt;
    if(rightCnt == 0 && wrongCnt == 0)
    {
        lblStatistics->setText("正确率: 0% (0 / 0)");
    }
    else
    {
        lblStatistics->setText(QString("正确率: %1% ( %2 / %3 )")
                                   .arg(100.0 * rightCnt / (rightCnt + wrongCnt), 0, 'f', 1)  // 格式化为小数点后一位
                                   .arg(rightCnt)
                                   .arg(rightCnt + wrongCnt));
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

void GameWidget::goBack()
{
    // 发送返回信号
    emit backClicked();
}

GameWidget::~GameWidget()
{
}

