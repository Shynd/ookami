set(TARGET_NAME "injector")

set(ENABLE_WARNINGS_SETTINGS OFF)

# Always list the files explicitly
add_executable(${TARGET_NAME}
  src/main.cpp
)

# Add headers as sources automatically - this makes them show up in some IDEs
# but it is not strictly necesarry (unless using CMAKE_AUTOMOC)
file(GLOB_RECURSE HEADERS "include/*.hpp")
target_sources(${TARGET_NAME} PRIVATE ${HEADERS})

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${TARGET_NAME}
  PUBLIC # Useful for libraries
    #libraries/targets to link when linking this library.
    #this will automatically setup the needed flags and dependencies when linking against this target.
    spdlog
  PRIVATE
    # add deps
)

# Requires c++14, this is better than setting CMAKE_CXX_STANDARD since it won't pollute other targets
# note: cxx_std_* features were added in CMake 3.8.2
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_14)

# CMake scripts extensions
target_set_warnings(${TARGET_NAME} ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Helper that can set default warning flags for you
target_enable_lto(${TARGET_NAME} optimized) #enable lto if available for non-debug configurations
copy_dlls_for_debug(${TARGET_NAME} "" "") # Copy dependencies next to the executable (DLLs for example)

# Install in the desired folder
install(TARGETS ${TARGET_NAME}
  EXPORT ${PROJECT_TARGETS}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)