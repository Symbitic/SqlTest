import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtCharts 2.0
import SqlTester 1.0

ListView {
    id: results
    anchors.fill: parent
    anchors.leftMargin: 25
    anchors.rightMargin: 25

    Component.onCompleted: {
        console.log(typeof SqlTest.Starting);
    }

    header: Label {
        width: parent.width
        text: qsTr("Results")
        horizontalAlignment: Text.AlignHCenter
        color: "white"
        font.pointSize: 45
        font.bold: true
        font.underline: true
    }

    model: tester.tests

    delegate: RowLayout {
        property string name: model.modelData.name
        property bool finished: model.modelData.finished
        property bool passed: model.modelData.passed
        property string status: model.modelData.status;
        width: results.width
        height: 75

        function getColor(status) {
            switch (status) {
                case 'Pass':
                    return '#00FF00'
                case 'Fail':
                    return '#FF0000'
                case 'Skip':
                    return 'yellow'
                case 'Unfinished':
                default:
                    return '#000000'
            }
        }

        Label {
            text: name
            color: "#FFFFFF"
            font.pointSize: 35
            font.bold: true
            elide: Text.ElideRight
            Layout.preferredWidth: results.width / 2
            Layout.alignment: Qt.AlignLeft
        }
        Label {
            text: finished ? status.toUpperCase() : ""
            color: getColor(status)
            font.pointSize: 35
            font.bold: true
            Layout.alignment: Qt.AlignRight
        }
    }

    footer: ChartView {
        id: testchart
        width: parent.width * 0.75
        height: testchart.width
        anchors.horizontalCenter: parent.horizontalCenter
        visible: model.length > 0

        // White background
        title: "Test Results"
        titleFont.pointSize: 20
        titleFont.underline: false
        legend.alignment: Qt.AlignBottom
        legend.font.pixelSize: 18
        antialiasing: true

        /*
        // Black background, but white pixels noticable in chart.
        legend.alignment: Qt.AlignBottom
        legend.borderColor: "transparent"
        legend.labelColor: "white"
        legend.font.pixelSize: 18
        backgroundColor: "transparent"
        */

        PieSeries {
            id: testseries
            PieSlice {
                label: "Passed"
                value: tester.passed
                color: "#00FF00"
            }
            PieSlice {
                label: "Failed"
                value: tester.failed
                color: "#FF0000"
            }
            PieSlice {
                label: "Skipped"
                value: tester.skipped
                color: "yellow"
            }
            PieSlice {
                label: "Unfinished"
                value: model.length - (tester.passed + tester.failed + tester.skipped)
                color: "#0000FF"
            }
        }
    }
}
