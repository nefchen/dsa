cmake_minimum_required(VERSION 3.16.3)
project(deadly_space_adventures)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++)

add_executable(dsa
	main.cpp
	views/view.cpp
	views/start_screen/start_screen.cpp
	views/game_screen/game_screen.cpp
	widgets/label.cpp
	widgets/cycle_menu.cpp
)

target_include_directories(dsa PRIVATE .)
target_link_libraries(dsa SDL2 SDL2_ttf pthread)

