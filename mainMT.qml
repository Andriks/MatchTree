import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.0


ApplicationWindow {
    title: qsTr("match3")
    width: dataModel.width * 120
    height: dataModel.height * 130
    visible: true

    property int delay: 0

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

        onRunningChanged:
        {
            if (!move_animation.running) {
                dataModel.execPackCnt--;
                dataModel.execNextPackage();
            } else {
                dataModel.execPackCnt++;
            }
        }
    }

    //////////////////////////////////////////////////////////////////
    Transition {
        id: moveDisplaced_animation
        NumberAnimation {
            properties: "x,y"
            duration: 200 + delay
            easing.type: "OutBack"
        }
    }

    //////////////////////////////////////////////////////////////////
    Transition {
        id: add_animation
            NumberAnimation {
                properties: "y"
                duration: 250 + delay
                easing.type: Easing.OutCirc
            }

        onRunningChanged:
        {
            if (!add_animation.running) {
                dataModel.execPackCnt--;
                dataModel.execNextPackage();
            } else {
                dataModel.execPackCnt++;
            }
        }
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

                            onRunningChanged:
                            {
                                if (!opacity_animation.running) {
                                    dataModel.execPackCnt--;
                                    dataModel.execNextPackage();
                                } else {
                                    dataModel.execPackCnt++;
                                }
                            }
                        }
                    }

                    Behavior on scale {
                        NumberAnimation {
                            id: scale_animation
                            duration: 300
                        }
                    }

//                    Text {
//                        x: 10; y: 10
//                        text: "%1".arg(model.index)
//                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: dataModel.moveTile(model.index)
                    }
                }
            }
        }
    }



    Timer {
        id: scale_timer
        objectName: "scale_timer"
        interval: 250
        repeat: true
        onTriggered: dataModel.provideScaleAnimation()
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
