import QtQuick 2.2
import QtQuick.Dialogs 1.0
import QtWebKit 3.0
import QtWebView 1.1
import QtQuick.Controls 1.0
import QtQuick.Window 2.0



Window {
    id:webview
    visible: true
    width: 600
    height: 400

FileDialog {
    id: fileDialog
    nameFilters: [ "HTML files (*.htm *.html)", "All files (*)" ]

    title: "Please choose a file"
    folder: shotrcuts.home
    onAccepted: {
        console.log("You chose: " + fileDialog.fileUrls)
        web.url(fileDialog.fileUrls)

       // Qt.quit()
    }
    onRejected: {
        console.log("Canceled")
        Qt.quit()
    }
    Component.onCompleted: visible = true
}
WebView{
    id : web
    anchors.fill: parent

}

}
