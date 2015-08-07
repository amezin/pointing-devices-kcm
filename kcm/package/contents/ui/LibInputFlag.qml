import QtQuick 2.4

DependentProperty {
    id: root

    property string name
    property string availableName
    property int index

    property: ListItemProperty {
        index: root.index
        property: SimpleProperty {
            name: root.name
        }
    }

    availableFlag: ListItemProperty {
        index: root.index
        property: SimpleProperty {
            name: root.availableName
        }
    }
}
