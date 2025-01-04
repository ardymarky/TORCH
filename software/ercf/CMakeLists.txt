# v3.14 required for FetchContent_MakeAvailable
cmake_minimum_required(VERSION 3.14)
if (DEFINED MCU)
  include(FetchContent)
  FetchContent_Declare(
    mcu_support
    GIT_REPOSITORY https://github.com/bolderflight/mcu-support.git
    GIT_TAG v1.1.0
  )
  FetchContent_MakeAvailable(mcu_support)
  # Setting up the toolchain
  set(CMAKE_TOOLCHAIN_FILE "${mcu_support_SOURCE_DIR}/cmake/cortex.cmake")
  # Project information
  project(ERCF  
    VERSION 1.0.2
    DESCRIPTION "ERCF driver"
    LANGUAGES CXX
  )
  # Grab the processor and set up definitions and compile options
  include(${mcu_support_SOURCE_DIR}/cmake/config_mcu.cmaFerke)
  configMcu(${MCU} ${mcu_support_SOURCE_DIR})
  # Fetch core
  FetchContent_Declare(
    core
    GIT_REPOSITORY https://github.com/bolderflight/core.git
    GIT_TAG v3.1.3
  )
  FetchContent_MakeAvailable(core)
  # Add the library target
  add_library(ercf
    src/ercf.cpp
    src/ercf.h
  )
  # Link libraries
  target_link_libraries(ercf
    PUBLIC
      core
  )
  # Setup include directories 
  target_include_directories(ercf PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
  )
endif()