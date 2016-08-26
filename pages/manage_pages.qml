import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import en.wtl.org 1.0
import en.wtl.model 1.0


Pane {
    padding: 0







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
            model: myModel
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

                        Text {
                            id: name
                            text: title
                            padding: 10
                            visible: true
                            Layout.fillWidth: true


                        }

                        Button{
                            id: delete_button
                            text: "delete"

                            onClicked: {
                                dbman.del(id)
                                mod.deletepages(model.index);
                                console.log(path);

                            }
                        }
                        Button{
                            id: update_button

                            text: "update"
                            onClicked: {
                                dbman.update_page(id);
                                mod.update(id,model.index);
                                console.log(path);

                            }
                        }



                    }
                }
            }
        }









        RowLayout{
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

            Button{
                text:"Update All"
                onClicked: {
                    dbman.update();
                }


            }
            Button{
                text:"Delete All"
                onClicked: {
                    mod.deletelist();
                    dbman.deleteAll();
                }
            }
        }
    }
}
