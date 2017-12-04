#include "graphics.hpp"

using namespace std;

Graphics::Graphics()
{
  balls = 2;
}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height)
{
  // Used for linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  //For physics
  m_physics = new Physics();
  if(!m_physics->Initialize())
  {
    printf("Physics Failed to Initialize\n");
    return false;
  }

  readNamesFromConfig();

  // Create the objects
  for (int index = 0; index < objectNames.size(); index++)
  {
  	int mass;
  	btTriangleMesh* objTriMesh = new btTriangleMesh();
  	genericObject = new Object(objectNames[index], objTriMesh);

  	btCollisionShape *genericShape;

    //cout << "INDEX: "  << index << " NAME: " << objectNames[index] << endl;

    //for satics 
  	if ((objectNames[index].find("table") != string::npos) || (objectNames[index].find("bumper") != string::npos) 
         || (objectNames[index].find("lane") != string::npos) || (objectNames[index].find("corner") != string::npos) 
         || (objectNames[index].find("gutter") != string::npos) || (objectNames[index].find("backboard") != string::npos)
         || (objectNames[index].find("Triangle") != string::npos))

  	{
  		genericShape = new btBvhTriangleMeshShape(objTriMesh, true);
  		mass = 0;
  	}
  	else
  	{
	  genericShape = new btConvexTriangleMeshShape(objTriMesh, true);
	  mass = 1;
      if( objectNames[index].find("ball") != string::npos )
      {
        mass = 10;
      }
  	}

  	btDefaultMotionState* genericMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(glmToBullet(genericObject->GetPosition()))));

    objectVector.push_back(genericObject);
    m_physics->addObject(genericShape, genericMotionState, mass, objectNames[index]);

  }

  cout << endl << endl << endl << "SCORE           ";  

  paddleLindex = lookupObjectIndex( "leftPaddle");
  paddleRindex = lookupObjectIndex( "rightPaddle");
  ballIndex = lookupObjectIndex( "ball" );

  /*
  m_physics->rotate( glm::radians(180.) , paddleRindex );
  m_physics->rotate( glm::radians(1.) , paddleLindex   );
  */
  m_physics->addGroundPlane(3);

  // Set up the shaders
  m_passthrough = new Shader();
  if(!m_passthrough->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }
  m_fragment = new Shader();
  if(!m_fragment->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }
  m_vertex = new Shader();
  if(!m_vertex->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add shaders for each program //////////////////////////////////////////////
  // For Passthrough Lighting Mode
  // Add the vertex shader
  if(!m_passthrough->AddShader(GL_VERTEX_SHADER, "normVShader.vert"))
  {
    printf("Passthrough vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_passthrough->AddShader(GL_FRAGMENT_SHADER, "normFShader.frag"))
  {
    printf("Passthrough fragment Shader failed to Initialize\n");
    return false;
  }

  // For Fragment Lighting Mode
  // Add the vertex shader
  if(!m_fragment->AddShader(GL_VERTEX_SHADER, "fLightingvShader.vert"))
  {
    printf("Per fragment vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_fragment->AddShader(GL_FRAGMENT_SHADER, "fLightingfShader.frag"))
  {
    printf("Per fragment fragment Shader failed to Initialize\n");
    return false;
  }

  // For Vertex Lighting Mode
  // Add the vertex shader
  if(!m_vertex->AddShader(GL_VERTEX_SHADER, "vLightingvShader.vert"))
  {
    printf("Per vertex vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_vertex->AddShader(GL_FRAGMENT_SHADER, "vLightingfShader.frag"))
  {
    printf("Per vertex fragment Shader failed to Initialize\n");
    return false;
  }

  ///////////////////////////////////////////////////////////////////////

  // Connect the programs
  if(!m_passthrough->Finalize())
  {
    std::cout << "Unable to finalize passthrough shader" << std::endl;
    return false;
  }
  if(!m_fragment->Finalize())
  {
    std::cout << "Unable to finalize fragment shader" << std::endl;
    return false;
  }
  if(!m_vertex->Finalize())
  {
    std::cout << "Unable to finalize vertex shader" << std::endl;
    return false;
  }

  m_passthrough->BuildUniforms();
  m_fragment->BuildUniforms();
  m_vertex->BuildUniforms();

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt)
{
  glm::mat4 temp;


  for (int index = 0; index < objectVector.size(); index++)
  {
    temp = m_physics->Update(dt, index);
    objectVector[index]->Update(dt, temp);
  }
  
  ballz = m_physics->getPos( ballIndex )[2];

  if( ballz > 9.0 )
  {
    ballDrained();
  }
  plungerz = m_physics->getPos( lookupObjectIndex("plunger"))[2];

  if( plungerz < -4.5 )
  {
    m_physics->clearForce( lookupObjectIndex("plunger"));
    m_physics->setPosition( btVector3(4.3,.1 , -4.5), lookupObjectIndex("plunger"));
  }
}

void Graphics::Render(ShaderSelector selector, MenuVars* menu)
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //shaderSwap(shaderSelector);

  // Start the correct program
  switch (selector)
  {
    case PASSTHROUGH:
      m_passthrough->Enable();
      SendUniforms(m_passthrough, menu);
      break;
    case PER_FRAGMENT:
      m_fragment->Enable();
      SendUniforms(m_fragment, menu);
      break;
    case PER_VERTEX:
      m_vertex->Enable();
      SendUniforms(m_vertex, menu);
      break;
  }
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

Camera* Graphics::getCamera()
{
  return m_camera;
}

Physics* Graphics::getPhysics()
{
  return m_physics;
}

bool Graphics::SendUniforms(Shader *m_shader, MenuVars* menu)
{
  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_shader->m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
  glUniformMatrix4fv(m_shader->m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

  // Send in lighting values
  glUniform4f(m_shader->m_ambient, menu->ambient.x, menu->ambient.y, menu->ambient.z, 0.0);
  glUniform4f(m_shader->m_diffuse, menu->diffuse.x, menu->diffuse.y, menu->diffuse.z, 0.0);
  glUniform4f(m_shader->m_lightPosition, menu->light_pos[0], menu->light_pos[1], menu->light_pos[2], 0.0);

/////////////////////////////////////////////////////////////////////////

  // Render the objects
//do in loop
  for (int index = 0; index < objectVector.size(); index++)
  {
    glUniformMatrix4fv(m_shader->m_modelMatrix, 1, GL_FALSE, glm::value_ptr(objectVector[index]->GetModel()));
    glUniform4f(m_shader->m_specular, objectVector[index]->GetSpecular().x, objectVector[index]->GetSpecular().y, objectVector[index]->GetSpecular().z, objectVector[index]->GetSpecular().w);
    glUniform1f(m_shader->m_shininess, objectVector[index]->GetShininess());
    objectVector[index]->Render();
  }
    // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

void Graphics::readNamesFromConfig()
{
	ifstream fin;

	fin.open("../config/config.txt");

	if (!fin.is_open())
	{
		cout << "Error opening config file." << endl;
	}
	else
	{
		string tempString;

		while (fin >> tempString)
		{
			if (tempString.find("objName") != string::npos)
			{
				fin >> tempString;
				//cout << tempString << endl;
				objectNames.push_back(tempString);
			}
		}
	}

	fin.close();
}

btVector3 Graphics::glmToBullet(const glm::vec3& v) 
{ 
	return btVector3(v.x, v.y, v.z); 
}

void Graphics::applyForcePlunger( float force )
{
  m_physics->applyForce( btVector3(0,0,force * -1.), lookupObjectIndex("plunger"));
}

int Graphics::lookupObjectIndex( string input )
{
  int index;

  for( index = 0; index < objectNames.size(); ++index )
  {
    if( objectNames[index].find( input ) != string::npos )
    {
      return index;
    }
    
  }

  return -1;
}

void Graphics::ballDrained()
{
  char ans;
  balls--;
  if( balls < 0 )
  {
    cout << "GAME OVER" << endl;
    cout << "Do you want to play again? Y/N" << endl;
    cin >> ans;
    if( ans == 'Y')
    {
      balls = 3;
      getPhysics()->setPosition( btVector3( 4.2,0,4), ballIndex );
      getPhysics()->setPosition( btVector3( 4.3,.1,6), lookupObjectIndex("plunger"));
    }
    else
    {
      exit(0);
    }
  }
  else
  { 
    cout << endl;
    if( balls == 0 )
    {
      cout << "LAST BALL" << endl << endl;
      cout << "SCORE           ";
    }
    else
    {
      cout << "You have " << balls << " balls left!" << endl << endl;
      cout << "SCORE           ";
    }
      

    getPhysics()->setPosition( btVector3( 4.2,0,4), ballIndex );
    getPhysics()->setPosition( btVector3( 4.3,.1,6), lookupObjectIndex("plunger"));
  }

}