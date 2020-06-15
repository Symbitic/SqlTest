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
    minimumWidth: 800
    minimumHeight: 900

    Shortcut {
        sequence: StandardKey.Quit
        onActivated: Qt.quit()
    }

    Shortcut {
        sequence: StandardKey.Open
        onActivated: openDialog.open()
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: openDialog.open()
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

    FileDialog {
        id: openDialog
        nameFilters: [ "JSON files (*.json)" ]
        onAccepted: {
            tester.start(openDialog.fileUrl);
        }
    }

    SqlTest {
        id: tester
    }

    ResultsView {
        id: mainview
        anchors.margins: 15
        width: parent.width
    }
}
