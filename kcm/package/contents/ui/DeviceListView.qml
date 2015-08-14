import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

import org.kde.kquickcontrolsaddons 2.0
import org.kde.plasma.core 2.0 as PlasmaCore

ListView {
    orientation: Qt.Horizontal
    model: kcm.deviceList
    highlightMoveDuration: units.shortDuration
    contentHeight: contentItem.childrenRect.height

    SystemPalette {
        id: systemPalette
    }

    highlight: Rectangle {
        color: systemPalette.highlight
    }

    delegate: MouseArea {
        id: item
        implicitWidth: itemLayout.implicitWidth + units.largeSpacing
        height: itemLayout.implicitHeight + units.largeSpacing

        ColumnLayout {
            id: itemLayout
            anchors.centerIn: parent

            QIconItem {
                Layout.alignment: Qt.AlignHCenter
                width: units.iconSizes.huge
                height: width

                function isTouchpad(properties) {
                    if (!properties) return false
                    for (var i = 0; i < properties.length; i++) {
                        if (properties[i].indexOf("Tap") >= 0) {
                            return true
                        }
                    }
                    return false
                }

                icon: isTouchpad(device.supportedProperties) ? "input-touchpad"
                                                             : "preferences-desktop-mouse"
            }

            Label {
                Layout.alignment: Qt.AlignHCenter
                text: name
                color: item.ListView.isCurrentItem ? systemPalette.highlightedText
                                                   : systemPalette.text
                Behavior on color {
                    ColorAnimation {
                        duration: units.shortDuration
                    }
                }
            }
        }

        onClicked: ListView.view.currentIndex = index
    }
}
