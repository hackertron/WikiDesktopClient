import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

import en.wtl.org 1.0

ApplicationWindow {
    visible: true
    width: 600
    height: 400
    color : "black"
    opacity: 0.5

    Dialog {
        id: myDialog
        visible: false
        title: "Delete Page"
        contentItem: Rectangle {
            color: "orange"

            implicitWidth: 400
            implicitHeight: 100
            Column {
                Text {
                    text: "Enter Page ID !"
                }
                TextField {
                    id: myTextField
                    width: 300
                    text: ""
                }
                Row {
                    Button {
                        text: "Ok"
                        onClicked: {
                            responseText.text = dbman.del(myTextField.text)
                            myDialog.close()
                        }
                    }
                    Button {
                        text: "Cancel"
                        onClicked: myDialog.close()
                    }
                }
            }
        }
    }

    Row {
        spacing: 20
        Button {
            text: "Delete"
            onClicked: myDialog.open();
        }
        Text {
            id: responseText
        }
    }

}
