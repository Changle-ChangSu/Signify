#include "recordwidget.h"
// #include "./ui_widget.h"

/**
 * @brief Constructs the RecordWidget and initializes its UI components.
 * @param parent The parent widget, if any.
 */
RecordWidget::RecordWidget(QWidget *parent)
    : QWidget(parent)
{
    // ui->setupUi(this);
    setFixedSize(810, 720);
    this->setStyleSheet("background-color: rgba(247, 249, 252, 1);");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    // ---------------------------profile photo ------------------------------
    QHBoxLayout *userLayout = new QHBoxLayout();
    QLabel *avatar = new QLabel(this);
    QPixmap avatarPixmap(":/record/images/record/user.png");

    avatarPixmap = avatarPixmap.scaled(148, 148, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QBitmap mask(148, 148);
    mask.fill(Qt::color0);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::color1);
    painter.drawEllipse(0, 0, 148, 148);
    avatarPixmap.setMask(mask);

    avatar->setPixmap(avatarPixmap);
    avatar->setFixedSize(148, 148);

    QHBoxLayout *avatarLayout = new QHBoxLayout();
    avatarLayout->addWidget(avatar);
    avatarLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(avatarLayout);


    // -------------------------User Info Layout--------------------------------
    QVBoxLayout *userInfoLayout = new QVBoxLayout();

    QLabel *username = new QLabel("<b>code_all_night</b>", this);
    // Load other custom font but not globally used
    int fontId1 = QFontDatabase::addApplicationFont(":/HamonyOS/fonts/HarmonyOS Sans/HarmonyOS_Sans_TC/HarmonyOS_Sans_TC_Black.ttf");
    QString fontFamily1 = QFontDatabase::applicationFontFamilies(fontId1).at(0);
    username->setStyleSheet(QString("color: rgba(16,16,16,1); font-size: 26px; font-family: '%1';").arg(fontFamily1));

    QLabel *status = new QLabel("Our vision: Empowering connection through sign language!", this);
    // Load other custom font but not globally used
    int fontId2 = QFontDatabase::addApplicationFont(":/HamonyOS/fonts/HarmonyOS Sans/HarmonyOS_Sans_TC/HarmonyOS_Sans_TC_Bold.ttf");
    QString fontFamily2 = QFontDatabase::applicationFontFamilies(fontId2).at(0);
    status->setStyleSheet(QString("color: rgba(252,61,74,1); font-size: 20px; font-family: '%1';").arg(fontFamily2));

    userInfoLayout->addWidget(username);
    userInfoLayout->addWidget(status);

    // -------------------Add the avatar and user information layout to userLayout--------------------------------

    userLayout->addWidget(avatar);
    userLayout->addSpacing(20);
    userLayout->addLayout(userInfoLayout);

    // Center the overall layout of user avatars and other infomation
    QHBoxLayout *centeredUserLayout = new QHBoxLayout();
    centeredUserLayout->addLayout(userLayout);
    centeredUserLayout->setAlignment(Qt::AlignCenter);  // centring

    mainLayout->addLayout(centeredUserLayout);
    mainLayout->addSpacing(60);

    // Create a container to control the alignment of the center
    QWidget *centerWidget = new QWidget(this);
    QVBoxLayout *centerLayout = new QVBoxLayout(centerWidget);
    centerWidget->setFixedWidth(400);

    // -------------------------------Learning Progress Line---------------------------------------

    QHBoxLayout *learningProgressLayout = new QHBoxLayout();
    QLabel *learningProgressIcon = new QLabel(this);
    QPixmap progressPixmap(":/record/images/record/learn.png");
    learningProgressIcon->setPixmap(progressPixmap.scaled(30, 30, Qt::KeepAspectRatio));

    learningProgressText = new QLabel("Learning Progress:", this);
    learningProgressValue = new QLabel(this);

    // load the learning progress data from the config file
    QString configFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/config.ini";
    QSettings settings(configFilePath, QSettings::IniFormat);
    int checkedCount = settings.value("LearningProgressCount", 0).toInt();
    int totalCount = 26;  // there are 26 letters in total

    QString progress = QString("%1/%2").arg(checkedCount).arg(totalCount);
    learningProgressValue->setText(progress);

    // set its format
    learningProgressText->setStyleSheet(QString("color: rgba(16,16,16,1); font-size: 22px; font-family: '%1';").arg(fontFamily2));
    learningProgressValue->setStyleSheet(QString("color: rgba(16,16,16,1); font-size: 22px; font-family: '%1';").arg(fontFamily2));

    // add to the learningProgressLayout
    learningProgressLayout->addWidget(learningProgressIcon);
    learningProgressLayout->addSpacing(5);
    learningProgressLayout->addWidget(learningProgressText, 0, Qt::AlignLeft);
    learningProgressLayout->addStretch();
    learningProgressLayout->addWidget(learningProgressValue, 0, Qt::AlignRight);

    // ---------------------------------Learning Time Line---------------------------------

    QHBoxLayout *learningTimeLayout = new QHBoxLayout();
    QLabel *learningTimeIcon = new QLabel(this);
    QPixmap timePixmap(":/record/images/record/time.png");
    learningTimeIcon->setPixmap(timePixmap.scaled(30, 30, Qt::KeepAspectRatio));

    QLabel *learningTimeText = new QLabel("Learning time", this);
    learningTimeText->setStyleSheet(QString("color: rgba(16,16,16,1); font-size: 22px; font-size: 22px; font-family: '%1';").arg(fontFamily2));

    learningTimeValue = new QLabel("4 Days", this);
    learningTimeValue->setStyleSheet(QString("color: rgba(16,16,16,1); font-size: 22px; font-size: 22px; font-family: '%1';").arg(fontFamily2));

    // add to the learningTimeLayout
    learningTimeLayout->addWidget(learningTimeIcon);
    learningTimeLayout->addSpacing(5);
    learningTimeLayout->addWidget(learningTimeText, 0, Qt::AlignLeft);
    learningTimeLayout->addStretch();
    learningTimeLayout->addWidget(learningTimeValue, 0, Qt::AlignRight);

    // -----------------------------------Best Score Line-------------------------------------

    QHBoxLayout *bestScoreLayout = new QHBoxLayout();
    QLabel *bestScoreIcon = new QLabel(this);
    QPixmap scorePixmap(":/record/images/record/score.png");
    bestScoreIcon->setPixmap(scorePixmap.scaled(30, 30, Qt::KeepAspectRatio));

    QLabel *bestScoreText = new QLabel("Best score", this);
    bestScoreText->setStyleSheet(QString("color: rgba(16,16,16,1); font-size: 22px; font-family: '%1';").arg(fontFamily2));

    // read the best score from the config file
    int bestScore = settings.value("HighScore", 0).toInt();
    bestScoreValue = new QLabel(QString::number(bestScore), this);
    bestScoreValue->setStyleSheet(QString("color: rgba(16,16,16,1); font-size: 22px; font-family: '%1';").arg(fontFamily2));

    // add to the bestScoreLayout
    bestScoreLayout->addWidget(bestScoreIcon);
    bestScoreLayout->addSpacing(5);
    bestScoreLayout->addWidget(bestScoreText, 0, Qt::AlignLeft);
    bestScoreLayout->addStretch();
    bestScoreLayout->addWidget(bestScoreValue, 0, Qt::AlignRight);

    // --------------------------------------------------------------------------------------------

    // add all things to the centerLayout
    centerLayout->addLayout(learningProgressLayout);
    centerLayout->addSpacing(30);
    centerLayout->addLayout(bestScoreLayout);
    centerLayout->addSpacing(30);
    centerLayout->addLayout(learningTimeLayout);

    // add centerLayout to the mainLayout
    mainLayout->addWidget(centerWidget, 0, Qt::AlignCenter);
    setLayout(mainLayout);

    // --------------------------------Follow us Part--------------------------------------------

    QLabel *followUsLabel = new QLabel("Follow us", this);
    followUsLabel->setStyleSheet(QString("color: rgba(115,123,137,1); font-size: 18px; font-family: '%1';").arg(fontFamily2));
    followUsLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *socialLayout = new QHBoxLayout();
    socialLayout->setSpacing(20);  // adding space between icons

    QLabel *icon1 = new QLabel(this);
    QPixmap pixmap1(":/record/images/record/tiktok.png");
    icon1->setPixmap(pixmap1.scaled(24, 24, Qt::KeepAspectRatio));
    socialLayout->addWidget(icon1);

    QLabel *icon2 = new QLabel(this);
    QPixmap pixmap2(":/record/images/record/facebook.png");
    icon2->setPixmap(pixmap2.scaled(24, 24, Qt::KeepAspectRatio));
    socialLayout->addWidget(icon2);

    QLabel *icon3 = new QLabel(this);
    QPixmap pixmap3(":/record/images/record/instagram.png");
    icon3->setPixmap(pixmap3.scaled(24, 24, Qt::KeepAspectRatio));
    socialLayout->addWidget(icon3);

    QLabel *icon4 = new QLabel(this);
    QPixmap pixmap4(":/record/images/record/weibo.png");
    icon4->setPixmap(pixmap4.scaled(24, 24, Qt::KeepAspectRatio));
    socialLayout->addWidget(icon4);

    QLabel *icon5 = new QLabel(this);
    QPixmap pixmap5(":/record/images/record/github.png");
    icon5->setPixmap(pixmap5.scaled(24, 24, Qt::KeepAspectRatio));
    socialLayout->addWidget(icon5);

    QHBoxLayout *centeredSocialLayout = new QHBoxLayout();
    centeredSocialLayout->addLayout(socialLayout);
    centeredSocialLayout->setAlignment(Qt::AlignCenter);

    mainLayout->addStretch();  // Add elastic space to make sure the bottom content is at the bottom
    mainLayout->addWidget(followUsLabel, 0, Qt::AlignCenter);  // centralise "Follow us"
    mainLayout->addLayout(centeredSocialLayout);

    mainLayout->setContentsMargins(0, 40, 0, 40);
}

/**
 * @brief Updates the high score display from saved configuration data.
 */
void RecordWidget::setTextHighScore() {
    // Read the highest score in the Settings file and display it
    QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
    int bestScore = settings.value("HighScore", 0).toInt();

    // If bestScoreValue already exists, update its text
    if (bestScoreValue) {
        bestScoreValue->setText(QString::number(bestScore));
    } else {
        // If not, create the QLabel dynamically
        bestScoreValue = new QLabel(QString::number(bestScore), this);
    }
}

/**
 * @brief Updates the learning progress and best score display.
 * @param progress The number of completed letters or tasks.
 * @param total The total number of letters or tasks.
 * @param score The highest score achieved by the user.
 */
void RecordWidget::updateLearningProgressAndBestScore(int progress, int total, int score)
{
    learningProgressValue->setText(QString("%1/%2").arg(progress).arg(total));
    bestScoreValue->setText(QString("%1").arg(score));
}

/**
 * @brief Sets the learning progress display text.
 * @param count The current number of completed letters.
 */
void RecordWidget::setTextLearningProgress(int count) {
    QString progress = QString::number(count) + "/26";
    learningProgressValue->setText(progress);
}


/**
 * @brief Destructor for the RecordWidget.
 */
RecordWidget::~RecordWidget()
{
}



