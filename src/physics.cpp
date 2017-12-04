#include "physics.hpp"

Physics::Physics()
{
  
  gravAngle = 0;
  zGrav = 9.798*sin( gravAngle );
  yGrav = -9.798*cos( gravAngle );
}

Physics::~Physics()
{
  dynamicsWorld->removeRigidBody(physicsObjectVector[0]);
  delete physicsObjectVector[0]->getMotionState();
  delete physicsObjectVector[0];

  dynamicsWorld->removeRigidBody(groundRigidBody);
  delete groundRigidBody->getMotionState();
  delete groundRigidBody;

  delete dynamicsWorld;
  delete solver;
  delete collisionConfiguration;
  delete dispatcher;
  delete broadphase;
}

//sets up the physics world
bool Physics::Initialize()
{
   broadphase = new btDbvtBroadphase();

   collisionConfiguration = new btDefaultCollisionConfiguration();
   dispatcher = new btCollisionDispatcher(collisionConfiguration);

   solver = new btSequentialImpulseConstraintSolver;

   dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

   dynamicsWorld->setGravity(btVector3(0, yGrav, zGrav));

   //dynamicsWorld->setInternalTickCallback(myTickCallback, static_cast<void *>(this));

   btTransform ceilingTransform;
   btTransform plungerWallTransform;

   ceilingTransform.setOrigin(btVector3(0, 2, 0));
   plungerWallTransform.setOrigin(btVector3(-48, 3, -8.5));

   btStaticPlaneShape* ceiling = new btStaticPlaneShape(btVector3(0.0, -1, 0.0), 0);
   btBoxShape* plungerWall = new btBoxShape(btVector3(2.5, 3, .0001));

   btMotionState* ceilingMotion = new btDefaultMotionState(ceilingTransform);
   btMotionState* plungerWallMotion = new btDefaultMotionState(plungerWallTransform);

   btRigidBody::btRigidBodyConstructionInfo ceilingInfo(0, ceilingMotion, ceiling);
   btRigidBody::btRigidBodyConstructionInfo plungerWallInfo(0, plungerWallMotion, plungerWall);

   ceilingInfo.m_restitution = 0.5f;
   plungerWallInfo.m_restitution = 0.5f;

   ceilingPlane = new btRigidBody(ceilingInfo);
   plungerWallPlane = new btRigidBody(plungerWallInfo);

   ceilingPlane->setActivationState(DISABLE_DEACTIVATION);

   dynamicsWorld->addRigidBody(ceilingPlane);

   score = 0;

   return true;
}

//steps the simulation for an object, returns the physic's model matrix to use for graphics
glm::mat4 Physics::Update(unsigned int dt, int index)
{
  btScalar m[16];
  dynamicsWorld->stepSimulation(dt, 1);
  myTickCallback();
  physicsObjectVector[index]->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);

  //std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;

  return glm::make_mat4(m);
}

//adds a ground plane at the specified height
void Physics::addGroundPlane(int height)
{
  btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 0, 0), 1);
  btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, height, 0)));
  btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
  groundRigidBody = new btRigidBody(groundRigidBodyCI);

  int temp = groundRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT;
  groundRigidBody->setCollisionFlags(temp);

  groundRigidBody->setActivationState(DISABLE_DEACTIVATION);

  dynamicsWorld->addRigidBody(groundRigidBody);
}

//adds an object to the physics world
void Physics::addObject(btCollisionShape* shape, btDefaultMotionState* motionState, int mass, std::string name )
{
  btVector3 fallInertia(0, 0, 0);
  shape->calculateLocalInertia(mass, fallInertia);
  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motionState, shape, fallInertia);
  btRigidBody* tempBody = new btRigidBody(fallRigidBodyCI);
  if( name == "plunger" )
  {
    tempBody->setLinearFactor(btVector3(1,1,1));
    tempBody->setAngularFactor(btVector3(0,0,0));
    tempBody->setRestitution( 5 );
  }

  if( name == "ball" )
  {
    tempBody->setLinearFactor(btVector3(1,0,1));

    //tempBody->setRestitution( 1 );
  }

  tempBody->setFriction(50.5f);

  if( name == "paddleL" )
  {
    //tempBody->setLinearFactor(btVector3(0,0,0));
    //tempBody->setAngularFactor(btVector3(0,1,0));
  }
  if( name == "paddleR" )
  {
  	
  }
  
  physicsObjectVector.push_back(tempBody);
  dynamicsWorld->addRigidBody(physicsObjectVector.back());
  
}
void Physics::applyForce(btVector3 force, int index)
{
  physicsObjectVector[ index ]->applyForce(force, btVector3(0.0, 0.0, 0.0));
}

void Physics::applyTorque(btVector3 torque, int index)
{
  physicsObjectVector[ index ]->applyTorque( torque );
}

btVector3 Physics::getPos( int index )
{
  /*
  btVector3 x;
  btTransform temp;
  physicsObjectVector[ index ]->getMotionState()->getWorldTransform(temp);
  x = temp.getOrigin();
  return x;
  */
}

void Physics::setPosition( btVector3 pos, int index )
{
  btTransform transform = physicsObjectVector[ index ]->getCenterOfMassTransform();
  transform.setOrigin(pos);
  physicsObjectVector[ index ]->setCenterOfMassTransform(transform);
  //Set the velocity yo zero
  physicsObjectVector[ index ]->setLinearVelocity( btVector3( 0, 0, 0 ));
  physicsObjectVector[ index ]->setAngularVelocity( btVector3( 0, 0, 0 ));
}

void Physics::rotate( float angle, int index )
{
  btTransform tempTran;
  btQuaternion tempQ;
  btMotionState *tempM;

  tempM = physicsObjectVector[ index ]->getMotionState();
  tempM->getWorldTransform( tempTran );
  tempQ = tempTran.getRotation();


  tempQ.setRotation( btVector3(0,1,0), angle);

  tempTran.setRotation( tempQ );

  tempM->setWorldTransform( tempTran );
  physicsObjectVector[ index ]->setMotionState( tempM );
}

void Physics::clearForce( int index )
{
  physicsObjectVector[ index ]->setAngularVelocity( btVector3(0,0,0));
}

//adds a hinge constrant to an object 
void Physics::addHingeConstraint(int selector)
{
	btVector3 axis(0, 1, 0);
	btVector3 pivot(0, 0, 0);

	btHingeConstraint* tempHinge = new btHingeConstraint(*physicsObjectVector[selector], pivot, axis);

	//if left paddle, else right paddle
	if (selector == 1)
	{
		//std::cout << "LEFT" << std::endl;
		tempHinge->setLimit(glm::radians(-15.), glm::radians(90.));
		tempHinge->enableAngularMotor(true, -1, 10);
	}
	else
	{
		//std::cout << "RIGHT" << std::endl;
		tempHinge->setLimit(glm::radians(135.), glm::radians(195.));
		tempHinge->enableAngularMotor(true, 1, 10);
	}

	dynamicsWorld->addConstraint(tempHinge);

	hingeVector.push_back(tempHinge);
}

//Function for calculating score
void Physics::myTickCallback()
{
  
  /*
  int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
  for (int i = 0; i < numManifolds; i++)
  {
    btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
    const btCollisionObject* obA = contactManifold->getBody0();
    const btCollisionObject* obB = contactManifold->getBody1();
    
    if(obA->getUserIndex() == -1 || obB->getUserIndex() == -1) continue;

    int numContacts = contactManifold->getNumContacts();
    for (int j = 0; j < numContacts; j++)
    {
      btManifoldPoint& pt = contactManifold->getContactPoint(j);
      if (pt.getDistance() < 0.10f)
      {
        const btVector3& ptA = pt.getPositionWorldOnA();
        const btVector3& ptB = pt.getPositionWorldOnB();
        const btVector3& normalOnB = pt.m_normalWorldOnB;

        //loop through and check for objects

        btRigidBody* tempObject1;
        btRigidBody* tempObject2;

        int tempA = 0;
        int tempB = 0;

        for (int index = 0; index < physicsObjectVector.size(); index++)
        {
        	if (physicsObjectVector[index]->getUserIndex() == obA->getUserIndex())
        	{
        		tempObject1 = physicsObjectVector[index];
        	}
        	if (physicsObjectVector[index]->getUserIndex() == obB->getUserIndex())
        	{
        		tempObject2 = physicsObjectVector[index];
        	}
        }


        cout << "OBJ1: " << tempObject1->getUserIndex() << " OBJ2: " << tempObject2->getUserIndex() << endl;

        switch (tempA + tempB)
        {
        	case 9:
        	case 10:
        	case 11: 
        		cout << "BALL AND BUMPER" << endl;
        		score += 25;
        		break;

        	case 12:
        	case 13:
        		cout << "BALL AND TRIANGLE" << endl;
        		score += 50;
        		break;
        }
        
      }
    }
  }
  */
}