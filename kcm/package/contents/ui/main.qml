import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

ColumnLayout {
    ScrollView {
        Layout.fillWidth: true
        frameVisible: true
        highlightOnFocus: true
        Layout.preferredHeight: viewport.anchors.topMargin + viewport.anchors.bottomMargin + flickableItem.contentHeight

        DeviceListView {
            id: deviceList
        }
    }

    property var currentDevice: kcm.deviceList.settings(deviceList.currentIndex)
    Connections {
        target: currentDevice
        onDeviceDisconnected: deviceList.currentIndex = -1
    }

    Loader {
        Layout.fillWidth: true
        Layout.fillHeight: true
        active: !!currentDevice
        sourceComponent: DeviceSettingsUI { }
    }
}
