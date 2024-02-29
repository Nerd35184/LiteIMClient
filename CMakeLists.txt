cmake_minimum_required(VERSION 3.5)

project(LiteIMClient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network WebSockets)

set(PROJECT_SOURCES
        main.cpp


)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LiteIMClient
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        LiteIMClient.h LiteIMClient.cpp
        MenuWidget.h MenuWidget.cpp
        MainWidget.h MainWidget.cpp
        Util.h Util.cpp
        ObjGuard.h ObjGuard.cpp
        ItemListWidget.h ItemListWidget.cpp
        ItemDetailWidget.h ItemDetailWidget.cpp
        TitleWidget.h TitleWidget.cpp
        UserInfo.h UserInfo.cpp
        SearchWidget.h SearchWidget.cpp
        JsonStructs.h JsonStructs.cpp
        SessDetailWidget.h SessDetailWidget.cpp
        ContactDetailWidget.h ContactDetailWidget.cpp
        UserInfoWidget.h UserInfoWidget.cpp
        Resource.qrc
        ContactListItem.h ContactListItem.cpp
        SessListItem.h SessListItem.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET LiteIMClient APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(LiteIMClient SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(LiteIMClient
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(LiteIMClient PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::WebSockets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.LiteIMClient)
endif()
set_target_properties(LiteIMClient PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS LiteIMClient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LiteIMClient)
endif()
