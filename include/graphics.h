#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "physics.h"
#include "object.h"

class Graphics
{
  public:
    //constructor
    Graphics();
    
    //destructor
    ~Graphics();

    //Initializes all of the members of Graphics
    bool Initialize(int width, int height);

    //Updates the physics object, then the graphics
    void Update(unsigned int dt);

    //Renders all of the stuff that needs rendering
    void Render(ShaderSelector, MenuVars*);

    //returns m_camera
    Camera* getCamera();

    //returns m_physics
    Physics* getPhysics();

    //Sending uniforms into shaders
    bool SendUniforms(Shader*, MenuVars*);

    //Reads the object names from the config into a string vector
    void readNamesFromConfig();

    //function to convert a vec3 into a btVector3
    btVector3 glmToBullet(const glm::vec3& v);

    //Applies a force to the plunger object
    void applyForcePlunger( float force );

    //Looks up an index for an object using the name vector
    int lookupObjectIndex( string input );

    //Checks if a ball has been lost
    void ballDrained();

  private:

    //Handles error messages
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Physics *m_physics;

    Shader *m_passthrough;
    Shader *m_fragment;
    Shader *m_vertex;

    Object *genericObject;

    vector<Object*> objectVector;
    vector<string> objectNames;

    int paddleLindex, paddleRindex, ballIndex;
    float ballz;
    float plungerz;
    int balls;

    

};

#endif /* GRAPHICS_H */
