# Project specific options :
#   - BUILD_USE_DOXYGEN
#   - BUILD_BUILD_TESTS (requires BUILD_TESTING set to ON)
# Other options might be available through the cmake scripts including (not exhaustive):
#   - BUILD_ENABLE_WARNINGS_SETTINGS
#   - BUILD_ENABLE_LTO
#

cmake_minimum_required(VERSION 3.8.2)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

set(PROJECT_NAME "ookami")
set(PROJECT_TARGETS "ookamiTargets")

project(${PROJECT_NAME} CXX)

#### Setup Installation directories
include(GNUInstallDirs)
set(CMAKE_INSTALL_PDBDIR ${CMAKE_INSTALL_BINDIR}) # typically, we want the PDB files in the same directory

# output all binaries and libs into a common folder across all libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

############################
##  Modules and scripts   ##
############################

include(CTest) # Must be called before adding tests but after calling project(). This automatically calls enable_testing() and configures ctest targets when using Make/Ninja
include(CMakeDependentOption)# This is a really useful scripts that creates options that depends on other options. It can even be used with generator expressions !

# Custom modules and scripts

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(LTO)
include(Warnings)
include(CopyDllsForDebug)
include(Coverage)

###############
##  OPTIONS  ##
###############

option(BUILD_USE_DOXYGEN "Add a doxygen target to generate the documentation" ON)

cmake_dependent_option(BUILD_BUILD_TESTS
    "Enable ookami project tests targets" ON # By default we want tests if CTest is enabled
    "BUILD_TESTING" OFF # Stay coherent with CTest variables
)

# External dependencies
add_subdirectory(external EXCLUDE_FROM_ALL)

# It is always easier to navigate in an IDE when projects are organized in folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)



# Check for LTO support (needs to be after project(...) )
find_lto(CXX)

add_subdirectory(injector)
add_subdirectory(ookami)

# Setup the injector as the startup project for VS so people don't have to do it manually.
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT injector)

###############
##   TESTS   ##
###############

# Todo

###############
##  Doxygen  ##
###############

# Todo