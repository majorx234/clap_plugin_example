cmake_minimum_required(VERSION 3.16)
project(clap_plugin_example VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(CLAP_PLUGIN_TEST_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)
set(ECHO_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/echo.c
  ${CMAKE_CURRENT_SOURCE_DIR}/include/echo.h
)

add_library(echo)
target_sources(echo
 PRIVATE
  ${ECHO_SOURCES}
)
target_include_directories(echo
 PRIVATE
  include
)
target_link_libraries(echo
)


add_executable(clap_plugin_test)
target_sources(clap_plugin_test
 PRIVATE
  ${CLAP_PLUGIN_TEST_SOURCES}
)
target_include_directories(clap_plugin_test
 PRIVATE
)
target_compile_definitions(clap_plugin_test PUBLIC LIN=1)
target_link_libraries(clap_plugin_test PUBLIC -ldl)
