import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.0


ApplicationWindow {
    title: qsTr("match3")
    width: dataModel.width * 120
    height: dataModel.height * 120
    visible: true

    property int delay: 0

    //////////////////////////////////////////////////////////////////
    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")

            MenuItem {
                text: qsTr("&New game")
                onTriggered: dataModel.newGame()
            }

            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    //////////////////////////////////////////////////////////////////
    Component {
        id: highlight
        Rectangle {
            height: view.cellHeight
            width: view.cellWidth
            radius: 10
            color: "lightgrey"
        }
    }

    //////////////////////////////////////////////////////////////////
    Transition {
        id: move_animation
        NumberAnimation {
            properties: "x,y"
            duration: 200 + delay
            easing.type: "OutBack"
        }
    }

    Transition {
        id: moveDisplaced_animation
        NumberAnimation {
            properties: "x,y"
            duration: 200 + delay
            easing.type: "OutBack"
        }
    }

    Transition {
        id: add_animation
        NumberAnimation {
            properties: "y"
            duration: 500 + delay
            easing.type: Easing.OutCirc
        }
    }

    //////////////////////////////////////////////////////////////////

    Timer {
        id: pack_timer
        objectName: "pack_timer"
        interval: dataModel.packDelay
        onTriggered: dataModel.execNextPackage()
    }

    Timer {
        id: scale_timer
        objectName: "scale_timer"
        interval: 250
        repeat: true
        onTriggered: dataModel.provideScaleAnimation()
    }

    //////////////////////////////////////////////////////////////////

    Rectangle {
        width: parent.height; height: parent.width
        anchors.fill: parent
        anchors.centerIn: parent

        color: "#f1ead8"

        GridView {
            id: view
            objectName: "view"
            model: dataModel

            anchors.fill: parent
            anchors.centerIn: parent
            cellHeight: parent.height / dataModel.height;
            cellWidth: parent.width / dataModel.width;

            interactive: false
            verticalLayoutDirection: GridView.BottomToTop
            snapMode: GridView.SnapToRow

            move: move_animation
            moveDisplaced: moveDisplaced_animation
            add: add_animation


            delegate: Item {
                property var view: GridView.view

                height: view.cellHeight
                width: view.cellWidth

                Item {
                    anchors.fill: parent

                    Image {
                        height: view.cellHeight
                        width: view.cellWidth

                        anchors.fill: parent
                        source: model.type
                    }

                    opacity: model.opacity
                    scale: model.scale

                    Behavior on opacity {
                        NumberAnimation {
                            id: opacity_animation
                            duration: 700 + delay
                        }
                    }

                    Behavior on scale {
                        NumberAnimation {
                            id: scale_animation
                            duration: 300
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
    statusBar: StatusBar {
        id: status_bar
        RowLayout {
            anchors.fill: parent
            Label { text: dataModel.status }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    MessageDialog {
        id: messageDialog
        objectName: "messageDialog"
        title: qsTr("Game complited")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }
}
