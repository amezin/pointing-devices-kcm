import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

ColumnLayout {
    RowLayout {
        Label {
            text: i18n("Adaptive Deceleration:")
        }
        Slider {
            minimumValue: 1
            maximumValue: 10
            value: currentDevice.accelAdaptiveDeceleration
        }
    }
    RowLayout {
        Label {
            text: i18n("Constant Deceleration:")
        }
        Slider {
            minimumValue: 1
            maximumValue: 10
            value: currentDevice.accelConstantDeceleration
        }
    }
    RowLayout {
        Label {
            text: i18n("Velocity Scaling:")
        }
        Slider {
            minimumValue: 0
            maximumValue: 100
            value: currentDevice.accelVelocityScaling
        }
    }
}
