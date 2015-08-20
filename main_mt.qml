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
                NumberAnimation { properties: "x,y"; duration: 800; easing.type: Easing.OutBounce }
            }

            delegate: Item {
                property var view: GridView.view

                height: view.cellHeight
                width: view.cellWidth
                TileForm {
                    anchors.margins: 5
                    radius: 15
                    anchors.fill: parent
                    color: model.color

                    Text {
                        anchors.centerIn: parent
                        renderType: Text.NativeRendering
                        text: model.name
                    }

                    MouseArea {
                        anchors.fill: parent
//                        onClicked: dataModel.moveCell(model.index)
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
