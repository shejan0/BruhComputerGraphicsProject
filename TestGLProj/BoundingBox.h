#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
/*
 * BoundingBox.h
 * GitHub link: https://github.com/shejan0/BruhComputerGraphicsProject
 * Authors:
 *    Shejan Shuza
 *    Gabriel Vidaurri
 *    Joseph Daau
 *    Tyler Everett
 *    Christopher Urista
 * Description: A subclass of Model that using another model pointer, reads the vertices and creates an appropriate bounding box to the passed Model
 *
 *
 *
 *
 */
#include "Model.h"
#include "Error.h"
class BoundingBox : public Model{
public:
	BoundingBox(Shader* shader, Model* model);
	void updateBuffers();
	void render(glm::mat4 ModelView, glm::mat4 Projection);
	bool FindBoundaries();
	friend class Model; //this allows BoundingBox to see private data in Models, primarily ones passed in.
protected:
	Model* inmodel;
	float vertex3d[8][3]; //there are 3 values for all 8 vertices (24 points)
	const GLuint index[24] = { 0,1,1,2,2,3,3,0,0,4,1,5,2,6,3,7,4,5,5,6,6,7,7,4 }; //preconstruct indices, always constant for box
	float xmin, xmax, ymin, ymax, zmin, zmax;
	
};
#endif