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



	// create and bind indices VBO
	glGenBuffers(1, &_vboIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(unsigned int), data.indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}


Geometry::~Geometry()
{
	glDeleteBuffers(1, &_vboPositions);
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


GeometryData Geometry::createSphereGeometry(unsigned int longitudeSegments, unsigned int latitudeSegments, float radius)
{
	GeometryData data;

	data.positions.push_back(glm::vec3(0.0f, radius, 0.0f));
	data.positions.push_back(glm::vec3(0.0f, -radius, 0.0f));


	// first and last ring
	for (unsigned int j = 0; j < longitudeSegments; j++) {
		data.indices.push_back(0);
		data.indices.push_back(j == longitudeSegments - 1 ? 2 : (j + 3));
		data.indices.push_back(2 + j);

		data.indices.push_back(2 + (latitudeSegments - 2)*longitudeSegments + j);
		data.indices.push_back(j == longitudeSegments - 1 ? 2 + (latitudeSegments - 2)*longitudeSegments : 2 + (latitudeSegments - 2)*longitudeSegments + j + 1);
		data.indices.push_back(1);
	}

	// vertices and rings
	for (unsigned int i = 1; i < latitudeSegments; i++) {
		float verticalAngle = float(i) * glm::pi<float>() / float(latitudeSegments);
		for (unsigned int j = 0; j < longitudeSegments; j++) {
			float horizontalAngle = float(j) * 2.0f * glm::pi<float>() / float(longitudeSegments);
			glm::vec3 position = glm::vec3(
				radius * glm::sin(verticalAngle) * glm::cos(horizontalAngle),
				radius * glm::cos(verticalAngle),
				radius * glm::sin(verticalAngle) * glm::sin(horizontalAngle)
			);
			data.positions.push_back(position);


			if (i == 1) continue;

			data.indices.push_back(2 + (i - 1)*longitudeSegments + j);
			data.indices.push_back(j == longitudeSegments - 1 ? 2 + (i - 2)*longitudeSegments : 2 + (i - 2)*longitudeSegments + j + 1);
			data.indices.push_back(j == longitudeSegments - 1 ? 2 + (i - 1)*longitudeSegments : 2 + (i - 1)*longitudeSegments + j + 1);

			data.indices.push_back(j == longitudeSegments - 1 ? 2 + (i - 2)*longitudeSegments : 2 + (i - 2)*longitudeSegments + j + 1);
			data.indices.push_back(2 + (i - 1)*longitudeSegments + j);
			data.indices.push_back(2 + (i - 2)*longitudeSegments + j);
		}
	}


	return std::move(data);
}