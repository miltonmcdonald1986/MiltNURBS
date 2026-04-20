#include <expected>
#include <print>
#include <string>

#include <entt/entt.hpp>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

struct App
{
    struct {
        struct {
            struct {
                int major{ 3 };
                int minor{ 3 };
            } version;

            int profile{ GLFW_OPENGL_CORE_PROFILE };
        } gl;

        struct {
            const char* title{ "horner1 Demo" };
            GLFWmonitor* pMonitor{ nullptr };
            GLFWwindow* pShare{ nullptr };
        } win;
    } config;

    struct {
        struct {
            GLFWwindow* pHandle{ nullptr };
            int height{ 600 };
            int width{ 800 };
        } win;

        entt::registry reg;
    } state;
};

void HandleGLFWError(int error_code, const char* description)
{
	std::print("GLFW Error (code {}): {}\n", error_code, description);
}

// This function will be called whenever the window is resized, allowing us to adjust the OpenGL viewport accordingly.
void HandleGLFWFramebufferSizeEvent(GLFWwindow* pWindow, int w, int h)
{
    if (App* app = static_cast<App*>(glfwGetWindowUserPointer(pWindow)))
    {
        app->state.win.width = w;
        app->state.win.height = h;
    }

    glViewport(0, 0, w, h);
}

void Shutdown(App& app)
{
    glfwDestroyWindow(app.state.win.pHandle);
    app.state.win.pHandle = nullptr;
    glfwTerminate();
}

// Initializes GLFW, creates a window, and sets up the OpenGL context. Returns an error message if any step fails.
std::expected<void, std::string> InitPlatform(App& app)
{
    glfwSetErrorCallback(HandleGLFWError);

    if (glfwInit() != GLFW_TRUE)
        return std::unexpected("Failed to initialize GLFW");

    // Set the OpenGL version and profile we want to use.
	auto& gl = app.config.gl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl.version.major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl.version.minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, gl.profile);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a window.
	auto& winState = app.state.win;
	auto& winConfig = app.config.win;
    if (GLFWwindow* window = glfwCreateWindow(winState.width, winState.height, winConfig.title, winConfig.pMonitor, winConfig.pShare))
        winState.pHandle = window;
    else
    {
        Shutdown(app);
		return std::unexpected("Failed to create GLFW window");
    }

    glfwSetWindowUserPointer(winState.pHandle, &app);
    glfwSetFramebufferSizeCallback(winState.pHandle, HandleGLFWFramebufferSizeEvent);
    
	return {};
}

std::expected<void, std::string> InitGLContext(App& app)
{
	// Make the OpenGL context of our window current on the calling thread.
    glfwMakeContextCurrent(app.state.win.pHandle);
    
	// Enable V-Sync
    glfwSwapInterval(1);

    // Initialize OpenGL function pointers using gl3w (or any other OpenGL loader).
    if (gl3wInit() != 0) 
        return std::unexpected("Failed to initialize gl3w");

    return {};
}

std::expected<void, std::string> InitGLState(App &app)
{
    // Set the initial OpenGL viewport to match the window size.
    int w, h;
    glfwGetFramebufferSize(app.state.win.pHandle, &w, &h);
    glViewport(0, 0, w, h);

	// Enable depth testing to ensure correct rendering of 3D objects based on their distance from the camera.
    glEnable(GL_DEPTH_TEST); 
    
	// Set the depth function to GL_LESS, which means that a fragment will be drawn if it is closer to the camera than the existing fragment at that pixel.
    glDepthFunc(GL_LESS);
    
    // Set the clear color to a dark gray.
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Check for any OpenGL errors that may have occurred during initialization.
    if (glGetError() != GL_NO_ERROR)
        return std::unexpected("Failed to init GL state");

    return {};
}

struct Shader
{
	GLuint id{ 0 };
};

struct MeshGL {
    GLuint vao{ 0 };
    GLuint vbo{ 0 };
    GLsizei vertexCount{ 0 };
};

std::expected<void, std::string> Update(App& /*app*/)
{
    return {};
}

std::expected<void, std::string> Render(App& app)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto view = app.state.reg.view<Shader, MeshGL>();
    for (auto [entity, shader, mesh] : view.each())
    {
        glUseProgram(shader.id);
        glBindVertexArray(mesh.vao); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
    }

    return {};
}

std::expected<GLuint, std::string> CreateBasicShader()
{
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        return std::unexpected(std::format("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}\n", infoLog));
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        return std::unexpected(std::format("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}\n", infoLog));
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        return std::unexpected(std::format("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}\n", infoLog));
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

std::expected<MeshGL, std::string>
CreateMeshGL(const float* vertices, std::size_t floatCount,
    GLint componentsPerVertex = 3)
{
    MeshGL mesh{};

    // Compute vertex count
    if (floatCount % componentsPerVertex != 0)
        return std::unexpected("Vertex data size does not match attribute layout");

    mesh.vertexCount = static_cast<GLsizei>(floatCount / componentsPerVertex);

    // --- VAO ---
    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    // --- VBO ---
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER,
        floatCount * sizeof(float),
        vertices,
        GL_STATIC_DRAW);

    // --- Vertex Attribute ---
    glVertexAttribPointer(
        0,                          // location
        componentsPerVertex,        // vec2, vec3, vec4
        GL_FLOAT,
        GL_FALSE,
        componentsPerVertex * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    if (glGetError() != GL_NO_ERROR)
        return std::unexpected("Failed to create MeshGL");

    return mesh;
}

int main(void)
{
	// Create an instance of our application state (if we had any).
	App app{};
	
    if (auto result = InitPlatform(app); !result)
    {
        std::print("Failed to initialize GLFW: {}\n", result.error());
        return -1;
    }

	if (auto result = InitGLContext(app); !result)
    {
        std::print("Failed to initialize OpenGL context: {}\n", result.error());
        return -1;
    }

    if (auto result = InitGLState(app); !result)
    {
        std::print("Failed to initialize OpenGL state: {}\n", result.error());
        return -1;
    }

	// Let's create a triangle for demo purposes.
	entt::entity ent_triangle = app.state.reg.create();
	app.state.reg.emplace<Shader>(ent_triangle, CreateBasicShader().value_or(0U));
	constexpr float triangleVerts[] = { 0.0f,  0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f };
	app.state.reg.emplace<MeshGL>(ent_triangle, CreateMeshGL(triangleVerts, 9U).value_or(MeshGL{}));

    entt::entity ent_triangle_upside_down = app.state.reg.create();
	app.state.reg.emplace<Shader>(ent_triangle_upside_down, CreateBasicShader().value_or(0U));
    constexpr float triangleVerts2[] = { 0.0f, -0.5f, 0.0f, -0.5f,  0.5f, 0.0f, 0.5f,  0.5f, 0.0f };
    app.state.reg.emplace<MeshGL>(ent_triangle_upside_down, CreateMeshGL(triangleVerts2, 9U).value_or(MeshGL{}));

	auto* pWin = app.state.win.pHandle;
    while (!glfwWindowShouldClose(pWin))
    {
		// Poll for and process events (e.g., keyboard input, mouse movement, window resizing).
        glfwPollEvents();

		if (auto result = Update(app); !result)
			std::print("Failed to update frame: {}\n", result.error());

        if (auto result = Render(app); !result)
			std::print("Failed to render frame: {}\n", result.error());

		// Swap front and back buffers (display the rendered image).
        glfwSwapBuffers(pWin);
    }

    Shutdown(app);
    return 0;
}
