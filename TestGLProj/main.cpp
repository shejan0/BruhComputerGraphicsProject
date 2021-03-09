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
#include "Shader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

glm::vec3 move(0.0f,0.0f, 15.0f);
glm::vec3 center(0.0f,0.0f,0.0f);
Shader shader; // loads our vertex and fragment shaders
Model *cylinder; //a cylinder 
Model *plane; //a plane
Model *sphere; //a sphere
Model *cube; //a cube
glm::mat4 projection; // projection matrix
glm::mat4 view; // where the camera is looking
glm::mat4 sphereTrans; // where the model is located wrt the camera
glm::mat4 cubeTrans; // where the model is located wrt the camera
glm::mat4 cylinderTrans; // where the model is located wrt the camera
float angle = 0;
/* report GL errors, if any, to stderr */
void checkError(const char *functionName)
{
	GLenum error;
	while (( error = glGetError() ) != GL_NO_ERROR) {
		std::cerr << "GL error " << error << " detected in " << functionName << std::endl;
	}
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
	projection = glm::perspective(45.0f, 800.0f/600.0f, 1.0f, 1000.0f);

	
	// Load identity matrix into model matrix (no initial translation or rotation)
	

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

	//Initial camera: positioned at 15 on the z axis, looking into the screen down -z.
	view = glm::lookAt(move, center, glm::vec3(0.0f, 1.0f, 0.0f));

	rotation += 0.1f; // Update rotation angle if rotation is enabled.
	
	cylinderTrans = /*glm::rotate(rotation, 0.0f, 0.0f, 1.0f)*/glm::translate(0.0f, 0.0f, 0.0f);/*glm::translate(-2.0f,1.0f,-6.0f) */glm::scale(1.0, 1.0, 1.0); // rotates the model by the current rotation angle.

	cylinder->render(view * cylinderTrans, projection); // Render the cylinder

	sphereTrans = cylinderTrans*glm::rotate(0.0f, 0.0f, 0.0f, 1.0f)* glm::translate(0.0f, 2.2f, 0.0f);
	sphere->render(view * sphereTrans, projection); // Render the cube in another spot

/*	cubeTrans = sphereTrans * glm::translate(2.0f,0.0f,0.0f) * glm::rotate(rotation*2.0f,1.0f,0.0f,0.0f); 
	cube->render(view * cubeTrans, projection); // Render the cube in another spot*/
	
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
	checkError ("reshape");
}

/*Called when a normal key is pressed*/
void keyboard(unsigned char key, int x, int y)
{
	glm::vec3 x1 = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), center - move));
	glm::vec3 y1 = glm::normalize(glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), center - move));
	glm::vec3 lookatdir = glm::normalize(center - move);
	switch (key) {
	case 27: // this is an ascii value
		exit(0);
		break;
	case 'w':
		// move forward
		move += lookatdir;
		center += lookatdir;
		break;

	case 's':
		// move back
		move -= lookatdir;
		center -= lookatdir;
		break;

	case 'a':
		// move left
		move += x1;
		center += x1;
		break;

	case 'd':
		// move right
		move = move - x1;
		center -= x1;
		break;
	case 'f':
		move = move + y1;
		center += y1;
		break;
	case 'r':
		move = move - y1;
		center -= y1;
		break;
	case 'q':
		// turn left
		
		center = center + x1*.5f;
		break;

	case 'e':
		// turn right
		center = center + x1*-.5f;
		break;
	}
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB | GLUT_DEPTH);
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
	glEnable(GL_DEPTH_TEST);

	cylinder = new Model(&shader, "models/cylinder.obj");
	plane = new Model(&shader, "models/plane.obj");
	sphere = new Model(&shader, "models/sphere.obj");
	cube = new Model(&shader, "models/unitcube.obj", "models/");

	glutMainLoop();

	return 0;
}