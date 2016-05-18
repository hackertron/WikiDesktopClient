import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    visible: true

    MouseArea {
        anchors.fill: parent
        onClicked: {
            parsing();
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
    }

    Text {

        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
}

