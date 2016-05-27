import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4



Window {
    id:root
    visible: true
    width : Screen.width /4
    height: Screen.height/4


Column{

    id : menu
    anchors.centerIn: parent
    spacing : 8

    Button{
        id : view

        width: root.width
        height: root.height /3
        text: "SHOW PAGE"
        Loader{
            id : webview
        }

        onClicked: {
            onClicked: webview.source = "Mywebview.qml"

        }
    }
    Button{
        id : save
        width: root.width
        height: root.height /3
        text: "SAVE PAGE"
        onClicked: {

        }
    }
    Button{
        id : exit
        width: root.width
        height: root.height /3
        text: "EXIT"
        onClicked: {
            Qt.quit();
        }
    }


    }

}
