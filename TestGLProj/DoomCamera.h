#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class Camera
{


public:

	struct CameraMovement
	{
		glm::vec3 eyeReturn;
		glm::vec3 centerReturn;
	};

	//this will be called in main's keyboard
	CameraMovement CustomCameraKeyboard(unsigned char key, glm::vec3 move, glm::vec3 center);
	CameraMovement FlyCameraKeyboard(int key, glm::vec3 move, glm::vec3 center);
	~Camera(void) {} // default destructor

};

