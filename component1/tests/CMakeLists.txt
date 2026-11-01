cmake_minimum_required(VERSION 3.19)
project(testkit VERSION 1.0.0 LANGUAGES CXX)

file(GLOB_RECURSE SRC_FILES src/*.cc src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${COMPONENT_NAME})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_compile_definitions(${PROJECT_NAME} PRIVATE EXECUTABLE_NAME=\"${PROJECT_NAME}\")
target_compile_definitions(${PROJECT_NAME} PRIVATE MULTILIB_NAME=\"${MULTILIB_NAME}\")
target_compile_definitions(${PROJECT_NAME} PRIVATE COMPONENT_NAME=\"${COMPONENT_NAME}\")
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>: -O3>)

