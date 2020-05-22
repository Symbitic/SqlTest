import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Window 2.0
import SqlTester 1.0

ApplicationWindow {
    id: root
    visible: true
    title: qsTr("SQL Tester")
    minimumWidth: 600
    minimumHeight: 800

    Shortcut {
        sequence: StandardKey.Quit
        onActivated: Qt.quit()
    }

    MessageDialog {
        id: errorDialog
        icon: StandardIcon.Critical
        standardButtons: StandardButton.Close
        onRejected: {
            Qt.quit()
        }
    }

    ResultsView {
        id: mainview
        anchors.fill: parent
    }
    
    Database {
        id: db

        Component.onCompleted: {
            if (db.drivers.length < 1) {
                errorDialog.text = qsTr("No SQL Drivers detected! Please recompile Qt with at least 1 driver enabled.")
                errorDialog.visible = true
            } else {
                connectionDialog.open()
            }
        }
    }

    ConnectionDialog {
        id: connectionDialog

        onFinished: {
            db.connect(driver, database, username, password, hostname, port, usedefaultdb);
        }
    }
}
