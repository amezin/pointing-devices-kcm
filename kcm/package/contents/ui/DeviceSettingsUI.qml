import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

Item {
    id: settingsRoot

    SimpleProperty {
        id: libinputProperty
        name: "libinput Send Events Mode Enabled"
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
        active: synapticsProperty.available
        sourceComponent: SynapticsSettings { }
    }

    Loader {
        anchors.left: parent.left
        anchors.right: parent.right
        active: !libinputProperty.available && !synapticsProperty.available
        sourceComponent: EvdevSettings { }
    }
}
