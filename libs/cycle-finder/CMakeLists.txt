add_library(cycle-finder STATIC 
            "src/max_cycle.cpp"
            "src/strongly_connected_components.cpp"
            "src/max_cycle_solver.cpp"
            )
target_include_directories(cycle-finder PUBLIC "include")
target_link_libraries(cycle-finder core)