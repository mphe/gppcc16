set(CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES
    main.cpp
    BoxerController.cpp
    HealthComponent.cpp
    HealthEvents.cpp
    TargetComponent.cpp
    EnduranceComponent.cpp
    Gui.cpp
    BoxerAI.cpp
    GameEvents.cpp
    Fader.cpp
    Logic.cpp
)

set(EXT_LIBRARIES
    ${SFML_LIBRARIES}
    gamelib
)

source_group(${PROJECT_NAME} FILES ${PROJECT_SOURCES})

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/extlib/GameLib/include
    ${PROJECT_SOURCE_DIR}/extlib/GameLib/extlib/cppmath/include
    ${PROJECT_SOURCE_DIR}/extlib/GameLib/extlib/jsoncpp/include
    ${PROJECT_SOURCE_DIR}/extlib/GameLib/extlib/imgui
    ${PROJECT_SOURCE_DIR}/extlib/GameLib/extlib/imgui-sfml
    ${PROJECT_SOURCE_DIR}/extlib/GameLib/extlib/imguifilesystem
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} ${EXT_LIBRARIES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

add_custom_target(rungame
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} editor assets/map.json
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
