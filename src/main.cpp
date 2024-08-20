#include <QApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QScreen>
#include "csvviewer.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    // Enable high DPI scaling
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);
    QApplication::setApplicationName("Qt CSV Viewer");
    QApplication::setApplicationVersion("1.0");

    // Set the application's DPI awareness
    app.setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    // Optional: Print the current screen's DPI for debugging
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        qDebug() << "Logical DPI:" << screen->logicalDotsPerInch();
        qDebug() << "Physical DPI:" << screen->physicalDotsPerInch();
    }

    QCommandLineParser parser;
    parser.setApplicationDescription("Simple CSV Viewer");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "CSV file to open");
    
    QCommandLineOption headerOption(QStringList() << "H" << "header",
        "Use first line as header");
    parser.addOption(headerOption);

    parser.process(app);

    const QStringList args = parser.positionalArguments();
    QString filename;

    if (args.isEmpty()) {
        qWarning("No CSV file specified. Use --help for usage information.");
        return 1;
    } else {
        filename = args.first();
        QFileInfo fileInfo(filename);
        if (!fileInfo.exists() || !fileInfo.isFile()) {
            qWarning("Specified file does not exist or is not a regular file.");
            return 1;
        }
    }

    bool useHeader = parser.isSet(headerOption);

    CSVViewer viewer;
    viewer.loadCSV(filename, useHeader);
    viewer.show();

    return app.exec();
}