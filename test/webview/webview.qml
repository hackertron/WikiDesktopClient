import QtQuick 2.5
import QtQuick.Window 2.2
import QtWebEngine 1.1
import QtWebKit 3.0


Window {
    visible: true
    width: 600
    height: 400

    function parsing() {
        var http = new XMLHttpRequest();
        var json , parse , text ;

        http.onreadystatechange = function(){
            if(http.readyState == 4 && http.status == 200)
            { json = http.responseText;

                parse = JSON.parse(json);

                text = parse.parse.text["*"];
                console.log(text);
                webEngineView.loadHtml(text);  // <-- LOOK HERE
                return (text);
            }
        };
        http.open('GET','http://en.wikitolearn.org/api.php?action=parse&page=Linear%20Algebra/Sets&format=json');
        http.send();
    }


    WebView{
        id: webEngineView
        anchors.fill: parent
    }

    Component.onCompleted: parsing()
}
