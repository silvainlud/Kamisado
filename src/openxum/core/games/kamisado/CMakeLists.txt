INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}/src)

LINK_DIRECTORIES(
        ${Boost_LIBRARY_DIRS})

SET(OPENXUM_WS_CPP_CORE_GAMES_KAMISADO_HPP coordinates.hpp engine.hpp game_type.hpp decision.hpp state.hpp)
SET(OPENXUM_WS_CPP_CORE_GAMES_KAMISADO_CPP coordinates.cpp engine.cpp decision.cpp state.cpp)

ADD_LIBRARY(openxum-core-games-kamisado SHARED ${OPENXUM_WS_CPP_CORE_GAMES_KAMISADO_HPP};${OPENXUM_WS_CPP_CORE_GAMES_KAMISADO_CPP})

INSTALL(FILES ${OPENXUM_WS_CPP_CORE_GAMES_KAMISADO_HPP} DESTINATION
        ${OPENXUM_WS_CPP_INCLUDE_DIRS}/openxum/core/games/kamisado/)
