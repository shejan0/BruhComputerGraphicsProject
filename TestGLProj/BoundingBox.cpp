#include "BoundingBox.h"
/*
 * BoundingBox.cpp
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
BoundingBox::BoundingBox(Shader* shader, Model* model):Model(shader){
	BoundingBox::m_shader = shader;
	inmodel = model;
	
	xmax = std::numeric_limits<float>::min();
	//std::cout << std::numeric_limits<float>::min() << std::endl;
	ymax = std::numeric_limits<float>::min();
	zmax = std::numeric_limits<float>::min();
	xmin = std::numeric_limits<float>::max();
	ymin = std::numeric_limits<float>::max();
	zmin = std::numeric_limits<float>::max();
	for (int n = 0; n < 8; n++) {
		BoundingBox::m_VBO.push_back(0); //making space inside of the buffer for the vertices
		BoundingBox::m_NBO.push_back(0); //making space inside of the buffer for the normals
		BoundingBox::m_IBO.push_back(0); //making space inside of the buffer for the indices
		glGenBuffers(1, &BoundingBox::m_VBO[n]); // generate a 'name' for the VBO
		glGenBuffers(1, &BoundingBox::m_NBO[n]); // generate a 'name' for the NBO
		glGenBuffers(1, &BoundingBox::m_IBO[n]); // generate a 'name' for the IBO
	}
	/*if (!FindBoundaries()) {
		std::cerr << "Unable to find the boundaries of box" << std::endl;
	}*/
	updateBuffers();
}
void BoundingBox::render(glm::mat4 ModelView, glm::mat4 Projection) {
	BoundingBox::m_shader->Activate();
	BoundingBox::m_shader->SetUniform("Projection", Projection);
	BoundingBox::m_shader->SetUniform("ModelView", ModelView);
	BoundingBox::m_shader->SetUniform("lightPosition", glm::vec4(1.0, 0.0, 0.0, 1.0));
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]); //Bind VBO, there is only one
	glEnableVertexAttribArray((*BoundingBox::m_shader)["vertexPosition"]);
	glVertexAttribPointer((*BoundingBox::m_shader)["vertexPosition"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO[0]); // Bind IBO, there is only one
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT,0); //draw lines from indices
	BoundingBox::m_shader->DeActivate();
	checkError("render");
}
bool BoundingBox::FindBoundaries() {
	try {
		//printf("x%f x%f y%f y%f z%f z%f\n", xmin, xmax, ymin, ymax, zmin, zmax);
		//std::cout << xmin << xmax << ymin << ymax << zmin << zmax << std::endl;
		for (unsigned int n = 0; n < BoundingBox::inmodel->shapes[0].mesh.positions.size(); n += 3) {
			float curx = BoundingBox::inmodel->shapes[0].mesh.positions[n];
			float cury = BoundingBox::inmodel->shapes[0].mesh.positions[n + 1];
			float curz = BoundingBox::inmodel->shapes[0].mesh.positions[n + 2];

			if (curx > xmax) {
				xmax = curx;
			}
			if (curx < xmin) {
				xmin = curx;
			}
			if (cury > ymax) {
				ymax = cury;
			}
			if (cury < ymin) {
				ymin = cury;
			}
			if (curz > zmax) {
				zmax = curz;
			}
			if (curz < zmin) {
				zmin = curz;
			}
		}
		printf("x%f x%f y%f y%f z%f z%f\n", xmin, xmax, ymin, ymax, zmin, zmax);
		return true;
	}
	catch (...) {
		return false;
	}
}

void BoundingBox::updateBuffers() {
	if (!FindBoundaries()) {
		std::cerr << "Unable to find the boundaries of box" << std::endl;
	}
	//Construct vertex buffer
	//Bottom square
	vertex3d[0][0] = xmin;
	vertex3d[0][1] = ymin;
	vertex3d[0][2] = zmin;

	vertex3d[1][0] = xmax;
	vertex3d[1][1] = ymin;
	vertex3d[1][2] = zmin;

	vertex3d[2][0] = xmax;
	vertex3d[2][1] = ymin;
	vertex3d[2][2] = zmax;

	vertex3d[3][0] = xmin;
	vertex3d[3][1] = ymin;
	vertex3d[3][2] = zmax;

	//Top Square
	vertex3d[4][0] = xmin;
	vertex3d[4][1] = ymax;
	vertex3d[4][2] = zmin;

	vertex3d[5][0] = xmax;
	vertex3d[5][1] = ymax;
	vertex3d[5][2] = zmin;

	vertex3d[6][0] = xmax;
	vertex3d[6][1] = ymax;
	vertex3d[6][2] = zmax;

	vertex3d[7][0] = xmin;
	vertex3d[7][1] = ymax;
	vertex3d[7][2] = zmax;
	//Finished construction.

	//buffer vertices into vertex buffer object.
	glBindBuffer(GL_ARRAY_BUFFER, BoundingBox::m_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 8 * sizeof(float), &vertex3d[0][0], GL_STATIC_DRAW);
	//buffer indices into index buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BoundingBox::m_IBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * 2 * sizeof(GLuint), index, GL_STATIC_DRAW);
	checkError("updateBuffers");
}
