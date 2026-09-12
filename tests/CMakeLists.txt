# Test Dependency
include(FetchContent)

# Download GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
FetchContent_MakeAvailable(googletest)

# Test executable
add_executable(libcsv_unit_tests
    main.cxx
    error_test.cxx
    options_test.cxx
    reader_test.cxx
    writer_test.cxx
)

# Link test libaries
target_link_libraries(libcsv_unit_tests
    PRIVATE
        libcsv::csv
        GTest::gtest
)

# Compile Flags
if(MSVC)
    target_compile_options(libcsv_unit_tests PRIVATE /W4)
else()
    target_compile_options(libcsv_unit_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Add test to CTest
include(GoogleTest)
gtest_discover_tests(libcsv_unit_tests)
