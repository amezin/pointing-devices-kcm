import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

import org.kde.kquickcontrolsaddons 2.0
import org.kde.plasma.core 2.0 as PlasmaCore

MouseArea {
    id: item
    implicitWidth: itemLayout.implicitWidth + units.largeSpacing
    height: itemLayout.implicitHeight + units.largeSpacing

    readonly property alias icon: iconItem
    readonly property alias label: labelItem

    ColumnLayout {
        id: itemLayout
        anchors.centerIn: parent

        QIconItem {
            id: iconItem
            Layout.alignment: Qt.AlignHCenter
            width: units.iconSizes.huge
            height: width
            icon: item.icon
        }

        Label {
            id: labelItem
            Layout.alignment: Qt.AlignHCenter
            text: item.text
            color: textColor
        }
    }
}
