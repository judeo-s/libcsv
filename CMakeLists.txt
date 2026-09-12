cmake_minimum_required(VERSION 3.14)

project(libcsv VERSION 0.1.0 LANGUAGES CXX)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(LIBCSV_IS_TOP_LEVEL ON)
else()
    set(LIBCSV_IS_TOP_LEVEL OFF)
endif()

option(LIBCSV_BUILD_TESTS "Build libcsv tests" ${LIBCSV_IS_TOP_LEVEL})
option(LIBCSV_BUILD_EXAMPLES "Build libcsv examples" ${LIBCSV_IS_TOP_LEVEL})

add_library(libcsv
    src/error.cxx
    src/reader.cxx
    src/writer.cxx
)
add_library(libcsv::csv ALIAS libcsv)

set_target_properties(libcsv
    PROPERTIES
    OUTPUT_NAME csv
)

target_compile_features(libcsv PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(libcsv PRIVATE /W4)
else()
    target_compile_options(libcsv PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_include_directories(libcsv
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(LIBCSV_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(LIBCSV_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
