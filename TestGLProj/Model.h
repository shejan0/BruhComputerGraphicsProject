#ifndef MODEL_H
#define MODEL_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include "tiny_obj_loader.h"
#include <string>
#include "Shader.h"

/*This class represents a mesh - a collection of triangles. It stores a list of vertices, which are interpreted by openGL as every 3 vertices makes a triangle.*/
class Model{

public:

	/*This struct represents a vertex in 3D space*/
	struct Vertex 
	{
		Vertex() : Position(0.0f,0.0f,0.0f), Normal(0.0f,0.0f,1.0f){} // default values
		glm::vec3 Position; /// x,y,z 
		glm::vec3 Normal; // determines which way the vertex is 'facing'

	};

	Model(Shader *shader,  const char* filename, const char * materialPath = NULL); //load a model
	~Model(void){} // default destructor
	void render(glm::mat4 ModelView, glm::mat4 Projection); // render the model
	
private:
	Shader *m_shader; // shader program
	std::vector<tinyobj::shape_t> shapes; //a list of meshes and their respective materials
	std::vector<GLuint> m_VBO;// vertex buffer IDs, each corresponding to a shape
	std::vector<GLuint> m_NBO;// normal buffer IDs, each corresponding to a shape
	std::vector<GLuint> m_IBO;// index buffer IDs, each corresponding to a shape

	void updateBuffers(); //initialize your VBO and update when triangles are added
	
	




	


};

#endif

