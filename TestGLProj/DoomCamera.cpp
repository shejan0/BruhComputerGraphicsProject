#include "DoomCamera.h"
#include <stdio.h>



Camera::CameraMovement sendValCamcustom = Camera::CameraMovement();

glm::vec4 vec4Center;
glm::vec3 lookUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec4 vec4Eye;

//limiter of left and right
int degreeCounter = 0;


//points for our camera
//doom camera movement
Camera::CameraMovement Camera::CustomCameraKeyboard(unsigned char key, glm::vec3 eye, glm::vec3 center)
{
	glm::vec3 lookatdir = glm::normalize(center - eye);
	vec4Center = glm::vec4(center, 1.0f); // Converts center from vec3 to vec4 temporarily in order to rotate it with glm::rotate

	switch (key)
	{
		case 27: // this is an ascii value
			exit(0);
			break;

		// move forward
		case 'w':
			eye += lookatdir;
			center += lookatdir;
			break;

		// move back
		case 's':
			eye -= lookatdir;
			center -= lookatdir;
			break;


		//look left
		case 'a':
			printf("in camera case a \n");

			//apply rotation to the vector
			vec4Center = glm::translate(eye.x, eye.y, eye.z)
				* glm::rotate(5.0f, 0.0f, 1.0f, 0.0f)
				* glm::translate(-eye.x, -eye.y, -eye.z)
				* vec4Center;
			center = glm::vec3(vec4Center);
			break;

		// look right
		case 'd':

			//apply rotation to the vector
			vec4Center = glm::translate(eye.x, eye.y, eye.z)
				* glm::rotate(-5.0f, 0.0f, 1.0f, 0.0f)
				* glm::translate(-eye.x, -eye.y, -eye.z)
				* vec4Center;
			center = glm::vec3(vec4Center);
			break;



		//for flyby camera controlls
		//f and GLUT_KEY_RIGHT do not like eachother so we need to seperate the two
		//move forward in the direction the camera is looking
		case 'f':
			printf("moving in f \n");
			eye += lookatdir;
			center += lookatdir;
			break;


		//move backwards in the direction the camera is looking
		case 'v':
			printf("moving in v \n");
			eye -= lookatdir;
			center -= lookatdir;
			break;

	}

	//return our values back to the caller
	sendValCamcustom.centerReturn = center;
	sendValCamcustom.eyeReturn = eye;
	return sendValCamcustom;
}


//for our special key input
Camera::CameraMovement Camera::FlyCameraKeyboard(int key, glm::vec3 move, glm::vec3 center)
{

	printf("Center is: %f %f %f \n", center.x, center.y, center.z);
	printf("Eye is: %f %f %f \n", move.x, move.y, move.z);


	//make the center a vector
	vec4Center = glm::vec4(center, 1);
	vec4Eye = glm::vec4(move, 1);


	printf("Flyby camera moving \n");

	//x2 is distance to move forward in 1 key
	//crossproduct of what we call up and the lookat direction
	glm::vec3 x2 = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), center - move));

	glm::vec3 lookatdir = glm::normalize(center - move);

	switch (key)
	{
		//loop up
		//45deg max
		//we translate the center and eye up by a couple of degrees -> glm::rotate(5.0f, 0.0f, 1.0f, 0.0f);
	case GLUT_KEY_UP:

		//update degree counter if we can
		if (degreeCounter < 10)
			degreeCounter += 1;

		printf("degree counter is %d \n", degreeCounter);

		if (degreeCounter * 5 <= 45 && degreeCounter * 5 >= -45)
		{
			center = center + lookUp;
		}
		break;


		//look down
		//45deg max
		//we translate down on the same points ->  glm::rotate(5.0f, 0.0f, -1.0f, 0.0f);
	case GLUT_KEY_DOWN:

		//update degree counter if we can
		if (degreeCounter > -10)
			degreeCounter -= 1;

		printf("degree counter is %d \n", degreeCounter);

		if (degreeCounter * 5 <= 45 && degreeCounter * 5 >= -45)
		{
			center = center - lookUp;
		}
		break;

		//turn left
		//we translate to the left both centerand eye
	case GLUT_KEY_LEFT:
		move += x2;
		center += x2;
		break;

		//turn right
		//we translate to the right both centerand eye
	case GLUT_KEY_RIGHT:
		move -= x2;
		center -= x2;
		break;
	}

	printf("Center is: %f %f %f \n", center.x, center.y, center.z);
	printf("Eye is: %f %f %f \n", move.x, move.y, move.z);

	sendValCamcustom.centerReturn = center;
	sendValCamcustom.eyeReturn = move;
	return sendValCamcustom;
}
