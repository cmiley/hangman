#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <fstream>
#include "graphics_headers.hpp"

class Shader
{
  public:
    //constructor
    Shader();

    //deconstructor
    ~Shader();

    //sets it up
    bool Initialize();

    //enables a ceratin shader
    void Enable();

    //adds a shader to thing
    bool AddShader(GLenum ShaderType, std::string);

    //loads a shader to thing
    std::string LoadShader(GLenum ShaderType, std::string);
    
    //compiles shader program
    bool Finalize();

    //for passing in uniforms to shaders
    GLint GetUniformLocation(const char* pUniformName);
    
    //Set up the uniforms
    bool BuildUniforms();
    
    //Check if location is valid
    bool CheckUniformLocation(GLint, std::string);

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_ambient;
    GLint m_specular;
    GLint m_diffuse;
    GLint m_shininess;
    GLint m_lightPosition;

    GL_Light m_light[NUM_LIGHTS];

  private:

    GLuint m_shaderProg;
    GLuint m_vertexLightProg; 
    GLuint m_fragmentLightProg;
    std::vector<GLuint> m_shaderObjList;
    std::string shader_prog_id;
};

#endif  /* SHADER_H */
