#include "studywidget.h"

/**
 * @brief Constructs a StudyWidget, initializes UI elements, loads progress from a configuration file, and sets up signals and slots.
 * @param parent The parent widget of StudyWidget, default is nullptr for an independent window.
 */
StudyWidget::StudyWidget(QWidget *parent)
    : QWidget(parent),
    configFilePath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/config.ini"),
    checkedCount(0)
{
    // Set up layout
    layout = new QGridLayout(this);

    // Load learning progress from the configuration file
    QSettings settings(configFilePath, QSettings::IniFormat);

    // Initialize letter descriptions for American Sign Language (ASL) alphabet
    letterDescriptions["A"] = "Make a fist with the thumb resting on the side of the other fingers.";
    letterDescriptions["B"] = "Hold the palm outward with the four fingers extended together and the thumb bent across the palm.";
    letterDescriptions["C"] = "Curve your fingers and palm to form the shape of the letter C facing outward.";
    letterDescriptions["D"] = "Extend the index finger upward while the other fingers form a fist with the thumb resting over the bent middle and ring fingers.";
    letterDescriptions["E"] = "Curl the fingers into a fist with the fingertips touching the palm, and place the thumb over the front of the fingers.";
    letterDescriptions["F"] = "Touch the thumb and index finger to form a circle, while the other three fingers are extended straight outward.";
    letterDescriptions["G"] = "Extend the index finger and thumb parallel to the ground, with the other fingers curled in.";
    letterDescriptions["H"] = "Extend the index and middle fingers together while the palm faces inward and the remaining fingers form a fist.";
    letterDescriptions["I"] = "Extend the pinky finger while the other fingers are in a fist, with the thumb resting on the side of the fist.";
    letterDescriptions["J"] = "Extend the pinky finger and curve it downward in a J motion, while the other fingers form a fist.";
    letterDescriptions["K"] = "Extend the index and middle fingers together, with the thumb resting between them, while the other fingers form a fist.";
    letterDescriptions["L"] = "Extend the thumb and index finger to form the shape of the letter L, while the other fingers are curled in.";
    letterDescriptions["M"] = "Form a fist, with the thumb tucked under the three fingers (index, middle, and ring fingers).";
    letterDescriptions["N"] = "Form a fist, with the thumb tucked under two fingers (index and middle fingers).";
    letterDescriptions["O"] = "Curve all fingers so that the fingertips touch, forming the shape of the letter O.";
    letterDescriptions["P"] = "Touch the thumb and index finger together at the tips, with the middle finger extended downward and the other fingers curled in.";
    letterDescriptions["Q"] = "Form a circle with the thumb and index finger, and extend the middle finger forward, while the other fingers are curled in.";
    letterDescriptions["R"] = "Cross the index and middle fingers, with the remaining fingers curled into a fist.";
    letterDescriptions["S"] = "Form a fist with the thumb resting over the front of the bent fingers.";
    letterDescriptions["T"] = "Form a fist with the thumb tucked between the index and middle fingers.";
    letterDescriptions["U"] = "Extend the index and middle fingers together, with the remaining fingers curled into a fist.";
    letterDescriptions["V"] = "Extend the index and middle fingers in a V shape, while the remaining fingers form a fist.";
    letterDescriptions["W"] = "Extend the index, middle, and ring fingers, spreading them apart, while the thumb and pinky are curled in.";
    letterDescriptions["X"] = "Curl the index finger to form a hook shape, while the remaining fingers form a fist.";
    letterDescriptions["Y"] = "Extend the pinky and thumb, while the other fingers are curled into the palm.";
    letterDescriptions["Z"] = "Extend the index finger and draw the shape of the letter Z in the air.";

    // Set up image and description labels
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);

    descriptionLabel = new QLabel(this);
    descriptionLabel->setAlignment(Qt::AlignCenter);  // Center the image
    descriptionLabel->setStyleSheet("color: white; font-size: 16px; border-radius: 5px");
    descriptionLabel->setFixedSize(500, 120);
    descriptionLabel->setWordWrap(true);

    // Initialize the letter menu with checkboxes for each letter
    letterMenu = new QListWidget(this);
    letterMenu->setFixedWidth(200);
    letterMenu->setFixedHeight(400);
    letterMenu->setVisible(true);

    // Set up the navigation buttons
    nextButton = new QPushButton("NEXT", this);
    nextButton->setFixedSize(150, 40);
    nextButton->setStyleSheet("QPushButton {"
                              "background-color: #fc3d4a;"
                              "color: white;"
                              "border-radius: 20px;"
                              "font-size: 20px;"
                              "padding: 10px;"
                              "}"
                              "QPushButton:hover {"
                              "background-color: darkred;"
                              "}"
                              );

    lastButton = new QPushButton("PREVIOUS", this);
    lastButton->setFixedSize(150, 40);
    lastButton->setStyleSheet("QPushButton {"
                              "background-color: #fc3d4a;"
                              "color: white;"
                              "border-radius: 20px;"
                              "font-size: 20px;"
                              "padding: 10px;"
                              "}"
                              "QPushButton:hover {"
                              "background-color: darkred;"
                              "}"
                              );

    // Add letters to the letter menu and load their checked state from the configuration file
    QStringList letters = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    for (const QString &letter : letters) {
        QListWidgetItem *item = new QListWidgetItem(letter, letterMenu);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);

        // OPEN ISSUE HERE: memory function is not activated!!!
        bool isChecked = settings.value(letter, false).toBool();
        item->setCheckState(isChecked ? Qt::Checked : Qt::Unchecked);

        if (isChecked) {
            checkedCount++;
        }

        letterMenu->addItem(item);
    }

    // Initialize the letter menu with checkboxes for each letter
    letterMenu->setStyleSheet(
        "QListWidget {"
        "    background-color: transparent;"
        "    border: 1px solid #D8DEE9;"
        "    border-radius: 5px;"
        "    color: black;"
        "    font-size: 24px;"
        "    padding: 10px;"
        "    margin-left: 15px;"
        "}"
        "QListWidget::item {"
        "    padding: 5px;"
        "}"
        "QListWidget::item:hover {"
        "    background-color: #f0f0f0;"
        "    border-radius: 5px;"
        "}"
        "QListWidget::item:selected {"
        "    background-color: #fc3d4a;"
        "    color: white;"
        "    border-radius: 5px;"
        "    margin: 2px;"
        "}"
        "QScrollBar:vertical {"
        "    width: 8px;"
        "    background: #f0f0f0;"
        "    margin: 0px 0px 0px 0px;"
        "    border-radius: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #c0c0c0;"
        "    min-height: 20px;"
        "    border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #a0a0a0;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "    background: transparent;"
        "}"
        );

    descriptionLabel->setStyleSheet("QLabel {"
                                    "border: 2px solid #D8DEE9;"
                                    "border-radius: 10px;"
                                    "background-color: transparent;"
                                    "color: black;"
                                    "padding: 10px;"
                                    "font-size: 18px;"
                                    "text-align: left;"
                                    "}");

    // Add widgets to the layout
    layout->addWidget(imageLabel, 0, 0, 1, 1, Qt::AlignRight);
    layout->setSpacing(30);
    layout->addWidget(descriptionLabel, 1, 0, 1, 1, Qt::AlignLeft);
    layout->addWidget(letterMenu, 0, 1, 2, 1, Qt::AlignRight);
    layout->setColumnStretch(1, 2);
    layout->setContentsMargins(0, 0, 90, 0);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(lastButton);
    buttonLayout->addWidget(nextButton);
    buttonLayout->addStretch();
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    layout->addLayout(buttonLayout, 2, 1, Qt::AlignCenter);

    // Set layout properties and window title
    layout->setContentsMargins(40, 40, 40, 0);
    setLayout(layout);
    setWindowTitle("Signify");
    setFixedSize(810, 720);
    this->setStyleSheet("background-color: #2E3440;");

    // Connect item check/uncheck to update learning progress
    connect(letterMenu, &QListWidget::itemChanged, this, [=](QListWidgetItem *item) {
        if (letterMenu && item) {
            if (item->checkState() == Qt::Checked) {
                checkedCount++;
            } else {
                checkedCount--;
            }
            // qDebug() << "当前勾选的字母数量：" << checkedCount;

            saveLearningProgress();
            emit checkedCountChanged(checkedCount);

            showLetterPage(item);
        }
    });

    // Connect item click to display the selected letter's page
    connect(letterMenu, &QListWidget::itemClicked, this, [=](QListWidgetItem *item) {
        if (letterMenu && item) {
            showLetterPage(item);
        }
    });

    // Connect navigation buttons to display the next or previous letter
    connect(nextButton, &QPushButton::clicked, this, [=]() {
        int currentRow = letterMenu->currentRow();
        if (currentRow < letterMenu->count() - 1) {
            letterMenu->setCurrentRow(currentRow + 1);
            showLetterPage(letterMenu->currentItem());
        }
    });

    connect(lastButton, &QPushButton::clicked, this, [=]() {
        int currentRow = letterMenu->currentRow();
        if (currentRow > 0) {
            letterMenu->setCurrentRow(currentRow - 1);
            showLetterPage(letterMenu->currentItem());
        }
    });

    // Save checked states when the widget is destroyed
    connect(this, &StudyWidget::destroyed, this, &StudyWidget::saveCheckedState);

    // Display the first letter by default
    showLetterPage(letterMenu->item(0));
}

/**
 * @brief Destructor for StudyWidget, saves checked state of letters and outputs debug info.
 */
StudyWidget::~StudyWidget() {
    saveCheckedState();
    // qDebug() << "StudyWidget is being destroyed";
}

/**
 * @brief Saves the current number of learned letters to the configuration file.
 */
void StudyWidget::saveLearningProgress()
{
    QSettings settings(configFilePath, QSettings::IniFormat);
    settings.setValue("LearningProgressCount", checkedCount);
}

/**
 * @brief Displays the specified letter’s image and description on the screen.
 * @param item The letter item selected in the letter menu.
 */
void StudyWidget::showLetterPage(QListWidgetItem *item)
{
    QString letter = item->text();
    QString imagePath = ":/study/images/study/" + letter + ".png";

    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        imageLabel->setPixmap(pixmap.scaled(300, 300, Qt::KeepAspectRatio));
        QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(imageLabel);
        shadowEffect->setBlurRadius(20);
        shadowEffect->setColor("gray");
        shadowEffect->setOffset(5, 5);
        imageLabel->setGraphicsEffect(shadowEffect);
    } else {
        qDebug() << "圖片加載失敗：" << imagePath;
    }

    // updating check
    if (letterDescriptions.contains(letter)) {
        descriptionLabel->setText(letterDescriptions[letter]);
    } else {
        descriptionLabel->setText("找不到這個字母的說明。");
    }

    // refresh the layout
    layout->update();
}

/**
 * @brief Gets the number of letters marked as learned by the user.
 * @return The count of letters checked as learned.
 */
int StudyWidget::getCheckedCount() const {
    return checkedCount;
}

/**
 * @brief Saves the checked state of each letter to the configuration file.
 */
void StudyWidget::saveCheckedState()
{
    QSettings settings(configFilePath, QSettings::IniFormat);
    settings.setValue("LearningProgressCount", checkedCount);
    // 遍历所有字母项并保存其勾选状态
    for (int i = 0; i < letterMenu->count(); ++i) {
        QListWidgetItem *item = letterMenu->item(i);
        QString letter = item->text();
        bool isChecked = (item->checkState() == Qt::Checked);
        settings.setValue(letter, isChecked);
    }
}

