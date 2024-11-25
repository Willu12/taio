add_library(core STATIC 
            "include/core.hpp"
            "core.cpp"
            )
target_include_directories(core PUBLIC "include")
