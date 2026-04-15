add_executable(horner1 main.cpp)

target_link_libraries(horner1 PRIVATE miltnurbs)

# Configure Emscripten output
set_target_properties(horner1 PROPERTIES
    SUFFIX ".js"
)

# Emscripten linker flags
target_link_options(horner1 PRIVATE
    "SHELL:-s WASM=1"
    "SHELL:-s ALLOW_MEMORY_GROWTH=1"
    "SHELL:-s EXPORTED_RUNTIME_METHODS=ccall,cwrap"
    "SHELL:-s EXPORTED_FUNCTIONS=_init,_evaluateCurve,_getLastX,_getLastY,_getCurveDegree,_getNumCoeffs,_getCoeffX,_getCoeffY,_getBoundsMinX,_getBoundsMaxX,_getBoundsMinY,_getBoundsMaxY,_nextCurve,_prevCurve,_toggleAnimation,_updateAnimation"
    "SHELL:-s SINGLE_FILE=1"
)

# Copy index.html to build directory after build
add_custom_command(TARGET horner1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy 
        ${CMAKE_CURRENT_SOURCE_DIR}/index.html
        $<TARGET_FILE_DIR:horner1>/index.html
    COMMENT "Copying index.html to build directory"
)

# Copy to dist folder for deployment
add_custom_command(TARGET horner1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/dist
    COMMAND ${CMAKE_COMMAND} -E copy 
        $<TARGET_FILE:horner1>
        ${CMAKE_SOURCE_DIR}/dist/horner1.js
    COMMAND ${CMAKE_COMMAND} -E copy 
        ${CMAKE_CURRENT_SOURCE_DIR}/index.html
        ${CMAKE_SOURCE_DIR}/dist/index.html
    COMMENT "Copying files to dist folder"
)
