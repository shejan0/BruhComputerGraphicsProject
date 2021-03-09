#ifndef GOBJECT_H
#define GOBJECT_H

#include "Model.h"
#include <glm/glm.hpp>

class GObject {
public:
	GObject(float trans[3]);
	GObject(float trans[3], float rotat[3], float scale[3]);
	GObject(glm::mat4 projection = glm::mat4(), Model* m=NULL);
	void draw(glm::mat4 modelView=glm::mat4(),glm::mat4 worldProjection=glm::mat4()); //default to identity
	~GObject(); //default deconstruct children
	GObject* removeChild(GObject* child = NULL);
	//GObject* createChild();
	void addChild(GObject* newChild);
	glm::mat4 rotate();
	GObject* addModel(Model* model);
private: 
	std::vector<GObject *> children;
	glm::mat4 localProjection;
	Model* bruh;
};

#endif
