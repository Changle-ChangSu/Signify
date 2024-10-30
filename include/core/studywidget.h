/**
 * @class StudyWidget
 * @brief A widget that allows users to study sign language letters with descriptions and images and track their learning progress.
 *
 * The StudyWidget class provides an interface for users to study the American Sign Language alphabet.
 * It includes letter descriptions and images, and allows users to track their learning progress by marking letters as learned.
 * The progress is saved in a configuration file so users can resume their progress across sessions.
 *
 * ### Key Components
 * - `QLabel *imageLabel` - Displays the image of the currently selected sign language letter.
 * - `QLabel *descriptionLabel` - Displays a description of how to form the selected sign language letter.
 * - `QListWidget *letterMenu` - A list of sign language letters, allowing users to mark letters as learned with checkboxes.
 * - `QPushButton *nextButton`, `*lastButton` - Navigation buttons to move to the next or previous letter.
 * - `QMap<QString, QString> letterDescriptions` - A map containing descriptions of how to form each letter.
 * - `int checkedCount` - Tracks the number of letters the user has marked as learned.
 * - `QString configFilePath` - Path to the configuration file where progress is saved.
 *
 * ### Main Methods
 * - `void showLetterPage(QListWidgetItem *item)` - Displays the selected letter’s image and description.
 * - `void saveLearningProgress()` - Saves the number of letters the user has marked as learned.
 * - `void saveCheckedState()` - Saves the checked state of each letter in the configuration file.
 *
 * ### Signals
 * - `void checkedCountChanged(int count)` - Emitted whenever the count of learned letters changes.
 *
 */


#ifndef STUDYWIDGET_H
#define STUDYWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>
#include <QSettings>
#include <QFile>
#include <QStandardPaths>
#include <QObject>
#include <QGraphicsDropShadowEffect>


class StudyWidget : public QWidget
{
    Q_OBJECT

public:
    StudyWidget(QWidget *parent = nullptr);
    ~StudyWidget();
    int getCheckedCount() const;  // Public method to get the currently selected number
    void saveLearningProgress();

private slots:
    void showLetterPage(QListWidgetItem *item);
    void saveCheckedState();

signals:
    void checkedCountChanged(int count);  // Emitted whenever the count of learned letters changes.

private:
    QString configFilePath;
    int checkedCount;
    QLabel *learningProgressText;
    QLabel *learningProgressValue;
    QGridLayout *layout;
    QCamera *camera;
    QMediaCaptureSession captureSession;
    QVideoWidget *videoWidget;
    QLabel *imageLabel;
    QLabel *descriptionLabel;
    QListWidget *letterMenu;
    QPushButton *nextButton;
    QPushButton *lastButton;
    QMap<QString, QString> letterDescriptions;  // A map containing descriptions of how to form each letter.
};

#endif // STUDYWIDGET_H
