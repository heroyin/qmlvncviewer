#ifndef QMLVNCVIEWER_PLUGIN_H
#define QMLVNCVIEWER_PLUGIN_H

#include <QQmlExtensionPlugin>

class QmlvncviewerPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // QMLVNCVIEWER_PLUGIN_H
