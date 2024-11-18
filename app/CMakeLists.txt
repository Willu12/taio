add_executable (app "app.cpp")
set_target_properties(app PROPERTIES CXX_STANDARD 23)

target_link_libraries(app cycle-finder hamilton metric)
