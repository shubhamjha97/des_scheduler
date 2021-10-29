set (CMAKE_CXX_STANDARD 11)
project(scheduler)
add_executable(main main.cpp helper.h Process.cpp RNG.cpp Process.cpp FcfsScheduler.cpp Event.cpp SrtfScheduler.cpp SrtfScheduler.h RrScheduler.cpp RrScheduler.h PrioScheduler.cpp PrioScheduler.h PrePrioScheduler.cpp PrePrioScheduler.h LcfsScheduler.cpp LcfsScheduler.h)
set_target_properties(main PROPERTIES OUTPUT_NAME "scheduler")
