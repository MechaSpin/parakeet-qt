cmake_minimum_required (VERSION 3.5)
project(parakeet-qt VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

find_package(Qt5 COMPONENTS Widgets Core SerialPort REQUIRED)

set(CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
set(CMAKE_FIND_PACKAGE_SORT_DIRECTION DEC)
find_package(Parakeet REQUIRED)

set(PROJECT_SOURCE
    src/IPv4Validator.h
    src/IPv4Validator.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/ScanDataViewModel.h
    src/ScanDataViewModel.cpp
    src/QCustomGraphicsView.h
    src/QCustomGraphicsView.cpp
    src/UI/Dialogs/ProEConfigurationDialog.h
    src/UI/Dialogs/ProEConfigurationDialog.cpp
    src/UI/Dialogs/ProEConfigurationDialog.ui
    src/UI/Dialogs/ProESetSourceIPv4Dialog.h
    src/UI/Dialogs/ProESetSourceIPv4Dialog.cpp
    src/UI/Dialogs/ProESetSourceIPv4Dialog.ui
    src/UI/Dialogs/ProESetDestinationIPv4Dialog.h
    src/UI/Dialogs/ProESetDestinationIPv4Dialog.cpp
    src/UI/Dialogs/ProESetDestinationIPv4Dialog.ui
)

add_executable(parakeet-viewer ${PROJECT_SOURCE})
target_include_directories(parakeet-viewer PRIVATE Parakeet::Parakeet src/)
target_link_libraries(parakeet-viewer PRIVATE Qt5::Widgets Qt5::SerialPort Parakeet::Parakeet)
