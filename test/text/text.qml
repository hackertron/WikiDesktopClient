import QtQuick 2.5
import QtQuick.Window 2.2
import QtWebKit 3.0

Window {
    visible: true

    MouseArea {
        anchors.fill: parent
        onClicked: {
            parsing();

        }
        function parsing() {
        var http = new XMLHttpRequest();
       var json , parse , text ;

         http.onreadystatechange = function(){
           if(http.readyState == 4 && http.status == 200)
           { json = http.responseText;
            // alert(json);
            parse = JSON.parse(json);
           // console.log(parse.parse.text["*"]);
               text = parse.parse.text["*"];
               console.log(text);
               label.text = text
           }
         };
               http.open('GET','http://en.wikitolearn.org/api.php?action=parse&page=Intro%20cryptography&format=json');
              http.send();
    }
    }

    Text {
        id:label
       
        text:  "click me "
        anchors.centerIn: parent
    }


   }



