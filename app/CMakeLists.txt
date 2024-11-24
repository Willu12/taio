add_executable (app "app.cpp" "multigraph_cli.cpp")
set_target_properties(app PROPERTIES CXX_STANDARD 23)

target_link_libraries(app
                      cycle-finder
                      hamilton
                      metric
                      CLI11::CLI11)
