# CMakeList.txt : CMake project for wsl_cmake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.5)

set(MSG "=======My cmake file=======")

message(${MSG})

#find_package(CycloneDDS)

project(wsl_cmake)

set(CYCLONEDDS_CXX_INC "/opt/cyclonedds-cxx/include/ddscxx")
set(CYCLONEDDS_CXX_LIB "/opt/cyclonedds-cxx/lib")
set(CYCLONEDDS_LIB "/opt/cyclonedds/lib")
set(CYCLONEDDS_INC "/opt/cyclonedds/include")

add_library(TIMER STATIC timer.cpp timer.h)

add_executable (wsl_cmake main.cpp)

target_include_directories(wsl_cmake PRIVATE ${CYCLONEDDS_CXX_INC})
target_include_directories(wsl_cmake PRIVATE ${CYCLONEDDS_INC})

target_link_libraries(wsl_cmake PUBLIC TIMER pthread)
target_link_libraries(wsl_cmake PUBLIC ${CYCLONEDDS_CXX_LIB}/libddscxx.so
                                       ${CYCLONEDDS_CXX_LIB}/libidlcxx.so
                                       ${CYCLONEDDS_LIB}/libddsc.so
                                       ${CYCLONEDDS_LIB}/libidl.so
                                       )

set_target_properties(
    wsl_cmake PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
   )