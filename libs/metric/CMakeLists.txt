add_library(metric STATIC "include/metric.hpp" "metric.cpp")
set_target_properties(metric PROPERTIES CXX_STANDARD 23)
target_include_directories(metric PUBLIC "include")
target_link_libraries(metric core)