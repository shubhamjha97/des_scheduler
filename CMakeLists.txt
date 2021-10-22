set (CMAKE_CXX_STANDARD 11)
project(scheduler)
add_executable(main main.cpp helper.cpp Process.cpp RNG.cpp Process.cpp Scheduler.cpp)
set_target_properties(main PROPERTIES OUTPUT_NAME "scheduler")
