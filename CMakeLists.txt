cmake_minimum_required(VERSION 3.19)
project(MultiLib VERSION 1.0.0 LANGUAGES CXX)

set(MULTILIB_NAME ${PROJECT_NAME})

option(USE_ASAN "Toggle for Asan [ON] if not arm" ON)
option(TESTS "Toggle for unit tests [ON] if ${PROJECT_NAME} is primary build target" ON)

include(GNUInstallDirs)
file(GLOB_RECURSE SRC_FILES src/*.cc src/*.cpp)

add_library(${PROJECT_NAME} INTERFACE)

message("Platform: ${CMAKE_SYSTEM_PROCESSOR}")
message("Compiler: ${CMAKE_CXX_COMPILER_ID}")
message("Build type: ${CMAKE_BUILD_TYPE}")

if(NOT ${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  if (${TESTS})
    message("[${PROJECT_NAME}]: Disabling tests as not primary target of the build")
    set(TESTS OFF)
  endif()
  if (${USE_ASAN})
    message("[${PROJECT_NAME}]: Disabling Asan as not primary target of the build")
    set(USE_ASAN OFF)
  endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release" AND USE_ASAN)
  message(WARNING "Asan requested, but disabled because build type: Release")
  set(USE_ASAN OFF)
endif()

# Condition for arm as of now there's an upstream issue with pagesize
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "(arm|aarch64)")
  if(${USE_ASAN})
    message(WARNING "Asan requested, but platform ${CMAKE_SYSTEM_PROCESSOR} \ndisabling Asan due to pagesize issues.")
    set(USE_ASAN OFF)
  endif()
endif()

if(${USE_ASAN})
  message("Asan [enabled]")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND USE_ASAN)
  target_compile_definitions(${PROJECT_NAME} INTERFACE $<$<BOOL:${USE_ASAN}>: -fsanitize=address>)
endif()

# MSVC specific flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # Maximal MSVC optimizaton
  target_compile_options(${PROJECT_NAME} INTERFACE $<$<CONFIG:Release>: -O2>)
  # Extra warnings
  target_compile_options(${PROJECT_NAME} INTERFACE $<$<CONFIG:Debug>: -Wall>)
  # Fixes Asan MSVC issues
  target_compile_definitions(${PROJECT_NAME} INTERFACE $<$<BOOL:${USE_ASAN}>: _DISABLE_VECTOR_ANNOTATION _DISABLE_STRING_ANNOTATION _DISABLE_OPTIONAL_ANNOTATION>)
else()
  # Maximal optimization
  target_compile_options(${PROJECT_NAME} INTERFACE $<$<CONFIG:Release>: -O3>)
  # Extra warnings
  target_compile_options(${PROJECT_NAME} INTERFACE $<$<CONFIG:Debug>: -Wall -Wextra -Wpedantic -fno-omit-frame-pointer>)
  # Linking Asan required on linux
  target_link_options(${PROJECT_NAME} INTERFACE $<$<BOOL:${USE_ASAN}>:-fsanitize=address>)
endif()

# Gets global .cc files
file(GLOB_RECURSE SHARED_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
# Adds the global_cc files to the build
target_sources(${PROJECT_NAME} INTERFACE ${SHARED_SRCS})

# Get component headers for install and build
file(GLOB_RECURSE COPY_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/component1/include/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/component1/include/*.hpp
)

# Copies headers to build/include/$SUBDIRS 
file(COPY ${COPY_HEADERS} DESTINATION ${CMAKE_BINARY_DIR}/include/${PROJECT_NAME}/)
add_subdirectory(component1)

# Adds global headers and headers copied from components
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

# Links components to the multilib
target_link_libraries(${PROJECT_NAME} INTERFACE component1)

# Cmake install setup
install(EXPORT "${PROJECT_NAME}Targets"
  FILE "${PROJECT_NAME}Targets.cmake"
  NAMESPACE "${PROJECT_NAME}::"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
