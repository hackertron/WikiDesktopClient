import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import Qt.labs.settings 1.0
import QtWebView 1.1
import QtQuick.Controls.Styles 1.4
import en.wtl.model 1.0

ApplicationWindow {
    id: window
    width: 520
    height: 320
    visible: true
    title: "WikiToLearn"

    property string searchterm: "";
    property alias searchwebview: webview;
    property string current_title : "";


    // function parsing is used to request and  parse the JSON returned by the API
    function parsing()
    {

        var http = new XMLHttpRequest();

        var json , parse , html , search;


        http.onreadystatechange = function(){


            /*
              checks if the
              request finished and response is ready ( readystate 4)
              200: "OK"
              */
            if(http.readyState == 4 && http.status == 200)
            {
                loading.visible=false
                json = http.responseText;
                parse = JSON.parse(json);
                search = parse.query.search;
                html = "<!doctype html>";
                html = html + "<html> <head> </head> <body>";
                html =  html + "<h5> Search results  </h5> <br />";
                for(var i in search){

                    var title = search[i].title; // get titles of search
                    var snippet = search[i].snippet; // small description of each title

                    title = "<a href = \"http://en.wikitolearn.org/api.php?action=parse&page="+title+ "&format=json\">"+title+"</a>";
                    html = html + title + "<br />";
                    html =  html + snippet + "<br /> <br />" ;

                }


                html = html + "<br />";
                html =  html+"</body> </html>";
                //console.log(html);

            }

            webview.loadHtml(html);
        };
        loading.visible=true
        http.open('GET',"http://en.wikitolearn.org/api.php?action=query&list=search&srsearch="+searchterm+"&format=json");
        http.send();
    }

    /* extract_json takes the requried info from the JSON
       that was passed by the API .

      */
    function extract_json()
    {

        var p_url = searchwebview.url

        console.log(p_url);


        var http = new XMLHttpRequest();
        var json , parse , text  , title;
        var styling = " <head>
 <link rel=\"stylesheet\" href=\"http://en.wikitolearn.org/load.php?debug=false&amp;lang=en&amp;modules=ext.gadget.ColiruCompiler%7Cext.math.desktop.styles%7Cext.math.styles%7Cext.visualEditor.desktopArticleTarget.noscript%7Cmediawiki.legacy.commonPrint%2Cshared%7Cmediawiki.sectionAnchor&amp;only=styles&amp;skin=neverland\"/>
<link rel=\"stylesheet\" href=\"http://en.wikitolearn.org/skins/Neverland/css/bootstrap.css?v=1.0.3?303\" media=\"screen\"/><link rel=\"stylesheet\" href=\"/skins/Neverland/css/bootstrap-social.css?303\" media=\"screen\"/><link rel=\"stylesheet\" href=\"/skins/Neverland/css/font-awesome-4.4.0/css/font-awesome.css?303\" media=\"screen\"/>
<meta name=\"ResourceLoaderDynamicStyles\" content=\"\"/>
<link rel=\"stylesheet\" href=\\http://en.wikitolearn.org/load.php?debug=false&amp;lang=en&amp;modules=site&amp;only=styles&amp;skin=neverland\"/>
<script async=\"\" src=\"http://en.wikitolearn.org/load.php?debug=false&amp;lang=en&amp;modules=startup&amp;only=scripts&amp;skin=neverland\"></script>
<meta name=\"generator\" content=\"MediaWiki 1.27.0\"/>
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/>
<link rel=\"shortcut icon\" href=\"http://en.wikitolearn.org/favicon.ico\"/>
<link rel=\"search\" type=\"application/opensearchdescription+xml\" href=\"/opensearch_desc.php\" title=\"WikiToLearn - collaborative textbooks (en)\"/>
<link rel=\"EditURI\" type=\"application/rsd+xml\" href=\"http://en.wikitolearn.org/api.php?action=rsd\"/>
<link rel=\"copyright\" href=\"/Project:Copyright\"/>
<link rel=\"alternate\" type=\"application/atom+xml\" title=\"WikiToLearn - collaborative textbooks Atom feed\" href=\"http://en.wikitolearn.org/index.php?title=Special:RecentChanges&amp;feed=atom\"/>
</head>";

        http.onreadystatechange = function(){
            if(http.readyState == 4 && http.status == 200)
            { json = http.responseText;

                parse = JSON.parse(json);

                text = parse.parse.text["*"];
                title = parse.parse.title;
                current_title = title;
                console.log(text);
                // <-- STRIP ME (o.O) Basically construct the url
                while(text.match(/&#039;\/\/restbase\.wikitolearn\.org/)){
                    text = text.replace(/&#039;\/\/restbase\.wikitolearn\.org/, "http://restbase.wikitolearn.org");
                    text = text.replace(/&#039;\);/, ");");
                }

                while(text.match(/src=\"\/\/pool.wikitolearn.org/))
                {
                    text = text.replace(/src=\"\/\/pool.wikitolearn.org/ , "src=\"http://pool.wikitolearn.org");
                }


                text = styling + text;
                console.log(text); // after strip :p .

                webview.loadHtml(text);
            }
        };
        http.open('GET',p_url);
        http.send();



    }


    /* Material Settings as per the google guidelines


     */

    Settings {
        id: settings
        property string style: "Material"
    }

    header: ToolBar {
        Material.foreground: "white"


        RowLayout {
            spacing: 20
            anchors.fill: parent

            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/images/drawer.png"
                }
                onClicked: drawer.open()
            }

            Label {
                id: titleLabel
                text: "WikiToLearn"
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/images/menu.png"
                }
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    MenuItem {
                        text: "Settings"
                        onTriggered: settingsPopup.open()
                    }
                    MenuItem {
                        text: "About"
                        onTriggered: aboutDialog.open()
                    }
                }
            }
        }
    }

    Drawer {
        id: drawer
        width: Math.min(window.width, window.height) / 3 * 2
        height: window.height


        ColumnLayout {
            anchors.fill: parent
            Rectangle {
                id: search_field
                width: drawer.width
                height: 50

                TextField{
                    id: search_text
                    placeholderText: " Search WikiToLearn"
                    width: drawer.width
                    onAccepted: {
                        listView.currentIndex = -1
                        searchterm=text

                        if(stackView.currentItem!==searchview)
                        { stackView.push(searchview)
                            webview.visible=true

                        }
                        else{webview.visible=true}
                        drawer.close();

                        parsing();
                    }



                    Image {
                        id: search_button
                        anchors.right: search_text.right
                        anchors.verticalCenter: search_text.verticalCenter
                        source: "qrc:/images/search.png"
                        MouseArea{
                            anchors.fill: search_button
                            onClicked: {
                                searchterm=search_text.text

                                if(stackView.currentItem!==searchview)
                                { stackView.push(searchview)
                                    webview.visible=true
                                }
                                else{webview.visible=true}

                                drawer.close();
                                listView.currentIndex = -1
                                parsing();

                            }



                        }
                    }

                }
            }
            Item{
                visible: false
                Text{
                    visible: false
                    id: responsetext
                    text:""
                }
            }

            Button{
                width: drawer.width
                height: 40

                text:"save offline"
                onClicked: {
                    dbman.clear_list();
                    responsetext.text = dbman.add(current_title);
                    mod.new_page(current_title);
                    drawer.close();

                }
            }






            ListView {
                id: listView
                currentIndex: -1
                Layout.fillWidth: true
                Layout.fillHeight: true

                delegate: ItemDelegate {
                    width: parent.width
                    text: model.title
                    highlighted: ListView.isCurrentItem
                    onClicked: {
                        if (listView.currentIndex != index) {
                            listView.currentIndex = index
                            titleLabel.text = model.title
                            stackView.replace(model.source)

                        }

                        drawer.close()
                    }
                }
                model: ListModel {

                    ListElement { title: "Home"; source:""}
                    ListElement { title: "Manage Pages"; source: "qrc:/pages/manage_pages.qml" }
                    ListElement { title: "Saved pages";  source: "qrc:/pages/offline_pages.qml"}
                    ListElement { title: "Quit";         source: "qrc:/pages/quit.qml"         }

                }

                ScrollIndicator.vertical: ScrollIndicator { }
                onCurrentIndexChanged: {
                    if(currentIndex===0)stackView.push(pane)
                }
            }
        }

    }
    StackView {
        id: stackView
        anchors.fill: parent


        initialItem: Pane {
            id: pane
            // anchors.fill: parent


            Image {
                id: logo
                width: pane.availableWidth / 2
                height: pane.availableHeight / 2
                anchors.centerIn: parent
                anchors.verticalCenterOffset: -50
                fillMode: Image.PreserveAspectFit
                source: "qrc:/images/wtl-logo.png"
            }

            Label {
                text: "WikiToLearn wants to provide free, collaborative and accessible text books to the whole world \“knowledge only grows if shared\""
                anchors.margins: 20
                anchors.top: logo.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                wrapMode: Label.Wrap
            }




        }
    }


    Item{
        id:searchview

        WebView{
            id: webview
            visible: false
            anchors.fill: searchview
            onUrlChanged: {
                //  console.log(searchwebview.url);

                extract_json();

            }
            onLoadingChanged: {
                if(webview.loadProgress==="100" && url.toString().match("about:blank")){
                    loadHtml("<html><head><title></title></head><body><center>Page not Found</center></body></html>");
                }
            }
        }
        AnimatedImage {
            id: loading

            anchors.centerIn: webview
            source: "qrc:/images/loading.gif"
            visible: false
        }

    }


    Popup {
        id: settingsPopup
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        height: settingsColumn.implicitHeight + topPadding + bottomPadding
        modal: true
        focus: true

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20

            Label {
                text: "Settings"
                font.bold: true
            }

            RowLayout {
                spacing: 10

                Label {
                    text: "Style:"
                }

                ComboBox {
                    id: styleBox
                    property int styleIndex: -1
                    model: ["Default", "Material", "Universal"]
                    Component.onCompleted: {
                        styleIndex = find(settings.style, Qt.MatchFixedString)
                        if (styleIndex !== -1)
                            currentIndex = styleIndex
                    }
                    Layout.fillWidth: true
                }
            }

            Label {
                text: "Restart required"
                color: "#e41e25"
                opacity: styleBox.currentIndex !== styleBox.styleIndex ? 1.0 : 0.0
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            RowLayout {
                spacing: 10

                Button {
                    id: okButton
                    text: "Ok"
                    onClicked: {
                        settings.style = styleBox.displayText
                        settingsPopup.close()
                    }

                    Material.foreground: Material.primary
                    Material.background: "transparent"
                    Material.elevation: 0

                    Layout.preferredWidth: 0
                    Layout.fillWidth: true
                }

                Button {
                    id: cancelButton
                    text: "Cancel"
                    onClicked: {
                        styleBox.currentIndex = styleBox.styleIndex
                        settingsPopup.close()
                    }

                    Material.background: "transparent"
                    Material.elevation: 0

                    Layout.preferredWidth: 0
                    Layout.fillWidth: true
                }
            }
        }
    }

    Popup {
        id: aboutDialog
        modal: true
        focus: true
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        contentHeight: aboutColumn.height

        Column {
            id: aboutColumn
            spacing: 20

            Label {
                text: "About"
                font.bold: true
            }

            Label {
                width: aboutDialog.availableWidth
                text: "WikiToLearn wants to provide free, collaborative and accessible text books to the whole world.
   Our philosophy is synthetized in the sentence: “knowledge only grows if shared”"
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: "We provide a platform where learners and teachers can together complete,"
                      + "refine and re-assemble notes, lecture notes in order to create text books, "
                      + "tailored precisely to their needs, so that you can “stand on the shoulders of giants”"
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
        }
    }




}

