#pragma once

#include "../Definitions.h"

#include "../wave/Wave.h"

namespace m2 {
	class Surface
	{
	public:
		Surface();
		Surface(std::shared_ptr<Wave>);

		void FillRenderInfo(float, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	private:
		int size;

		float time;
		std::shared_ptr<Wave> wave;

		float texCoords[2 * 6 * (SURF_DEF_VERT - 1) * (SURF_DEF_VERT - 1)];
		float vertices[2 * 6 * (SURF_DEF_VERT - 1) * (SURF_DEF_VERT - 1)];

		std::unique_ptr<Shader> shader;
		void LoadShader();
	};
}

