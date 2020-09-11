cmake_minimum_required(VERSION 3.16)
project(Synth C)

set(CMAKE_C_STANDARD 99)

if (PORTAUDIO_LIBRARIES AND PORTAUDIO_INCLUDE_DIRS)
    # in cache already
    set(PORTAUDIO_FOUND TRUE)
else (PORTAUDIO_LIBRARIES AND PORTAUDIO_INCLUDE_DIRS)
    if (NOT WIN32)
        include(FindPkgConfig)
        pkg_check_modules(PORTAUDIO2 portaudio-2.0)
    endif (NOT WIN32)

    if (PORTAUDIO2_FOUND)
        set(PORTAUDIO_INCLUDE_DIRS
                ${PORTAUDIO2_INCLUDE_DIRS}
                )
        if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            set(PORTAUDIO_LIBRARIES "${PORTAUDIO2_LIBRARY_DIRS}/lib${PORTAUDIO2_LIBRARIES}.dylib")
        else (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            set(PORTAUDIO_LIBRARIES
                    ${PORTAUDIO2_LIBRARIES}
                    )
        endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(PORTAUDIO_VERSION
                19
                )
        set(PORTAUDIO_FOUND TRUE)
    else (PORTAUDIO2_FOUND)
        find_path(PORTAUDIO_INCLUDE_DIR
                NAMES
                portaudio.h
                PATHS
                /usr/include
                /usr/local/include
                /opt/local/include
                /sw/include
                )

        find_library(PORTAUDIO_LIBRARY
                NAMES
                portaudio
                PATHS
                /usr/lib
                /usr/local/lib
                /opt/local/lib
                /sw/lib
                )

        find_path(PORTAUDIO_LIBRARY_DIR
                NAMES
                portaudio
                PATHS
                /usr/lib
                /usr/local/lib
                /opt/local/lib
                /sw/lib
                )

        set(PORTAUDIO_INCLUDE_DIRS
                ${PORTAUDIO_INCLUDE_DIR}
                )
        set(PORTAUDIO_LIBRARIES
                ${PORTAUDIO_LIBRARY}
                )

        set(PORTAUDIO_LIBRARY_DIRS
                ${PORTAUDIO_LIBRARY_DIR}
                )

        set(PORTAUDIO_VERSION
                18
                )

        if (PORTAUDIO_INCLUDE_DIRS AND PORTAUDIO_LIBRARIES)
            set(PORTAUDIO_FOUND TRUE)
        endif (PORTAUDIO_INCLUDE_DIRS AND PORTAUDIO_LIBRARIES)

        if (PORTAUDIO_FOUND)
            if (NOT Portaudio_FIND_QUIETLY)
                message(STATUS "Found Portaudio: ${PORTAUDIO_LIBRARIES}")
            endif (NOT Portaudio_FIND_QUIETLY)
        else (PORTAUDIO_FOUND)
            if (Portaudio_FIND_REQUIRED)
                message(FATAL_ERROR "Could not find Portaudio")
            endif (Portaudio_FIND_REQUIRED)
        endif (PORTAUDIO_FOUND)
    endif (PORTAUDIO2_FOUND)


    # show the PORTAUDIO_INCLUDE_DIRS and PORTAUDIO_LIBRARIES variables only in the advanced view
    mark_as_advanced(PORTAUDIO_INCLUDE_DIRS PORTAUDIO_LIBRARIES)

endif (PORTAUDIO_LIBRARIES AND PORTAUDIO_INCLUDE_DIRS)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_RELEASE} -march=native -g -o3")

add_executable(Synth main.c synth.c synth.h api/device.c api/device.h devices/generator.h devices/generator.c devices/printer.c devices/printer.h devices/speaker.c devices/speaker.h utils/PA.c utils/PA.h utils/queue.c utils/queue.h devices/microphone.c devices/microphone.h devices/filter.c devices/filter.h)

target_link_libraries(Synth m)
TARGET_LINK_LIBRARIES(Synth ${PORTAUDIO_LIBRARIES})