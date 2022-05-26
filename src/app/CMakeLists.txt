INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}/src
        ${OPENXUM_WS_CPP_BINARY_DIR}/src)

LINK_DIRECTORIES()

CONFIGURE_FILE(version.hpp.in
        ${OPENXUM_WS_CPP_BINARY_DIR}/src/app/version.hpp)

add_definitions(-O3)

find_package(Torch REQUIRED FILES PATHS ../../lib/libtorch)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")


ADD_EXECUTABLE(openxum-ws-cpp-main main.cpp)

TARGET_LINK_LIBRARIES(openxum-ws-cpp-main openxum-core-games-kamisado openxum-ai-common ${TORCH_LIBRARIES})
set_property(TARGET openxum-ws-cpp-main PROPERTY CXX_STANDARD 14)