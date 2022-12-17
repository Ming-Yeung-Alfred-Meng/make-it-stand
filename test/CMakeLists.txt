project(Google_tests)

include(FetchContent)

FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(Google_Tests_run example_test.cpp)
target_link_libraries(Google_Tests_run core)
target_link_libraries(Google_Tests_run gtest gtest_main)
