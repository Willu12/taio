add_subdirectory("hamilton")
add_subdirectory("cycle-finder")
add_subdirectory("metric")
add_subdirectory("core")
add_subdirectory("CLI11")

set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
add_subdirectory("google-benchmark")
