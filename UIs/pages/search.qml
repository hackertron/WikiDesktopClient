import QtQuick 2.0
import QtWebView 1.1

Item {


    var search_text ;

    gallery{

    id :search


    WebView{
      id:webview
      anchors.fill: parent
      Component.onCompleted: url= "http://en.wikitolearn.org/Main_Page"

    }

    }

}
