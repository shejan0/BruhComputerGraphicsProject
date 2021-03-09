#include "GObject.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>



GObject::GObject(float trans[3]) {
	glm::mat4 projection = glm::translate(trans[0], trans[1], trans[2]);
	GObject::GObject(projection);
}

GObject::GObject(float trans[3], float rotat[3], float scale[3]) {
	glm::mat4 projection = glm::scale(scale[0], scale[1], scale[2]) * glm::rotate(rotat[0], 1.0f, 0.0f, 0.0f)* glm::rotate(rotat[1], 0.0f, 1.0f, 0.0f)* glm::rotate(rotat[2], 0.0f, 0.0f, 1.0f) *glm::translate(trans[0], trans[1], trans[2]);
	GObject::GObject(projection);
}

GObject::GObject(glm::mat4 projection, Model* m) {
	children = std::vector<GObject *>();
	localProjection = projection;
	bruh = m;
}

/*GObject* GObject::createChild() { 
	GObject *b = new GObject(); //check if this gets destroyed during stack removal
	children.push_back(b);
	return b;
}*/

GObject* GObject::removeChild(GObject* child) {
	GObject* d;
	if (child != NULL) {
		for (int n = 0; n < children.size(); n++) {
			if (children[n] == child) {
				d = children[n];
				children.erase(children.begin() + n);
				break;
			}
		}
	}
	return d;
}
GObject::~GObject() { //destroy all of the Object and all children
	for (int n = 0; n < children.size(); n++) {
		children.erase(children.begin() + n);
		delete children[n]; //run its constructor
	}
}
void GObject::addChild(GObject* newChild) {
	children.push_back(newChild);
}
GObject* GObject::addModel(Model* model) {
	bruh = model;
	return this;
}
void GObject::draw(glm::mat4 modelView,glm::mat4 worldProjection) {
	if (bruh != NULL) {
		bruh->render(modelView, localProjection * worldProjection);
	}
	for (int n = 0; n < children.size(); n++) {
		children[n]->draw(modelView, worldProjection);
	}
}
