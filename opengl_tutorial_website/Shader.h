#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>



class Shader {

public:

	//the program ID
	unsigned int ID;

	//constructor reads and builds the shader

	Shader(const char* vertexPath, const char* fragmentPath) {

		// 1. retrieve the vertex / fragment source code from filepath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		//ensure ifstream objects can throw exceptions

		vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {

			//open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			std::stringstream vShaderstream, fShaderstream;
			//read file's buffer contents into streams
			vShaderstream << vShaderFile.rdbuf();
			fShaderstream << fShaderFile.rdbuf();

			//close file handlers
			vShaderFile.close();
			fShaderFile.close();

			//convert stream into string
			vertexCode = vShaderstream.str();
			fragmentCode = fShaderstream.str();
		} 
		catch (std::ifstream::failure e) {
			std::cout << "VITUIKS MENI::ERROR::SHADER::FILENOT READ " << std::endl;
			std::cout << e.what() << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//COMPILE AND LINK THE SHADERS

		unsigned int vertex, fragment;
		int success; //debug 
		char infoLog[512]; //Debug

		//VERTEX SHADER

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		//print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR:SHADER:VERTEX:COMPILATION FAIELD \n" << infoLog << std::endl;
		}

		//FRAGMENT SHADER

		fragment = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		//print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success); 
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR:SHADER:FRAGMENT:COMPILATION FAILED\n" << infoLog << std::endl;
		}

		
		//SHADER PROGRAM

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		//PRINT LINKING ERRORS IF ANY
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog); 
			std::cout << "ERROR::SHADER::PROGRA;M:LINKING FAILED \n" << infoLog << std::endl;
		}

		//delete shaders as they're now linked
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	//use, activate shader
	void use() { glUseProgram(ID); }

	//utility uniform functions
	//uniform SETTEr functions
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setMat4(const std::string &name, glm::mat4 &mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

};



#endif // !SHADER_H
