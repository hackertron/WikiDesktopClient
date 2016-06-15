import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtWebEngine 1.1
import QtWebKit 3.0
import QtQuick 2.5



Window {
    id:root
    visible: true
    width : Screen.width /5
    height: Screen.height/5
    color : "black"

    MessageDialog {
        id: msg
        visible: false
        title: "Page Saved"
        text: "Requested  page has been saved"
        onAccepted: {
            console.log("OK!!")
        }

    }


Column{

    id : menu
    anchors.centerIn: parent
    spacing : 8


    Button{
        id : view

        width: root.width
        height: root.height /5
        text: "LOAD PAGE"


        onClicked: {
            //create loadwindow
            var component= Qt.createComponent("Mywebview.qml")
            var loadwin = component.createObject(root)
            loadwin.show()
        }
    }



    Button{
        id : save
        width: root.width
        height: root.height /5
        text: "SAVE PAGE"
        onClicked: {
                msg.visible = true
                dbm.add()

                
        }
    }

    Button{
        id : local
        width: root.width
        height: root.height /5
        text: "local PAGE"
        onClicked: {

            var component= Qt.createComponent("local.qml")
                        var local_win = component.createObject(root)
                        local_win.show()
            }

}

    Button{
        id : del
        width: root.width
        height: root.height /5
        text: "DELETE PAGE"
        onClicked: {
            msg.title = "Deleted"
            msg.text = "Page has been deleted"
            msg.icon = StandardIcon.Warning
            msg.visible = true
            dbm.del()
        }
    }
    Button{
        id : exit
        width: root.width
        height: root.height /5
        text: "EXIT"
        onClicked: {
            Qt.quit();
        }
    }


    }

}
