#include "Surface.h"

m2::WaveDetails::WaveDetails(GLuint texture, glm::mat3 localTexCoordToTexCoord, glm::mat3 texCoordSpeed) {
	this->texture = texture;
	this->localTexCoordToTexCoord = localTexCoordToTexCoord;
	this->texCoordSpeed = texCoordSpeed;
}