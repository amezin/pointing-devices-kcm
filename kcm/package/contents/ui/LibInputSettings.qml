import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

ColumnLayout {
    RowLayout {
        enabled: accelSpeed.property.writable
        visible: accelSpeed.property.available

        Label {
            text: i18n("Pointer speed:")
        }

        SliderEditor {
            id: accelSpeed
            property: SimpleProperty { name: "libinput Accel Speed" }
            minimumValue: -1.0
            maximumValue: 1.0
            Layout.fillWidth: true
        }
    }

    CheckBox {
        id: enabledCheckBox
        text: i18n("Device enabled")

        SimpleProperty {
            id: x11Enabled
            name: "Device Enabled"
        }

        LibInputFlag {
            id: libinputDisabled
            name: "libinput Send Events Mode Enabled"
            availableName: "libinput Send Events Modes Available"
            index: 0
        }

        visible: x11Enabled.available || libinputDisabled.available
        enabled: x11Enabled.writable || libinputDisabled.writable

        Binding on checked {
            when: x11Enabled.available && libinputDisabled.available
            value: x11Enabled.value && !libinputDisabled.value
        }

        Binding on checked {
            when: x11Enabled.available && !libinputDisabled.available
            value: x11Enabled.value
        }

        Binding on checked {
            when: !x11Enabled.available && libinputDisabled.available
            value: !libinputDisabled.value
        }

        onClicked: {
            var value = checked
            x11Enabled.setValue(value)
            libinputDisabled.setValue(!value)
        }
    }

    CheckboxEditor {
        property: SimpleProperty { name: "libinput Disable While Typing Enabled" }
        text: i18n("Disable while typing")
    }

    CheckboxEditor {
        property: LibInputFlag {
            name: "libinput Send Events Mode Enabled"
            availableName: "libinput Send Events Modes Available"
            index: 1
        }
        text: i18n("Disable when an external mouse is plugged in")
    }

    CheckboxEditor {
        property: SimpleProperty { name: "libinput Left Handed Enabled" }
        text: i18n("Left handed mode")
    }

    CheckboxEditor {
        property: SimpleProperty { name: "libinput Middle Emulation Enabled" }
        text: i18n("Middle button emulation")
    }

    CheckboxEditor {
        id: tapToClick
        property: SimpleProperty { name: "libinput Tapping Enabled" }
        text: i18n("Tap-to-click")
    }

    CheckboxEditor {
        id: dragLock
        property: SimpleProperty { name: "libinput Tapping Drag Lock Enabled" }
        text: i18n("Drag-lock during tapping")
        enabled: property.writable && tapToClick.checked
        Layout.leftMargin: 20
    }

    GroupBox {
        title: i18n("Scrolling")
        Layout.fillWidth: true
        visible: naturalScroll.property.available ||
                 twoFingerScroll.property.available ||
                 edgeScroll.property.available ||
                 buttonScroll.property.available

        ColumnLayout {
            ExclusiveGroup {
                id: scrollMethod
            }

            RadioButton {
                exclusiveGroup: scrollMethod
                text: (twoFingerScroll.property.available ||
                       edgeScroll.property.available) ? i18n("No scrolling")
                                                      : i18n("Wheel only")
                checked: !twoFingerScroll.checked &&
                         !edgeScroll.checked &&
                         !buttonScroll.checked
                visible: twoFingerScroll.visible ||
                         edgeScroll.visible ||
                         buttonScroll.visible
            }

            RadioButtonEditor {
                id: twoFingerScroll
                exclusiveGroup: scrollMethod
                property: LibInputFlag {
                    index: 0
                    name: "libinput Scroll Method Enabled"
                    availableName: "libinput Scroll Methods Available"
                }
                text: i18n("Two-finger scrolling")
            }

            RadioButtonEditor {
                id: edgeScroll
                exclusiveGroup: scrollMethod
                property: LibInputFlag {
                    index: 1
                    name: "libinput Scroll Method Enabled"
                    availableName: "libinput Scroll Methods Available"
                }
                text: i18n("Edge scrolling")
            }

            RadioButtonEditor {
                id: buttonScroll
                exclusiveGroup: scrollMethod
                property: LibInputFlag {
                    index: 2
                    name: "libinput Scroll Method Enabled"
                    availableName: "libinput Scroll Methods Available"
                }
                text: i18n("Scroll by dragging with middle button")
            }

            CheckboxEditor {
                id: naturalScroll
                property: SimpleProperty { name: "libinput Natural Scrolling Enabled" }
                text: i18n("Natural scrolling")
            }
        }
    }

    GroupBox {
        Layout.fillWidth: true
        title: i18n("Click emulation")
        visible: softButtons.property.available ||
                 clickFinger.property.available

        ColumnLayout {
            ExclusiveGroup {
                id: clickMethod
            }

            RadioButton {
                text: i18n("No click emulation")
                checked: !softButtons.checked &&
                         !clickFinger.checked
                exclusiveGroup: clickMethod
            }

            RadioButtonEditor {
                id: softButtons
                exclusiveGroup: clickMethod
                property: LibInputFlag {
                    name: "libinput Click Method Enabled"
                    availableName: "libinput Click Methods Available"
                    index: 0
                }
                text: i18n("Software button areas")
            }

            RadioButtonEditor {
                id: clickFinger
                exclusiveGroup: clickMethod
                property: LibInputFlag {
                    name: "libinput Click Method Enabled"
                    availableName: "libinput Click Methods Available"
                    index: 1
                }
                text: i18n("Clickfinger behavior")
            }
        }
    }

}
