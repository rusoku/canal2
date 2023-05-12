cmake_minimum_required(VERSION 3.24)
project(DeviceInterfaceList C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER)
set(CMAKE_CXX_COMPILER)
#set(CMAKE_RC_COMPILER "C:/Program Files/LLVM")

#add_definitions(-DUNICODE)
#add_definitions(-D_UNICODE)
#add_definitions(-D_MBCS)

add_executable(DeviceInterfaceList main.c CAN_interface_list.c CAN_interface_list.h)

target_link_libraries(DeviceInterfaceList
        cfgmgr32
        )

#Unicode, wchar_t based wide-characters and strings encoded as UTF-16.
#Multibyte Character Sets (MBCS), char based single or double-byte characters and strings encoded in a locale-specific character set.

#SET(TOOLCHAIN «/usr/local/arm/gcc-arm/bin/»)
#SET(CMAKE_C_COMPILER ${TOOLCHAIN}/arm-none-eabi-gcc)
#SET(CMAKE_CXX_COMPILER ${TOOLCHAIN}/arm-none-eabi-gcc)
