add_executable(GravitySimulation WIN32
    Main.cpp
    GravitySimulationApp.h
    GravitySimulationApp.cpp
    "Components/SimulationComponent.h"
    "Components/SimulationComponent.cpp" 
    "PhysicsCore.h" "PhysicsCore.cpp" 
    "PhysicsHelpers.h")

target_include_directories(GravitySimulation PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Linking the engine pulls in SDL3, SDL3_ttf, glm, ImGui transitively.
target_link_libraries(GravitySimulation PRIVATE gameEngine_lib)

ge_apply_warnings(GravitySimulation)

#----------------------------
# Platform-specific post-build steps.
#----------------------------
if(WIN32)
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GravitySimulation)
    set_target_properties(GravitySimulation PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:GravitySimulation>")

    set(SDL_RUNTIME_DLLS
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
    )
    foreach(dllTarget IN LISTS SDL_RUNTIME_DLLS)
        add_custom_command(
            TARGET GravitySimulation POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:${dllTarget}>"
                "$<TARGET_FILE_DIR:GravitySimulation>"
            VERBATIM
        )
    endforeach()

    add_custom_command(TARGET GravitySimulation POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${CMAKE_SOURCE_DIR}/resources"
            "$<TARGET_FILE_DIR:GravitySimulation>/resources"
    )
endif()
