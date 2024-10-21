#include "widget.h"
#include "./ui_widget.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QPixmap>
#include <QPainter>
Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 用户头像与信息布局
    QHBoxLayout *userLayout = new QHBoxLayout();

    // 创建头像标签
    QLabel *avatar = new QLabel(this);
    QPixmap avatarPixmap(":/images/user.jpg");  // 头像图片路径
    avatarPixmap = avatarPixmap.scaled(148, 148, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 强制缩放为正方形

    // 创建一个圆形的蒙版
    QBitmap mask(148, 148);
    mask.fill(Qt::color0);  // 清空背景
    QPainter painter(&mask);
    painter.setBrush(Qt::color1);  // 用圆形填充
    painter.drawEllipse(0, 0, 148, 148);  // 创建一个148x148的圆

    // 将蒙版应用到头像
    avatarPixmap.setMask(mask);  // 设置蒙版，确保图片为圆形
    avatar->setPixmap(avatarPixmap);  // 设置头像
    avatar->setFixedSize(148, 148);  // 确保 QLabel 宽高相等

    // 将头像放入布局中
    QHBoxLayout *avatarLayout = new QHBoxLayout();
    avatarLayout->addWidget(avatar);
    avatarLayout->setAlignment(Qt::AlignCenter);  // 头像居中

    mainLayout->addLayout(avatarLayout);


    // 用户名和状态信息布局
    QVBoxLayout *userInfoLayout = new QVBoxLayout();
    QLabel *username = new QLabel("<b>code_all_night</b>", this);
    username->setStyleSheet("color: rgba(16,16,16,1); font-size: 28px; font-family: 'HarmonyOS_Sans_TC-black';");
    QLabel *status = new QLabel("Eating, sleeping and playing", this);
    status->setStyleSheet("color: rgba(115,123,137,1); font-size: 20px; font-family: 'HarmonyOS_Sans_TC-bold';");

    userInfoLayout->addWidget(username);
    userInfoLayout->addWidget(status);

    // 将头像和用户信息布局添加到 userLayout 中
    userLayout->addWidget(avatar);  // 头像放在左边
    userLayout->addLayout(userInfoLayout);  // 用户名和标语放在右边

    // 将用户头像与信息的整体布局居中
    QHBoxLayout *centeredUserLayout = new QHBoxLayout();
    centeredUserLayout->addLayout(userLayout);
    centeredUserLayout->setAlignment(Qt::AlignCenter);  // 整体居中

    mainLayout->addLayout(centeredUserLayout);  // 将居中的用户布局添加到主布局

    // 学习进度图标和标签
    QLabel *learningProgressIcon = new QLabel(this);
    QPixmap progressPixmap(":/images/learn.png");
    learningProgressIcon->setPixmap(progressPixmap.scaled(30, 30, Qt::KeepAspectRatio));  // 将图标放大到 30x30

    QLabel *learningProgressText = new QLabel("Learning progress: ", this);
    learningProgressText->setStyleSheet("color: rgba(16,16,16,0.78); font-size: 24px; font-family: 'HarmonyOS_Sans_TC-bold';");  // 增大字体大小到 24px

    QLabel *learningProgressValue = new QLabel("15/36", this);
    learningProgressValue->setStyleSheet("color: rgba(16,16,16,0.78); font-size: 24px; font-family: 'HarmonyOS_Sans_TC-bold';");  // 增大字体大小到 24px

    // 学习进度布局
    QHBoxLayout *progressLayout = new QHBoxLayout();
    progressLayout->addWidget(learningProgressIcon);
    progressLayout->addWidget(learningProgressText);
    progressLayout->addWidget(learningProgressValue);
    progressLayout->setAlignment(Qt::AlignCenter);  // 水平居中

    mainLayout->addLayout(progressLayout);  // 将学习进度布局添加到主布局

    // 最高分图标和标签
    QLabel *bestScoreIcon = new QLabel(this);
    QPixmap scorePixmap(":/images/score.png");
    bestScoreIcon->setPixmap(scorePixmap.scaled(30, 30, Qt::KeepAspectRatio));  // 将图标放大到 30x30

    QLabel *bestScoreText = new QLabel("Best score: ", this);
    bestScoreText->setStyleSheet("color: rgba(16,16,16,0.78); font-size: 24px; font-family: 'HarmonyOS_Sans_TC-bold';");  // 增大字体大小到 24px

    QLabel *bestScoreValue = new QLabel("53", this);
    bestScoreValue->setStyleSheet("color: rgba(16,16,16,0.78); font-size: 24px; font-family: 'HarmonyOS_Sans_TC-bold';");  // 增大字体大小到 24px

    // 最高分布局
    QHBoxLayout *scoreLayout = new QHBoxLayout();
    scoreLayout->addWidget(bestScoreIcon);
    scoreLayout->addWidget(bestScoreText);
    scoreLayout->addWidget(bestScoreValue);
    scoreLayout->setAlignment(Qt::AlignCenter);  // 水平居中

    mainLayout->addLayout(scoreLayout);  // 将最高分布局添加到主布局


    // 创建 QLabel "Follow us" 标签
    QLabel *followUsLabel = new QLabel("Follow us", this);
    followUsLabel->setStyleSheet("color: rgba(115,123,137,1); font-size: 16px; font-family: 'HarmonyOS_Sans_TC-bold';");
    followUsLabel->setAlignment(Qt::AlignCenter);

    // 社交图标布局
    QHBoxLayout *socialLayout = new QHBoxLayout();
    socialLayout->setSpacing(15);  // 设置图标之间的水平间距

    QLabel *icon1 = new QLabel(this);
    QPixmap pixmap1(":/images/tiktok.png");
    icon1->setPixmap(pixmap1.scaled(24, 24, Qt::KeepAspectRatio));
    socialLayout->addWidget(icon1);

    QLabel *icon2 = new QLabel(this);
    QPixmap pixmap2(":/images/facebook.png");
    icon2->setPixmap(pixmap2.scaled(24, 24, Qt::KeepAspectRatio));
    socialLayout->addWidget(icon2);

    QLabel *icon3 = new QLabel(this);
    QPixmap pixmap3(":/images/instagram.png");
    icon3->setPixmap(pixmap3.scaled(24, 24, Qt::KeepAspectRatio));
    socialLayout->addWidget(icon3);

    QLabel *icon4 = new QLabel(this);
    QPixmap pixmap4(":/images/weibo.png");
    icon4->setPixmap(pixmap4.scaled(24, 24, Qt::KeepAspectRatio));
    socialLayout->addWidget(icon4);

    // 社交图标布局居中
    QHBoxLayout *centeredSocialLayout = new QHBoxLayout();
    centeredSocialLayout->addLayout(socialLayout);
    centeredSocialLayout->setAlignment(Qt::AlignCenter);  // 水平居中社交图标

    // 添加弹性空间使 "Follow us" 和社交图标位于页面底部
    mainLayout->addStretch();  // 添加弹性空间确保底部内容位于底部
    mainLayout->addWidget(followUsLabel, 0, Qt::AlignCenter);  // "Follow us" 标签居中
    mainLayout->addLayout(centeredSocialLayout);  // 添加社交图标


}

void Widget::updateLearningProgressAndBestScore(int progress, int total, int score)
{
    learningProgressValue->setText(QString("%1/%2").arg(progress).arg(total));
    bestScoreValue->setText(QString("%1").arg(score));
}

Widget::~Widget()
{
    delete ui;
}

