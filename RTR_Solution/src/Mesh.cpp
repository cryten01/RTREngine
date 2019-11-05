#include "Mesh.h"


Mesh::Mesh(glm::mat4 modelMatrix, MeshData data, std::shared_ptr<Material> material)
	: _elementCount(data.indices.size()), _modelMatrix(modelMatrix), _material(material)
{
	// create VAO
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);


	// Create positions VBO
	glGenBuffers(1, &_vboPositions);
	glBindBuffer(GL_ARRAY_BUFFER, _vboPositions);
	glBufferData(GL_ARRAY_BUFFER, data.positions.size() * sizeof(glm::vec3), data.positions.data(), GL_STATIC_DRAW);
	// Bind positions to location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Create normals VBO
	glGenBuffers(1, &_vboNormals);
	glBindBuffer(GL_ARRAY_BUFFER, _vboNormals);
	glBufferData(GL_ARRAY_BUFFER, data.normals.size() * sizeof(glm::vec3), data.normals.data(), GL_STATIC_DRAW);
	// Bind normals to location 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Create uvs VBO
	glGenBuffers(1, &_vboUV);
	glBindBuffer(GL_ARRAY_BUFFER, _vboUV);
	glBufferData(GL_ARRAY_BUFFER, data.uv.size() * sizeof(glm::vec2), data.uv.data(), GL_STATIC_DRAW);
	// Bind uvs to location 2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Create and bind indices VBO
	glGenBuffers(1, &_vboIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(unsigned int), data.indices.data(), GL_STATIC_DRAW);


	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// TODO: why transformMatrix can't be empty?
	_transformMatrix = glm::mat4(1.0f);
}


Mesh::~Mesh()
{
	glDeleteBuffers(1, &_vboPositions);
	glDeleteBuffers(1, &_vboNormals);
	glDeleteBuffers(1, &_vboUV);
	glDeleteBuffers(1, &_vboIndices);
	glDeleteVertexArrays(1, &_vao);
}

void Mesh::render()
{
	// Apply transformations
	glm::mat4 accumModel = _transformMatrix * _modelMatrix;

	// Select shader
	Shader* shader = _material->getShader();
	shader->use();

	// Set uniforms
	shader->setUniform("modelMatrix", accumModel);
	shader->setUniform("normalMatrix", glm::mat3(glm::transpose(glm::inverse(accumModel)))); // Fixes non uniform scaling issues, done on CPU for performance reasons
	_material->setUniforms();
	
	// Bind _vao
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _elementCount, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void Mesh::transform(glm::mat4 transformation)
{
	_modelMatrix = transformation * _modelMatrix;
}

std::shared_ptr<Material> Mesh::getMaterial()
{
	return this->_material;
}

void Mesh::setTransformMatrix(glm::mat4 transformMatrix)
{
	_transformMatrix = transformMatrix;
}

void Mesh::resetModelMatrix()
{
	_modelMatrix = glm::mat4(1);
}

float Mesh::normalizeUV(float value, float min, float max)
{
	return (value - min) / (max - min);
}

MeshData Mesh::createCubeGeometry(float width, float height, float depth)
{
	MeshData data;


	data.positions = {
		// front
		glm::vec3(-width / 2.0f, -height / 2.0f,  depth / 2.0f),
		glm::vec3(width / 2.0f, -height / 2.0f,  depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f,  depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f,  depth / 2.0f),
		// back
		glm::vec3(width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		glm::vec3(-width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f,  -depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f,  -depth / 2.0f),
		// right
		glm::vec3(width / 2.0f, -height / 2.0f,  depth / 2.0f),
		glm::vec3(width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f,  -depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f,  depth / 2.0f),
		// left
		glm::vec3(-width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		glm::vec3(-width / 2.0f, -height / 2.0f,  depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f,  depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f,  -depth / 2.0f),
		// top
		glm::vec3(-width / 2.0f, height / 2.0f,  -depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f,  depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f,  depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f,  -depth / 2.0f),
		// bottom
		glm::vec3(-width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		glm::vec3(width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		glm::vec3(width / 2.0f, -height / 2.0f,  depth / 2.0f),
		glm::vec3(-width / 2.0f, -height / 2.0f,  depth / 2.0f)
	};

	data.normals = {
		// front
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		// back
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		// right
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		// left
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		// top
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		// bottom
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0)
	};

	data.uv = {
		// front
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1),
		// back
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1),
		// right
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1),
		// left
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1),
		// top
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1),
		// bottom
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1)
	};


	data.indices = {
		// front
		0, 1, 2,
		2, 3, 0,
		// back
		4, 5, 6,
		6, 7, 4,
		// right
		8, 9, 10,
		10, 11, 8,
		// left
		12, 13, 14,
		14, 15, 12,
		// top
		16, 17, 18,
		18, 19, 16,
		// bottom
		20, 21, 22,
		22, 23, 20
	};

	return std::move(data);
}


MeshData Mesh::createSphereGeometry(unsigned int longitudeSegments, unsigned int latitudeSegments, float radius)
{
	MeshData data;

	// Constant (2 replaced by 2n)
	int poles = 2 * longitudeSegments;

	// Poles
	for (unsigned int j = 0; j < longitudeSegments; j++) {
		float horizontalAngle = float(j) * 2.0f * glm::pi<float>() / float(longitudeSegments);
		GLfloat U = normalizeUV(horizontalAngle, 0, 2.0f * glm::pi<float>());

		data.positions.push_back(glm::vec3(0.0f, radius, 0.0f)); // even
		data.positions.push_back(glm::vec3(0.0f, -radius, 0.0f)); // odd

		data.uv.push_back(glm::vec2(U, 0));
		data.uv.push_back(glm::vec2(U, 1));

		data.normals.push_back(glm::vec3(0.0f, radius, 0.0f));
		data.normals.push_back(glm::vec3(0.0f, -radius, 0.0f));
	}

	// Index first and last ring
	for (unsigned int j = 0; j < longitudeSegments; j++) {

		data.indices.push_back(j * 2);			 // 0,2,4,6 
		data.indices.push_back(poles + (j + 1)); // 9,10,11,8(12)
		data.indices.push_back(poles + j);		 // 8,9,10,11

		int bodyVerts = (latitudeSegments - 2)*(longitudeSegments + 1);
		int step = poles + bodyVerts;

		data.indices.push_back(step + j);		// 13,14,15,16
		data.indices.push_back(step + (j + 1)); // 14,15,16,13(17)
		data.indices.push_back((j * 2) + 1);	// 1,3,5,7
	}


	// vertices and rings 
	for (unsigned int i = 1; i < latitudeSegments; i++) {
		float verticalAngle = float(i) * glm::pi<float>() / float(latitudeSegments);

		// Sphere Fix (needs +1 vertex/ring)
		for (unsigned int j = 0; j < longitudeSegments + 1; j++) {
			float horizontalAngle = float(j) * 2.0f * glm::pi<float>() / float(longitudeSegments);

			glm::vec3 position = glm::vec3(
				radius * glm::sin(verticalAngle) * glm::cos(horizontalAngle),
				radius * glm::cos(verticalAngle),
				radius * glm::sin(verticalAngle) * glm::sin(horizontalAngle)
			);

			GLfloat U = normalizeUV(horizontalAngle, 0, 2.0f * glm::pi<float>());
			GLfloat V = normalizeUV(verticalAngle, 0, glm::pi<float>());

			// Position, Normal, UV
			data.positions.push_back(position);
			data.normals.push_back(glm::normalize(position));
			data.uv.push_back(glm::vec2(U, V));
		}


		for (unsigned int j = 0; j < longitudeSegments; j++) {

			if (i == 1) continue; // -> skips indexing if i == 1

			int lowerRing = (i - 1) * (longitudeSegments + 1); // Must be i-1 in order to avoid entering the last ring
			int upperRing = (i - 2) * (longitudeSegments + 1);

			data.indices.push_back(poles + lowerRing + j);
			data.indices.push_back(poles + upperRing + j + 1);
			data.indices.push_back(poles + lowerRing + j + 1);

			data.indices.push_back(poles + upperRing + j + 1);
			data.indices.push_back(poles + lowerRing + j);
			data.indices.push_back(poles + upperRing + j);
		}
	}

	return std::move(data);
}