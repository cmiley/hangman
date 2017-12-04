#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include <vector>

#include "graphics_headers.h"

using namespace std;

enum collisiontypes {
    COL_NOTHING = 0,
    COL_BALL = 1,
    COL_EVERYTHING = 2,
};

class Physics
{
  public:
    //constructor
    Physics();

    //destructor
    ~Physics();

    //Sets up the physics world
    bool Initialize();

    //updates the physics by stepping the simulation
    glm::mat4 Update(unsigned int, int);

    //adds a ground plan at a specified height
    void addGroundPlane(int);

    //applies a force to right paddle, to use press D key
    void applyRightPaddleForce();

    //applies a force to left paddle, to use press A key
    void applyLeftPaddleForce();

    //adds an object to the physics world
    void addObject(btCollisionShape*, btDefaultMotionState*, int, std::string name);

    //affects the "angle" of the table, actually just changes a factor in gravity calculation
    void setTabelAngle(float newAngle );

    //applies force to a selected object
    void applyForce(btVector3 force, int index);

    //applies torque to a selected object
    void applyTorque(btVector3 torque, int index);

    //returns the position of an object in the physics world
    btVector3 getPos( int index );

    //sets the position of a object in the physics world
    void setPosition( btVector3 pos, int index );

    //rotates an object in the physics world
    void rotate( float angle, int index );

    //returns the y rotation
    float getYRotatation( int index );

    //set the Y rotation
    void  setYRotatation( float angle, int index);

    //clears the force of an object
    void clearForce( int index );

    //applies an impulse to an object
    void applyImpulse(int);

    //adds a hinge constraint for the paddles
    void addHingeConstraint(int);

    //adds a slider constraint to the plunger
    void addPlungerConstraint(int);

    //checks if a ball is out
    bool isBallOut();

    //used for detecting collisions between certain objects
    void myTickCallback();

  private:
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

    btTransform trans;

    btRigidBody* groundRigidBody;

    btRigidBody* ceilingPlane;
    btRigidBody* plungerWallPlane;

    std::vector<btHingeConstraint*> hingeVector;

    float gravAngle;
    float zGrav, yGrav;

    int ballCollidesWith = COL_EVERYTHING;
    int everythingCollidesWith = COL_NOTHING;

    std::vector<btRigidBody*> physicsObjectVector;

    unsigned int score;


};

#endif /* PHYSICS_H */