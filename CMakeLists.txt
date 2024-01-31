cmake_minimum_required(VERSION 3.16)

project(kiss-pm-ctrl VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick WaylandClient REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick WaylandClient REQUIRED)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
find_package(Wayland COMPONENTS Client REQUIRED)
find_package(WaylandScanner)
find_package(QtWaylandScanner)
ecm_add_qtwayland_client_protocol(PROTOCOL_GENERATED_SOURCES PROTOCOL protocols/ext-idle-notify-v1.xml BASENAME ext-idle-notify-v1)

find_package(Qt5Gui)
include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})

set(PROJECT_SOURCES
        main.cpp
	backlightctrl.h backlightctrl.cpp
	suspendctrl.h suspendctrl.cpp
	idlenotifier.cpp idlenotifier.h
        qml.qrc
)

add_executable(kiss-pm-ctrl
  ${PROJECT_SOURCES}
  ${PROTOCOL_GENERATED_SOURCES}
)

target_compile_definitions(kiss-pm-ctrl PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(kiss-pm-ctrl PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Wayland::Client Qt::WaylandClient udev)

install(TARGETS kiss-pm-ctrl DESTINATION ${CMAKE_INSTALL_BIN})
