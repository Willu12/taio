add_library(metric STATIC 
            "metric.cpp"
            )
target_include_directories(metric PUBLIC "include")
target_link_libraries(metric core)