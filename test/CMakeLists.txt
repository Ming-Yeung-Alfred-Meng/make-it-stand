project(Google_tests)

include(FetchContent)

FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/3fa7f983c69f780378b4d1ad44d36030ca951ba6.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(Google_Tests_run center_of_mass_test.cpp inner_carving_test.cpp cube_test.cpp helper_g_test.cpp linear_index_test.cpp voxel_contouring_test.cpp voxel_indices_test.cpp)
target_link_libraries(Google_Tests_run core)
target_link_libraries(Google_Tests_run gtest gtest_main)
