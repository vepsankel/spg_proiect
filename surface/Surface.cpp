#include "Surface.h"

bool m2::Surface::isShaderLoaded = false;
std::unordered_map<int, m2::SurfaceSpecifier> m2::Surface::sizeToSurface;
std::unique_ptr<Shader> m2::Surface::shader;
std::unordered_map<std::string, std::shared_ptr<m2::WaveDetails>> m2::Surface::waves;

m2::Surface::Surface(int size)
{
	this->time = 0;
	this->size = size;
	LoadShader();
	GenerateBuffersForSize(size);
}

m2::Surface::Surface(std::vector<std::string> names, int size) : Surface(size)
{
	this->names = names;
	this->validNames = names; // TODO
}

void m2::Surface::FillRenderInfo(float dt, const glm::mat4 & modelMatrix, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix)
{
	time += dt;
	
	glBindVertexArray(sizeToSurface[size].vao);
	shader->Use();

	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int loc;

	std::vector<glm::mat3> texCoordPositions, texCoordSpeeds;
	std::vector<GLuint> textures;

	texCoordPositions.clear();
	texCoordSpeeds.clear();
	textures.clear();

	int i = 0;
	for (std::string name : validNames) {
		std::shared_ptr<WaveDetails> wd = waves[name];

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, wd->texture);

		textures.push_back(i);
		texCoordPositions.push_back(wd->localTexCoordToTexCoord);
		texCoordSpeeds.push_back(wd->texCoordSpeed);

		
		std::string tex_name = "texs[" + std::to_string(i) + "]";
		loc = glGetUniformLocation(shader->program, tex_name.c_str());
		glUniform1i(loc, i);

		i++;
	}

	loc = glGetUniformLocation(shader->program, "texCoordPositions");
	glUniformMatrix3fv(loc, validNames.size(), GL_TRUE, (GLfloat*)&texCoordPositions[0]);

	loc = glGetUniformLocation(shader->program, "texCoordSpeeds");
	glUniformMatrix3fv(loc, validNames.size(), GL_TRUE, (GLfloat*)&texCoordSpeeds[0]);

	loc = glGetUniformLocation(shader->program, "wave_num");
	glUniform1i(loc, validNames.size());

	loc = glGetUniformLocation(shader->program, "time");
	glUniform1f(loc, time);

	glDrawArrays(GL_TRIANGLES, 0, 2 * 6 * (size - 1) * (size- 1));
	glBindVertexArray(0);
}

void m2::Surface::GenerateBuffersForSize(int size)
{
	if (sizeToSurface.find(size) == sizeToSurface.end()) {
		// NOT PRESENT
		std::cout << "Generating Buffers for size = " << size << "\n";
			
		SurfaceSpecifier ss;
		
		std::shared_ptr<float[]> vertices(new float[(size - 1) * (size - 1) * 12]);
		std::shared_ptr<float[]> texCoords(new float[(size - 1) * (size - 1) * 12]);

		ss.vertices = vertices;
		ss.texCoords = texCoords;

		
		for (int x = 0; x < size - 1; x++) {
			for (int z = 0; z < size - 1; z++) {
				int x_p = x + 1;
				int z_p = z + 1;

				float x_fl = (float)x / ((float)size - 1);
				float z_fl = (float)z / ((float)size - 1);

				float x_p_fl = (float)x_p / ((float)size - 1);
				float z_p_fl = (float)z_p / ((float)size - 1);

				int offset = 6 * 2 * (x * (size - 1) + z);

				// z
				// 0 1 
				// 2 3 x

				// 2 x & z
				ss.vertices[offset + 0] = x_fl;
				ss.vertices[offset + 1] = z_fl;

				// 0 x & z
				ss.vertices[offset + 2] = x_fl;
				ss.vertices[offset + 3] = z_p_fl;

				// 3 x & z
				ss.vertices[offset + 4] = x_p_fl;
				ss.vertices[offset + 5] = z_fl;

				// 0 x & z
				ss.vertices[offset + 6] = x_fl;
				ss.vertices[offset + 7] = z_p_fl;

				// 1 x & z
				ss.vertices[offset + 8] = x_p_fl;
				ss.vertices[offset + 9] = z_p_fl;

				// 3 x & z
				ss.vertices[offset + 10] = x_p_fl;
				ss.vertices[offset + 11] = z_fl;

				// TEX_COORDS
				ss.texCoords[offset + 0] = x_fl;
				ss.texCoords[offset + 1] = z_fl;
				ss.texCoords[offset + 2] = x_fl;
				ss.texCoords[offset + 3] = z_p_fl;
				ss.texCoords[offset + 4] = x_p_fl;
				ss.texCoords[offset + 5] = z_fl;
				ss.texCoords[offset + 6] = x_fl;
				ss.texCoords[offset + 7] = z_p_fl;
				ss.texCoords[offset + 8] = x_p_fl;
				ss.texCoords[offset + 9] = z_p_fl;
				ss.texCoords[offset + 10] = x_p_fl;
				ss.texCoords[offset + 11] = z_fl;
			}
		}
		
		glGenBuffers(1, &ss.vboPos);
		glGenBuffers(1, &ss.vboTexCoord);

		// Bind the VBOs and fill them with the vertex data
		glBindBuffer(GL_ARRAY_BUFFER, ss.vboPos);
		glBufferData(GL_ARRAY_BUFFER, (size - 1) * (size - 1) * 12 * sizeof(float), (const void *) &ss.vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, ss.vboTexCoord);
		glBufferData(GL_ARRAY_BUFFER, (size - 1) * (size - 1) * 12 * sizeof(float), (const void *) &ss.texCoords[0], GL_STATIC_DRAW);

		// Create the VAO
		glGenVertexArrays(1, &ss.vao);
		glBindVertexArray(ss.vao);

		// Enable the vertex attrib arrays and specify the format and stride of the data
		glEnableVertexAttribArray(0);  // Vertex position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);  // Vertex tex coord
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		sizeToSurface[size] = ss;
	}
}

void m2::Surface::LoadShader()
{
	if (isShaderLoaded)
		return;

	std::string shaderPath = "/src/lab_m2/spg_proiect/surface/Surface_";

	char buff[200]; //create string buffer to hold path
	GetCurrentDir(buff, 200);
	std::string current_working_dir(buff);
	std::cout << current_working_dir;

	shader = std::make_unique<Shader>("DrawToText");
	shader->AddShader(current_working_dir + shaderPath + "VS.glsl", GL_VERTEX_SHADER);
	shader->AddShader(current_working_dir + shaderPath + "FS.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();

	isShaderLoaded = true;
}

void m2::Surface::GenerateValidNames() {
	validNames.clear();

	for (std::string name : names) {
		if (Surface::hasWave(name))
			validNames.push_back(name);
	}
}

void m2::Surface::addWave(std::string name, std::shared_ptr<WaveDetails> wave) {
	waves[name] = wave;

	//GenerateValidNames(); TODO
}

void m2::Surface::removeWave(std::string name) {
	waves.erase(name);

	//GenerateValidNames(); TODO
}

bool m2::Surface::hasWave(std::string name) {
	if (waves.find(name) == waves.end())
		return false;
	return true;
}