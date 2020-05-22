import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

ColumnLayout {
    id: form
    property alias driver: driver
    property alias database: database
    property alias username: username
    property alias password: password
    property alias hostname: hostname
    property alias port: port
    property alias checkbox: checkbox
    anchors.fill: parent
    anchors.margins: 8
    spacing: 20

    GroupBox {
        id: settingsBox
        //title: qsTr("Connection settings")
        Layout.fillWidth: true
        Layout.margins: 8
        //Layout.topMargin: 20
        enabled: !checkbox.checked

        /*
        background: Rectangle {
            //y: settingsBox.topPadding - settingsBox.bottomPadding
            //width: parent.width
            //height: parent.height - settingsBox.topPadding + settingsBox.bottomPadding
            color: "transparent"
            border.color: "black"
            radius: 3
        }
        */
        

        GridLayout {
            id: grid
            property int minimumInputSize: 175
            anchors.fill: parent
            rows: 6
            columns: 2

            Label {
                text: qsTr("Driver")
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            }

            ComboBox {
                id: driver
                Layout.fillWidth: true
                model: db.drivers;
            }

            Label {
                text: qsTr("Database Name")
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            }

            TextField {
                id: database
                Layout.fillWidth: true
                Layout.minimumWidth: grid.minimumInputSize
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            }

            Label {
                text: qsTr("Username")
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            }

            TextField {
                id: username
                Layout.fillWidth: true
                Layout.minimumWidth: grid.minimumInputSize
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            }

            Label {
                text: qsTr("Password")
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            }

            TextField {
                id: password
                echoMode: TextInput.Password
                Layout.fillWidth: true
                Layout.minimumWidth: grid.minimumInputSize
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            }

            Label {
                text: qsTr("Hostname")
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            }

            TextField {
                id: hostname
                Layout.fillWidth: true
                Layout.minimumWidth: grid.minimumInputSize
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            }

            Label {
                text: qsTr("Port")
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            }

            SpinBox {
                id: port
                property string text: displayText
                from: -1
                to: 65535
                value: -1
                Layout.fillWidth: true
                Layout.minimumWidth: grid.minimumInputSize
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline

                validator: IntValidator {
                    locale: port.locale.name
                    bottom: Math.min(port.from, port.to)
                    top: Math.max(port.from, port.to)
                }

                textFromValue: function(value, locale) {
                    return value === -1 ? qsTr("Default") : Number(value).toLocaleString(locale, 'f', 0)
                }

                valueFromText: function(text, locale) {
                    return value === qsTr("Default") ? Number(-1) : Number.fromLocaleString(locale, text)
                }
            }
        }
    }

    CheckBox {
        id: checkbox
        //Layout.fillWidth: true
        Layout.leftMargin: 8
        Layout.rightMargin: 8
        text: qsTr("Use predefined in-memory database")
    }

}
