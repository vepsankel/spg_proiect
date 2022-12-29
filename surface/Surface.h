#pragma once

#include "../Definitions.h"

#include "../wave/Wave.h"

#define SURFACE_DIR_UNITY glm::mat3(1)
#define SURFACE_DIR_ROTATE(a) glm::mat3(cos(a), -sin(a), 0, sin(a), cos(a), 0, 0, 0, 1)

#define SURF_SPEED_DOWN(s) glm::mat3(0,0,0,0,0,s,0,0,1)
#define SURF_SPEED_RIGHT(s) glm::mat3(0,0,s,0,0,0,0,0,1)

namespace m2 {
	struct SurfaceSpecifier {
		std::shared_ptr<float[]> vertices, texCoords;

		GLuint vboPos, vboTexCoord;
		GLuint vao;
	};

	struct WaveDetails {
		WaveDetails(GLuint texture, glm::mat3 localTexCoordToTexCoord, glm::mat3 texCoordSpeed);

		GLuint texture;
		glm::mat3 localTexCoordToTexCoord;
		glm::mat3 texCoordSpeed;
	};

	class Surface
	{
	public:
		Surface(int size = SURF_DEF_VERT);
		Surface(std::vector<std::string> names, int size = SURF_DEF_VERT);

		void FillRenderInfo(float dt, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

		static void addWave(std::string name, std::shared_ptr<WaveDetails> wave);
		static void removeWave(std::string name);
		static bool hasWave(std::string name);

	private:
		int size;
		float time;
		std::vector<std::string> names, validNames;
		void GenerateValidNames();
		
		//std::shared_ptr<Wave> wave;
		static std::unordered_map<std::string, std::shared_ptr<WaveDetails>> waves;

		static bool isShaderLoaded;
		static std::unique_ptr<Shader> shader;
		void LoadShader();

		static std::unordered_map<int, SurfaceSpecifier> sizeToSurface;		
		void GenerateBuffersForSize(int size);
	};
}

