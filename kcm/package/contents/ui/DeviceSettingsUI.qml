import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

Item {
    id: settingsRoot

    SimpleProperty {
        id: libinputProperty
        name: "libinput Accel Speed"
    }

    SimpleProperty {
        id: evdevProperty
        name: "Evdev Axis Inversion"
    }

    SimpleProperty {
        id: synapticsProperty
        name: "Synaptics Off"
    }

    Loader {
        anchors.horizontalCenter: settingsRoot.horizontalCenter
        active: libinputProperty.available
        sourceComponent: LibInputSettings { }
    }

    Loader {
        anchors.left: parent.left
        anchors.right: parent.right
        active: evdevProperty.available
        sourceComponent: EvdevSettings { }
    }

    Loader {
        anchors.left: parent.left
        anchors.right: parent.right
        active: synapticsProperty.available
        sourceComponent: SynapticsSettings { }
    }
}
