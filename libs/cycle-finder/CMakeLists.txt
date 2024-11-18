add_library(cycle-finder STATIC 
            "cycle_finder.cpp"
            )
target_include_directories(cycle-finder PUBLIC "include")
target_link_libraries(cycle-finder core)