import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

ColumnLayout {
    CheckboxEditor {
        text: i18n("Enable device")
        property: SimpleProperty {
            name: "Device Enabled"
        }
    }

    GroupBox {
        Layout.fillWidth: true
        title: i18n("Pointer Acceleration")

        XInputAccel {
            anchors.left: parent.left
            anchors.right: parent.right
        }
    }

    GroupBox {
        Layout.fillWidth: true
        title: i18n("Pointer Movement")

        ColumnLayout {
            anchors.left: parent.left
            anchors.right: parent.right

            SimpleProperty {
                id: axisInversionProp
                name: "Evdev Axis Inversion"
            }

            CheckboxEditor {
                text: i18n("Invert X axis")
                property: ListItemProperty {
                    property: axisInversionProp
                    index: 0
                }
            }

            CheckboxEditor {
                text: i18n("Invert Y axis")
                property: ListItemProperty {
                    property: axisInversionProp
                    index: 1
                }
            }

            CheckboxEditor {
                text: i18n("Swap axes")
                property: SimpleProperty {
                    name: "Evdev Axes Swap"
                }
            }
        }
    }
}
