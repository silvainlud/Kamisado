INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${OPENXUM_WS_CPP_BINARY_DIR}/src)

LINK_DIRECTORIES()

ADD_EXECUTABLE(openxum-ws-cpp-test main.cpp)

TARGET_LINK_LIBRARIES(openxum-ws-cpp-test openxum-core-games-kikotsoka openxum-ai-common)

ADD_EXECUTABLE(openxum-ws-cpp-test-kikotsoka-polyomino main_kikotsoka_polyomino.cpp)

TARGET_LINK_LIBRARIES(openxum-ws-cpp-test-kikotsoka-polyomino openxum-core-games-kikotsoka-polyomino openxum-ai-common)

ADD_EXECUTABLE(openxum-ws-cpp-test-kamisado main_kamisado.cpp)

TARGET_LINK_LIBRARIES(openxum-ws-cpp-test-kamisado openxum-core-games-kamisado openxum-ai-common)
