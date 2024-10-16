#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // 禁用放大按钮（可以取消），保证分辨率一致
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    // 创建 QStackedWidget
    stackedWidget = new QStackedWidget(this);

    // 创建 HomePage 和 GameWidget
    homePage = new HomePage(this);
    studyWidget = new StudyWidget(this);
    gameWidget = new GameWidget(this); // GameWidget 现在不需要传递HomePage

    // 将 HomePage 和 GameWidget 添加到 QStackedWidget 中
    stackedWidget->addWidget(homePage);   // 页面 0
    stackedWidget->addWidget(studyWidget);
    stackedWidget->addWidget(gameWidget); // 页面 1

    // // 设置布局
    // QVBoxLayout *layout = new QVBoxLayout(this);
    // layout->addWidget(stackedWidget);
    // setLayout(layout);

    // 设置初始显示为 PracticePage（后续更改一下）默认打开是哪个页面就写这里，下面还要确定哪个按钮checked
    stackedWidget->setCurrentWidget(gameWidget);

    // =====================================创建左侧的导航栏==================================================

    navigationBar = new QFrame(this);
    navigationBar->setFrameShape(QFrame::Box); // 设置边框样式（可选）
    navigationBar->setFixedWidth(270);         // 设置导航栏固定宽度 270px

    // 添加一些示例内容到导航栏中（如按钮）
    navLayout = new QVBoxLayout(navigationBar);

    // Home按钮，如果不需要则注释掉
    // btnHome = new QPushButton("Home", this);
    // navLayout->addWidget(btnHome);

    // Study按钮 ==============================================
    btnStudy = new QPushButton("Study", this);
    btnStudy->setCheckable(true);  // 设置为可选中按钮
    btnStudy->setStyleSheet(
        "QPushButton {"
        "background-color: transparent;"
        "color: #737B89;"
        "border-radius: 10px;"
        "padding: 10px 15px;"
        "font-size: 16px;"
        "text-align: left;"
        "}"
        "QPushButton:hover {"
        "background-color: #E4E8EC;"
        "}"
        "QPushButton:checked {"
        "background-color: #FC3D4A;"
        "color: white;"
        "}"
        );
    btnStudy->setIcon(QIcon(":/navigator_icons/icons/study_icon_gray.png"));  // 添加图标
    // 当选中和未选中时改变图标
    connect(btnStudy, &QPushButton::toggled, [=](bool checked){
        if (checked) {
            // 选中时设置白色图标
            btnStudy->setIcon(QIcon(":/navigator_icons/icons/study_icon_white.png"));
        } else {
            // 未选中时设置灰色图标
            btnStudy->setIcon(QIcon(":/navigator_icons/icons/study_icon_gray.png"));
        }
    });
    btnStudy->setIconSize(QSize(20, 20));
    btnStudy->setFixedHeight(50);  // 固定按钮高度
    navLayout->addWidget(btnStudy);


    // Game按钮，复用Study按钮的stylesheet ==================================================
    btnGame = new QPushButton("Game", this);
    btnGame->setCheckable(true);  // 设置为可选中按钮
    btnGame->setStyleSheet(btnStudy->styleSheet());
    btnGame->setIcon(QIcon(":/navigator_icons/icons/game_icon_gray.png"));  // 添加图标
    // 当选中和未选中时改变图标
    connect(btnGame, &QPushButton::toggled, [=](bool checked){
        if (checked) {
            // 选中时设置白色图标
            btnGame->setIcon(QIcon(":/navigator_icons/icons/game_icon_white.png"));
        } else {
            // 未选中时设置灰色图标
            btnGame->setIcon(QIcon(":/navigator_icons/icons/game_icon_gray.png"));
        }
    });
    btnGame->setIconSize(btnStudy->iconSize());
    btnGame->setFixedHeight(btnStudy->height());  // 固定按钮高度
    navLayout->addWidget(btnGame);


    // Record按钮，复用Study按钮的stylesheet =========================================
    btnRecord = new QPushButton("Record", this);
    btnRecord->setCheckable(true);  // 设置为可选中按钮
    btnRecord->setStyleSheet(btnStudy->styleSheet());
    btnRecord->setIcon(QIcon(":/navigator_icons/icons/record_icon_gray.png"));  // 添加图标
    // 当选中和未选中时改变图标
    connect(btnRecord, &QPushButton::toggled, [=](bool checked){
        if (checked) {
            // 选中时设置白色图标
            btnRecord->setIcon(QIcon(":/navigator_icons/icons/record_icon_white.png"));
        } else {
            // 未选中时设置灰色图标
            btnRecord->setIcon(QIcon(":/navigator_icons/icons/record_icon_gray.png"));
        }
    });
    btnRecord->setIconSize(btnStudy->iconSize());
    btnRecord->setFixedHeight(btnStudy->height());  // 固定按钮高度
    navLayout->addWidget(btnRecord);

    navLayout->addStretch(); // 将控件推到上方

    // 创建按钮组，设置为互斥模式
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->setExclusive(true);  // 互斥模式，保证只能选中一个按钮

    // 将按钮添加到按钮组
    btnGroup->addButton(btnStudy);
    btnGroup->addButton(btnGame);
    btnGroup->addButton(btnRecord);

    // 设置默认选中 "Game" 按钮
    btnGame->setChecked(true);  // 默认打开是哪个页面就写这里

    // 创建主布局，水平布局
    mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(navigationBar);     // 左侧导航栏
    mainLayout->addWidget(stackedWidget);     // 右侧的 QStackedWidget

    setLayout(mainLayout); // 设置主窗口布局

    // 相关按钮连接
    connect(homePage, &HomePage::practiceClicked, this, &MainWindow::showGamePage);
    // 连接 GameWidget 的返回按钮信号到切换回HomePage的槽函数
    // connect(gameWidget, &GameWidget::backClicked, this, &MainWindow::showHomePage);

    // 示例：连接导航栏的按钮与页面切换
    // connect(btnHome, &QPushButton::clicked, this, &MainWindow::showHomePage);
    connect(btnStudy, &QPushButton::clicked, this, &MainWindow::showStudyPage);
    connect(btnGame, &QPushButton::clicked, this, &MainWindow::showGamePage);
}


// 控制画面的初始位置居中
void MainWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    // 获取当前屏幕的几何信息
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    // 计算窗口的左右居中的水平位置 (x 坐标)
    int x = (screenGeometry.width() - this->width()) / 2;

    // 将窗口的顶部对齐屏幕顶部 (y 坐标为 0)
    int y = (screenGeometry.height() - this->height()) / 2;

    // 移动窗口到计算出的 (x, y) 位置
    this->move(x, y);
}

void MainWindow::showHomePage()
{
    // 切换回 HomePage
    stackedWidget->setCurrentWidget(homePage);
    homePage->setFocus();
}

void MainWindow::showStudyPage()
{
    // 切换到 StudyWidget
    stackedWidget->setCurrentWidget(studyWidget);
    studyWidget->setFocus();
}


void MainWindow::showGamePage()
{
    // 切换到 GameWidget
    stackedWidget->setCurrentWidget(gameWidget);
    gameWidget->setFocus();
}


