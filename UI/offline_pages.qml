import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import Qt.labs.folderlistmodel 2.1

Pane{

    ListView {
        width: 200; height: 250

        model: myModel
        delegate: Text { text: "pages: " + title + ", " + id }

    }
}


