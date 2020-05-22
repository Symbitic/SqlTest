import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
//import QtCharts 2.0

ColumnLayout {
    id: main
    anchors.fill: parent
    anchors.margins: 8

    /*
    ChartView {
        id: chart
        title: "Test Results"
        legend.alignment: Qt.AlignBottom
        antialiasing: true
        Layout.fillWidth: true

        PieSeries {
            id: series
            PieSlice { label: "Successes"; value: 3 }
            PieSlice { label: "Failures"; value: 1 }
        }
    }
    */
    

    TableView {
        id: view
        Layout.fillWidth: true
        Layout.fillHeight: true
        columnSpacing: 1
        rowSpacing: 1
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        ScrollBar.horizontal: ScrollBar {}
        ScrollBar.vertical: ScrollBar {}

        property int cellWidth: (view.width / view.columns) - (view.columnSpacing * 2)
        property int cellHeight: (view.height / view.rows) - (view.rowSpacing * 2)

        onCellWidthChanged: {
            forceLayout()
        }

        onCellHeightChanged: {
            forceLayout()
        }

        model: db.model

        delegate: Rectangle {
            implicitWidth: Math.max(50, view.cellWidth)
            implicitHeight: Math.max(25, view.cellHeight)
            border.width: 1
            Text {
                text: display
                anchors.centerIn: parent
            }
        }
    }

    GroupBox {
        id: querybox
        title: qsTr("SQL Query")
        Layout.fillWidth: true
        Layout.minimumHeight: 300
        Layout.margins: 8

        ColumnLayout {
            id: maincolumn
            anchors.fill: parent

            TextArea {
                id: query
                text: "SELECT * FROM Names"
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 30
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 6

                Item {
                    Layout.fillWidth: true
                }

                Button {
                    id: clear
                    text: qsTr("Clear")
                    onClicked: {
                        query.text = ''
                    }
                }

                Button {
                    id: submit
                    text: qsTr("Submit")
                    enabled: db.connected
                    onClicked: {
                        db.exec(query.text);
                    }
                }
            }
        }
    }
}