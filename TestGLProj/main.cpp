
#define PROJECT_NAME "Final Project for CS 4383 Quarles-Spring 2021"
#define GROUP_NUM "Group 42"
#define LAST_EDIT_DATE "5/8/21 4:00AM"
#define LAST_EDITOR "Gabe Vidaurri"

#include <GL/glew.h>
#include <GL/freeglut.h>

//glm library
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.h"
#include "GObject.h"
#include "Scene.h" // Scene>GObject>Model>Shader all defined in recursion
#include "Shader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Camera.h" // Camera for our game
#include "Error.h"


/*
 * main.cpp
 * GitHub link: https://github.com/shejan0/BruhComputerGraphicsProject
 * Authors:
 *    Shejan Shuza
 *    Gabriel Vidaurri
 *    Joseph Daau
 *    Tyler Everett
 *    Christopher Urista
 * Description: Defines the main() for the program and acts as controller for many core elements.
 *
 */



/* -- Doom Camera Declarations -- */
Camera::CameraMovement retValCamcustom = Camera::CameraMovement(); // Struct to hold our values for 1st person camera
Camera::CameraMovement retValCamcustomFly = Camera::CameraMovement(); // Struct to hold our values for fly mode camera
Camera customCam;
glm::vec3 eye(0.0f, 0.0f, 0.0f);  // The eye of the camera in first-person
glm::vec3 center(0.0f, 0.0f, 1.0f); // The center of the camera's focus in first-person

glm::vec3 eyeFly; // The eye of the camera in fly mode
glm::vec3 centerFly; // The center of the camera's focus in fly mode

bool isFirstPersonCamera = true; // Bool for camera being first person or not
/* -- End of Doom Camera Declarations -- */


/* -- Matrix Declarations -- */
glm::mat4 modelMatrix; // Where the overall model is located with respect to the camera
glm::mat4 viewMatrix; // Where the camera is looking
glm::mat4 projectionMatrix; // Projection matrix

glm::mat4 headModelMatrix; // Model matrix representing the head's position

glm::mat4 sphereTransMatrix; // Where the sphere model is located wrt the camera
glm::mat4 cubeTransMatrix; // Where the cube model is located wrt the camera
glm::mat4 cylinderTransMatrix; // Where the cylinder model is located wrt the camera
glm::mat4 planeTransMatrix; // (The ground) Where the plane model is located wrt the camera
/*-- End of Matrix Declarations --*/


/* -- Shader, Model, and Scene Declarations -- */
Shader shader; // loads our vertex and fragment shaders

Model* cylinder; // a cylinder 
Model* ground; // a plane representing the ground
Model* sphere; // a sphere
Model* cube; // a cube
Model* obamium;
Model* demon;
//all the walls
Model* wall1, * wall2, * wall3, * wall4, * wall5, * wall6, * wall7, 
     * wall8, * wall9, * wall10, * wall11, * wall12, * wall13,  *wall14,
	 * wall15, * wall16, * wall17, * wall18, * wall19, * wall20, * wall21;
GObject* cylOb, * planOb, * sphOb, * cubOb, *Obam, *groundOb,*demonOb;
Scene scene;
/* -- Shader, Model, and Scene Declarations End Here -- */

float rotation = 0.0f; // Float to handle rotation speed


/* Reports GL errors, if any, to stderr. */
/*void checkError(const char *functionName)
{
	GLenum error;
	while (( error = glGetError() ) != GL_NO_ERROR) {
		std::cerr << "GL error " << error << " detected in " << functionName << std::endl;
	}
}*/


glm::mat4 getProjection(float nearfield, float fov) {
	return glm::infinitePerspective(fov, (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT), nearfield);
}


void initShader(void)
{
	shader.InitializeFromFile("shaders/phong3text.vert", "shaders/phong3text.frag");
	shader.AddAttribute("vertexPosition");
	shader.AddAttribute("vertexNormal");

	checkError ("initShader");
}


void initRendering(void)
{
	glClearColor (0.117f, 0.565f, 1.0f, 0.0f); // Dodger Blue Background Color
	checkError ("initRendering");
}


void init(void) 
{	
	// Perspective projection matrix.
	//projection = glm::perspective(45.0f, (float)glutGet(GLUT_WINDOW_WIDTH)/(float)glutGet(GLUT_WINDOW_HEIGHT), 1.0f, 1000.0f);
	//projection = glm::infinitePerspective(45.0f, (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT), 1.0f);
	projectionMatrix = getProjection(1.0f, 45.0f);
	
	// Calculates the initial model matrix of the head in order to be able to set the eye of the first-person camera at the robot's starting position.
	modelMatrix = sphereTransMatrix;
	headModelMatrix = modelMatrix;
	eye = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z - 10.0f);

	// Initializes the data for our struct for our camera
	retValCamcustom.eyeReturn = eye;
	retValCamcustom.centerReturn = center;

	initShader ();
	initRendering ();
}


/* This prints in the console when you start the program. */
void dumpInfo(void)
{
	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
	checkError ("dumpInfo");
}

void renderWalls()
{
	//furthest right walls, to make the outside border
	wall1->render(viewMatrix * glm::scale(1.0f, 20.0f, 400.0f) * glm::translate(-100.0f, 0.2f, 0.24f), projectionMatrix);
	wall2->render(viewMatrix * glm::scale(1.0f, 20.0f, 400.0f) * glm::translate(-100.0f, 0.2f, -.24f), projectionMatrix);

	//vertical walls within the border walls. positioned from left to right 
	wall3->render(viewMatrix * glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(-67.0f, 0.2f, -1.3f), projectionMatrix);
	wall4->render(viewMatrix * glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(-25.0f, 0.2f, -1.3f), projectionMatrix);
	wall5->render(viewMatrix * glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(-20.0f, 0.2f, 0.0f), projectionMatrix);
	wall6->render(viewMatrix * glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(-60.0f, 0.2f, 0.0f), projectionMatrix);
	wall7->render(viewMatrix * glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(20.0f, 0.2f, 0.0f), projectionMatrix);
	wall8->render(viewMatrix * glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(30.0f, 0.2f, -1.3f), projectionMatrix);
	wall9->render(viewMatrix * glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(60.0f, 0.2f, 0.0f), projectionMatrix);
	wall10->render(viewMatrix * glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(70.0f, 0.2f, -1.3f), projectionMatrix);
	wall11->render(viewMatrix * glm::scale(1.0f, 20.0f, 300.0f) * glm::translate(90.0f, 0.2f, 0.0f), projectionMatrix);
	
	//horizontal walls to make top and bottom border 
	wall14->render(viewMatrix * glm::scale(200.0f, 20.0f, 5.0f) * glm::translate(0.0f, 0.2f, 60.5f), projectionMatrix);
	wall15->render(viewMatrix * glm::scale(200.0f, 20.0f, 5.0f) * glm::translate(0.0f, 0.2f, -60.5f), projectionMatrix);

	//furthest left walls, makes the outside border
	wall12->render(viewMatrix * glm::scale(1.0f, 20.0f, 400.0f) * glm::translate(100.0f, 0.2f, 0.24f), projectionMatrix);
	wall13->render(viewMatrix * glm::scale(1.0f, 20.0f, 400.0f) * glm::translate(100.0f, 0.2f, -.24f), projectionMatrix);
}

void wallModels() 
{
	wall1 = new Model(&shader, "models/unitcube.obj", "models/");
	wall2 = new Model(&shader, "models/unitcube.obj", "models/");
	wall3 = new Model(&shader, "models/unitcube.obj", "models/");
	wall4 = new Model(&shader, "models/unitcube.obj", "models/");
	wall5 = new Model(&shader, "models/unitcube.obj", "models/");
	wall6 = new Model(&shader, "models/unitcube.obj", "models/");
	wall7 = new Model(&shader, "models/unitcube.obj", "models/");
	wall8 = new Model(&shader, "models/unitcube.obj", "models/");
	wall9 = new Model(&shader, "models/unitcube.obj", "models/");
	wall10 = new Model(&shader, "models/unitcube.obj", "models/");
	wall11 = new Model(&shader, "models/unitcube.obj", "models/");
	wall12 = new Model(&shader, "models/unitcube.obj", "models/");
	wall13 = new Model(&shader, "models/unitcube.obj", "models/");
	wall14 = new Model(&shader, "models/unitcube.obj", "models/");
	wall15 = new Model(&shader, "models/unitcube.obj", "models/");
	wall16 = new Model(&shader, "models/unitcube.obj", "models/");
	wall17 = new Model(&shader, "models/unitcube.obj", "models/");
	wall18 = new Model(&shader, "models/unitcube.obj", "models/");
	wall19 = new Model(&shader, "models/unitcube.obj", "models/");
}



/* This gets called when the OpenGL is asked to display. This is where all the main rendering calls go. */
void display(void)
{
	/* The transformation heirarchy is cylinder -> sphere -> cube */
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the framebuffer data from the last frame

	// Camera view for our Doom game
	// Changes the view matrix depending on which camera type is being used in the program.
	if (isFirstPersonCamera == true)
		viewMatrix = glm::lookAt(retValCamcustom.eyeReturn, retValCamcustom.centerReturn, glm::vec3(0.0f, 1.0f, 0.0f));
	else
		viewMatrix = glm::lookAt(retValCamcustomFly.eyeReturn, retValCamcustomFly.centerReturn, glm::vec3(0.0f, 1.0f, 0.0f));
	
	/*shader.SetUniform("lightPosition", viewMatrix * glm::vec4(0.0f, 0.0f, 3.0f, 1.0f));
	shader.SetUniform("lightDiffuse", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("lightSpecular", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("lightAmbient", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("linearAttenuationCoefficient", .3f);*/
	// Updates the model matrix representing our head.
	headModelMatrix = modelMatrix;

	// Renders the ground.
	ground->render(viewMatrix * glm::translate(0.0f, -5.0f, 0.0f) * glm::scale(100.0f, 100.0f, 300.0f), projectionMatrix);

	renderWalls();


	cubOb->setRotation(glm::vec3(rotation * 2.0f, 0.0f, 0.0f));
	Obam->setRotation(glm::vec3(0.0f, rotation * 2.0f, 0.0f));
	
	scene.draw(projectionMatrix, viewMatrix);

	glutSwapBuffers(); // Swap the buffers.
	checkError ("display");
}

/* This gets called when nothing is happening (OFTEN) */
void idle()
{
	glutPostRedisplay(); // Creates a display event. Display calls as fast as CPU will allow when put in the idle function.
}

/* Called when the window is resized. */
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	projectionMatrix = getProjection(1.0f,45.0f);
	checkError ("reshape");
}

/* Called when a normal key is pressed. */
void keyboard(unsigned char key, int x, int y)
{
	// Toggles between first-person and fly camera with 'c'
	if (key == 'c' && isFirstPersonCamera == true)
	{
		// Makes it so that the fly camera always starts viewing at angle towards where the player currently is
		retValCamcustomFly.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y + 20.0f, headModelMatrix[3].z - 40.0f); // The eye starting point will be further back
		retValCamcustomFly.centerReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y + 5.0f, headModelMatrix[3].z + 10.0f);
		isFirstPersonCamera = false;
	}
	else if (key == 'c' && isFirstPersonCamera == false)
	{
		isFirstPersonCamera = true;
	}

	glm::vec3 x1 = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), center - eye));

	// Activates each case depending on which key on the keyboard is pressed
	switch (key) 
	{
		case 27: // This is an ASCII value respresenting the ESC key
			exit(0);
			break;

		case 'q': // Strafes left //DOES NOT WORK
			// Sets up the values to send to our camera
			retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z - 10.0f);

			// Calls our custom keyboard camera
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);

			// Updates the model matrix of our character to strafe to the left
			modelMatrix = glm::translate(x1) * modelMatrix;
			break;

		case 'e': // Strafes right //DOES NOT WORK
			// Sets up the values to send to our camera
			retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z - 10.0f);

			// Calls our custom keyboard camera
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);

			// Updates the model matrix of our character to strafe to the right
			modelMatrix = glm::translate(-x1) * modelMatrix;
			break;

		case 'w': // Moves our character forward
			// Sets up the values to send to our camera
			retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z - 10.0f);

			// Calls our custom keyboard camera
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
			
			// Updates the model matrix of our character to move forward
			modelMatrix = glm::translate(retValCamcustom.lookatdirReturn) * modelMatrix;

			break;

		case 's': // Moves our character back
			// Sets up the values to send to our camera
			retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z - 10.0f);

			// Calls our custom keyboard camera
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
			
			// Updates the model matrix of our character to move back
			modelMatrix = glm::translate(-retValCamcustom.lookatdirReturn) * modelMatrix;

			break;

		case 'a': // Rotates our character to the left
			// Sets up the values to send to our camera
			retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z - 10.0f);

			// Calls our custom keyboard camera
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
			
			// Rotates the model matrix of our character to the left
			modelMatrix = modelMatrix * glm::rotate(5.0f, 0.0f, 1.0f, 0.0f);

			break;

		case 'd': // Rotates our character to the right	
			// Sets up the values to send to our camera
			retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z - 10.0f);

			// Calls our custom keyboard camera
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
			
			// Rotates the model matrix of our character to the right
			modelMatrix = modelMatrix * glm::rotate(-5.0f, 0.0f, 1.0f, 0.0f);

			break;
	
		case 'b': // Activates the bounding boxes of objects in the scene
			scene.changeBoxState();
			break;

		case 'f': // Zooms in (Fly camera)
			retValCamcustomFly = customCam.CustomCameraKeyboard(key, retValCamcustomFly.eyeReturn, retValCamcustomFly.centerReturn);
			break;

		case 'v': // Zooms out (Fly camera)
			retValCamcustomFly = customCam.CustomCameraKeyboard(key, retValCamcustomFly.eyeReturn, retValCamcustomFly.centerReturn);
			break;
	}
}


// Fly Camera calls are done here
void SpecialKeyHandler(int key, int x, int y)
{
	if (isFirstPersonCamera == false)
	{
		// Call our custom keyboard camera
		retValCamcustomFly = customCam.FlyCameraKeyboard(key, retValCamcustomFly.eyeReturn, retValCamcustomFly.centerReturn);
	}
}



int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGBA | GLUT_DEPTH |GLUT_MULTISAMPLE);
	glutSetOption(GLUT_MULTISAMPLE, 4);
	glutInitWindowSize (800, 600); 
	glutInitWindowPosition (100, 100);
	std::string windowtitle = std::string(PROJECT_NAME)+"-"+std::string(GROUP_NUM)+"-"+std::string(LAST_EDIT_DATE)+"-"+std::string(LAST_EDITOR)+"-"+std::string(argv[0]);
	//glutCreateWindow (argv[0]);
	glutCreateWindow(windowtitle.c_str());
	glewInit();
	dumpInfo ();
	init ();
	glutDisplayFunc(display); 
	glutIdleFunc(idle); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc (keyboard);
	glutSpecialFunc(SpecialKeyHandler); // Special keyboard
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ARB_debug_output);
	//glDebugMessageCallbackARB(MessageCallback, 0);
	fprintf(stderr, "Scene %p\n", &scene);
	fprintf(stderr, "Shader: %p\n", &shader);
	cylinder = new Model(&shader, "models/cylinder.obj");
	ground = new Model(&shader, "models/plane.obj"); // Loads the plane model for the ground
	sphere = new Model(&shader, "models/sphere.obj"); // !!! Used temporarily as our "head" until we have a character model
	cube = new Model(&shader, "models/unitcube.obj", "models/");
	wallModels();
	//obamium = new Model(&shader, "models/obamium.obj");
	obamium = new Model(&shader, "models/obamid.obj","models/");
	demon = new Model(&shader, "models/cacodemon.obj", "models/");
	//fprintf(stderr, "Cylinder:%p,Plane:%p,Sphere:%p,Cube:%p\n", cylinder, plane, sphere, cube);
	cylinder->setOverrideDiffuseMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	ground->setOverrideDiffuseMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	sphere->setOverrideDiffuseMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	cube->setOverrideDiffuseMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	obamium->setOverrideDiffuseMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	demon->setOverrideDiffuseMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	cylinder->setOverrideSpecularMaterial(glm::vec4(0.5f, 0.5f, 0.0f, 0.3f));
	cylOb = new GObject(cylinder);
	planOb = new GObject(ground);
	sphOb = new GObject(sphere);
	cubOb = new GObject(cube);
	Obam = new GObject(obamium);
	groundOb = new GObject(ground);
	demonOb = new GObject(demon);
	//fprintf(stderr, "GObject: Cylinder:%p,Plane:%p,Sphere:%p,Cube:%p\n", cylOb, planOb, sphOb, cubOb);
	scene.addChild(groundOb);
	scene.addChild(demonOb);
	scene.addChild(cylOb);
	cylOb->addChild(sphOb);
	sphOb->addChild(cubOb);
	scene.addChild(planOb);
	sphOb->addChild(Obam);
	//scene.addChild(planOb);
	//scene.addChild(sphOb);
	//scene.addChild(cubOb);
	//demonOb->useObjectMaterials(true);
	//Obam->useObjectMaterials(true);
	demonOb->setRotation(glm::vec3(0.0f, 180.0f, 0.0f));
	demonOb->setPosition(glm::vec3(3.0f, 0.0f, 0.0f));
	groundOb->setPosition(glm::vec3(0.0f, -5.0f, 0.0f));
	groundOb->setScale(glm::vec3(20.0f, 1.0f, 20.0f));
	cylOb->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	cylOb->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	sphOb->setPosition(glm::vec3(0.0, 2.2f, 0.0f));
	sphOb->setRotation(glm::vec3(0.0f, 0.0f, 1.0f));
	cubOb->setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
	Obam->setPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
	planOb->setPosition(glm::vec3(0.0f, 1.5f, 0.0f));

	glutMainLoop();

	return 0;
}