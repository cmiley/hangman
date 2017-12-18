#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include <vector>

#include "graphics_headers.hpp"

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

    //adds an object to the physics world
    btRigidBody* addObject(btCollisionShape*, btDefaultMotionState*, int, std::string name);

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

    //clears the force of an object
    void clearForce( int index );

    //adds a hinge constraint for the paddles
    void addHingeConstraint(int);

    //used for detecting collisions between certain objects
    void myTickCallback();

    //used for setting up the rope
    void createRope(btCollisionShape*, int);

    //returns height of object using an index
    float getHeight(int);

    //attaches the limbs together and attaches the head to the rope
    void attachLimbs(int);

    //change gravity
    void changeGravity(float);

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

    btAlignedObjectArray<btRigidBody*> rope;

    float gravAngle;
    float zGrav, yGrav;

    int ballCollidesWith = COL_EVERYTHING;
    int everythingCollidesWith = COL_NOTHING;

    std::vector<btRigidBody*> physicsObjectVector;

    unsigned int score;
    float grav;


};

#endif /* PHYSICS_H */