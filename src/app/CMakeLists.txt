INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}/src
        ${OPENXUM_WS_CPP_BINARY_DIR}/src)

LINK_DIRECTORIES()

CONFIGURE_FILE(version.hpp.in
        ${OPENXUM_WS_CPP_BINARY_DIR}/src/app/version.hpp)

# add_definitions(-O2)

ADD_EXECUTABLE(openxum-ws-cpp-main main.cpp)

TARGET_LINK_LIBRARIES(openxum-ws-cpp-main openxum-core-games-kamisado openxum-ai-common)
