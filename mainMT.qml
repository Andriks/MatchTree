import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.0


ApplicationWindow {
    title: qsTr("match3")
    width: dataModel.config.width * 120
    height: dataModel.config.height * 120
    visible: true

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

    Transition {
        id: move_animation
        NumberAnimation {
            properties: "x,y"
            duration: dataModel.durations.move
            easing.type: "OutBack"
        }
    }

    Transition {
        id: moveDisplaced_animation
        NumberAnimation {
            properties: "x,y"
            duration: dataModel.durations.move
            easing.type: "OutBack"
        }
    }

    Transition {
        id: add_animation
        NumberAnimation {
            properties: "y"
            duration: dataModel.durations.create
            easing.type: Easing.OutCirc
        }
    }

    //////////////////////////////////////////////////////////////////

    Timer {
        id: pack_timer
        objectName: "pack_timer"
        interval: dataModel.config.packDelay
        onTriggered: dataModel.execNextPackage()
    }

    Timer {
        id: scale_timer
        objectName: "scale_timer"
        interval: dataModel.durations.scale
        repeat: true
        onTriggered: dataModel.provideScaleAnimation()
    }

    //////////////////////////////////////////////////////////////////

    Connections {
        target: dataModel.config

        onStartPackTimer: pack_timer.start()

        onStartScaleTimer: scale_timer.start()
        onStopScaleTimer: scale_timer.stop()

        onShowMessage: messageDialog.show(text)
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
            cellHeight: parent.height / dataModel.config.height;
            cellWidth: parent.width / dataModel.config.width;

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

//                    Text {
//                        x: 10
//                        y: 10
//                        text: "%1".arg(model.index);
//                    }

                    opacity: model.opacity
                    scale: model.scale

                    Behavior on opacity {
                        NumberAnimation {
                            id: opacity_animation
                            duration: dataModel.durations.opacity
                        }
                    }

                    Behavior on scale {
                        NumberAnimation {
                            id: scale_animation
                            duration: dataModel.durations.scale
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
            Label { text: dataModel.config.status }
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
