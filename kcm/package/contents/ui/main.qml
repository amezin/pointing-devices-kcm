import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

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

    RowLayout {
        Label {
            text: i18n("Device:")
        }

        ComboBox {
            id: deviceCombo
            model: deviceList
            textRole: "name"
            Layout.fillWidth: true
        }
    }

    property var currentDevice: (deviceCombo.currentIndex >= 0 &&
                                 deviceCombo.currentIndex < deviceList.count)
                                ? deviceList.get(deviceCombo.currentIndex).device
                                : null

    CheckBox {
        text: i18n("Enable Device")
        checked: currentDevice.enabled
    }

    Loader {
        active: currentDevice.supportedProperties.indexOf("accelProfile") >= 0 ||
                currentDevice.supportedProperties.indexOf("accelConstantDeceleration") >= 0 ||
                currentDevice.supportedProperties.indexOf("accelAdaptiveDeceleration") >= 0 ||
                currentDevice.supportedProperties.indexOf("accelVelocityScaling") >= 0
        sourceComponent: XInputAcceleration { }
    }
}
