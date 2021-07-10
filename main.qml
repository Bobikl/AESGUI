import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import AesCalculator 1.0

import QtQuick.Dialogs 1.2

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    AES {
        id: aesTest
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            spacing: 20
            Rectangle {
                Layout.fillWidth: true
            }
            Text {
                text: "选择文件"
            }

            Rectangle {
                width: 300
                height: 45
                TextField {
                    id: textField
                    anchors.fill: parent
                    text: "file:///home/lbw/Desktop/png/kll.txt"
                    font.pixelSize: 15
                }
            }

            Button {
                width: 80
                height: 40
                text: "选择"
                onClicked: {
//                    aesTest.aesTest()
                    fileDialog.open()
                }
            }

            FileDialog {
                id: fileDialog
                title: "choose file"
                folder: shortcuts.desktop
                nameFilters: ["txt文件(*.txt)"]
                onAccepted: {
//                    textField.text = String(fileUrl)
                }
            }

            Rectangle {
                Layout.fillWidth: true
            }
        }

        RowLayout {
            spacing: 20

            Rectangle {
                Layout.fillWidth: true
            }

            Text {
                text: "输入密钥"
            }

            Rectangle {
                width: 300
                height: 45
                TextField {
                    id: password
                    text: "abcdefghijklmnop"
                    anchors.fill: parent
                    font.pixelSize: 15
                }
            }

            Button {
                width: 80
                height: 40
                text: "开始"
                onClicked: {
                    aesTest.encryptChoose(remove(), password.text)
                }
                function remove(){
                    var text = textField.text
                    var reg = new RegExp("file://")
                    var a = text.replace(reg, "");
                    return a
                }
            }

            Rectangle {
                Layout.fillWidth: true
            }

        }
    }
}
