import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

ColumnLayout {

    RowLayout {
        Label {
            text: i18n("Device:")
        }

        ComboBox {
            id: deviceCombo
            model: kcm.deviceList
            textRole: "name"
            Layout.fillWidth: true
        }
    }

    property var currentDevice: kcm.deviceList.get(deviceCombo.currentIndex)

    Loader {
        Layout.fillWidth: true
        active: currentDevice
        sourceComponent: DeviceSettings { device: currentDevice }
    }
}
