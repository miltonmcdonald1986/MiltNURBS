# Test executable
set(TEST_SOURCES
	test_bernstein.cpp
    test_bezier.cpp
    test_bmp.cpp
	test_horner.cpp
    test_main.cpp
    test_vector.cpp
)

add_executable(miltnurbs_tests ${TEST_SOURCES})

target_link_libraries(miltnurbs_tests
    PRIVATE
        miltnurbs
        GTest::gtest
        GTest::gtest_main
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(miltnurbs_tests)