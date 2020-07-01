cmake_minimum_required(VERSION 3.15)
project(Engine)

set(CMAKE_MODULE_PATH  ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(SDL2_ttf REQUIRED)
include_directories(${SDLTTF_INCLUDE_DIR})

add_executable(Engine main.cpp test/TestClass.h test/Object.h src/options/UIValues.h src/options/EngineValues.h src/base/Libraries.h src/support/Util.h src/math/MathConstants.h src/base/drawer/Drawer.h src/math/Transformer.h src/base/events/KeyboardHandler.h src/base/interaction/Importer.h src/options/Path.h)

target_link_libraries(Engine ${SDL2_LIBRARY} ${SDLTTF_LIBRARY})