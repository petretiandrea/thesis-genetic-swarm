
add_library(
        evolution_loop SHARED
        evolution_loop.h evolution_loop.cpp Circle.h
        EvaluationFunctions.h)

target_link_libraries(evolution_loop
        utility
        bn_controller
        argos3core_simulator
        argos3plugin_simulator_dynamics2d
        argos3plugin_simulator_entities
        argos3plugin_simulator_footbot
        argos3plugin_simulator_genericrobot
        argos3plugin_simulator_media)

