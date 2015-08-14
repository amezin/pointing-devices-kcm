import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

import org.kde.plasma.core 2.0 as PlasmaCore

ColumnLayout {
    spacing: units.smallSpacing

    CheckBox {
        id: enabledCheckBox
        text: i18n("Enable device")

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
