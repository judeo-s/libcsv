# Example executable
add_executable(libcsv_basic
    basic.cxx
)

# Link library
target_link_libraries(libcsv_basic
    PRIVATE
        libcsv::csv
)

# Compile Flags
target_compile_options(libcsv_basic PRIVATE -Wall -Wextra -Wpedantic)
