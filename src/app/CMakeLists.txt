INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}/src
        ${OPENXUM_WS_CPP_BINARY_DIR}/src)

LINK_DIRECTORIES()

CONFIGURE_FILE(version.hpp.in
        ${OPENXUM_WS_CPP_BINARY_DIR}/src/app/version.hpp)

add_definitions(-O3)

find_package(Torch REQUIRED FILES PATHS ../../lib/libtorch)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")


ADD_EXECUTABLE(find-lambda main.cpp)
ADD_EXECUTABLE(ia ia-mode.cpp)
ADD_EXECUTABLE(lambda lambda-mode.cpp)

TARGET_LINK_LIBRARIES(find-lambda openxum-core-games-kamisado openxum-ai-common ${TORCH_LIBRARIES})
TARGET_LINK_LIBRARIES(ia openxum-core-games-kamisado openxum-ai-common ${TORCH_LIBRARIES})
TARGET_LINK_LIBRARIES(lambda openxum-core-games-kamisado openxum-ai-common ${TORCH_LIBRARIES})
set_property(TARGET find-lambda PROPERTY CXX_STANDARD 14)
set_property(TARGET ia PROPERTY CXX_STANDARD 14)
set_property(TARGET lambda PROPERTY CXX_STANDARD 14)