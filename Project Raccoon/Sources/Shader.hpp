#pragma once

#include "Common.hpp"


//Había un void bind() const {} que hay que reexplorar



struct Shader {

	std::string filePath;
	unsigned int ID;

	struct ShaderProgramSource {
		std::string VertexSource;
		std::string FragmentSource;
	};

	Shader(const std::string& filepath)
		:filePath(filepath), ID(0) { //we are maintaining filePath only for debugging purpouses
		ShaderProgramSource source = parseShader(filepath);
		ID = createShader(source.VertexSource, source.FragmentSource);

		
	}

	//Converts a .shader into 2 separate streams and each one into strings for glShaderSource in compileShader
	ShaderProgramSource parseShader(const string& filepath);


	unsigned int compileShader(unsigned int type, const string& source);

	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	


	// Checks if the different Shaders have compiled properly
	void compileErrors(unsigned int shader, const char* type);

	~Shader() {
		glDeleteProgram(ID);
	}
	void bind();

	void unbind();
};



//Initializes glfw and glew
GLFWwindow* initialize();




//#define CHECK_GL_ERROR() {\
//    GLenum err = glGetError();\
//    while (err != GL_NO_ERROR) {\
//        std::cout << "OpenGL error: " << std::hex << err << std::dec << " line:" << __LINE__ << std::endl;\
//        err = glGetError();\
//    }\
//}
//std::set<std::string> printedErrors;
//
//void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) { //Only adds errors once
//	std::string errorMsg(message);
//
//	if (printedErrors.find(errorMsg) == printedErrors.end()) {
//		std::cerr << message << std::endl;
//
//		printedErrors.insert(errorMsg);
//	}
//}