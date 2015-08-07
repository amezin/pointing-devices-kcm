import QtQuick 2.4
import QtQuick.Controls 1.3

CheckBox {
    property var property

    visible: property && property.available
    enabled: property && property.writable
    checked: property && property.available && property.value
    onCheckedChanged: if (property && property.writable) property.setValue(checked)
}
