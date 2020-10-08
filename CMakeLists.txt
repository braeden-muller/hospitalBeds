cmake_minimum_required(VERSION 3.5)

project(hospitalBeds LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(cpprestsdk REQUIRED)
find_package(SQLite3 REQUIRED)

#if(ANDROID)
#  add_library(hospitalBeds SHARED
#    main.cpp
#    hospitalwindow.cpp
#    hospitalwindow.h
#    hospitalwindow.ui
#    doctorwindow.cpp
#    doctorwindow.h
#    doctorwindow.ui
#    server.cpp
#    server.h
#  )
#else()
  add_executable(hospitalBeds
    main.cpp
    hospitalwindow.cpp hospitalwindow.h hospitalwindow.ui
    doctorwindow.cpp   doctorwindow.h   doctorwindow.ui
    server.cpp         server.h
    client.cpp         client.h
    db_connection.cpp  db_connection.h
    bed.cpp            bed.h
    hospital.cpp       hospital.h
    sqlite3.c          sqlite3.h
                       json_def.h    
  )
#endif()

target_link_libraries(hospitalBeds PRIVATE
    ssl
    crypto
    Qt5::Widgets
    boost_system
    cpprestsdk::cpprest
    curses
    sqlite3
  )
