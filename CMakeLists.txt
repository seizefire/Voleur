cmake_minimum_required(VERSION 3.16)

project(Voleur VERSION 1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

##############################################
# CHANGE THESE TO WHEREVER YOUR QT, LIBTOMMATH, AND LIBTOMCRYPT ARE
##############################################
set(CMAKE_PREFIX_PATH C:/Libs/Qt/6.6.1/Build-Bare/lib/cmake;C:/Libs/libtommath/lib/cmake;C:/Libs/libtomcrypt/lib/cmake)
include_directories(C:/Libs/libtommath/include)
include_directories(C:/Libs/libtomcrypt/include)
##############################################

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "/O1")
SET(CMAKE_EXE_LINKER_FLAGS  "/LTCG /OPT:REF")

find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets)
find_package(libtommath REQUIRED)
find_package(libtomcrypt REQUIRED)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(Voleur
    src/jitbit.cpp
    src/main.cpp
    src/window.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(Voleur PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Voleur
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)


target_link_libraries(Voleur
    PRIVATE Qt6::Widgets
    PRIVATE libtommath
    PRIVATE libtomcrypt
)

include(GNUInstallDirs)
install(TARGETS Voleur
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
