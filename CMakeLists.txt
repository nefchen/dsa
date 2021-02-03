cmake_minimum_required(VERSION 3.16.3)
project(deadly_space_adventures)

set(CMAKE_CXX_STANDARD 20)

add_executable(dsa
	src/main.cpp
	src/window.cpp
	src/views/view.cpp
	src/views/start_screen.cpp
	src/views/widgets/label.cpp
	src/views/widgets/cycle_menu.cpp
)

target_include_directories(dsa PRIVATE src)
target_link_libraries(dsa SDL2 SDL2_ttf pthread)

