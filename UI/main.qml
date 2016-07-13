import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import Qt.labs.settings 1.0
import QtWebView 1.1


ApplicationWindow {
    id: window
    width: 360
    height: 520
    visible: true
    title: "WikiToLearn"

    Loader{

        id: loader
            anchors.fill: parent

    }


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
               width: drawer.width
               height: 50

               TextField{
                   id: search_text
                   placeholderText: " Search WikiToLearn"
                   width: drawer.width

                   Image {
                       id: search_button
                       anchors.right: search_text.right
                       anchors.verticalCenter: search_text.verticalCenter
                       source: "qrc:/images/search.png"
                       MouseArea{
                           anchors.fill: search_button
                           onClicked: {
                              loader.source = "qrc:/pages/search.qml"
                               stackView.push(loader.item)
                               listView.currentIndex = -1
                           }



                       }
                   }

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

                ListElement { title: "Manage Pages"; source: "qrc:/pages/manage_pages.qml" }


            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

}
    StackView {
           id: stackView
           anchors.fill: parent


           initialItem: Pane {
               id: pane



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

               WebView{
                   id:webview
                   anchors.fill: parent

                   Component.onCompleted: url = "http://en.wikitolearn.org/"
               }



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
