import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

import en.wtl.org 1.0

ApplicationWindow {
    visible: true
    width: 60
    height: 40
    color: "black"

    Dialog {
        id: myDialog
        visible: false
        title: "Enter page Title"
        contentItem: Rectangle {
            color: "orange"
            implicitWidth: 400
            implicitHeight: 100
            Column {
                Text {
                    text: "Enter title!"
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
                            responseText.text = dbman.add(myTextField.text)
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
            text: "Add"
            onClicked: myDialog.open();
        }
        Text {
            id: responseText
        }
    }

}
