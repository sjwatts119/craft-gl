#include <render/shader.h>

#include "material/light.h"

std::string Shader::readShaderFile(const char *path) {
    std::string contents;
    std::ifstream fileStream;

    // Ensure the file stream object can throw exceptions:
    fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // Open the file and init a string stream to hold its contents
        fileStream.open(path);
        std::stringstream shaderStream;

        // Read the file's buffer contents into the string stream
        shaderStream << fileStream.rdbuf();

        // Close the file handler
        fileStream.close();

        // Convert the string stream into a string
        contents = shaderStream.str();
    } catch (std::ifstream::failure &exception) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << exception.what() << std::endl;
    }

    if (contents.empty()) {
        std::cerr << "ERROR::SHADER::FILE_EMPTY: " << path << std::endl;
    }

    return contents;
}

unsigned int Shader::setupShader(const char *source, const unsigned int type) {
    // Create a shader object of the given type
    const auto id = glCreateShader(type);

    // Attach the shader source code to the shader object and compile it.
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    // Was the shader compiled successfully?
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    // If not, print the error log.
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        std::cerr << infoLog << std::endl;
    }

    return id;
}

unsigned int Shader::setupShaderProgram(const std::vector<unsigned int> &shaderIds) {
    // Create a shader program object
    const unsigned int programId = glCreateProgram();

    // Attach the shaders to the program
    for (const auto shaderId: shaderIds) {
        glAttachShader(programId, shaderId);
    }

    // Link the shader program
    glLinkProgram(programId);

    // Did the linking succeed?
    int success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);

    // If not, print the error log.
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programId, 512, nullptr, infoLog);
        std::cerr << infoLog << std::endl;
    }

    // We can delete the shader objects now as they are linked into the program and no longer needed.
    for (const auto shaderId: shaderIds) {
        glDeleteShader(shaderId);
    }

    return programId;
}

Shader::Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath) {
    const auto vertexCode = readShaderFile(vertexPath);
    const auto fragmentCode = readShaderFile(fragmentPath);
    const std::string geometryCode = geometryPath ? readShaderFile(geometryPath) : "";

    auto shaderIds = std::vector<unsigned int>();

    shaderIds.push_back(setupShader(vertexCode.c_str(), GL_VERTEX_SHADER));
    shaderIds.push_back(setupShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER));
    if (geometryPath) {
        shaderIds.push_back(setupShader(geometryCode.c_str(), GL_GEOMETRY_SHADER));
    }

    _id = setupShaderProgram(shaderIds);
}

void Shader::use() const {
    glUseProgram(_id);
}

void Shader::setBool(const std::string &name, const bool value) const {
    glUniform1i(glGetUniformLocation(_id, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, const int value) const {
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, const float value) const {
    glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vector) const
{
    glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, glm::value_ptr(vector));
}

void Shader::setMat4(const std::string &name, const glm::mat4 &matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setLight(const std::string &name, const Light &light) const {
    setVec3(name + ".position", light.position);
    setVec3(name + ".ambient", light.ambient);
    setVec3(name + ".diffuse", light.specular);
    setVec3(name + ".specular", light.specular);
}
