file(
    TO_CMAKE_PATH
    ${PROJECT_BINARY_DIR}/data/airglow.rc
    AIRGLOW_RC
)

file(
    TO_CMAKE_PATH
    ${PROJECT_BINARY_DIR}/data/icon.ico
    AIRGLOW_ICON
)

file(
    GENERATE
    OUTPUT ${AIRGLOW_RC}
    CONTENT "101 ICON \"icon.ico\""
)

set(GLOW_ICON_SOURCE
    ${PROJECT_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico
)

add_custom_command(
    OUTPUT ${AIRGLOW_ICON}
    COMMAND ${CMAKE_COMMAND} -E copy ${GLOW_ICON_SOURCE} ${AIRGLOW_ICON}
    COMMENT "Copying icon..."
)

add_custom_target(
    CopyIcon
    ALL
    DEPENDS ${AIRGLOW_ICON}
)

add_custom_target(
    Vite
    COMMAND pnpm build
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
