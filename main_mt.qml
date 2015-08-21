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
            cellHeight: parent.height / model.height;
            cellWidth: parent.width / model.width;

            interactive: false

            move: Transition {
                SequentialAnimation {
                //ParallelAnimation {
                    NumberAnimation { properties: "x,y"; duration: 1000; easing.type: Easing.OutBounce }
                }
            }

            delegate: Item {
                property var view: GridView.view

                height: view.cellHeight
                width: view.cellWidth


                TileForm {
                    id: tile

                    anchors.margins: 5
                    radius: 15
                    anchors.fill: parent
                    color: model.color
                    opacity: model.opacity

                    Behavior on opacity {
                        NumberAnimation { duration: 1000; }
                    }

                    Text {
                        anchors.centerIn: parent
                        renderType: Text.NativeRendering
                        text: model.name
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
                onTriggered: dataModel.someSlot()
            }
            MenuItem {
                text: qsTr("&Open")
                onTriggered: messageDialog.show(qsTr("Open action triggered"));
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
