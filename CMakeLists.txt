cmake_minimum_required(VERSION 3.12)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
set(THIRD_PARTY_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
include(dependencies)

add_executable(tencent_reader "main.cpp")

set(BINARY_NAME "tencent_reader")
string(TOLOWER ${BINARY_NAME} BINARY_NAME)

# Global properties
# =================
set_target_properties(tencent_reader PROPERTIES OUTPUT_NAME "${BINARY_NAME}")
set_target_properties(tencent_reader PROPERTIES CXX_STANDARD              17)
set_target_properties(tencent_reader PROPERTIES CXX_STANDARD_REQUIRED     ON)
set_target_properties(tencent_reader PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Link Flags
# ==========
set_property(TARGET tencent_reader APPEND_STRING PROPERTY LINK_FLAGS " -rdynamic")

# Linked Libraries
# ================
target_link_libraries(tencent_reader PUBLIC -ldl -lz)
target_link_libraries(tencent_reader PUBLIC liblief spdlog)

