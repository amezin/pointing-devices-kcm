import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

ColumnLayout {
    GroupBox {
        Layout.fillWidth: true
        Layout.margins: 5
        title: i18n("Pointer speed")

        SimpleProperty {
            id: moveSpeedProp
            name: "Synaptics Move Speed"
        }

        GridLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            columns: 2

            AttachedLabel {
                text: i18n("Minimum speed:")
                target: minSpeedSlider
            }

            SliderEditor {
                Layout.fillWidth: true
                id: minSpeedSlider
                minimumValue: 0
                maximumValue: 255
                property: ListItemProperty {
                    property: moveSpeedProp
                    index: 0
                }
            }

            AttachedLabel {
                text: i18n("Maximum speed:")
                target: maxSpeedSlider
            }

            SliderEditor {
                Layout.fillWidth: true
                id: maxSpeedSlider
                minimumValue: 0
                maximumValue: 255
                property: ListItemProperty {
                    property: moveSpeedProp
                    index: 1
                }
            }

            AttachedLabel {
                text: i18n("Acceleration factor:")
                target: accelFactorSlider
            }

            SliderEditor {
                Layout.fillWidth: true
                id: accelFactorSlider
                minimumValue: 0
                maximumValue: 1
                property: ListItemProperty {
                    property: moveSpeedProp
                    index: 2
                }
            }
        }
    }

    GroupBox {
        Layout.fillWidth: true
        Layout.margins: 5
        title: i18n("Tap to click")

        SimpleProperty {
            id: tapActionProp
            name: "Synaptics Tap Action"
            readonly property var buttons: [i18n("Disable"),
                                            i18n("Left button"),
                                            i18n("Middle button"),
                                            i18n("Right button")]
        }

        GridLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            columns: 2

            AttachedLabel {
                text: i18n("One-finger tap:")
                target: oneFingerTapCombo
            }

            ComboboxEditor {
                Layout.fillWidth: true
                id: oneFingerTapCombo
                model: tapActionProp.buttons
                property: ListItemProperty {
                    property: tapActionProp
                    index: 4
                }
            }

            AttachedLabel {
                text: i18n("Two-finger tap:")
                target: twoFingerTapCombo
            }

            ComboboxEditor {
                Layout.fillWidth: true
                id: twoFingerTapCombo
                model: tapActionProp.buttons
                property: ListItemProperty {
                    property: tapActionProp
                    index: 5
                }
            }

            AttachedLabel {
                text: i18n("Three-finger tap:")
                target: twoFingerTapCombo
            }

            ComboboxEditor {
                Layout.fillWidth: true
                id: threeFingerTapCombo
                model: tapActionProp.buttons
                property: ListItemProperty {
                    property: tapActionProp
                    index: 6
                }
            }
        }
    }

    GroupBox {
        Layout.fillWidth: true
        Layout.margins: 5
        title: i18n("Scrolling")

        SimpleProperty {
            id: edgeScrollProp
            name: "Synaptics Edge Scrolling"
        }

        SimpleProperty {
            id: twoFingerScrollProp
            name: "Synaptics Two-Finger Scrolling"
        }

        ColumnLayout {
            anchors.left: parent.left
            anchors.right: parent.right

            CheckboxEditor {
                text: i18n("Vertical two-finger scrolling")
                property: ListItemProperty {
                    property: twoFingerScrollProp
                    index: 0
                }
            }

            CheckboxEditor {
                text: i18n("Horizontal two-finger scrolling")
                property: ListItemProperty {
                    property: twoFingerScrollProp
                    index: 1
                }
            }

            CheckboxEditor {
                text: i18n("Vertical edge scrolling")
                property: ListItemProperty {
                    property: edgeScrollProp
                    index: 0
                }
            }

            CheckboxEditor {
                text: i18n("Horizontal edge scrolling")
                property: ListItemProperty {
                    property: edgeScrollProp
                    index: 1
                }
            }
        }
    }
}
