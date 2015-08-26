import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2


ApplicationWindow {
    title: qsTr("Hello World")
    width: 700
    height: 900
    visible: true


    Rectangle {
        width: parent.height; height: parent.width
        anchors.fill: parent
        anchors.centerIn: parent

        GridView {
            id: view
            objectName: "view"
            model: dataModel

            anchors.fill: parent
            anchors.centerIn: parent
            cellHeight: parent.height / dataModel.height;
            cellWidth: parent.width / dataModel.width;

            interactive: false

            move: Transition {
                id: move_animation
                NumberAnimation {
                    properties: "x,y"
                    duration: 500
                    easing.type: Easing.OutCirc
                }

                onRunningChanged:
                {
                    if (!move_animation.running) {
//                       console.log("move stop");
                       dataModel.execNextPackage();
                    } else {
//                        console.log("move start");
                    }
                }
            }

            add: Transition {
                id: add_animation
                NumberAnimation {
                    properties: "y"
                    duration: 1000
                    easing.type: Easing.OutCirc
                }

                onRunningChanged:
                {
                    if (!add_animation.running) {
//                       console.log("add stop");
                       dataModel.execNextPackage();
                    } else {
//                        console.log("add start");
                    }
                }
            }

            delegate: Item {
                property var view: GridView.view

                height: view.cellHeight
                width: view.cellWidth

                Rectangle {
                    id: tile_

                    anchors.margins: 5
                    radius: 15
                    anchors.fill: parent
                    color: model.color
                    opacity: model.opacity

                    Behavior on opacity {
                        NumberAnimation {
                            id: opacity_animation
                            duration: 500

                            onRunningChanged:
                            {
                                if (!opacity_animation.running) {
//                                    console.log(model.index, "op stop");
                                    dataModel.execNextPackage();
                                } else {
//                                    console.log(model.index, "op start");
                                }

                            }
                        }
                    }


                    MouseArea {
                        anchors.fill: parent
                        onClicked: dataModel.moveTile(model.index)
                    }
                }
            }
        }
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("test")
                onTriggered: dataModel.someSlot();
            }
            MenuItem {
                text: qsTr("&rem matches")
                onTriggered: dataModel.createPackages();
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }
}
