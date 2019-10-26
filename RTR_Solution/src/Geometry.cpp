#include "Geometry.h"


Geometry::Geometry(glm::mat4 modelMatrix, GeometryData& data, std::shared_ptr<Material> material)
	: _elements(data.indices.size()), _modelMatrix(modelMatrix), _material(material)
{
	// create VAO
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);


	// create positions VBO
	glGenBuffers(1, &_vboPositions);
	glBindBuffer(GL_ARRAY_BUFFER, _vboPositions);
	glBufferData(GL_ARRAY_BUFFER, data.positions.size() * sizeof(glm::vec3), data.positions.data(), GL_STATIC_DRAW);
	// bind positions to location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	// create normals VBO
	glGenBuffers(1, &_vboNormals);
	glBindBuffer(GL_ARRAY_BUFFER, _vboNormals);
	glBufferData(GL_ARRAY_BUFFER, data.normals.size() * sizeof(glm::vec3), data.normals.data(), GL_STATIC_DRAW);
	// bind normals to location 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);


	// create UVs VBO
	glGenBuffers(1, &_vboUV);
	glBindBuffer(GL_ARRAY_BUFFER, _vboUV);
	glBufferData(GL_ARRAY_BUFFER, data.uv.size() * sizeof(glm::vec2), data.uv.data(), GL_STATIC_DRAW);
	// bind positions to location 2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);


	// create and bind indices VBO
	glGenBuffers(1, &_vboIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(unsigned int), data.indices.data(), GL_STATIC_DRAW);


	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// TODO: why transformMatrix can't be empty?
	_transformMatrix = glm::mat4(1.0f);
}


Geometry::~Geometry()
{
	glDeleteBuffers(1, &_vboPositions);
	glDeleteBuffers(1, &_vboNormals);
	glDeleteBuffers(1, &_vboIndices);
	glDeleteVertexArrays(1, &_vao);
}

void Geometry::draw()
{
	glm::mat4 accumModel = _transformMatrix * _modelMatrix;
	Shader* shader = _material->getShader();
	shader->use();

	shader->setUniform("modelMatrix", accumModel);
	_material->setUniforms();

	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _elements, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void Geometry::transform(glm::mat4 transformation)
{
	_modelMatrix = transformation * _modelMatrix;
}

void Geometry::setTransformMatrix(glm::mat4 transformMatrix)
{
	_transformMatrix = transformMatrix;
}

void Geometry::resetModelMatrix()
{
	_modelMatrix = glm::mat4(1);
}

float Geometry::normalizeUV(float value, float min, float max)
{
	return (value - min) / (max - min);
}


GeometryData Geometry::createSphereGeometry(unsigned int longitudeSegments, unsigned int latitudeSegments, float radius)
{
	GeometryData data;

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