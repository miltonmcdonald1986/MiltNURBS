#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <miltnurbs/vector.h>
#include <miltnurbs/horner.h>
#include <iostream>
#include <vector>
#include <array>
#include <cmath>

// Simple vertex shader
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
uniform float aspectRatio;

void main() {
    gl_Position = vec4(aPos.x / aspectRatio, aPos.y, 0.0, 1.0);
}
)";

// Simple fragment shader
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 color;

void main() {
    FragColor = vec4(color, 1.0);
}
)";

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << "\n";
    }
    return shader;
}

GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed:\n" << infoLog << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

std::vector<float> generateCircleVertices(float centerX, float centerY, float radius, int segments = 64) {
    std::vector<float> vertices;
    vertices.reserve((segments + 2) * 2);

    vertices.push_back(centerX);
    vertices.push_back(centerY);

    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * 3.14159265359f * i / segments;
        float x = centerX + radius * std::cos(angle);
        float y = centerY + radius * std::sin(angle);
        vertices.push_back(x);
        vertices.push_back(y);
    }

    return vertices;
}

std::vector<float> generateArrowheadVertices(float startX, float startY, float endX, float endY, float arrowSize = 0.08f) {
    std::vector<float> vertices;
    vertices.reserve(6);

    float dirX = endX - startX;
    float dirY = endY - startY;
    float dirLen = std::sqrt(dirX * dirX + dirY * dirY);

    if (dirLen > 0.001f) {
        dirX /= dirLen;
        dirY /= dirLen;

        float perpX = -dirY;
        float perpY = dirX;

        // Tip
        vertices.push_back(endX);
        vertices.push_back(endY);

        // Back left
        vertices.push_back(endX - dirX * arrowSize - perpX * arrowSize * 0.5f);
        vertices.push_back(endY - dirY * arrowSize - perpY * arrowSize * 0.5f);

        // Back right
        vertices.push_back(endX - dirX * arrowSize + perpX * arrowSize * 0.5f);
        vertices.push_back(endY - dirY * arrowSize + perpY * arrowSize * 0.5f);
    }

    return vertices;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // GL 3.3 + GLSL 330
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Horner1 Demo - Animated Evaluation", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize gl3w (MUST be called after context is current)
    if (gl3wInit()) {
        std::cerr << "Failed to initialize gl3w\n";
		glfwDestroyWindow(window);
		glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLAD initialized successfully!\n";

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Create shader program
    GLuint shaderProgram = createShaderProgram();
    GLint colorLoc = glGetUniformLocation(shaderProgram, "color");
    GLint aspectRatioLoc = glGetUniformLocation(shaderProgram, "aspectRatio");

    // Coefficients for a cubic Bézier curve
    using Vec2 = miltnurbs::vector::Vector<double, 2>;
    std::array<Vec2, 4> coefficients{
        Vec2(-0.8, -0.5),
        Vec2(-0.3,  0.8),
        Vec2( 0.3, -0.8),
        Vec2( 0.8,  0.5)
    };

    // Create VAO and VBO for circle
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 66 * 2 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Create VAO and VBO for curve path
    GLuint pathVAO, pathVBO;
    glGenVertexArrays(1, &pathVAO);
    glGenBuffers(1, &pathVBO);
    glBindVertexArray(pathVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pathVBO);

    int pathSegments = 200;
    std::vector<float> pathVertices;
    pathVertices.reserve(pathSegments * 2);
    for (int i = 0; i < pathSegments; ++i) {
        double t = static_cast<double>(i) / (pathSegments - 1);
        Vec2 pathPoint = miltnurbs::horner::horner1(coefficients, t);
        pathVertices.push_back(static_cast<float>(pathPoint[0]));
        pathVertices.push_back(static_cast<float>(pathPoint[1]));
    }

    glBufferData(GL_ARRAY_BUFFER, pathVertices.size() * sizeof(float), pathVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Create VAO and VBO for coefficient point
    GLuint coeffVAO, coeffVBO;
    glGenVertexArrays(1, &coeffVAO);
    glGenBuffers(1, &coeffVBO);
    glBindVertexArray(coeffVAO);
    glBindBuffer(GL_ARRAY_BUFFER, coeffVBO);
    glBufferData(GL_ARRAY_BUFFER, 66 * 2 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Create VAO and VBO for C1 vector
    GLuint c1VAO, c1VBO;
    glGenVertexArrays(1, &c1VAO);
    glGenBuffers(1, &c1VBO);
    glBindVertexArray(c1VAO);
    glBindBuffer(GL_ARRAY_BUFFER, c1VBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Create VAO and VBO for C2 vector
    GLuint c2VAO, c2VBO;
    glGenVertexArrays(1, &c2VAO);
    glGenBuffers(1, &c2VBO);
    glBindVertexArray(c2VAO);
    glBindBuffer(GL_ARRAY_BUFFER, c2VBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Create VAO and VBO for C1 arrowhead
    GLuint c1ArrowVAO, c1ArrowVBO;
    glGenVertexArrays(1, &c1ArrowVAO);
    glGenBuffers(1, &c1ArrowVBO);
    glBindVertexArray(c1ArrowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, c1ArrowVBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Create VAO and VBO for C2 arrowhead
    GLuint c2ArrowVAO, c2ArrowVBO;
    glGenVertexArrays(1, &c2ArrowVAO);
    glGenBuffers(1, &c2ArrowVBO);
    glBindVertexArray(c2ArrowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, c2ArrowVBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Animation state
    double u = 0.0;
    double speed = 0.3;
    bool animate = true;
    float circleRadius = 0.1f;
    int circleSegments = 64;
    bool showPath = true;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Animate u
        if (animate) {
            u += speed * io.DeltaTime;
            if (u > 1.0) {
                u = 0.0;
            }
        }

        // Evaluate curve at current u using Horner's method
        Vec2 point = miltnurbs::horner::horner1(coefficients, u);

        // Generate circle vertices centered at the evaluated point
        std::vector<float> circleVertices = generateCircleVertices(
            static_cast<float>(point[0]),
            static_cast<float>(point[1]),
            circleRadius,
            circleSegments
        );

        // Update circle vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, circleVertices.size() * sizeof(float), circleVertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Generate coefficient circle vertices with same radius
        std::vector<float> coeffVertices = generateCircleVertices(
            static_cast<float>(coefficients[0][0]),
            static_cast<float>(coefficients[0][1]),
            circleRadius,
            circleSegments
        );

        // Update coefficient circle vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, coeffVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, coeffVertices.size() * sizeof(float), coeffVertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Generate C1 vector vertices (from C0 to C0 + C1)
        float c1Vertices[4] = {
            static_cast<float>(coefficients[0][0]),
            static_cast<float>(coefficients[0][1]),
            static_cast<float>(coefficients[0][0] + coefficients[1][0]),
            static_cast<float>(coefficients[0][1] + coefficients[1][1])
        };

        // Update C1 vector vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, c1VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(float), c1Vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Generate C2 vector vertices (from C0 to C0+C2)
        float c2Vertices[4] = {
            static_cast<float>(coefficients[0][0]),
            static_cast<float>(coefficients[0][1]),
            static_cast<float>(coefficients[0][0] + coefficients[2][0]),
            static_cast<float>(coefficients[0][1] + coefficients[2][1])
        };

        // Update C2 vector vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, c2VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(float), c2Vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Generate C1 arrowhead vertices
        std::vector<float> c1ArrowVertices = generateArrowheadVertices(
            static_cast<float>(coefficients[0][0]),
            static_cast<float>(coefficients[0][1]),
            static_cast<float>(coefficients[0][0] + coefficients[1][0]),
            static_cast<float>(coefficients[0][1] + coefficients[1][1]),
            0.08f
        );

        // Update C1 arrowhead vertex buffer
        if (!c1ArrowVertices.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, c1ArrowVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, c1ArrowVertices.size() * sizeof(float), c1ArrowVertices.data());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        // Generate C2 arrowhead vertices
        std::vector<float> c2ArrowVertices = generateArrowheadVertices(
            static_cast<float>(coefficients[0][0]),
            static_cast<float>(coefficients[0][1]),
            static_cast<float>(coefficients[0][0] + coefficients[2][0]),
            static_cast<float>(coefficients[0][1] + coefficients[2][1]),
            0.08f
        );

        // Update C2 arrowhead vertex buffer
        if (!c2ArrowVertices.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, c2ArrowVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, c2ArrowVertices.size() * sizeof(float), c2ArrowVertices.data());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        // ImGui UI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Horner1 Animation");
        ImGui::Text("Evaluating curve using Horner's method");
        ImGui::Separator();
        
        ImGui::Text("Coefficients:");
        for (size_t i = 0; i < coefficients.size(); ++i) {
            ImGui::Text("  a[%zu]: (%.2f, %.2f)", i, coefficients[i][0], coefficients[i][1]);
        }
        
        ImGui::Separator();
        double u0{ 0. };
        double u1{ 1. };
        ImGui::SliderScalar("u", ImGuiDataType_Double, &u, &u0, &u1);
        ImGui::Text("Current point: (%.3f, %.3f)", point[0], point[1]);
        ImGui::Separator();
        
		ImGui::Checkbox("Animate", &animate);
		double s0{ 0.1 };
		double s1{ 2.0 };
		ImGui::SliderScalar("Speed", ImGuiDataType_Double, &speed, &s0, &s1);
		ImGui::SliderFloat("Circle Radius", &circleRadius, 0.01f, 0.1f);
		ImGui::Checkbox("Show Path", &showPath);
		ImGui::Separator();
		ImGui::Text("%.1f FPS", io.Framerate);
		ImGui::End();

		// Add coefficient label to draw list
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		float aspectRatio = static_cast<float>(display_w) / static_cast<float>(display_h);

		auto drawList = ImGui::GetForegroundDrawList();
		float coeffScreenX = (static_cast<float>(coefficients[0][0]) / aspectRatio + 1.0f) * display_w / 2.0f;
		float coeffScreenY = (1.0f - static_cast<float>(coefficients[0][1])) * display_h / 2.0f;

		char coeffLabel[64];
		snprintf(coeffLabel, sizeof(coeffLabel), "C0 = (%.2f, %.2f)", coefficients[0][0], coefficients[0][1]);
		drawList->AddText(ImVec2(coeffScreenX + 10.0f, coeffScreenY - 10.0f), IM_COL32(100, 255, 100, 255), coeffLabel);

		// Draw C1 vector label
		float c1EndX = static_cast<float>(coefficients[0][0] + coefficients[1][0]);
		float c1EndY = static_cast<float>(coefficients[0][1] + coefficients[1][1]);
		float c1ScreenX = (c1EndX / aspectRatio + 1.0f) * display_w / 2.0f;
		float c1ScreenY = (1.0f - c1EndY) * display_h / 2.0f;

		char c1Label[64];
		snprintf(c1Label, sizeof(c1Label), "C1 = (%.2f, %.2f)", coefficients[1][0], coefficients[1][1]);
		drawList->AddText(ImVec2(c1ScreenX + 10.0f, c1ScreenY - 10.0f), IM_COL32(255, 255, 100, 255), c1Label);

		// Draw C2 vector label
		float c2EndX = static_cast<float>(coefficients[0][0] + coefficients[2][0]);
		float c2EndY = static_cast<float>(coefficients[0][1] + coefficients[2][1]);
		float c2ScreenX = (c2EndX / aspectRatio + 1.0f) * display_w / 2.0f;
		float c2ScreenY = (1.0f - c2EndY) * display_h / 2.0f;

		char c2Label[64];
		snprintf(c2Label, sizeof(c2Label), "C2 = (%.2f, %.2f)", coefficients[2][0], coefficients[2][1]);
		drawList->AddText(ImVec2(c2ScreenX + 10.0f, c2ScreenY - 10.0f), IM_COL32(100, 200, 255, 255), c2Label);

		// Rendering
		ImGui::Render();
		glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform1f(aspectRatioLoc, aspectRatio);

        // Draw the curve path
        if (showPath) {
            glUniform3f(colorLoc, 0.5f, 0.5f, 0.8f);
            glBindVertexArray(pathVAO);
            glDrawArrays(GL_LINE_STRIP, 0, 200);
            glBindVertexArray(0);
        }

        // Draw the coefficient point C0
        glUniform3f(colorLoc, 0.2f, 1.0f, 0.2f);
        glBindVertexArray(coeffVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, circleSegments + 2);
        glBindVertexArray(0);

        // Draw the C1 vector
        glUniform3f(colorLoc, 1.0f, 1.0f, 0.0f);
        glBindVertexArray(c1VAO);
        glLineWidth(2.0f);
        glDrawArrays(GL_LINE_STRIP, 0, 2);
        glLineWidth(1.0f);
        glBindVertexArray(0);

        // Draw the C2 vector
        glUniform3f(colorLoc, 0.4f, 0.78f, 1.0f);
        glBindVertexArray(c2VAO);
        glLineWidth(2.0f);
        glDrawArrays(GL_LINE_STRIP, 0, 2);
        glLineWidth(1.0f);
        glBindVertexArray(0);

        // Draw the C1 arrowhead
        glUniform3f(colorLoc, 1.0f, 1.0f, 0.0f);
        glBindVertexArray(c1ArrowVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // Draw the C2 arrowhead
        glUniform3f(colorLoc, 0.4f, 0.78f, 1.0f);
        glBindVertexArray(c2ArrowVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // Draw the animated circle
        glUniform3f(colorLoc, 1.0f, 0.5f, 0.2f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, circleSegments + 2);
        glBindVertexArray(0);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &pathVAO);
    glDeleteBuffers(1, &pathVBO);
    glDeleteVertexArrays(1, &coeffVAO);
    glDeleteBuffers(1, &coeffVBO);
    glDeleteVertexArrays(1, &c1VAO);
    glDeleteBuffers(1, &c1VBO);
    glDeleteVertexArrays(1, &c2VAO);
    glDeleteBuffers(1, &c2VBO);
    glDeleteVertexArrays(1, &c1ArrowVAO);
    glDeleteBuffers(1, &c1ArrowVBO);
    glDeleteVertexArrays(1, &c2ArrowVAO);
    glDeleteBuffers(1, &c2ArrowVBO);
    glDeleteProgram(shaderProgram);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


