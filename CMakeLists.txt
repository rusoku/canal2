cmake_minimum_required(VERSION 3.24)
set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)

project(canal)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_COMPILER)
set(CMAKE_CXX_COMPILER)

cmake_host_system_information(RESULT PRETTY_NAME QUERY DISTRIB_PRETTY_NAME)
message(STATUS "${PRETTY_NAME}")

#cmake_host_system_information(RESULT DISTRO QUERY DISTRIB_INFO)
#foreach(VAR IN LISTS DISTRO)
#    message(STATUS "${VAR}=`${${VAR}}`")
#endforeach()

#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#set(LLVM_STATIC_LINK_CXX_STDLIB true)

#add_definitions(-DUNICODE)
#add_definitions(-D_UNICODE)
add_definitions(-D_MBCS)
add_definitions(-D_WIN32_WINNT_WIN10)
#add_definitions(-DWIN32_LEAN_AND_MEAN)
#add_definitions(-DENABLE_SSE2=ON)

#add_compile_options(-Wall -Wextra -Wpedantic)
add_compile_options(-Wno-pragma-once-outside-header)
add_compile_options(-Wno-unused-parameter)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    #add_compile_definitions(canal DEBUG_CANAL)
    #set_target_properties(canal PROPERTIES SUFFIX "d")
endif()

#add_executable(DeviceInterfaceList main.c CAN_interface_list.c CAN_interface_list.h)
#include_directories(include)

add_library(canal SHARED
        CAN_interface_list.c
        debug.c
        CDllList.cpp
        CDllDrvObj.cpp
        CTouCANobj.cpp
        CTouCANobjCmdMsg.cpp
        CTouCANobjWinUSB.cpp
        include/CDllDrvObj.h include/canal.h include/CTouCANobj.h include/CDllList.h
        canal.cpp include/canal_macro.h utils.c Threads.cpp)

set_property(TARGET canal PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(canal cfgmgr32 winusb)
set_target_properties(canal PROPERTIES OUTPUT_NAME "canal$<$<CONFIG:Debug>:d>")



