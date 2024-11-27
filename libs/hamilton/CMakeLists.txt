add_library(hamilton STATIC
            "hamilton.cpp"
            "atsp_solver.cpp"
            )
target_include_directories(hamilton PUBLIC "include")
target_link_libraries(hamilton core)