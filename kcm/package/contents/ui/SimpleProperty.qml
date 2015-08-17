import QtQuick 2.4

import org.kde.PointingDevicesKCM 1.0

DeviceProperty {
    device: pageSettings

    function setValue(v) {
        if (available && writable) {
            value = v
        }
    }
}
