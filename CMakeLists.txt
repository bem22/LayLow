cmake_minimum_required(VERSION 3.15)
project(synapse_shot C)

# Use C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


# Default to RelWithDebInfo if unspecified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

# Find system libraries via pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBRAW REQUIRED libraw)
pkg_check_modules(LIBPNG REQUIRED libpng)

# Collect all .c sources under src/
file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/src/*.c"
)
if(NOT ALL_SOURCES)
  message(FATAL_ERROR "No source files found in src/")
endif()

# Executable target with every .c file
add_executable(synapse_shot ${ALL_SOURCES})

target_compile_definitions(synapse_shot PRIVATE
  PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

target_compile_definitions(synapse_shot PRIVATE
  DATADIR="${CMAKE_SOURCE_DIR}/data"
)

# Include your public headers plus libraw/libpng
target_include_directories(synapse_shot PRIVATE
  ${PROJECT_SOURCE_DIR}/include
  ${LIBRAW_INCLUDE_DIRS}
  ${LIBPNG_INCLUDE_DIRS}
)

# Link against raw + png
target_link_libraries(synapse_shot PRIVATE
  ${LIBRAW_LIBRARIES}
  ${LIBPNG_LIBRARIES}
  m
)
