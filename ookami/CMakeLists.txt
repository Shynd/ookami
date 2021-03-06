set(TARGET_NAME ookami)
set(TARGET_HEADER_SUBDIR "ookami")

set(ENABLE_WARNINGS_SETTINGS OFF)

add_library(${TARGET_NAME} SHARED
  src/dllmain.cpp
  src/detours/detour_manager.cpp
  src/detours/framescript.cpp
  src/functions/lua.cpp
  src/objmgr/objmgr.cpp
)

target_link_libraries(${TARGET_NAME}
  PUBLIC
    # no public dependencies - add libraries here which CONSUMERS of this
    # library require.
  PRIVATE
    # private dependencies that are only necessary to build this library itself.
)

# Define a macro that allows consumers of this lib to know they link against it
target_compile_definitions(${TARGET_NAME} PUBLIC HAS_OOKAMI)

# generate the export header for this library
include(GenerateExportHeader)
generate_export_header(${TARGET_NAME} EXPORT_FILE_NAME "${TARGET_HEADER_SUBDIR}${TARGET_NAME}_export.h")

# CMake scripts extensions
target_set_warnings(${TARGET_NAME} ENABLE ALL AS_ERROR ALL DISABLE Annoying)
target_enable_lto(${TARGET_NAME} optimized) #enable lto if available for non-debug configurations
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_14)

# group this lib into the 'libs' group for some IDEs
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER libs)

# add headers as sources automatically - this makes them show up in some IDEs
# but is not strictly necessary (unless using CMAKE_AUTOMOC)
file(GLOB_RECURSE HEADERS "include/*.hpp")
target_sources(${TARGET_NAME} PRIVATE ${HEADERS})

# Set the include directories of this library
target_include_directories(${TARGET_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

###### INSTALLATION

# Install in the desired folder
install(TARGETS ${TARGET_NAME}
  EXPORT ${PROJECT_TARGETS}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install includes
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# Install PDB file (MSVC debug symbols) for consumers of this library, if this library is 
# a shared library
get_target_property(target_type ${TARGET_NAME} TYPE)
if(target_type STREQUAL SHARED_LIBRARY)
  install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION ${CMAKE_INSTALL_PDBDIR} OPTIONAL)
endif()
# Install the export header
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_HEADER_SUBDIR}${TARGET_NAME}_export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${TARGET_HEADER_SUBDIR})