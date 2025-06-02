#include "Shader.hpp"

Shader::Shader(std::string vertexPath, std::string fragmentPath)
{

    std::string vertexSource = utils::read_file(vertexPath);
    std::string fragmentSource = utils::read_file(fragmentPath);

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();

    GLuint VertexShader = this->createShader(vertexSource, GL_VERTEX_SHADER);
    GLuint FragmentShader = this->createShader(fragmentSource, GL_FRAGMENT_SHADER);

    glAttachShader(shaderProgram, VertexShader);
    glAttachShader(shaderProgram, FragmentShader);

    glLinkProgram(shaderProgram);

    GLint success;
    GLchar infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR! Shader program linking failed\n"
                    << infoLog << std::endl;
        exit(-1);
    }
    else
    {
        std::cout << "Shader program created" << std::endl;
        this->isCompiled = true;
        this->ID = shaderProgram;
    }

    glDeleteShader(VertexShader);
    glDeleteShader(VertexShader);
};

Shader::~Shader()
{
    glDeleteProgram(ID);
}

bool Shader::use()
{
    glUseProgram(ID);
}

bool Shader::is_compiled()
{
    return this->isCompiled;
}

GLuint Shader::getID()
{
    return this->ID;
}

GLuint Shader::createShader(const std::string &source, const GLenum shaderType)
{

    GLuint pShader;

    pShader = glCreateShader(shaderType);

    if (source.empty())
    {
        std::cerr << "ERROR! Shader source is empty" << std::endl;
        exit(-1);
    }

    const char *code = source.c_str();

    glShaderSource(pShader, 1, &code, NULL);

    glCompileShader(pShader);

    GLint success;
    GLchar infoLog[512];

    glGetShaderiv(pShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(pShader, 512, NULL, infoLog);
        std::cerr << "ERROR! Vertex shader compilation failed\n"
                    << infoLog << std::endl;
        std::cerr << "Vertex shader source: \n"
                    << source << std::endl;
        exit(-1);
    }

    return pShader;
}

bool Shader::set_matrix4(std::string uniformName, glm::mat4 matrix)
{
    GLuint uniformLoc = glGetUniformLocation(this->ID, uniformName.c_str());
    std::cout << "mat loc " << uniformLoc << std::endl;
    if (uniformLoc == -1)
    {
        std::cerr << "WARNING: uniform named '" << uniformName << "' doesn't exist" << std::endl;
        return false;
    }
    else
    {
        glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
        return true;
    }
}

bool Shader::set_texture(std::string uniformName, GLuint textureID)
{
    GLuint uniformLoc = glGetUniformLocation(this->ID, uniformName.c_str());
    std::cout << "tex loc " << uniformLoc << std::endl;
    if (uniformLoc == -1)
    {
        std::cerr << "WARNING: uniform named '" << uniformName << "' doesn't exist" << std::endl;
        return false;
    }
    else
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(uniformLoc, 0);
        return true;
    }
}

