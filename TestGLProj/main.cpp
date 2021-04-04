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
#include "Scene.h" //Scene>GObject>Model>Shader all defined in recursion
#include "Shader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "DoomCamera.h"//first person camera



/* -- Doom Camera Declaration  --*/
Camera::CameraMovement retValCamcustom = Camera::CameraMovement();
Camera customCam;
glm::vec3 eye(0.0f, 0.0f, 0.0f);  // The eye of the camera in first-person
glm::vec3 center(0.0f, 1.0f, 10.0f); // The center of the camera's focus in first-pers
bool firstPersonCameraMain = true; //bool for camera being first person or not
int inFly = 1; //checker for fly camera values

/* -- Head Model Declaration- */
glm::mat4 headModelMatrix; // Model matrix representing the head's position
glm::mat4 sphereTrans; // this will be our head




glm::vec3 move(0.0f,0.0f, 15.0f);
//glm::vec3 center(0.0f,0.0f,0.0f);
Shader shader; // loads our vertex and fragment shaders
Model* cylinder; //a cylinder 
Model* plane; //a plane
Model* sphere; //a sphere
Model* cube; //a cube
Model* obamium;
GObject* cylOb, * planOb, * sphOb, * cubOb, *Obam;
glm::mat4 cubeTrans; // where the model is located wrt the camera
glm::mat4 cylinderTrans; // where the model is located wrt the camera
glm::mat4 planeTrans;
glm::mat4 view; // where the camera is looking
glm::mat4 projection; // projection matrix
Scene scene;

float angle = 0;
/* report GL errors, if any, to stderr */
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
	glClearColor (0.117f, 0.565f, 1.0f, 0.0f); // Dodger Blue
	checkError ("initRendering");
}

void init(void) 
{	
	// Perspective projection matrix.
	//projection = glm::perspective(45.0f, (float)glutGet(GLUT_WINDOW_WIDTH)/(float)glutGet(GLUT_WINDOW_HEIGHT), 1.0f, 1000.0f);
	//projection = glm::infinitePerspective(45.0f, (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT), 1.0f);
	projection = getProjection(1.0f, 45.0f);
	// Load identity matrix into model matrix (no initial translation or rotation)
	
	headModelMatrix = sphereTrans * glm::scale(0.55f, 0.55f, 0.55f) * glm::translate(0.0f, 3.0f, 0.0f);
	eye = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z);

	//initialize the current data for our struct
	retValCamcustom.eyeReturn = eye;
	retValCamcustom.centerReturn = center;


	initShader ();
	initRendering ();
}

/* This prints in the console when you start the program*/
void dumpInfo(void)
{
	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
	checkError ("dumpInfo");
}
float rotation = 0.0f;
/*This gets called when the OpenGL is asked to display. This is where all the main rendering calls go*/
void display(void)
{

/*The transformation heirarchy is cylinder -> sphere -> cube*/
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the framebuffer data from the last frame

	//First Person camera view for our doom game
	view = glm::lookAt(retValCamcustom.eyeReturn, retValCamcustom.centerReturn, glm::vec3(0.0f, 1.0f, 0.0f));

	
	cubOb->setRotation(glm::vec3(rotation * 2.0f, 0.0f, 0.0f));
	Obam->setRotation(glm::vec3(0.0f, rotation * 2.0f, 0.0f));
	

	scene.draw(projection, view);

	glutSwapBuffers(); // Swap the buffers.
	checkError ("display");
}

/*This gets called when nothing is happening (OFTEN)*/
void idle()
{
	glutPostRedisplay(); // create a display event. Display calls as fast as CPU will allow when put in the idle function
}

/*Called when the window is resized*/
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	projection = getProjection(1.0f,45.0f);
	checkError ("reshape");
}

/*Called when a normal key is pressed*/
void keyboard(unsigned char key, int x, int y)
{

	//toggle between firstPerson and fly camera with C
	if (key == 'c' && firstPersonCameraMain == true)
	{
		retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y + 20.0f, headModelMatrix[3].z - 20.0f); // the eye, starting point will be further back
		retValCamcustom.centerReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y + 5.0f, headModelMatrix[3].z + 10.0f);
		printf("Toggling first person mode off \n");
		firstPersonCameraMain = false;
	}
	else if (key == 'c' && firstPersonCameraMain == false)
	{

		printf("Toggling first person mode on \n");
		firstPersonCameraMain = true;
	}







	switch (key) 
	{
		case 27: // this is an ascii value
			exit(0);
			break;


		// move forward
		case 'w':
			
			//setup the values to send to our camera
			retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z);
			retValCamcustom.centerReturn = retValCamcustom.centerReturn;

			//call our custom keyboard camera
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
			break;

		// move back
		case 's':
			
			//setup the values to send to our camera
			retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z);
			retValCamcustom.centerReturn = retValCamcustom.centerReturn;

			//call our custom keyboard camera
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
			break;

		// move left
		case 'a':
			
			//setup the values to send to our camera
			retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z);
			retValCamcustom.centerReturn = retValCamcustom.centerReturn;

			//call our custom keyboard camera
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
			break;

		// move right
		case 'd':
			
			//setup the values to send to our camera
			retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z);
			retValCamcustom.centerReturn = retValCamcustom.centerReturn;

			//call our custom keyboard camera
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
			break;
	
		case 'b':
			scene.changeBoxState();
			break;

		//zoom in (FLY BY CAMERA)
		case 'f':
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
			break;

		//Zoom out (Fly By camera)
		case 'v':
			retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
			break;
	}
}




//freeflycamera calls are done here
void SpecialKeyHandler(int key, int x, int y)
{
	printf("in special keyboard \n");

	//check if in the special key

	if (firstPersonCameraMain == false)
	{


		//if we are new in the influ counter send new values in
		if (inFly == 1)
		{
			//flyby camera shall be moving
			retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y + 20.0f, headModelMatrix[3].z - 20.0f); // the eye, starting point will be further back
			retValCamcustom.centerReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y + 5.0f, headModelMatrix[3].z + 10.0f);

			//call our custom keyboard camera
			retValCamcustom = customCam.FlyCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
		}
		else //we don't change what is in our move and center
		{

			printf("in else \n");
			printf("Center in else is: %f %f %f \n", center.x, center.y, center.z);
			printf("Eye in else is: %f %f %f \n", move.x, move.y, move.z);
			//call our custom keyboard camera
			retValCamcustom = customCam.FlyCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
		}


		inFly = 0;
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
	glutSpecialFunc(SpecialKeyHandler);// special keyboard
	glEnable(GL_DEPTH_TEST);
	
	
	fprintf(stderr, "Scene %p\n", &scene);
	fprintf(stderr, "Shader: %p\n", &shader);
	cylinder = new Model(&shader, "models/cylinder.obj");
	plane = new Model(&shader, "models/plane.obj");
	sphere = new Model(&shader, "models/sphere.obj");
	cube = new Model(&shader, "models/unitcube.obj", "models/");
	obamium = new Model(&shader, "models/obamium.obj");
	//fprintf(stderr, "Cylinder:%p,Plane:%p,Sphere:%p,Cube:%p\n", cylinder, plane, sphere, cube);
	cylOb = new GObject(cylinder);
	planOb = new GObject(plane);
	sphOb = new GObject(sphere);
	cubOb = new GObject(cube);
	Obam = new GObject(obamium);
	//fprintf(stderr, "GObject: Cylinder:%p,Plane:%p,Sphere:%p,Cube:%p\n", cylOb, planOb, sphOb, cubOb);
	scene.addChild(cylOb);
	cylOb->addChild(sphOb);
	sphOb->addChild(cubOb);
	scene.addChild(planOb);
	sphOb->addChild(Obam);
	cylOb->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	cylOb->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	sphOb->setPosition(glm::vec3(0.0, 2.2f, 0.0f));
	sphOb->setRotation(glm::vec3(0.0f, 0.0f, 1.0f));
	cubOb->setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
	Obam->setPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
	planOb->setPosition(glm::vec3(0.0f, 1.5f, 0.0f));
	//scene.addChild(planOb);
	//scene.addChild(sphOb);
	//scene.addChild(cubOb);
	glutMainLoop();

	return 0;
}