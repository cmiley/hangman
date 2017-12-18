#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <fstream>
#include "graphics_headers.hpp"

class Object
{
  public:

    //constructor
    Object(std::string, btTriangleMesh*);
    
    //deconstructor
    ~Object();

    //updates the model matrix of the object
    void Update(unsigned int dt, glm::mat4, MenuVars*);

    //renders an object
    void Render();

    //reads object info from config file
    void configRead(std::string);

    //copies info from a object to another
    void copyPlanetValues(Object*);

    //loads a model, also creates the physics mesh
    void loadModel(btTriangleMesh*);

    //sets specularity of object
    void SetSpecular(glm::vec3 new_spec);

    //sets shininess of object
    void SetShininess(float new_shiny);

    //returns specularity
    glm::vec3 GetSpecular();

    //returns shininess
    float GetShininess();

    //returns the model
    glm::mat4 GetModel();

    //returns position of model
    glm::vec3 GetPosition();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint aTexture;
    GLuint IB;

    float angle;
    float p_rotationValue;
    float p_orbitValue;

    float m_rotationValue;
    float m_orbitValue;
    glm::vec3 specular_scaler;
    float shininess;
    glm::mat4 m_mvMatrix;
    glm::mat4 m_mvpMatrix;

    glm::vec3 objectPosition;

    std::string textureName;
    std::string modelName;

    Magick::Image image;
    Magick::Blob blob;
};

#endif /* OBJECT_H */
