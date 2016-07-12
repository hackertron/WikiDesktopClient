import QtQuick 2.6
import QtQuick.Controls 2.0
import QtWebView 1.1

Pane {
    id: pane




    WebView{
        id:webview
        anchors.fill: parent

        Component.onCompleted: url = "http://en.wikitolearn.org/"
    }
}
