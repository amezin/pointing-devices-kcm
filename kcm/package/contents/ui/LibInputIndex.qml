import QtQuick 2.4

QtObject {
    id: root

    property string name
    property string availableName

    property var property: SimpleProperty {
        name: root.name
    }

    property var availableProperty: SimpleProperty {
        name: availableName
    }

    readonly property bool available: property.available && availableProperty.available
    readonly property bool writable: available && property.writable

    function filterChoices(allChoices, available) {
        var filtered = [allChoices[0]]
        for (var i = 0; i + 1 < allChoices.length && i < available.length; i++) {
            if (available[i]) {
                filtered.push(allChoices[i + 1])
            }
        }
        return filtered
    }

    property var choices
    readonly property var availableChoices: available ? filterChoices(choices, availableProperty.value) : []

    function findIndex(value, available) {
        var availableIndex = 0
        for (var i = 0; i < value.length && i < available.length; i++) {
            if (available[i]) {
                availableIndex += 1
            }
            if (value[i]) {
                return availableIndex
            }
        }
        return 0
    }

    readonly property var value: available ? findIndex(property.value, availableProperty.value) : undefined

    function setValue(v) {
        if (available && writable) {
            var slice = property.value.slice()
            var availableIndex = 0
            for (var i = 0; i < slice.length && i < availableProperty.value.length; i++) {
                if (availableProperty.value[i]) {
                    availableIndex += 1
                }
                slice[i] = (availableProperty.value[i] && v === availableIndex)
            }
            property.setValue(slice)
        }
    }
}
