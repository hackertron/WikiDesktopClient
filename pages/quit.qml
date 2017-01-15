import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2

Item {
   id: root
   width: 580
   height: 400
   SystemPalette { id: palette }
   clip: true

    MessageDialog {
        id: quitDialog
        text: "Do you wish to quit?"
        visible: false
        icon : StandardIcon.Question
        modality: Qt.ApplicationModal
        standardButtons: StandardButton.Yes | StandardButton.No
        onAccepted: Qt.quit()
        Component.onCompleted: visible = true
        onYes : Qt.quit();
        onNo: console.log("Not quitting")
    }
}
