add_executable (tests 
"metric-tests.cpp"
"max-cycle-tests.cpp"
)

target_link_libraries(tests core metric cycle-finder hamilton benchmark::benchmark_main)
