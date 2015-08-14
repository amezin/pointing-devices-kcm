import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

import org.kde.plasma.core 2.0 as PlasmaCore

GridLayout {
    columns: 3
    rowSpacing: units.largeSpacing

    AttachedLabel {
        text: i18n("Pointer speed:")
        target: accelSpeed
    }

    RowLayout {
        Layout.columnSpan: 2
        Layout.fillWidth: true

        AttachedLabel {
            text: i18n("Slow")
            target: accelSpeed
        }

        SliderEditor {
            id: accelSpeed
            property: SimpleProperty { name: "libinput Accel Speed" }
            minimumValue: -1.0
            maximumValue: 1.0
            Layout.fillWidth: true
        }

        AttachedLabel {
            text: i18n("Fast")
            target: accelSpeed
        }
    }

    AttachedLabel {
        text: i18n("Middle button:")
        target: middleButtonEmulation
    }

    CheckboxEditor {
        Layout.columnSpan: 2
        id: middleButtonEmulation
        property: SimpleProperty { name: "libinput Middle Emulation Enabled" }
        text: i18n("Emulate with left and right button")
    }

    ExclusiveGroup {
        id: buttonOrder
    }

    AttachedLabel {
        text: i18n("Button order:")
        target: rightHanded
    }

    RadioButton {
        id: rightHanded
        text: i18n("Right handed")
        checked: !leftHanded.checked
        enabled: leftHanded.enabled
    }

    RadioButtonEditor {
        id: leftHanded
        property: SimpleProperty { name: "libinput Left Handed Enabled" }
        text: i18n("Left handed")
    }

    AttachedLabel {
        text: i18n("Scrolling direction:")
        target: scrollNormalDirection
    }

    ExclusiveGroup {
        id: scrollDirection
    }

    RadioButton {
        id: scrollNormalDirection
        exclusiveGroup: scrollDirection
        checked: !scrollReverseDirection.checked
        text: i18n("Traditional (mouse)")
        enabled: scrollReverseDirection.enabled
    }

    RadioButtonEditor {
        id: scrollReverseDirection
        exclusiveGroup: scrollDirection
        property: SimpleProperty { name: "libinput Natural Scrolling Enabled" }
        text: i18n("\"Natural\" (touch screen)")
    }

    AttachedLabel {
        text: i18n("Scrolling method:")
        target: scrollMethod
    }

    ComboboxEditor {
        id: scrollMethod
        Layout.columnSpan: 2
        Layout.fillWidth: true

        LibInputFlag {
            id: twoFingerScroll
            name: "libinput Scroll Method Enabled"
            availableName: "libinput Scroll Methods Available"
            index: 0
        }

        LibInputFlag {
            id: edgeScroll
            name: "libinput Scroll Method Enabled"
            availableName: "libinput Scroll Methods Available"
            index: 1
        }

        property: LibInputIndex {
            name: "libinput Scroll Method Enabled"
            availableName: "libinput Scroll Methods Available"
            choices: [(twoFingerScroll.available || edgeScroll.available) ? i18n("No scrolling")
                                                                          : i18n("Wheel only"),
                      i18n("Two-finger scrolling"),
                      i18n("Edge scrolling"),
                      i18n("Scroll by dragging with middle button")]
        }
        model: property.availableChoices
    }

    AttachedLabel {
        Layout.alignment: Qt.AlignTop | Qt.AlignRight
        text: i18n("Tapping:")
        target: tapToClick
    }

    ColumnLayout {
        Layout.columnSpan: 2
        spacing: units.smallSpacing

        CheckboxEditor {
            id: tapToClick
            text: i18n("Enable tap-to-click")
            property: SimpleProperty { name: "libinput Tapping Enabled" }
        }

        CheckboxEditor {
            id: dragLock
            property: SimpleProperty { name: "libinput Tapping Drag Lock Enabled" }
            text: i18n("Drag lock during tap-and-drag")
            enabled: property.writable && tapToClick.checked
        }
    }

    AttachedLabel {
        text: i18n("Software-emulated buttons:")
        target: clickMethod
    }

    ComboboxEditor {
        id: clickMethod
        Layout.columnSpan: 2
        Layout.fillWidth: true
        property: LibInputIndex {
            name: "libinput Click Method Enabled"
            availableName: "libinput Click Methods Available"
            choices: [i18n("No emulation"),
                      i18n("Software button areas"),
                      i18n("Right and middle click with multiple fingers")]
        }
        model: property.availableChoices
    }
}
