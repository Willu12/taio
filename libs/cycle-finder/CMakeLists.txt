add_library(cycle-finder STATIC 
            "src/max_cycle.cpp"
            "src/strongy_connected_components.cpp"
            )
target_include_directories(cycle-finder PUBLIC "include")
target_link_libraries(cycle-finder core)