import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import org.kde.plasma.core 2.0

ColumnLayout {

    ListModel {
        id: deviceList

        function indexOfDevice(device) {
            for (var i = 0; i < deviceList.count; i++) {
                var dev = deviceList.get(i)
                if (dev.device === device) {
                    return i
                }
            }
            return -1
        }

        function addDevice(device) {
            if (indexOfDevice(device) !== -1) {
                return
            }
            append({
                       name: device.name,
                       device: device
                   })
        }

        function removeDevice(device) {
            var index = indexOfDevice(device)
            if (index !== -1) {
                remove(index)
            }
        }
    }

    Connections {
        target: kcm
        onDeviceAdded: deviceList.addDevice(device)
        onDeviceRemoved: deviceList.removeDevice(device)
    }

    Component.onCompleted: {
        var devices = kcm.devices()
        for (var i = 0; i < devices.length; i++) {
            deviceList.addDevice(devices[i])
        }
    }

    ComboBox {
        model: deviceList
        textRole: "name"
        Layout.fillWidth: true
    }
}
