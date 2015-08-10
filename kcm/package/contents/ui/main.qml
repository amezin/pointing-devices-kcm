import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

import org.kde.PointingDevicesKCM 1.0

ColumnLayout {
    RowLayout {
        AttachedLabel {
            text: i18n("Device:")
            target: deviceCombo
        }

        ComboBox {
            id: deviceCombo
            model: kcm.deviceList
            textRole: "name"
            Layout.fillWidth: true
        }
    }

    property var currentDevice: kcm.deviceList.get(deviceCombo.currentIndex)
    Connections {
        target: currentDevice
        onDeviceDisconnected: deviceCombo.currentIndex = 0
    }

    Loader {
        Layout.fillWidth: true
        Layout.fillHeight: true
        active: !!currentDevice
        sourceComponent: DeviceSettingsUI { }
    }
}
