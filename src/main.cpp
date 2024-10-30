#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set the icon
    a.setWindowIcon(QIcon(":/icon/images/signify.ico")); // 使用资源文件或直接路径

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Signify_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // Load the global default custom font
    int fontId_regular = QFontDatabase::addApplicationFont(":/HamonyOS/fonts/HarmonyOS Sans/HarmonyOS_Sans_TC/HarmonyOS_Sans_TC_Regular.ttf");

    if (fontId_regular == -1) {
        qDebug() << "Failed to load font!";
    } else {
        // 获取字体系列名称
        QString fontFamily_regular = QFontDatabase::applicationFontFamilies(fontId_regular).at(0);

        // 创建 QFont 对象
        QFont customFont;
        customFont.setFamily(fontFamily_regular);
        // customFont.setPixelSize(12);  // 你可以根据需要调整字体大小
        // customFont.setBold(false);    // 根据需要设置粗体

        // 设置全局字体
        QApplication::setFont(customFont);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
