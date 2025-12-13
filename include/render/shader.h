#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <optional>
#include <vector>

#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader {
private:
    /**
     * Read in a shader file from a path, and return a string of its contents.
     */
    static std::string readShaderFile(const char *path);

    /**
     * Compile and return the ID of a shader of the given type from the given source code.
     */
    static unsigned int setupShader(const char *source, unsigned int type);

    /**
     * Link provided shaders to a shader program.
     */
    static unsigned int setupShaderProgram(const std::vector<unsigned int> &shaderIds);

public:
    unsigned int _id;

    Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr);

    /**
     * Use the shader program
     */
    void use() const;

    /**
     * Set a boolean uniform in the shader.
     */
    void setBool(const std::string &name, bool value) const;

    /**
     * Set an integer uniform in the shader.
     */
    void setInt(const std::string &name, int value) const;

    /**
     * Set a float uniform in the shader.
     */
    void setFloat(const std::string &name, float value) const;

    /**
     * Set a vec3 uniform in the shader.
     */
    void setVec3(const std::string &name, const glm::vec3 &vector) const;

    /**
     * Set a mat4 uniform in the shader.
     */
    void setMat4(const std::string &name, const glm::mat4 &matrix) const;
};
