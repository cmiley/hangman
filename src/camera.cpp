#include "camera.hpp"

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  positionX = 0.0;
  positionZ = -16.0;

  view = glm::lookAt( glm::vec3(0.0, 2.0, 0.0), //Eye Position
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 1000.0f); //Distance to the far plane, 
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}


bool Camera::Update(float rotation, float zoom)
{
  glm::vec3 tempVec = glm::vec3(-8.0, 2.0, -8.0);

  //glm::vec3 tempVec = glm::vec3(7.0, 4., 7.0);

  float angle = rotation * (float) M_PI / 180.0f;
  
  //tempVec = glm::rotate(tempVec, angle, glm::vec3(0.0, 1.0, 0.0));
  tempVec *= zoom;

  view = glm::lookAt( tempVec, //Eye Position
                      glm::vec3(sin(angle), 4.0, cos(angle)), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  //sin(angle) cos(angle)
}