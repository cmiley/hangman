#include "shader.hpp"

Shader::Shader()
{
  m_vertexLightProg = 0;
  m_fragmentLightProg = 0;
  m_shaderProg = 0;
}

Shader::~Shader()
{
  for(std::vector<GLuint>::iterator it = m_shaderObjList.begin() ; it != m_shaderObjList.end() ; it++)
  {
    glDeleteShader(*it);
  }

  if (m_vertexLightProg != 0)
  {
    glDeleteProgram(m_vertexLightProg);
    m_vertexLightProg = 0;
  }

  if (m_fragmentLightProg != 0)
  {
    glDeleteProgram(m_fragmentLightProg);
    m_fragmentLightProg = 0;
  }
  if (m_shaderProg != 0);
  {
    glDeleteProgram(m_shaderProg);
    m_shaderProg = 0;
  }
}

bool Shader::Initialize()
{
  m_shaderProg = glCreateProgram();

  if(m_shaderProg == 0)
  {
    std::cerr << "Error creating shader program" << std::endl;
    return false;
  }
//
//  m_vertexLightProg = glCreateProgram();
//
//  if(m_vertexLightProg == 0)
//  {
//    std::cerr << "Error creating shader program\n";
//    return false;
//  }
//
//  m_fragmentLightProg = glCreateProgram();
//
//  if (m_fragmentLightProg == 0)
//  {
//    std::cerr << "Error creating shader program\n";
//    return false;
//  }

  return true;
}

// Use this method to add shaders to the program. When finished - call finalize()
bool Shader::AddShader(GLenum ShaderType, std::string shaderFile)
{
  std::string s;
  s = LoadShader(ShaderType, shaderFile);

  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj == 0) 
  {
    std::cerr << "Error creating shader type " << ShaderType << std::endl;
    return false;
  }

  // Save the shader object - will be deleted in the destructor
  m_shaderObjList.push_back(ShaderObj);

  const GLchar* p[1];
  p[0] = s.c_str();
  GLint Lengths[1] = { (GLint)s.size() };

  glShaderSource(ShaderObj, 1, p, Lengths);

  glCompileShader(ShaderObj);

  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) 
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    std::cerr << "Error compiling: " << InfoLog << std::endl;
    return false;
  }

  glAttachShader(m_shaderProg, ShaderObj);

//  if (shaderFile.find("vLighting") != std::string::npos)
//  {
//    glAttachShader(m_vertexLightProg, ShaderObj);
//  }
//  else
//  {
//    glAttachShader(m_fragmentLightProg, ShaderObj);
//  }

  return true;
}

//Function to load the shaders from a file. 
//Called in AddShader()
std::string Shader::LoadShader(GLenum ShaderType, std::string shaderFile)
{
  std::string s;
  std::string line = "";

  std::string tempString = "../shaders/";
  tempString.append(shaderFile);

  if(ShaderType == GL_VERTEX_SHADER)
  {
    std::ifstream myFile;
    myFile.open(tempString);

    if (myFile.is_open())
    {
      while(std::getline(myFile, line))
      {
        s.append(line);
        s.append("\n");
      }
    }
    else
    {
      printf("Unable to open Vertex Shader File\n");
      return 0;
    }

    myFile.close();
  }
  else if(ShaderType == GL_FRAGMENT_SHADER)
  {
    std::ifstream myFile;
    myFile.open(tempString);

    if (myFile.is_open())
    {
      while(std::getline(myFile, line))
      {
        s.append(line);
        s.append("\n");
      }
    }
    else
    {
      printf("Unable to open Fragment Shader File\n");
      return 0;
    }

    myFile.close();
  }

  return s;
}

// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::Finalize()
{
  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  glLinkProgram(m_shaderProg);

  glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
  if (Success == 0)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
    return false;
  }

  glValidateProgram(m_shaderProg);
  glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
    return false;
  }

//  glLinkProgram(m_vertexLightProg);
//
//  glGetProgramiv(m_vertexLightProg, GL_LINK_STATUS, &Success);
//  if (Success == 0)
//  {
//    glGetProgramInfoLog(m_vertexLightProg, sizeof(ErrorLog), NULL, ErrorLog);
//    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
//    return false;
//  }
//
//  glValidateProgram(m_vertexLightProg);
//  glGetProgramiv(m_vertexLightProg, GL_VALIDATE_STATUS, &Success);
//  if (!Success)
//  {
//    glGetProgramInfoLog(m_vertexLightProg, sizeof(ErrorLog), NULL, ErrorLog);
//    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
//    return false;
//  }
//
//  glLinkProgram(m_fragmentLightProg);
//
//  glGetProgramiv(m_fragmentLightProg, GL_LINK_STATUS, &Success);
//  if (Success == 0)
//  {
//    glGetProgramInfoLog(m_fragmentLightProg, sizeof(ErrorLog), NULL, ErrorLog);
//    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
//    return false;
//  }
//
//  glValidateProgram(m_fragmentLightProg);
//  glGetProgramiv(m_fragmentLightProg, GL_VALIDATE_STATUS, &Success);
//  if (!Success)
//  {
//    glGetProgramInfoLog(m_fragmentLightProg, sizeof(ErrorLog), NULL, ErrorLog);
//    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
//    return false;
//  }

  // Delete the intermediate shader objects that have been added to the program
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
  {
    glDeleteShader(*it);
  }

  m_shaderObjList.clear();

  return true;
}

void Shader::Enable()
{
  glUseProgram(m_shaderProg);
}


GLint Shader::GetUniformLocation(const char* pUniformName)
{
    GLuint Location;
    Location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (Location == INVALID_UNIFORM_LOCATION) {
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return Location;
}

bool Shader::BuildUniforms()
{
  bool success = true;

  m_projectionMatrix = GetUniformLocation("projectionMatrix");
  success &= CheckUniformLocation(m_projectionMatrix, "m_projectionMatrix");

  // Locate the view matrix in the shader
  m_viewMatrix = GetUniformLocation("viewMatrix");
  success &= CheckUniformLocation(m_viewMatrix, "m_viewMatrix");

  // Locate the model matrix in the shader
  m_modelMatrix = GetUniformLocation("modelMatrix");
  success &= CheckUniformLocation(m_modelMatrix, "m_modelMatrix");

  m_ambient = GetUniformLocation("AmbientProduct");
  success &= CheckUniformLocation(m_ambient, "m_ambient");

  m_specular = GetUniformLocation("SpecularProduct");
  success &= CheckUniformLocation(m_specular, "m_specular");

  m_diffuse = GetUniformLocation("DiffuseProduct");
  success &= CheckUniformLocation(m_diffuse, "m_diffuse");

  m_shininess = GetUniformLocation("Shininess");
  success &= CheckUniformLocation(m_projectionMatrix, "m_shininess");

  m_lightPosition = GetUniformLocation("LightPosition");
  success &= CheckUniformLocation(m_shininess, "m_lightPosition");

  m_light[0].m_type = GetUniformLocation("Light[0].Type");
  success &= CheckUniformLocation(m_light[0].m_type, "m_light[0].m_type");
  m_light[0].m_position = GetUniformLocation("Light[0].Position");
  success &= CheckUniformLocation(m_light[0].m_position, "m_light[0].m_position");
  m_light[0].m_attenuation = GetUniformLocation("Light[0].Attenuation");
  success &= CheckUniformLocation(m_light[0].m_attenuation, "m_light[0].m_attenuation");
  m_light[0].m_direction = GetUniformLocation("Light[0].Direction");
  success &= CheckUniformLocation(m_light[0].m_direction, "m_light[0].m_direction");
  m_light[0].m_color = GetUniformLocation("Light[0].Color");
  success &= CheckUniformLocation(m_light[0].m_color, "m_light[0].m_color");

  return success;
}

bool Shader::CheckUniformLocation(GLint uni_loc, std::string name)
{
  if (uni_loc == INVALID_UNIFORM_LOCATION)
  {
    std::cout << name << "not found" << std::endl;
    return false;
  }
  return true;
}
