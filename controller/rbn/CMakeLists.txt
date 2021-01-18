

add_library(bn_controller SHARED
        Footbot.h
        Footbot.cpp
        BNController.h
        BNController.cpp
)

target_link_libraries(bn_controller
        utility
        core
        argos3core_simulator
        argos3plugin_simulator_footbot
        argos3plugin_simulator_genericrobot
)