#ifndef GOBJECT_H
#define GOBJECT_H

#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GObject {
public:
	GObject(float trans[3]) : GObject(glm::translate(trans[0], trans[1], trans[2])) {}
	GObject(float trans[3], float rotat[3], float scale[3]) : GObject(glm::scale(scale[0], scale[1], scale[2])* glm::rotate(rotat[0], 1.0f, 0.0f, 0.0f)* glm::rotate(rotat[1], 0.0f, 1.0f, 0.0f)* glm::rotate(rotat[2], 0.0f, 0.0f, 1.0f)* glm::translate(trans[0], trans[1], trans[2])) {}
	GObject(glm::mat4 projection = glm::mat4(), Model* m=NULL);
	GObject(Model* m) : GObject(glm::mat4(), m) {}
	void draw(glm::mat4 modelView=glm::mat4(),glm::mat4 worldProjection=glm::mat4()); //default to identity
	~GObject(); //default deconstruct children
	GObject* removeChild(GObject* child = NULL);
	//GObject* createChild();
	void addChild(GObject* newChild);
	//glm::mat4 rotate();
	glm::mat4 updateModelView(glm::mat4 localView);
	glm::mat4 updateLocalProjection(glm::mat4 projection);
	GObject* addModel(Model* model);
private: 
	std::vector<GObject *> children;
	glm::mat4 localProjection;
	Model* bruh;
};

#endif
