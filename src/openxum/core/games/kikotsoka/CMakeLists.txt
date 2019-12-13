INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}/src)

LINK_DIRECTORIES(
        ${Boost_LIBRARY_DIRS})

SET(OPENXUM_WS_CPP_CORE_GAMES_KIKOTSOKA_HPP color.hpp coordinates.hpp engine.hpp game_type.hpp decision.hpp phase.hpp state.hpp)
SET(OPENXUM_WS_CPP_CORE_GAMES_KIKOTSOKA_CPP coordinates.cpp engine.cpp decision.cpp state.cpp)

ADD_LIBRARY(openxum-core-games-kikotsoka SHARED ${OPENXUM_WS_CPP_CORE_GAMES_KIKOTSOKA_HPP};${OPENXUM_WS_CPP_CORE_GAMES_KIKOTSOKA_CPP})

INSTALL(FILES ${OPENXUM_WS_CPP_CORE_GAMES_KIKOTSOKA_HPP} DESTINATION
        ${OPENXUM_WS_CPP_INCLUDE_DIRS}/openxum/core/games/kikotsoka/)
