
set(BASE_LOOP BaseLoop.h BaseLoop.cpp Circle.h EvaluationFunctions.h Rect.h)
set(LINKING utility
        bn_controller
        argos3core_simulator
        argos3plugin_simulator_dynamics2d
        argos3plugin_simulator_entities
        argos3plugin_simulator_footbot
        argos3plugin_simulator_genericrobot
        argos3plugin_simulator_media)

add_library(loop_functions SHARED
        ${BASE_LOOP}
        LoopTask1.h LoopTask1.cpp
        LoopTask2.h LoopTask2.cpp
        LoopTask3.h LoopTask3.cpp
        LoopTask4.h LoopTask4.cpp)

target_link_libraries(loop_functions ${LINKING})

