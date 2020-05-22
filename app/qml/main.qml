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

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            Action {
                text: qsTr("Open...")
            }
            Action {
                text: qsTr("Save")
            }
            Action {
                text: qsTr("Save As...")
            }
            MenuSeparator {}
            Action {
                text: qsTr("Quit")
                shortcut: StandardKey.Quit
                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: qsTr("&Help")
            Action {
                text: qsTr("&About")
            }
        }
    }

    MessageDialog {
        id: errorDialog
        icon: StandardIcon.Critical
        standardButtons: StandardButton.Close
        onRejected: {
            Qt.quit()
        }
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

    
    ResultsView {
        id: mainview
        anchors.margins: 15
        width: parent.width
        successes: 2
        failures: 1
    }
    
    /*
    ChartView {
        id: chart
        anchors.fill: parent
        title: "Test Results"
        titleFont.pointSize: 25
        titleFont.underline: true
        legend.alignment: Qt.AlignBottom
        legend.font.pixelSize: 18
        antialiasing: true

        PieSeries {
            id: series
            PieSlice {
                label: "Successes"
                value: 2
                color: "#00FF00"
            }
            PieSlice {
                label: "Failures"
                value: 1
                color: "#FF0000"
            }
        }
    }
    */

}
