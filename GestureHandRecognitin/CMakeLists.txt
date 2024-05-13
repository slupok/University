cmake_minimum_required(VERSION 3.5)

project(GestureHandRecognition VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD            17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS         OFF)
set(CMAKE_AUTOMOC                 ON)
# set(CMAKE_PREFIX_PATH "C:\\Qt\\5.15.2\\mingw81-64\\")
set(BIN_BUILD_DIR ${CMAKE_BINARY_DIR}/bin)
set(LIB_BUILD_DIR ${CMAKE_BINARY_DIR}/lib)

set(       CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_BUILD_DIR})
set(       CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIB_BUILD_DIR})
set(       CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIB_BUILD_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${BIN_BUILD_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${LIB_BUILD_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${LIB_BUILD_DIR})

set(         CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_BUILD_DIR})
set(         CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIB_BUILD_DIR})
set(         CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIB_BUILD_DIR})

set(    CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${BIN_BUILD_DIR})
set(    CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${LIB_BUILD_DIR})
set(    CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${LIB_BUILD_DIR})

#OpenCL
find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(
  ${LIB_BUILD_DIR}
  ${BIN_BUILD_DIR}
  ${OpenCL_LIBRARY}
)

include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HDRS ${PROJECT_SOURCE_DIR}/src/*.h)

add_executable(${CMAKE_PROJECT_NAME} ${HDRS} ${SRCS})

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui Multimedia Multimediawidgets)
target_link_libraries(
  ${CMAKE_PROJECT_NAME}
  PRIVATE
  Qt5::Core
  Qt5::Widgets
  Qt5::Gui
  Qt5::Multimedia
  ${OpenCL_LIBRARY}
)

message("${CMAKE_CXX_COMPILER_ID} COMPILER")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

  target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC
    /W4
    # /WX TODO uncomment
  )
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC
    # https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
    # -Werror TODO uncomment
    -Wall
    -Wextra
    -Wpedantic
    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wenum-compare
    -Wfloat-equal
    -Wredundant-decls
    -Wsign-conversion
    # https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html
    -Wctor-dtor-privacy
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wsign-promo
  )
endif()