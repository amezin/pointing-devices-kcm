import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

Item {
    id: settingsRoot

    SimpleProperty {
        id: libinputProperty
        name: "libinput Accel Speed"
    }

    Loader {
        anchors.left: parent.left
        anchors.right: parent.right
        active: libinputProperty.available
        sourceComponent: LibInputSettings { }
    }
}
