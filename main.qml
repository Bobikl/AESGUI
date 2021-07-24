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

    property int fileSize: 0
    property bool miniSize: false

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
                nameFilters: ["txt文件(*.txt), rar文件(*.rar), zip文件(*.zip), png文件(*.png), xz文件(*.xz), stl文件(*.stl)"]
                onAccepted: {
                    textField.text = String(fileUrl)
                    remove(String(fileUrl))
                }
                function remove(text){
                    var reg = new RegExp("file://")
                    var a = text.replace(reg, "")
                    textField.text = a
                    getTime(a)
                    progressBar.value = 0
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
                    if (miniSize){
                        progressBar.value = 1
                    } else {
                        progressBar.value = 0
                    }

                    progressBarTimer.start()
                    threadTest.data = textField.text
                    threadTest.password = password.text
                    threadTest.acceptMessage(0)
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
                        id: animation
                        duration: progressBarTimer.interval
                    }
                }

                Timer {
                    id: progressBarTimer
                    interval: 100
                    repeat: true
                    running: false
                    onTriggered: {
                        if (parent.value < 1.0 && !miniSize){
                            parent.value = (aesTest.getEncryptFileSize(textField.text) / fileSize).toFixed(2)
                            percentage.text = parseInt(parent.value * 100) + " %"
                        } else {
                            percentage.text = "已完成"
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
        fileSize = size;
        if (size === 0) {
            t_Time = "预计" + 1 + "秒"
            miniSize = true
        } else {
            t_Time = "预计" + (size / 2.5321).toFixed(2) + "秒"
            miniSize = false
        }
        percentage.text = t_Time
    }
}
