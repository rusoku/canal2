cmake_minimum_required(VERSION 3.24)
set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)

project(canal2)

#https://devblogs.microsoft.com/cppblog/introducing-the-universal-crt/
#https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-library-features?view=msvc-170

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_COMPILER)
set(CMAKE_CXX_COMPILER)

#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#set(LLVM_STATIC_LINK_CXX_STDLIB true)

#add_definitions(-DUNICODE)
#add_definitions(-D_UNICODE)
#add_definitions(-D_MBCS)
add_definitions(-D_WIN32_WINNT_WIN10)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    #add_compile_definitions(canal2 DEBUG_CANAL_CONSOLE)
endif()

#add_executable(DeviceInterfaceList main.c CAN_interface_list.c CAN_interface_list.h)

include_directories(include)

add_library(canal2 SHARED
        CAN_interface_list.c
        debug.c
        CDllList.cpp
        CDllDrvObj.cpp
        CTouCANobj.cpp
        CTouCANobjCmdMsg.cpp
        CTouCANobjWinUSB.cpp
        include/CDllDrvObj.h include/canal.h include/CTouCANobj.h include/CDllList.h
        canal.cpp include/canal_macro.h utils.c Threads.cpp)

set_property(TARGET canal2 PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(canal2 cfgmgr32 winusb)

#Unicode, wchar_t based wide-characters and strings encoded as UTF-16.
#Multibyte Character Sets (MBCS), char based single or double-byte characters and strings encoded in a locale-specific character set.

#SET(TOOLCHAIN «/usr/local/arm/gcc-arm/bin/»)
#SET(CMAKE_C_COMPILER ${TOOLCHAIN}/arm-none-eabi-gcc)
#SET(CMAKE_CXX_COMPILER ${TOOLCHAIN}/arm-none-eabi-gcc)
