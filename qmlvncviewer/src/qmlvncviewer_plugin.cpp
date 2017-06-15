#include "qmlvncviewer_plugin.h"
#include "qquickvncviewer.h"

#include <qqml.h>


void QmlvncviewerPlugin::registerTypes(const char *uri)
{
    // @uri com.rooyeetone.vnc
    qmlRegisterType<QQuickVncViewer>(uri, 1, 0, "VncViewer");
}



