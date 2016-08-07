import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import en.wtl.org 1.0
import en.wtl.model 1.0

Pane{

    id: view


    ListView {
        width: 200; height: 250

        model: myModel
        delegate: Component{
            RowLayout{
                Text {
                    id: name
                    text: title
                    visible: true


                }
                Button{
                    text: "delete"
                    onClicked: {
                        dbman.del(id)
                        mod.deletepages(model.index)
                        console.log(path);

                    }
                }
            }
        }

    }
}


