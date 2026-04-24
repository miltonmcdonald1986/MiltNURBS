#include <graphics/factories/shader_factories.h>

#include <format>
#include <fstream>

namespace
{

	std::expected<GLuint, std::string> compile_shader(GLenum shader_type, const char* shader_source)
	{
		GLuint shader = glCreateShader(shader_type);
		glShaderSource(shader, 1, &shader_source, NULL);
		glCompileShader(shader);
		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::string shaderTypeStr = (shader_type == GL_VERTEX_SHADER)	? "VERTEX" : 
										(shader_type == GL_FRAGMENT_SHADER)	? "FRAGMENT" : 
										"UNKNOWN";
			return std::unexpected(std::format("ERROR::SHADER::{}::COMPILATION_FAILED\n{}\n", shaderTypeStr, infoLog));
		}

		return shader;
	}

	std::expected<GLuint, std::string> link_program(GLuint vertex_shader, GLuint fragment_shader)
	{
		GLuint shader_program = glCreateProgram();
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		glLinkProgram(shader_program);
		GLint success;
		GLchar infoLog[512];
		glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
			return std::unexpected(std::format("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}\n", infoLog));
		}
		return shader_program;
	}

	std::expected<GLuint, std::string> create_program_from_sources(const char* vertex_shader_source, const char* fragment_shader_source)
	{
		struct ShaderHandle
		{
			GLuint id{};

			~ShaderHandle()
			{
				if (id)
					glDeleteShader(id);
			}
		};

		auto vertex_shader_result = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
		if (!vertex_shader_result)
			return std::unexpected(vertex_shader_result.error());

		auto fragment_shader_result = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
		if (!fragment_shader_result)
			return std::unexpected(fragment_shader_result.error());

		ShaderHandle vertex_shader{ *vertex_shader_result };
		ShaderHandle fragment_shader{ *fragment_shader_result };

		auto shader_program_result = link_program(vertex_shader.id, fragment_shader.id);
		if (!shader_program_result)
			return std::unexpected(shader_program_result.error());

		return *shader_program_result;
	}

}

namespace graphics::factories::shader_factories
{

	std::expected<GLuint, std::string> create_basic_shader()
	{
		const char* vertex_shader_source = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";

		const char* fragment_shader_source = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\n\0";

		auto shader_program_result = create_program_from_sources(vertex_shader_source, fragment_shader_source);
		if (!shader_program_result)
			return std::unexpected(shader_program_result.error());

		return *shader_program_result;
	}

	std::expected<GLuint, std::string> create_color_shader()
	{
		const char* vertex_shader_source = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos, 1.0);\n"
			"}\0";

		const char* fragment_shader_source = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"uniform vec4 u_color;\n"
			"void main()\n"
			"{\n"
			"   FragColor = u_color;\n"
			"}\n\0";

		auto shader_program_result = create_program_from_sources(vertex_shader_source, fragment_shader_source);
		if (!shader_program_result)
			return std::unexpected(shader_program_result.error());

		return *shader_program_result;
	}

	std::expected<GLuint, std::string> create_shader_from_files(const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path)
	{
		auto load_text_file = [](const std::filesystem::path & path) -> std::expected<std::string, std::string>
		{
			std::ifstream file(path);
			if (!file.is_open())
				return std::unexpected(std::format("Failed to open file: {}", path.string()));

			std::stringstream buffer;
			buffer << file.rdbuf();
			return buffer.str();
		};

		// load vertex shader source
		auto vertex_src_result = load_text_file(vertex_path);
		if (!vertex_src_result)
			return std::unexpected(vertex_src_result.error());

		// load fragment shader source
		auto fragment_src_result = load_text_file(fragment_path);
		if (!fragment_src_result)
			return std::unexpected(fragment_src_result.error());

		// compile + link using existing helper
		return create_program_from_sources(vertex_src_result->c_str(), fragment_src_result->c_str());
	}

	std::expected<GLuint, std::string> create_textured_color_mvp_shader()
	{
		const char* vs = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec2 aUV;

        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProjection;

        out vec2 vUV;

        void main()
        {
            vUV = aUV;
            gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
        }
    )";

		const char* fs = R"(
        #version 330 core
        in vec2 vUV;

        uniform sampler2D uTexture;
        uniform vec4 u_color;

        out vec4 FragColor;

        void main()
        {
            vec4 texColor = texture(uTexture, vUV);
            FragColor = texColor * u_color;
        }
    )";

		return create_program_from_sources(vs, fs);
	}

	std::expected<GLuint, std::string> create_textured_mvp_shader()
	{
		const char* vertex_src = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aUV;

        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProjection;

        out vec2 vUV;

        void main()
        {
            gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
            vUV = aUV;
        }
    )";

		const char* fragment_src = R"(
        #version 330 core
        in vec2 vUV;
        out vec4 FragColor;

        uniform sampler2D uTexture;

        void main()
        {
            FragColor = texture(uTexture, vUV);
        }
    )";

		return create_program_from_sources(vertex_src, fragment_src);
	}

	std::expected<GLuint, std::string> create_textured_shader()
	{
		static const char* vertex_src = R"(
        #version 330 core

        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec2 aUV;

        out vec2 vUV;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            vUV = aUV;
        }
    )";

		static const char* fragment_src = R"(
        #version 330 core

        in vec2 vUV;
        out vec4 FragColor;

        uniform sampler2D uTexture;

        void main()
        {
            FragColor = texture(uTexture, vUV);
        }
    )";

		return create_program_from_sources(vertex_src, fragment_src);
	}


	std::expected<GLuint, std::string> create_vertex_color_shader()
	{
		const char* vertex_shader_source = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec3 aColor;\n"
			"out vec3 vertexColor;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos, 1.0);\n"
			"   vertexColor = aColor;\n"
			"}\0";

		const char* fragment_shader_source = "#version 330 core\n"
			"in vec3 vertexColor;\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(vertexColor, 1.0);\n"
			"}\0";

		return create_program_from_sources(vertex_shader_source, fragment_shader_source);
	}

}
