#pragma once

#include "../Definitions.h"

#include "../wave/Wave.h"

namespace m2 {
	struct SurfaceSpecifier {
		std::shared_ptr<float[]> vertices, texCoords;

		GLuint vboPos, vboTexCoord;
		GLuint vao;
	};

	class Surface
	{
	public:
		Surface(int size = SURF_DEF_VERT);
		Surface(std::shared_ptr<Wave>, int size = SURF_DEF_VERT);

		void FillRenderInfo(float dt, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

		//void FillRenderInfo(float, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	private:
		int size;

		float time;
		std::shared_ptr<Wave> wave;

		float texCoords[2 * 6 * (SURF_DEF_VERT - 1) * (SURF_DEF_VERT - 1)];
		float vertices[2 * 6 * (SURF_DEF_VERT - 1) * (SURF_DEF_VERT - 1)];

		static std::unique_ptr<Shader> shader;
		static std::unordered_map<int, SurfaceSpecifier> sizeToSurface;
		static bool isShaderLoaded;
		void GenerateBuffersForSize(int size);

		void LoadShader();
	};
}

