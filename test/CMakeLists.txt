INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${OPENXUM_WS_CPP_BINARY_DIR}/src)

LINK_DIRECTORIES()

ADD_EXECUTABLE(openxum-ws-cpp-test main.cpp)

TARGET_LINK_LIBRARIES(openxum-ws-cpp-test openxum-core-games-kamisado openxum-core-games-kikotsoka openxum-ai-common)
