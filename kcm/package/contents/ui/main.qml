import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.1

ColumnLayout {
    ScrollView {
        Layout.fillWidth: true
        frameVisible: true
        highlightOnFocus: true
        Layout.preferredHeight: viewport.anchors.topMargin + viewport.anchors.bottomMargin + flickableItem.contentHeight
        focus: true

        ListView {
            id: pageList
            focus: true
            orientation: Qt.Horizontal
            highlightMoveDuration: units.longDuration
            contentHeight: contentItem.childrenRect.height

            SystemPalette {
                id: systemPalette
                colorGroup: pageList.activeFocus ? SystemPalette.Active
                                                 : SystemPalette.Inactive
            }

            highlight: Rectangle {
                color: systemPalette.highlight
            }

            delegate: PageListDelegate {
                label.color: ListView.isCurrentItem ? systemPalette.highlightedText
                                                    : systemPalette.text
                Behavior on label.color {
                    ColorAnimation {
                        duration: units.shortDuration
                    }
                }

                onClicked: {
                    if (kcm.needsSave) {
                        unsavedChangesDialog.targetIndex = index
                        unsavedChangesDialog.open()
                    } else {
                        ListView.view.currentIndex = index
                    }
                }
            }

            model: ListModel {
                id: pageListModel

                function indexOfDevice(device) {
                    for (var i = 0; i < count; i++) {
                        if (get(i)["device"] === device) {
                            return i
                        }
                    }
                    return -1
                }

                function addDevice(device) {
                    var existing = indexOfDevice(device)
                    if (existing === -1) {
                        append({"device": device})
                    }
                }

                function removeDevice(device) {
                    var existing = indexOfDevice(device)
                    if (existing !== -1) {
                        remove(existing)
                    }
                }
            }
        }
    }

    Connections {
        target: kcm
        onDeviceAdded: {
            pageListModel.addDevice(device)
        }
        onDeviceRemoved: {
            pageListModel.removeDevice(device)
        }
    }

    Component.onCompleted: {
        var devices = kcm.allDevices()
        for (var i = 0; i < devices.length; i++) {
            pageListModel.addDevice(devices[i])
        }
    }

    StackView {
        id: pageLoader
        Layout.fillWidth: true
        Layout.fillHeight: true

        property Component sourceComponent: pageList.currentItem && pageList.currentItem.pageComponent
        property int prevIndex: 0
        property bool moveLeft: false
        onSourceComponentChanged: {
            moveLeft = (prevIndex > pageList.currentIndex)
            prevIndex = pageList.currentIndex
            if (!sourceComponent) {
                clear()
            } else {
                push({item: sourceComponent, replace: true})
            }
        }

        delegate: StackViewDelegate {
            replaceTransition: StackViewTransition {
                PropertyAnimation {
                    target: enterItem
                    property: "x"
                    from: pageLoader.moveLeft ? -target.width : target.width
                    to: 0
                    duration: units.longDuration
                    easing.type: Easing.OutCubic
                }
                PropertyAnimation {
                    target: exitItem
                    property: "x"
                    from: 0
                    to: pageLoader.moveLeft ? target.width : -target.width
                    duration: units.longDuration
                    easing.type: Easing.OutCubic
                }
            }
        }
    }

    MessageDialog {
        id: unsavedChangesDialog
        title: i18n("Apply Settings")
        text: i18n("The settings on the current page have changed.")
        informativeText: i18n("Do you want to apply the changes or discard them?")
        icon: StandardIcon.Warning
        standardButtons: StandardButton.Apply |
                         StandardButton.Discard |
                         StandardButton.Cancel
        property int targetIndex

        onApply: {
            kcm.save()
            pageList.currentIndex = targetIndex
        }
        onDiscard: {
            pageList.currentIndex = targetIndex
        }
    }
}
