project(component1 VERSION 1.0.0 LANGUAGES CXX)
set(COMPONENT_NAME ${PROJECT_NAME})
include(GNUInstallDirs)

file(GLOB_RECURSE SRC_FILES src/*.cc)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

set_target_properties(${PROJECT_NAME}
  PROPERTIES
  CMAKE_CXX_STANDARD 11
  PREFIX ""
  CMAKE_CXX_STANDARD_REQUIRED ON
)

# Dependency lookup, replace PACKAGE_NAME by your package
#find_package(PACKAGE_NAME REQUIRED Component)

# Report missing dependencies to user
#if(NOT PACKAGE_NAME_FOUND)
#  message(FATAL_ERROR "Could not find PACKAGE_NAME required components: Component")
#endif()

# Link dependencies
#target_link_libraries("${PROJECT_NAME}" PUBLIC PACKAGE_NAME::Component )
#target_link_libraries("${PROJECT_NAME}" PUBLIC component_other )


target_include_directories(${PROJECT_NAME}
  PUBLIC
  #"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../component_other/include>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${MULTILIB_NAME}>"
)

install(TARGETS ${PROJECT_NAME}
  EXPORT ${MULTILIB_NAME}Targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${MULTILIB_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${MULTILIB_NAME}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${MULTILIB_NAME}
)

file(GLOB_RECURSE PUBLIC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
install(FILES ${PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${MULTILIB_NAME}/)

# If tests enabled by multilib and multilib primary target
if(${TESTS})
  enable_testing()
  add_subdirectory(tests)
endif()
