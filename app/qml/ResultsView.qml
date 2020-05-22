import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtCharts 2.0

GridLayout {
    id: grid
    rows: 7
    columns: 2
    property int successes: 0;
    property int failures: 0;

    Label {
        Layout.topMargin: 10;
        Layout.fillWidth: true
        text: qsTr("Results")
        horizontalAlignment: Text.AlignHCenter
        color: "white"
        font.pointSize: 45
        font.bold: true
        font.underline: true
        Layout.columnSpan: 2
    }

    Item {
        Layout.preferredHeight: 50
        Layout.columnSpan: 2
    }

    Label {
        text: qsTr("SUCCESS:")
        color: "#00FF00"
        font.pointSize: 35
        font.bold: true
        Layout.leftMargin: 25
    }

    Label {
        text: grid.successes
        color: "#00FF00"
        font.pointSize: 35
        font.bold: true
        Layout.rightMargin: 25
    }

    Label {
        text: qsTr("FAIL:")
        color: "#FF0000"
        font.pointSize: 35
        font.bold: true
        Layout.leftMargin: 25
    }

    Label {
        text: grid.failures
        color: "#FF0000"
        font.pointSize: 35
        font.bold: true
        Layout.rightMargin: 25
    }

    ChartView {
        id: chart
        Layout.alignment: Qt.AlignHCenter
        Layout.columnSpan: 2
        Layout.rowSpan: 2
        Layout.minimumHeight: 350
        Layout.minimumWidth: 350
        //Layout.fillWidth: true
        Layout.fillHeight: true
        legend.alignment: Qt.AlignBottom
        legend.borderColor: "transparent"
        legend.labelColor: "white"
        legend.font.pixelSize: 18
        backgroundColor: "transparent"

        PieSeries {
            id: series
            PieSlice {
                label: "Successes"
                value: grid.successes
                color: "#00FF00"
            }
            PieSlice {
                label: "Failures"
                value: grid.failures
                color: "#FF0000"
            }
        }
    }
}
