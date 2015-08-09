import QtQuick 2.4

QtObject {
    id: root

    property var property
    property int index

    readonly property bool available: property.available &&
                                      property.value.length >= index
    readonly property bool writable: available && property.writable

    readonly property var value: available ? property.value[index] : undefined

    function setValue(v) {
        if (available && writable) {
            var slice = property.value.slice()
            slice[index] = v
            property.setValue(slice)
        }
    }
}
