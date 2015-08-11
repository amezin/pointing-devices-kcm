import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

ColumnLayout {
    id: root

    GroupBox {
        Layout.fillWidth: true
        title: i18n("Enable/disable device")

        ColumnLayout {
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
        }
    }

    RowLayout {
        Layout.leftMargin: 20

        enabled: accelSpeed.property.writable
        visible: accelSpeed.property.available

        AttachedLabel {
            text: i18n("Pointer speed:")
            target: accelSpeed
        }

        Label {
            text: i18n("Slow")
        }

        SliderEditor {
            id: accelSpeed
            property: SimpleProperty { name: "libinput Accel Speed" }
            minimumValue: -1.0
            maximumValue: 1.0
            Layout.fillWidth: true
        }

        Label {
            text: i18n("Fast")
        }
    }

    CheckboxEditor {
        Layout.leftMargin: 20
        property: SimpleProperty { name: "libinput Middle Emulation Enabled" }
        text: i18n("Middle button emulation")
    }

    CheckboxEditor {
        Layout.leftMargin: 20
        id: tapToClick
        property: SimpleProperty { name: "libinput Tapping Enabled" }
        text: i18n("Tap-to-click")
    }

    CheckboxEditor {
        Layout.leftMargin: 40
        id: dragLock
        property: SimpleProperty { name: "libinput Tapping Drag Lock Enabled" }
        text: i18n("Drag-lock during tapping")
        enabled: property.writable && tapToClick.checked
    }

    GroupBox {
        Layout.fillWidth: true
        title: i18n("Button Order")
        visible: leftHanded.property.available

        RowLayout {
            anchors.left: parent.left
            anchors.right: parent.right

            ExclusiveGroup {
                id: buttonOrder
            }

            property int itemWidth: Math.max(rightHanded.implicitWidth,
                                             leftHanded.implicitWidth)

            RadioButton {
                id: rightHanded
                text: i18n("Right handed")
                checked: !leftHanded.checked
                enabled: leftHanded.enabled
                Layout.preferredWidth: parent.itemWidth
            }

            RadioButtonEditor {
                id: leftHanded
                property: SimpleProperty { name: "libinput Left Handed Enabled" }
                text: i18n("Left handed")
                Layout.preferredWidth: parent.itemWidth
            }
        }
    }

    GroupBox {
        title: i18n("Scrolling Method")
        Layout.fillWidth: true
        visible: twoFingerScroll.property.available ||
                 edgeScroll.property.available ||
                 buttonScroll.property.available

        GridLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            columns: (visibleChildren.length > 2) ? 1 : 2

            ExclusiveGroup {
                id: scrollMethod
            }

            property int itemWidth: Math.max(noScroll.implicitWidth,
                                             twoFingerScroll.implicitWidth,
                                             edgeScroll.implicitWidth,
                                             buttonScroll.implicitWidth)

            RadioButton {
                id: noScroll
                exclusiveGroup: scrollMethod
                text: (twoFingerScroll.property.available ||
                       edgeScroll.property.available) ? i18n("No scrolling")
                                                      : i18n("Wheel only")
                checked: !twoFingerScroll.checked &&
                         !edgeScroll.checked &&
                         !buttonScroll.checked
                enabled: twoFingerScroll.enabled ||
                         edgeScroll.enabled ||
                         buttonScroll.enabled
                Layout.preferredWidth: parent.itemWidth
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
                Layout.preferredWidth: parent.itemWidth
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
                Layout.preferredWidth: parent.itemWidth
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
                Layout.preferredWidth: parent.itemWidth
            }
        }
    }

    GroupBox {
        Layout.fillWidth: true
        title: i18n("Scrolling Direction")
        visible: scrollReverseDirection.property.available

        RowLayout {
            anchors.left: parent.left
            anchors.right: parent.right

            ExclusiveGroup {
                id: scrollDirection
            }

            property int itemWidth: Math.max(scrollNormalDirection.implicitWidth,
                                             scrollReverseDirection.implicitWidth)

            RadioButton {
                id: scrollNormalDirection
                exclusiveGroup: scrollDirection
                checked: !scrollReverseDirection.checked
                text: i18n("Normal scrolling direction")
                enabled: scrollReverseDirection.enabled
                Layout.preferredWidth: parent.itemWidth
            }

            RadioButtonEditor {
                id: scrollReverseDirection
                exclusiveGroup: scrollDirection
                property: SimpleProperty { name: "libinput Natural Scrolling Enabled" }
                text: i18n("Reverse (natural) scrolling")
                Layout.preferredWidth: parent.itemWidth
            }
        }
    }

    GroupBox {
        Layout.fillWidth: true
        title: i18n("Click emulation")
        visible: softButtons.property.available ||
                 clickFinger.property.available

        GridLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            columns: (visibleChildren.length > 2) ? 1 : 2

            ExclusiveGroup {
                id: clickMethod
            }

            property int itemWidth: Math.max(noClick.implicitWidth,
                                             softButtons.implicitWidth,
                                             clickFinger.implicitWidth)

            RadioButton {
                id: noClick
                text: i18n("No click emulation")
                checked: !softButtons.checked &&
                         !clickFinger.checked
                exclusiveGroup: clickMethod
                enabled: softButtons.enabled ||
                         clickFinger.enabled
                Layout.preferredWidth: parent.itemWidth
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
                Layout.preferredWidth: parent.itemWidth
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
                Layout.preferredWidth: parent.itemWidth
            }
        }
    }

}
