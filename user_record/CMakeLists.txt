cmake_minimum_required(VERSION 3.16)

project(Qtrecord VERSION 0.1 LANGUAGES CXX)

# Enable Qt's automatic features
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the correct version of Qt (Qt6 or Qt5)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

# Define translation files
set(TS_FILES Qtrecord_zh_CN.ts)

# Define the project's source files
set(PROJECT_SOURCES
    main.cpp
    widget.cpp
    widget.ui
    resources.qrc  # 使用资源文件来管理图片等资源
    ${TS_FILES}
)

# Add the executable for Qt6 or Qt5
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Qtrecord
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

    # Translation for Qt6
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Qtrecord SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Qtrecord
            ${PROJECT_SOURCES}
        )
    endif()

    # Translation for Qt5
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# Link Qt libraries
target_link_libraries(Qtrecord PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Bundle information for macOS or iOS
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Qtrecord)
endif()

set_target_properties(Qtrecord PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install target information
include(GNUInstallDirs)
install(TARGETS Qtrecord
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize executable for Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Qtrecord)
endif()
