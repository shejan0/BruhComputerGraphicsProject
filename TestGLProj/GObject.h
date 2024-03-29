#ifndef GOBJECT_H
#define GOBJECT_H
/*
 * GObject.h
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
#define GREEN_VERT "shaders/phong.vert"
#define GREEN_FRAG "shaders/green.frag"

#include "Model.h"
#include "BoundingBox.h"
#include "Error.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>

class GObject {
public:
	GObject(glm::vec3 trans, glm::vec3 rotat, glm::vec3 scale = glm::vec3(1, 1, 1), Model* m = NULL); //real constructor, model is optional
	GObject(float trans[3]) : GObject(glm::vec3(trans[0],trans[1],trans[2]),glm::vec3(),glm::vec3(1,1,1)) {} //takes no model and 3 positions
	//takes 3 float arrays of 3 size and construct the scales, model is optional
	GObject(float trans[3], float rotat[3], float scale[3], Model *m=NULL) : GObject(glm::vec3(trans[0],trans[1],trans[2]),glm::vec3(rotat[0],rotat[1],rotat[2]),glm::vec3(scale[0],scale[1],scale[2]),m) {}
	//GObject(glm::mat4 transform = glm::mat4(), Model* m=NULL);
	GObject(Model* m=NULL) : GObject(glm::vec3(),glm::vec3(),glm::vec3(1,1,1), m) {} //only takes a model, scales to 1 and places at origin
	//Take the model parameters, constructs with passed constructor that only takes a model.
	GObject(Shader* shader, const char* filename/*, const char* materialPath = NULL*/) : GObject(new Model(shader, filename/*, materialPath*/)) {}
	glm::vec3 translate(glm::vec3 moveby);
	glm::vec3 rotate(glm::vec3 rotateby);
	glm::vec3 scale(glm::vec3 scaleby);
	glm::vec3 setPosition(glm::vec3 pos);
	glm::vec3 setRotation(glm::vec3 rot);
	glm::vec3 setScale(glm::vec3 scal);
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	void draw(glm::mat4 worldProjection=glm::mat4(),glm::mat4 transform=glm::mat4()); //default to identity
	~GObject(); //default deconstruct children
	GObject* removeChild(GObject* child = NULL);
	//GObject* createChild();
	void addChild(GObject* newChild);
	GObject* setModel(Model* model);
	void changeBoxState();
	void useObjectMaterials(bool obj);
private: 
	void createBoxState();
	std::vector<GObject *> children;
	glm::vec3 position, rotation, scales = glm::vec3(1,1,1);
	Model* model;
	BoundingBox* box=NULL;
	static Shader green;  
	bool boxOn=false;
	bool useMaterials = false;
	
};

#endif
