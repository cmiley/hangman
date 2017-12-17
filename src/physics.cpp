#include "physics.hpp"

Physics::Physics()
{
  grav = .1;
  gravAngle = 0;
  zGrav = grav*sin( gravAngle );
  yGrav = -grav*cos( gravAngle );
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


   return true;
}

//steps the simulation for an object, returns the physic's model matrix to use for graphics
glm::mat4 Physics::Update(unsigned int dt, int index)
{
  btScalar m[16];
  dynamicsWorld->stepSimulation(dt, 1);
  physicsObjectVector[index]->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);

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
btRigidBody* Physics::addObject(btCollisionShape* shape, btDefaultMotionState* motionState, int mass, std::string name )
{
  btVector3 fallInertia(0, 0, 0);
  shape->calculateLocalInertia(mass, fallInertia);
  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motionState, shape, fallInertia);
  btRigidBody* tempBody = new btRigidBody(fallRigidBodyCI);
  

  physicsObjectVector.push_back(tempBody);

  if (name == "rope" || name == "head" ||  name == "rArm" || name == "lArm" || name == "rLeg" || name == "lLeg")
  {
  	dynamicsWorld->addRigidBody(physicsObjectVector.back(), (1<<(0)), 0);
  }
  else
  {
  	dynamicsWorld->addRigidBody(physicsObjectVector.back());
  }

  tempBody->setFriction(50.);

  return tempBody;
}

void Physics::applyForce(btVector3 force, int index)
{
  physicsObjectVector[ index ]->applyForce(force, btVector3(0.0, 0.0, 0.0));
}

void Physics::applyTorque(btVector3 torque, int index)
{
  physicsObjectVector[ index ]->applyTorque( torque );
}


void Physics::setPosition( btVector3 pos, int index )
{
  btTransform transform = physicsObjectVector[ index ]->getCenterOfMassTransform();
  transform.setOrigin(pos);
  physicsObjectVector[ index ]->setCenterOfMassTransform(transform);
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

}

void Physics::createRope(btCollisionShape* colShape, int totalRope)
{
	btTransform startTransform;
	startTransform.setIdentity();
	btScalar  mass(1.f);

	int lastBoxIndex = totalRope - 1;

	std::string tempString = "rope";

	for(int i = 0; i < totalRope; i++) 
	{ 
		startTransform.setOrigin(btVector3(btScalar(0),btScalar(i+7),btScalar(0)));
		btDefaultMotionState* genericMotionState = new btDefaultMotionState(startTransform);
		rope.push_back(addObject(colShape, genericMotionState, ((i == lastBoxIndex) ? 0:mass), tempString));    
	}

	for(int i = 0; i < totalRope - 1; i++) 
	{
		btRigidBody* r1 = rope[i];
		btRigidBody* r2 = rope[i + 1];

		btPoint2PointConstraint* leftSpring = new btPoint2PointConstraint(*r1, *r2, btVector3(-0.1,0.5,0), btVector3(-0.1,-0.5,0));
		dynamicsWorld->addConstraint(leftSpring);

		btPoint2PointConstraint* rightSpring = new btPoint2PointConstraint(*r1, *r2, btVector3(0.1,0.5,0), btVector3(0.1,-0.5,0));
		dynamicsWorld->addConstraint(rightSpring);
	}
}

float Physics::getHeight(int index)
{
	btTransform trans;
	physicsObjectVector[index]->getMotionState()->getWorldTransform(trans);

	return trans.getOrigin().getY();
}

void Physics::attachLimbs(int indexOfHead)
{
	btRigidBody* r1 = rope[0];

	btPoint2PointConstraint* headToRopeR = new btPoint2PointConstraint(*r1, *physicsObjectVector[indexOfHead], btVector3(-0.1, -0.5, 0), btVector3(-0.1, 0.2, 0));
	dynamicsWorld->addConstraint(headToRopeR);

	btPoint2PointConstraint* headToRopeL = new btPoint2PointConstraint(*r1, *physicsObjectVector[indexOfHead], btVector3(0.1, -0.5, 0), btVector3(0.1, 0.2, 0));
	dynamicsWorld->addConstraint(headToRopeL);

	btPoint2PointConstraint* bodyToHeadR = new btPoint2PointConstraint(*physicsObjectVector[indexOfHead], *physicsObjectVector[indexOfHead + 1], btVector3(-0.1, -0.1, 0), btVector3(-0.1, 0.6, 0));
	dynamicsWorld->addConstraint(bodyToHeadR);

	btPoint2PointConstraint* bodyToHeadL = new btPoint2PointConstraint(*physicsObjectVector[indexOfHead], *physicsObjectVector[indexOfHead + 1], btVector3(0.1, -0.1, 0), btVector3(0.1, 0.6, 0));
	dynamicsWorld->addConstraint(bodyToHeadL);

	btPoint2PointConstraint* rightArmToBodyR = new btPoint2PointConstraint(*physicsObjectVector[indexOfHead + 1], *physicsObjectVector[indexOfHead + 2], btVector3(-0.23, 0.3, 0.1), btVector3(0.0, 0.0, 0.1));
	dynamicsWorld->addConstraint(rightArmToBodyR);

	btPoint2PointConstraint* rightArmToBodyL = new btPoint2PointConstraint(*physicsObjectVector[indexOfHead + 1], *physicsObjectVector[indexOfHead + 2], btVector3(-0.23, 0.3, -0.1), btVector3(0.0, 0.0, -0.1));
	dynamicsWorld->addConstraint(rightArmToBodyL);

	btPoint2PointConstraint* leftArmToBodyR = new btPoint2PointConstraint(*physicsObjectVector[indexOfHead + 1], *physicsObjectVector[indexOfHead + 3], btVector3(0.23, 0.3, 0.1), btVector3(0.0, 0.0, 0.1));
	dynamicsWorld->addConstraint(leftArmToBodyR);

	btPoint2PointConstraint* leftArmToBodyL = new btPoint2PointConstraint(*physicsObjectVector[indexOfHead + 1], *physicsObjectVector[indexOfHead + 3], btVector3(0.23, 0.3, -0.1), btVector3(0.0, 0.0, -0.1));
	dynamicsWorld->addConstraint(leftArmToBodyL);

	btPoint2PointConstraint* rightLegToBodyR = new btPoint2PointConstraint(*physicsObjectVector[indexOfHead + 1], *physicsObjectVector[indexOfHead + 4], btVector3(-0.1, -0.3, 0.1), btVector3(0.0, 0.0, 0.1));
	dynamicsWorld->addConstraint(rightLegToBodyR);

	btPoint2PointConstraint* rightLegToBodyL = new btPoint2PointConstraint(*physicsObjectVector[indexOfHead + 1], *physicsObjectVector[indexOfHead + 4], btVector3(-0.1, -0.3, -0.1), btVector3(0.0, 0.0, -0.1));
	dynamicsWorld->addConstraint(rightLegToBodyL);

	btPoint2PointConstraint* leftLegToBodyR = new btPoint2PointConstraint(*physicsObjectVector[indexOfHead + 1], *physicsObjectVector[indexOfHead + 5], btVector3(0.1, -0.3, 0.1), btVector3(0.0, 0.0, 0.1));
	dynamicsWorld->addConstraint(leftLegToBodyR);

	btPoint2PointConstraint* leftLegToBodyL = new btPoint2PointConstraint(*physicsObjectVector[indexOfHead + 1], *physicsObjectVector[indexOfHead + 5], btVector3(0.1, -0.3, -0.1), btVector3(0.0, 0.0, -0.1));
	dynamicsWorld->addConstraint(leftLegToBodyL);
}