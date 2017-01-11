import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import en.wtl.org 1.0
import en.wtl.model 1.0
import QtWebView 1.1

Pane{

    id: view
    property string local_url : ""
    property string pid : ""


    ListView {
        width: 200; height: 250

        model: myModel
        delegate: Component{
            RowLayout{
                Button {
                    id: name
                    text: title
                    visible: true
                    Layout.fillWidth: true
                    onClicked: {

                        local_url = path+"/WTL_appdata/"+id+"/"+id+".html"
                        console.log(local_url);
                        pid = id;
                        //webview.url = local_url
                        webview.visible  = true
                    }

                    }
                }

            }
        }




WebView{
    id: webview
    url: "file:///"+path+"/WTL_appdata/"+pid+"/"+pid+".html"
    visible: false
    anchors.fill: parent

    }

}

