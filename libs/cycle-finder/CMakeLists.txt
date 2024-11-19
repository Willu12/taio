add_library(cycle-finder STATIC 
            "src/cycle_finder.cpp"
            "src/strongy_connected_components.cpp"
            )
target_include_directories(cycle-finder PUBLIC "include")
target_link_libraries(cycle-finder core)