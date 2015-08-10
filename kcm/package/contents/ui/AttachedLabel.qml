import QtQuick 2.4
import QtQuick.Controls 1.3

Label {
    property Item target
    enabled: target.enabled
    visible: target.visible
}
