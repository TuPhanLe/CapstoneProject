# The following four lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

# (Not part of the boilerplate)
# This example uses an extra component for common functions such as Wi-Fi and Ethernet connection.
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/common_components/protocol_examples_common)
add_compile_options(-fdiagnostics-color=always  -Wno-error=format= -Wno-format)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(Source_esp32)

target_add_binary_data(Source_esp32.elf "main/mqtt_eclipseprojects_io.pem" TEXT)
