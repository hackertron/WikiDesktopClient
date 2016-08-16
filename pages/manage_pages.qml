import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0


Pane {
    padding: 0

    property var delegateComponentMap: {
        "page": itemDelegateComponent

    }

    Component {
        id: itemDelegateComponent

        ItemDelegate {
            text: labelText
            width: parent.width
        }
    }






    ColumnLayout {
        id: column
        spacing: 40
        anchors.fill: parent
        anchors.topMargin: 20

        Label {
            id: label1
            Layout.fillWidth: true
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Offline Pages "
        }

        ListView {
            id: listView

            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: ListModel {
                ListElement { type: "ItemDelegate"; labelText: "page1" }
                ListElement { type: "ItemDelegate"; labelText: "page2" }
                ListElement { type: "ItemDelegate"; labelText: "page3" }


            }
            spacing: 5

            section.property: "type"

            delegate: Component{

                Item{
                    id: aItem
                    width: listView.width //rowLayout.width. We got width from children elements before, now get width from listView
                    height: 30

                    RowLayout{

                        id: rowLayout
                        anchors.fill: parent
                        spacing: 10

                        Label{
                            id:page_name
                            padding: 10
                            text: labelText
                            Layout.fillWidth: true // !!! to fill most part of row width

                        }


                            Button{

                                text: qsTr("Delete")

                                id: delete_button

                            }
                            Button{

                                text: qsTr("Update")
                                id: update_button
                            }



                    }
                }
            }
        }









        RowLayout{
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

            Button{
                text:"Update All"


            }
            Button{
                text:"Delete All"
            }
        }
    }
}
