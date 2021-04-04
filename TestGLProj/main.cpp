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


/*
 * main.cpp
 * 
 * Authors:
 *    Shejan Shuza
 *    Gabriel Vidaurri
 *    Joseph Daau
 *    Tyler Everett
 *    Christopher Urista
 *
 * Last Modified Date: 4/4/21
 *
 * *************Description of this file goes here*************
 *
 */


/* -- Doom Camera Declarations -- */
Camera::CameraMovement retValCamcustom = Camera::CameraMovement();
Camera customCam;
glm::vec3 eye(0.0f, 0.0f, 0.0f);  // The eye of the camera in first-person
glm::vec3 center(0.0f, 0.0f, 1.0f); // The center of the camera's focus in first-person
bool firstPersonCameraMain = true; // Bool for camera being first person or not
bool inFly = true; // Checker for fly camera values
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

GObject* cylOb, * planOb, * sphOb, * cubOb, *Obam;

Scene scene;
/* -- Shader, Model, and Scene Declarations End Here -- */

float rotation = 0.0f; // Float to handle rotation speed


/* Reports GL errors, if any, to stderr. */
void checkError(const char *functionName)
{
	GLenum error;
	while (( error = glGetError() ) != GL_NO_ERROR) {
		std::cerr << "GL error " << error << " detected in " << functionName << std::endl;
	}
}


glm::mat4 getProjection(float nearfield, float fov) {
	return glm::infinitePerspective(fov, (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT), nearfield);
}


void initShader(void)
{
	shader.InitializeFromFile("shaders/phong.vert", "shaders/phong.frag");
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
	eye = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z);

	// Initializes the data for our struct for our camera
	retValCamcustom.eyeReturn = eye;
	retValCamcustom.centerReturn = center;

	initShader ();
	initRendering ();

	printf("Eye at start is: %f %f %f\n", eye.x, eye.y, eye.z);
	printf("Center at start is: %f %f %f\n", center.x, center.y, center.z);

	printf("Sphere[0]: %f %f %f %f\n", sphereTransMatrix[0].x, sphereTransMatrix[0].y, sphereTransMatrix[0].z, sphereTransMatrix[0].w);
	printf("Sphere[1]: %f %f %f %f\n", sphereTransMatrix[1].x, sphereTransMatrix[1].y, sphereTransMatrix[1].z, sphereTransMatrix[1].w);
	printf("Sphere[2]: %f %f %f %f\n", sphereTransMatrix[2].x, sphereTransMatrix[2].y, sphereTransMatrix[2].z, sphereTransMatrix[2].w);
	printf("Sphere[3]: %f %f %f %f\n", sphereTransMatrix[3].x, sphereTransMatrix[3].y, sphereTransMatrix[3].z, sphereTransMatrix[3].w);
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

/* This gets called when the OpenGL is asked to display. This is where all the main rendering calls go. */
void display(void)
{
	/* The transformation heirarchy is cylinder -> sphere -> cube */
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the framebuffer data from the last frame

	// Camera view for our Doom game
	viewMatrix = glm::lookAt(retValCamcustom.eyeReturn, retValCamcustom.centerReturn, glm::vec3(0.0f, 1.0f, 0.0f));
	
	// Updates the model matrix representing our head.
	headModelMatrix = modelMatrix;

	// Renders the ground.
	ground->render(viewMatrix * glm::translate(0.0f, -5.0f, 0.0f) * glm::scale(20.0f, 1.0f, 20.0f), projectionMatrix);

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
	if (key == 'c' && firstPersonCameraMain == true)
	{
		retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y + 20.0f, headModelMatrix[3].z - 40.0f); // The eye starting point will be further back
		retValCamcustom.centerReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y + 5.0f, headModelMatrix[3].z + 10.0f);
		firstPersonCameraMain = false;
	}
	else if (key == 'c' && firstPersonCameraMain == false)
		firstPersonCameraMain = true;

	// Activates each case depending on which key on the keyboard is pressed
	switch (key) 
	{
		case 27: // This is an ASCII value respresenting the ESC key
			exit(0);
			break;

		case 'w': // Moves our character forward
			// Sets up the values to send to our camera
			retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z);

			// Calls our custom keyboard camera
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
			
			printf("LookatdirReturn: %f %f %f\n", retValCamcustom.lookatdirReturn.x, retValCamcustom.lookatdirReturn.y, retValCamcustom.lookatdirReturn.z);

			// Updates the model matrix of our character to move forward
			modelMatrix = glm::translate(retValCamcustom.lookatdirReturn) * modelMatrix;

			break;

		case 's': // Moves our character back
			// Sets up the values to send to our camera
			retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z);

			// Calls our custom keyboard camera
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
			
			// Updates the model matrix of our character to move back
			modelMatrix = glm::translate(-retValCamcustom.lookatdirReturn) * modelMatrix;

			break;

		case 'a': // Rotates our character to the left
			// Sets up the values to send to our camera
			retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z);

			// Calls our custom keyboard camera
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
			
			// Rotates the model matrix of our character to the left
			modelMatrix = modelMatrix * glm::rotate(5.0f, 0.0f, 1.0f, 0.0f);

			break;

		case 'd': // Rotates our character to the right	
			// Sets up the values to send to our camera
			retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z);

			// Calls our custom keyboard camera
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
			
			// Rotates the model matrix of our character to the right
			modelMatrix = modelMatrix * glm::rotate(-5.0f, 0.0f, 1.0f, 0.0f);

			break;
	
		case 'b': // Activates the bounding boxes of objects in the scene
			scene.changeBoxState();
			break;

		case 'f': // Zooms in (Fly camera)
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
			break;

		case 'v': // Zooms out (Fly camera)
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
			break;
	}
}


// Fly Camera calls are done here
void SpecialKeyHandler(int key, int x, int y)
{
	if (firstPersonCameraMain == false)
	{
		// If we are new to using inFly, send new values in
		if (inFly == true)
		{
			// Flyby camera shall be moving
			retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y + 20.0f, headModelMatrix[3].z - 40.0f); // The eye starting point will be further back
			retValCamcustom.centerReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y + 5.0f, headModelMatrix[3].z + 10.0f);

			// Call our custom keyboard camera
			retValCamcustom = customCam.FlyCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
		}
		else // We don't change what is in our eye and center
			// Call our custom keyboard camera
			retValCamcustom = customCam.FlyCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);

		inFly = false;
	}
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB | GLUT_DEPTH |GLUT_MULTISAMPLE);
	glutSetOption(GLUT_MULTISAMPLE, 4);
	glutInitWindowSize (800, 600); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	glewInit();
	dumpInfo ();
	init ();
	glutDisplayFunc(display); 
	glutIdleFunc(idle); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc (keyboard);
	glutSpecialFunc(SpecialKeyHandler); // Special keyboard
	glEnable(GL_DEPTH_TEST);
	
	fprintf(stderr, "Scene %p\n", &scene);
	fprintf(stderr, "Shader: %p\n", &shader);

	cylinder = new Model(&shader, "models/cylinder.obj");
	ground = new Model(&shader, "models/plane.obj"); // Loads the plane model for the ground
	sphere = new Model(&shader, "models/sphere.obj"); // !!! Used temporarily as our "head" until we have a character model
	cube = new Model(&shader, "models/unitcube.obj", "models/");
	obamium = new Model(&shader, "models/obamium.obj");
	//fprintf(stderr, "Cylinder:%p,Plane:%p,Sphere:%p,Cube:%p\n", cylinder, plane, sphere, cube);
	
	cylOb = new GObject(cylinder);
	planOb = new GObject(ground);
	sphOb = new GObject(sphere);
	cubOb = new GObject(cube);
	Obam = new GObject(obamium);
	//fprintf(stderr, "GObject: Cylinder:%p,Plane:%p,Sphere:%p,Cube:%p\n", cylOb, planOb, sphOb, cubOb);
	
	scene.addChild(cylOb);
	cylOb->addChild(sphOb);
	sphOb->addChild(cubOb);
	scene.addChild(planOb);
	sphOb->addChild(Obam);
	//scene.addChild(planOb);
	//scene.addChild(sphOb);
	//scene.addChild(cubOb);

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