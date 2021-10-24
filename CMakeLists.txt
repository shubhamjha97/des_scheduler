set (CMAKE_CXX_STANDARD 11)
project(scheduler)
add_executable(main main.cpp helper.cpp Process.cpp RNG.cpp Process.cpp FcfsScheduler.cpp Event.cpp)
set_target_properties(main PROPERTIES OUTPUT_NAME "scheduler")
