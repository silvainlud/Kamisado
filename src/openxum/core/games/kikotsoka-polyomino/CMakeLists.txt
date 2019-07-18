INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src)

LINK_DIRECTORIES(
  ${Boost_LIBRARY_DIRS})

SET(OPENXUM_WS_CPP_CORE_GAMES_KIKOTSOKA_POLYOMINO_HPP color.hpp coordinates.hpp engine.hpp game_type.hpp move.hpp phase.hpp polyomino.hpp state.hpp)
SET(OPENXUM_WS_CPP_CORE_GAMES_KIKOTSOKA_POLYOMINO_CPP coordinates.cpp engine.cpp move.cpp polyomino.cpp state.cpp)

ADD_LIBRARY(openxum-core-games-kikotsoka-polyomino SHARED ${OPENXUM_WS_CPP_CORE_GAMES_KIKOTSOKA_POLYOMINO_HPP};${OPENXUM_WS_CPP_CORE_GAMES_KIKOTSOKA_POLYOMINO_CPP})

INSTALL(FILES ${OPENXUM_WS_CPP_CORE_GAMES_KIKOTSOKA_POLYOMINO_HPP} DESTINATION
  ${OPENXUM_WS_CPP_INCLUDE_DIRS}/openxum/core/games/kikotsoka-polyomino/)
