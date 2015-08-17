import QtQuick 2.4

IconWithLabel {
    readonly property var deviceProperties: device.supportedProperties

    function contains(list, value) {
        return list.indexOf(value) >= 0
    }

    readonly property bool isLibInput: contains(deviceProperties,
                                                "libinput Send Events Mode Enabled")
    readonly property bool isSynaptics: contains(deviceProperties,
                                                 "Synaptics Off")
    readonly property Component pageComponent: {
        if (isLibInput) return libinputPage
        if (isSynaptics) return synapticsPage
        return evdevPage
    }

    readonly property bool isTouchpad: isSynaptics || contains(deviceProperties,
                                                               "libinput Tapping Enabled")
    icon.icon: isTouchpad ? "input-touchpad" : "preferences-desktop-mouse"
    label.text: device.name

    Component {
        id: libinputPage
        DevicePage {
            enableDisableSection: LibInputEnableDisable { }
            settingsSection: LibInputSettings { }
        }
    }

    Component {
        id: synapticsPage
        DevicePage {
            settingsSection: SynapticsSettings { }
        }
    }

    Component {
        id: evdevPage
        DevicePage {
            settingsSection: EvdevSettings { }
        }
    }

    onClicked: {
        ListView.view.forceActiveFocus(Qt.MouseFocusReason)
    }
}
