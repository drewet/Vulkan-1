#
# VulKan ToolS (VKTS) CMake file.
#

cmake_minimum_required(VERSION 3.2)

project (VKTS)

add_definitions(-DVK_PROTOTYPES)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../VKTS_External/include ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

list(APPEND IGNORE_CPP_FILES )

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

	include_directories(AFTER C:/VulkanSDK/1.0.3.1/Include)

    if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)

        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
        
    endif ()        

    list(APPEND IGNORE_CPP_FILES    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/processor/fn_processor_android.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/processor/fn_processor_linux.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_xlib.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_xlib_key.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_android.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_android_key.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_none.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/fn_wsi_android.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/fn_wsi_xlib.cpp
    )

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    list(APPEND IGNORE_CPP_FILES    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/processor/fn_processor_android.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/processor/fn_processor_windows.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_win32.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_win32_key.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_android.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_android_key.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_none.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/fn_wsi_android.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/fn_wsi_win32.cpp
    )

endif ()

list(REMOVE_ITEM CPP_FILES ${IGNORE_CPP_FILES})

add_library(VKTS ${CPP_FILES})

set_property(TARGET VKTS PROPERTY CXX_STANDARD 11)
set_property(TARGET VKTS PROPERTY CXX_STANDARD_REQUIRED ON)
