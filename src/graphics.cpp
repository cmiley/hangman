
#include "graphics.hpp"

using namespace std;

Graphics::Graphics()
{

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

    genericShape = new btConvexTriangleMeshShape(objTriMesh, true);



    //if not the rope
    if ((objectNames[index].find("rope") == string::npos))
    {
      int mass;
      btTriangleMesh* objTriMesh = new btTriangleMesh();
      genericObject = new Object(objectNames[index], objTriMesh);

      btCollisionShape *genericShape;

      genericShape = new btConvexTriangleMeshShape(objTriMesh, true);

    	if ((objectNames[index].find("warehouseRoom") != string::npos) || (objectNames[index].find("warehouseRoof") != string::npos) 
    		|| (objectNames[index].find("warehouseFloor") != string::npos) || (objectNames[index].find("barrel") != string::npos) 
        || (objectNames[index].find("container") != string::npos) || (objectNames[index].find("man") != string::npos))

    	{
    		mass = 0;
    	}

    	else
    	{
  	    mass = 50;
    	}

    	btDefaultMotionState* genericMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(glmToBullet(genericObject->GetPosition()))));

      objectVector.push_back(genericObject);
      m_physics->addObject(genericShape, genericMotionState, mass, objectNames[index]);
    }

    //if it is rope
    else
    {
      
      int totalRope = 25;
      btCollisionShape *genericShape;
      
      for (int jindex = 0; jindex < totalRope; jindex++)
      {
        btTriangleMesh* objTriMesh = new btTriangleMesh();
        genericObject = new Object(objectNames[index], objTriMesh);

        genericShape = new btConvexTriangleMeshShape(objTriMesh, true);

        objectVector.push_back(genericObject);
        //std::cout << "size: " << objectVector.size() << endl;
      }
      m_physics->createRope(genericShape, totalRope);
      
    }
  } 

  m_physics->rotate( -90., lookupObjectIndex("man"));

  m_physics->attachLimbs(lookupObjectIndex("head"));

  paddleLindex = lookupObjectIndex( "leftPaddle");
  paddleRindex = lookupObjectIndex( "rightPaddle");
  ballIndex = lookupObjectIndex( "ball" );

  
  m_physics->addGroundPlane(0);

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

void Graphics::Update(unsigned int dt, MenuVars* menu)
{
  glm::mat4 temp;

  for (int index = 0; index < objectVector.size(); index++)
  {
    temp = m_physics->Update(dt, index);
    objectVector[index]->Update(dt, temp, menu);
  }
}

void Graphics::Render(ShaderSelector selector, MenuVars* menu, int gameState)
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
      SendUniforms(m_passthrough, menu, gameState);
      break;
    case PER_FRAGMENT:
      m_fragment->Enable();
      SendUniforms(m_fragment, menu, gameState);
      break;
    case PER_VERTEX:
      m_vertex->Enable();
      SendUniforms(m_vertex, menu, gameState);
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

bool Graphics::SendUniforms(Shader *m_shader, MenuVars* menu, int gameState)
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
    glUniform4f(m_shader->m_specular, objectVector[index]->GetSpecular().x, objectVector[index]->GetSpecular().y, objectVector[index]->GetSpecular().z, 0.0);
    glUniform1f(m_shader->m_shininess, objectVector[index]->GetShininess());
    //dont render body parts based on gamestate
    if (index < (lookupObjectIndex("head") + gameState) || index > (lookupObjectIndex("head") + 5))
    {
      objectVector[index]->Render();
    }
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