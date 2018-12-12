if(__add_dependencies)
	return()
endif()
set(__add_dependencies ON)
include(ExternalProject)

# spdlog
# ======
set(SPDLOG_VERSION 1.2.1)
set(SPDLOG_URL "${THIRD_PARTY_DIRECTORY}/spdlog-${SPDLOG_VERSION}.tar.gz" CACHE STRING "URL to the SPDLOG fiel")
ExternalProject_Add(spdlog-project
  URL               ${SPDLOG_URL}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   "")
ExternalProject_get_property(spdlog-project SOURCE_DIR)
set(SPDLOG_SOURCE_DIR "${SOURCE_DIR}")

add_library(spdlog INTERFACE)
add_dependencies(spdlog spdlog-project)
target_include_directories(spdlog INTERFACE ${SPDLOG_SOURCE_DIR}/include/)

# LIEF
# ================
set(LIEF_CMAKE_ARGS
  -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  -DLIEF_DOC=off
  -DLIEF_PYTHON_API=off
  -DLIEF_EXAMPLES=off
  -DLIEF_C_API=off
  -DLIEF_PE=off
  -DLIEF_MACHO=off
  -DLIEF_OAT=off
  -DLIEF_DEX=off
  -DLIEF_VDEX=off
  -DLIEF_ART=off
  -DLIEF_LOGGING=off
  -DLIEF_SHARED_LIB=off
  -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
  -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
)

set(LIEF_VERSION      "b52f6ff4d37d6aa0e7555d1b02c3f9d9de25c5be")
set(LIEF_GIT          "https://github.com/lief-project/LIEF.git")
set(LIEF_PREFIX       "${CMAKE_CURRENT_BINARY_DIR}/LIEF")
set(LIEF_INSTALL_DIR  "${LIEF_PREFIX}/install")
set(LIEF_INCLUDE_DIRS "${LIEF_INSTALL_DIR}/include")
set(LIEF_LIBRARIES
  "${LIEF_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}LIEF${CMAKE_STATIC_LIBRARY_SUFFIX}")


ExternalProject_Add(LIEF
  PREFIX           "${LIEF_PREFIX}"
  GIT_REPOSITORY   ${LIEF_GIT}
  GIT_TAG          ${LIEF_VERSION}
  INSTALL_DIR      ${LIEF_INSTALL_DIR}
  CMAKE_ARGS       ${LIEF_CMAKE_ARGS}
  BUILD_BYPRODUCTS ${LIEF_LIBRARIES}
  UPDATE_COMMAND   ""
)

add_library(liblief INTERFACE)
add_dependencies(liblief LIEF)
target_link_libraries(liblief INTERFACE ${LIEF_LIBRARIES})
target_include_directories(liblief INTERFACE ${LIEF_INSTALL_DIR}/include/)

