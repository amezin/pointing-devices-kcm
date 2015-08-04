import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

ColumnLayout {
    id: settingsRoot
    property var device: null

    Loader {
        Layout.fillWidth: true
        active: "accelAdaptiveDeceleration" in device.supportedProperties ||
                "accelConstantDeceleration" in device.supportedProperties ||
                "accelVelocityScaling" in device.supportedProperties
        sourceComponent: XInputAcceleration { device: settingsRoot.device }
    }
}
