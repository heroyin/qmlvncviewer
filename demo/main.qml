import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

import com.rooyeetone.vnc 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: qsTr("VNC viewer")

    Component.onCompleted: {
        vncViewer.start();
    }

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Connect..."
                onTriggered:{
                    console.log("connect")
                    vncViewer.start();
                }
            }
            MenuItem {
                text: "Close"
                onTriggered:{
                    console.log("close")
                    vncViewer.stop();
                }
            }
        }

        Menu {
            title: "View"
            MenuItem {
                text: "refresh"
                onTriggered: {
                    vncViewer.refresh();
                }
            }
            MenuItem { text: "Copy" }
            MenuItem { text: "Paste" }
        }
    }

    Rectangle{
        id:background
        anchors.fill: parent
        color: "black"

        ScrollView {
            id: scrollView
            anchors.centerIn: parent
            focus: true

            VncViewer {
                id: vncViewer
                focus: true

                host: "192.168.0.14"
                port: 13000
                password: "123456"
            }
        }

        onWidthChanged: {
            adjustView()
        }

        onHeightChanged: {
            adjustView()
        }
    }

    Connections{
        target: vncViewer

        onOnPropChanged:{
            adjustView()
        }

    }

    function adjustView(){
        scrollView.width = vncViewer.serverWidth+16>background.width?background.width:vncViewer.serverWidth+16
        scrollView.height = vncViewer.serverHeight+16>background.height?background.height:vncViewer.serverHeight+16
    }

}
