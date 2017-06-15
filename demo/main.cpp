#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    qputenv("QSG_RENDER_LOOP", "basic");
    qputenv("QT_LOGGING_RULES", "qt.qpa.gl=true");
    qputenv("QT_OPENGL_BUGLIST", ":/openglblacklist.json");

    QGuiApplication app(argc, argv);

    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL, true);

    QQmlApplicationEngine engine;
    engine.addImportPath(QString(QGuiApplication::applicationDirPath()+"/imports"));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
