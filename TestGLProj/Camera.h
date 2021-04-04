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

/*
 * Camera.h
 *
 * Authors:
 *    Gabriel Vidaurri
 *    Joseph Daau
 *
 * Last Modified Date: 4/4/21
 *
 * Header file for Camera.cpp
 *
 */

class Camera
{
	public:
		struct CameraMovement
		{
			glm::vec3 eyeReturn; // Represents the camera eye
			glm::vec3 centerReturn; // Represents the camera center
		};

		// This will be called in main()'s keyboard()
		CameraMovement CustomCameraKeyboard(unsigned char key, glm::vec3 move, glm::vec3 center);
		CameraMovement FlyCameraKeyboard(int key, glm::vec3 move, glm::vec3 center);
		
		~Camera(void) {} // Default destructor
};

