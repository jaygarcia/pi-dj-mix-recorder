cmake_minimum_required(VERSION 3.8)
project(djsr_clion)

set(CMAKE_C_STANDARD 99)

find_package(PkgConfig REQUIRED)

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

set(SOURCE_FILES main.c APlay/APlay.c APlay/APlay.h APlay/aconfig.h APlay/formats.h APlay/gettext.h app/app.c app/app.h)
add_executable(dj-mix-recorder ${SOURCE_FILES})

#Link the libraries
target_link_libraries(dj-mix-recorder ${GTK3_LIBRARIES} ${ALSA_LIBRARIES})
