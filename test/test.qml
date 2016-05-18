import QtQuick 2.0

Rectangle {
    width: 320
    height: 480
    ListView {
        id: view
        anchors.fill: parent
        delegate: Thumbnail {
            width: view.width
            text: modelData.title
            iconSource: modelData.media.m
        }


    function parsing() {
 var http = new XMLHttpRequest(); 
var json , parse ;

  http.onreadystatechange = function(){
    if(http.readyState == 4 && http.status == 200)
    { json = http.responseText;
     // alert(json);
     parse = JSON.parse(json); 
     console.log(parse.parse.text["*"]);
    }
  };
        http.open('GET','http://en.wikitolearn.org/api.php?action=parse&page=Intro%20cryptography&format=json');
       http.send();
}
    function webview(json){

    }

    Component.onCompleted: {
            parsing()
        }
}
}
