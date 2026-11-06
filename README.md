# Conway's Game of Life
## Prerequisite
- C Compiler
- cmake
- SDL3 dev package with CMake config files. Refer to [Wiki](https://github.com/libsdl-org/SDL/blob/main/INSTALL.md) if needed
## Build Instructions
From root of the project
```bash
$ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
$ cmake --build build -j
```
Binary will be `build/game_of_life`

## Usage
Execute the program i.e. `build/game_of_life`. Select the cells to make them alive/dead. Dragging also works.


For a random fill with `X%` fill launch the program from a terminal emulator and give the fill % as argument 1


Example, For a random fill of 10 %.
`$ ./game_of_life 10`

Press `Enter` to start the simulation

## Modifications
Modify the global constants in the code and recompile the program.