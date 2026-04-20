add_executable(horner1
	main.cpp
)

target_link_libraries(horner1 PRIVATE
	miltnurbs
	imgui
	glfw
	gl3w
	OpenGL::GL
)
