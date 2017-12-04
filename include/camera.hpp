#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.hpp"

class Camera
{
  public:
    //constructor
    Camera();
    //destructor
    ~Camera();
    //initializes the camera with width and hight
    bool Initialize(int w, int h);
    //Gets the projection matrix
    glm::mat4 GetProjection();
    //Gets the view matrix
    glm::mat4 GetView();
    //updates the zoom and rotation of the camera
  	bool Update(float, float);

  private:
    glm::mat4 projection;
    glm::mat4 view;

    float positionX;
    float positionZ;
};

#endif /* CAMERA_H */
