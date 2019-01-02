TARGET = goals
TEMPLATE = app

QT += qml quick

linux:!android {
    DEFINES += DESKTOP_MODE
}

android: {
    QT += androidextras
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/Android
    #DEFINES += ANDROID_MODE  Already defined by Q_OS_ANDROID
}

CONFIG += c++14 -Werror
QMAKE_CXXFLAGS += -std=c++14

#DESTDIR = build
OBJECTS_DIR = obj
MOC_DIR = moc
RCC_DIR = rcc
UI_DIR = ui

SOURCES += main.cpp \
    Json/json_base.cpp \
    Json/json_struct.cpp \
    Json/json_variable_base.cpp \
    Json/json_variable.cpp \
    Shared/mailbox.cpp \
    Shared/session.cpp \
    Shared/shared_state.cpp \
    Util/util.cpp \
    Util/lock.cpp \
    Util/geo.cpp \
    UI/mobile/ui_mobile.cpp \
    UI/ui_common.cpp \
    Platform/android_platform.cpp \
    Platform/platform.cpp \
    Storage/data_collection.cpp \
    Util/api_util.cpp

HEADERS += \
    Json/json_base.h \
    Json/json_struct.h \
    Json/json_variable.h \
    Json/json_variable_base.h \
    Shared/mailbox.h \
    Shared/session.h \
    Shared/shared_state.h \
    Util/lock.h \
    Util/geo.h \
    Util/util.h \
    UI/mobile/ui_mobile.h \
    UI/ui_common.h \
    Platform/android_platform.h \
    main.h \
    Platform/platform.h \
    Storage/data_collection.h \
    Util/api_util.h

RESOURCES += \
    UI/mobile/goals.qrc

OTHER_FILES +=\
    UI/mobile/goals.qml \
    UI/mobile/helper.js

/*
DISTFILES += \
    Android/AndroidManifest.xml \
    Android/gradle/wrapper/gradle-wrapper.jar \
    Android/gradlew \
    Android/res/values/libs.xml \
    Android/build.gradle \
    Android/gradle/wrapper/gradle-wrapper.properties \
    Android/gradlew.bat \
    Android/src/com/fluidsoftwaredesigns/radius/MyActivity.java \
    Android/src/com/fluidsoftwaredesigns/radius/GpsService.java \
    UI/mobile/map/MapCard.qml \
    UI/mobile/components/Feed.qml \
    UI/mobile/components/CaptureButton.qml \
    UI/mobile/components/HeaderCard.qml \
    UI/mobile/forms/UserSignup.qml \
    UI/mobile/map/MapScale.qml
*/
