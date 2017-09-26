cmake_minimum_required(VERSION 3.8)
project(djsr_clion)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

find_package(PkgConfig REQUIRED)
find_package(Threads)

#
#include(ExternalProject)
#
#ExternalProject_add(
#        PROJECT_PORTAUDIO
#        GIT_REPOSITORY "git@git.assembla.com:portaudio.git"
#        GIT_TAG ""
#        PREFIX "libs/portaudio"
#        CONFIGURE_COMMAND "<SOURCE_DIR>/configure"
#        UPDATE_COMMAND ""
#        INSTALL_COMMAND ""
#        LOG_DOWNLOAD 1
#        LOG_INSTALL 1
#        LOG_UPDATE 1
#        LOG_CONFIGURE 1
#
#)
#
#
#
#ExternalProject_Get_Property(PROJECT_PORTAUDIO BINARY_DIR)
#ExternalProject_Get_Property(PROJECT_PORTAUDIO SOURCE_DIR)
#
#SET(PORTAUDIO_LIB_DIR "${BINARY_DIR}/lib/.libs")
#SET(PORTAUDIO_INCLUDE_DIR "${SOURCE_DIR}/include")
#
#ADD_LIBRARY(portaudio STATIC IMPORTED)
#SET_PROPERTY(TARGET portaudio PROPERTY IMPORTED_LOCATION "${PORTAUDIO_LIB_DIR}/libportaudio.a")
#include_directories(${portaudio_inc_dir})


#GTK3
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

#ALSA
pkg_check_modules(ALSA REQUIRED alsa)
include_directories(${ALSA_INCLUDE_DIRS})
link_directories(${ALSA_LIBRARY_DIRS})
add_definitions(${ALSA_CFLAGS_OTHER})

#LibSoundFile
pkg_check_modules(LIBSNDFILE sndfile)
include_directories(${LIBSNDFILE_INCLUDE_DIRS})
link_directories(${LIBSNDFILE_LIBRARY_DIRS})
add_definitions(${LIBSNDFILE_CFLAGS_OTHER})


#PortAudio
#pkg_check_modules(PORTAUDIO portaudio)
#include_directories(${PORTAUDIO_INCLUDE_DIRS})
#link_directories(${PORTAUDIO_LIBRARY_DIRS})
#add_definitions(${PORTAUDIO_CFLAGS_OTHER})


set(SOURCE_FILES main.c app/app.c app/app.h pa/PaPlay.c pa/PaPlay.h)
add_executable(dj-mix-recorder ${SOURCE_FILES})

#Link the libraries
target_link_libraries(dj-mix-recorder ${GTK3_LIBRARIES} ${ALSA_LIBRARIES} ${CMAKE_THREADS_LIBS_INIT} ${LIBSNDFILE_LIBRARIES} portaudio)
