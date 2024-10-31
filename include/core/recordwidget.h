/**
 * @class RecordWidget
 * @brief A QWidget subclass that displays user progress, best score, and social media links.
 *
 * The RecordWidget class is designed to show a user's profile details, including avatar,
 * username, motivational message, learning progress, learning time, and highest game score.
 * Additionally, it provides social media icons at the bottom for the user to "Follow Us."
 *
 * Key Features:
 * - Circular avatar display.
 * - Dynamic progress and score updating.
 * - Configurable and updatable best score, learning progress, and learning time.
 * - Social media icons for engagement.
 *
 * This widget is intended to be used in applications that want to keep track of the user's
 * learning progress and encourage social engagement.
 */

#ifndef WIDGET_H
#define WIDGET_H
// #include "./ui_widget.h"
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QPushButton>
#include <QProgressBar>
#include <QPixmap>
#include <QPainter>
#include <QSettings>
#include <QStandardPaths>
#include <QFontDatabase>

class RecordWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RecordWidget(QWidget *parent = nullptr);
    ~RecordWidget();

    void updateLearningProgressAndBestScore(int progress, int total, int score);

private:
    // Ui::Widget *ui;
    QLabel *learningProgressText;
    QLabel *learningTimeValue;
    QLabel *learningProgressValue;
    QLabel *bestScoreText;
    QLabel *bestScoreValue;
    QVBoxLayout *rightInfoLayout;
    QHBoxLayout *mainLayout;

public slots:
    void setTextHighScore();
    void setTextLearningProgress(int count);
};

#endif // WIDGET_H
