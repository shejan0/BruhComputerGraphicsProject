#include "GObject.h"
/*
 * GObject.cpp
 * GitHub link: https://github.com/shejan0/BruhComputerGraphicsProject
 * Authors:
 *    Shejan Shuza
 *    Gabriel Vidaurri
 *    Joseph Daau
 *    Tyler Everett
 *    Christopher Urista
 * Description: Meant to be the core Object element for our "engine", it stores properties about Models (which stores Shaders), BoundingBoxes, has parental hierarchy,and abstracts the transformations using easy to understand degrees and units. 
 *
 *
 *
 *
 */
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
/*#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
*/
Shader GObject::green;
GObject::GObject(glm::vec3 trans, glm::vec3 rotat, glm::vec3 scale, Model* m) {
	children = std::vector<GObject *>();
	model = m;
	position = trans;
	rotation = rotat;
	scales = scale;
	
	
	//fprintf(stderr, "%p Model m at GOObject: %p %p\n",this,m,model);
}


GObject* GObject::removeChild(GObject* child) { //remove from parent a child, useful before destroying the object.
	GObject* d=NULL;
	if (child != NULL) {
		for (int n = 0; n < children.size(); n++) {
			if (children[n] == child) {
				d = children[n];
				children.erase(children.begin() + n);
				break;
			}
		}
	}
	return d; //on failure, NULL is returned
}
GObject::~GObject() { //destroy all of the Object and all children
	for (int n = 0; n < children.size(); n++) {
		children.erase(children.begin() + n);
		delete children[n]; //run its destructor
	}
}
void GObject::addChild(GObject* newChild) { //add a child to this object
	children.push_back(newChild);
}
GObject* GObject::setModel(Model* mod) { //if a model is defined somewhere, and wants to be changed.
	model = mod;
	if (model != NULL) {
		createBoxState();
	}
	return this;
}
glm::vec3 GObject::setPosition(glm::vec3 pos) { //hard set position
	position = pos;
	return position;
}
glm::vec3 GObject::setRotation(glm::vec3 rot) { //hard set values
	rotation = rot;
	return rotation;
}
glm::vec3 GObject::setScale(glm::vec3 scal) { //hard set scale
	scales = scal;
	return scales;
}
glm::vec3 GObject::getPosition() {
	return position;
}
glm::vec3 GObject::getRotation() {
	return rotation;
}
glm::vec3 GObject::getScale() {
	return scales; 
}
void GObject::createBoxState() {
	if (box != NULL) {
		delete box;
	}
	if (model != NULL) {
	box = new BoundingBox(&GObject::green, model);
	}
	
}
void GObject::changeBoxState() { //adjust the boundingbox state
	boxOn = !boxOn;
	if (!GObject::green.isInitialized()) {
		GObject::green.InitializeFromFile(GREEN_VERT, GREEN_FRAG);
	}
	if (box == NULL) {
		createBoxState();
	}
	for (int n = 0; n < children.size(); n++) {
		children[n]->changeBoxState();
	}
}
glm::vec3 GObject::translate(glm::vec3 moveby) { //additively translate
	position += moveby;
	return position;
}

glm::vec3 GObject::rotate(glm::vec3 rotateby) { //additively rotate
	rotation += rotateby;
	return rotation;
}
glm::vec3 GObject::scale(glm::vec3 scaleby) { //additively scale
	scales += scaleby;
	return scales;
}
void GObject::useObjectMaterials(bool obj) { //set requirement to use MTL is existent.
	useMaterials = obj;
}
void GObject::draw(glm::mat4 worldProjection,glm::mat4 transform) { //draw this object (and boundingbox if active)
	//fprintf(stderr, "%p is rendering %p\n", this,model);
	
	glm::mat4 transforml = transform * glm::translate(position)*glm::rotate(rotation[0],1.0f,0.0f,0.0f)*glm::rotate(rotation[1],0.0f,1.0f,0.0f)* glm::rotate(rotation[2], 0.0f, 0.0f, 1.0f)*glm::scale(scales);
	//std::cerr << glm::to_string(glm::scale(scales)) << std::endl;
	if (model != NULL) {
		model->render(transforml, worldProjection, useMaterials);
	}
	if (boxOn) {
		if (box != NULL) {
			box->render(transforml, worldProjection);
		}
	}
	if (children.size() > 0) {
		for (size_t n = 0; n < children.size(); n++) {
			children[n]->draw(worldProjection, transforml);
		}
	}
}
	
