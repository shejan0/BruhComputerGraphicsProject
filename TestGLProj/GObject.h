#ifndef GOBJECT_H
#define GOBJECT_H
#include "Model.h"
#include <glm/glm.hpp>

class GObject {
public:
	GObject();
	GObject(float trans[3]);
	GObject(float trans[3], float rotat[3], float scale[3]);
	GObject(glm::mat4 localProjection);
	void draw(glm::mat4 WorldProjection=glm::mat4()); //default to identity
private: 
	std::vector<GObject> children;
	glm::mat4 localProjection;
	Model bruh;
};

#endif
