import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Dialog {
    id: dialog
    focus: true
    modal: true
    title: qsTr("Add Connection")
    standardButtons: Dialog.Ok | Dialog.Cancel

    signal finished(
        string driver,
        string database,
        string username,
        string password,
        string hostname,
        int port,
        bool usedefaultdb
    )

    implicitWidth: parent.width * 0.75
    implicitHeight: parent.height * 0.75

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    function isready() {
        if (form.checkbox.checked) {
            return true;
        }
        return form.database.text.length > 0
            && form.username.text.length > 0
            && form.password.text.length > 0
            && form.hostname.text.length > 0
        ;
    }

    property bool ready: isready()

    onReadyChanged: {
        buttons.standardButton(Dialog.Ok).enabled = ready
    }

    // contentItem: ConnectionForm {}
    ConnectionForm {
        id: form
    }

    footer: DialogButtonBox {
        id: buttons
        standardButtons: Dialog.Ok | Dialog.Cancel
    }

    onOpened: {
        buttons.standardButton(Dialog.Ok).enabled = false;
    }

    onAccepted: finished(
        form.driver.currentValue,
        form.database.text,
        form.username.text,
        form.password.text,
        form.hostname.text,
        form.port.value,
        form.checkbox.checked
    )
}
