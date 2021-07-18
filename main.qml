import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import AesCalculator 1.0
import AddThread 1.0
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.0

Window {
    id: root
    width: 700
    height: 480
    minimumHeight: 480
    minimumWidth: 700
    visible: true
    title: qsTr("Hello World")

    AES {
        id: aesTest
    }
    ThreadTest {
        id: threadTest
    }

    Popup {
        id: popup
        anchors.centerIn: parent
        width: root.width / 2
        height: 20
        opacity: 0.5
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
                    text: settings.filePath
                    font.pixelSize: 15
                }
                Settings {
                    id: settings
                    property string filePath: textField.text
                }
                Component.onCompleted: {
                    getTime(settings.filePath)
                }

                Component.onDestruction: {
                    settings.filePath = textField.text
                }
            }

            Button {
                width: 80
                height: 40
                text: "选择"
                onClicked: {
                    fileDialog.open()
                }
            }

            FileDialog {
                id: fileDialog
                title: "choose file"
                folder: shortcuts.desktop
                nameFilters: ["txt文件(*.txt), rar文件(*.rar), zip文件(*.zip), png文件(*.png)"]
                onAccepted: {
                    textField.text = String(fileUrl)
                    remove(String(fileUrl))
                }
                function remove(text){
                    var reg = new RegExp("file://")
                    var a = text.replace(reg, "")
                    textField.text = a
                    getTime(a)
                    console.log(a)
                }
            }

            Rectangle {
                Layout.fillWidth: true
            }
        }

        RowLayout {
            spacing: 20
            Layout.alignment: Qt.AlignHCenter

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
                    progressBarTimer.start()
//                    aesTest.encryptChoose(textField.text, password.text)
//2.5321
//                    percentage.text = "预计" + (aesTest.getFileSize(textField.text) / 2.5321).toFixed(2) + "秒"
                    threadTest.data = textField.text
                    threadTest.password = password.text
                    threadTest.acceptMessage()
                }
            }

            Button {
                width: 80
                height: 40
                text: "解"
                onClicked: {

                }
            }
        }
        RowLayout {
            spacing: 10
            Layout.alignment: Qt.AlignHCenter
            ProgressBar {
                id: progressBar
                width: root.width / 2
                height: 15
                background: Rectangle {
                    implicitHeight: parent.height
                    implicitWidth: parent.width
                    color: "lightGrey"
                    opacity: 0.5
                }
                contentItem: Item {
                    Rectangle {
                        width: progressBar.visualPosition * progressBar.width
                        height: progressBar.height
                        color: "black"
                    }
                }

                Behavior on value {
                    NumberAnimation {
                        duration: progressBarTimer.interval
                    }
                }

                Timer {
                    id: progressBarTimer
                    interval: 100
                    repeat: true
                    running: false
                    onTriggered: {
                        if (parent.value < 1.0){
                            parent.value += 0.1
                            percentage.text = parseInt(parent.value * 100) + " %"
                        } else {
                            percentage.text = "已完成"
                            progressBar.value = 0
                            stop()
                        }
                    }
                }
            }
            Text {
                id: percentage
                text: "未开始"
            }
        }
        Rectangle {
            height: 15
        }
    }
    function getTime(time){
        var t_Time
        var size = aesTest.getFileSize(time)
        if (size === 0) {
            t_Time = "预计" + 1 + "秒"
        } else {
            t_Time = "预计" + (size / 2.5321).toFixed(2) + "秒"
        }
        percentage.text = t_Time
    }
}
