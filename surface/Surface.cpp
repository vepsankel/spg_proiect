#include "Surface.h"

m2::Surface::Surface()
{
	this->time = 0;
}

m2::Surface::Surface(std::shared_ptr<Wave> w)
{
	this->wave = w;
	this->time = 0;

	LoadShader();
}

void m2::Surface::FillRenderInfo(float dt, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	time += dt;

	for (int x = 0; x < SURF_DEF_VERT - 1; x++) {
		for (int z = 0; z < SURF_DEF_VERT - 1; z++) {
			int x_p = x + 1;
			int z_p = z + 1;

			float x_fl = (float)x / ((float)SURF_DEF_VERT - 1);
			float z_fl = (float)z / ((float)SURF_DEF_VERT - 1);

			float x_p_fl = (float)x_p / ((float)SURF_DEF_VERT - 1);
			float z_p_fl = (float)z_p / ((float)SURF_DEF_VERT - 1);

			int offset = 6 * 2 * (x * (SURF_DEF_VERT - 1) + z);

			// z
			// 0 1 
			// 2 3 x

			// 2 x & z
			vertices[offset + 0] = x_fl;
			vertices[offset + 1] = z_fl;

			// 0 x & z
			vertices[offset + 2] = x_fl;
			vertices[offset + 3] = z_p_fl;

			// 3 x & z
			vertices[offset + 4] = x_p_fl;
			vertices[offset + 5] = z_fl;

			// 0 x & z
			vertices[offset + 6] = x_fl;
			vertices[offset + 7] = z_p_fl;

			// 1 x & z
			vertices[offset + 8] = x_p_fl;
			vertices[offset + 9] = z_p_fl;

			// 3 x & z
			vertices[offset + 10] = x_p_fl;
			vertices[offset + 11] = z_fl;

			// TEX_COORDS
			texCoords[offset + 0] = x_fl;
			texCoords[offset + 1] = z_fl;
			texCoords[offset + 2] = x_fl;
			texCoords[offset + 3] = z_p_fl;
			texCoords[offset + 4] = x_p_fl;
			texCoords[offset + 5] = z_fl;
			texCoords[offset + 6] = x_fl;
			texCoords[offset + 7] = z_p_fl;
			texCoords[offset + 8] = x_p_fl;
			texCoords[offset + 9] = z_p_fl;
			texCoords[offset + 10] = x_p_fl;
			texCoords[offset + 11] = z_fl;
		}
	}
	
	// Create the VBOs
	GLuint vboPos, vboTexCoord;
	glGenBuffers(1, &vboPos);
	glGenBuffers(1, &vboTexCoord);

	// Bind the VBOs and fill them with the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vboPos);
	glBufferData(GL_ARRAY_BUFFER, (SURF_DEF_VERT - 1) * (SURF_DEF_VERT - 1) * 12 * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboTexCoord);
	glBufferData(GL_ARRAY_BUFFER, (SURF_DEF_VERT - 1) * (SURF_DEF_VERT - 1) * 12 * sizeof(float), texCoords, GL_STATIC_DRAW);

	// Create the VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Enable the vertex attrib arrays and specify the format and stride of the data
	glEnableVertexAttribArray(0);  // Vertex position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);  // Vertex tex coord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	// Draw the quad using glDrawArrays
	shader->Use();

	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wave->GetText());
	int texLocation = glGetUniformLocation(shader->program, "tex");
	glUniform1i(texLocation, 0);

	texLocation = glGetUniformLocation(shader->program, "time");
	glUniform1f(texLocation, time);

	glDrawArrays(GL_TRIANGLES, 0, 2*6*(SURF_DEF_VERT-1)*(SURF_DEF_VERT - 1));

	// Unbind the VAO
	glBindVertexArray(0);
}

void m2::Surface::LoadShader()
{
	std::string shaderPath = "/src/lab_m2/spg_proiect/surface/Surface_";

	char buff[200]; //create string buffer to hold path
	GetCurrentDir(buff, 200);
	std::string current_working_dir(buff);
	std::cout << current_working_dir;

	shader = std::make_unique<Shader>("DrawToText");
	shader->AddShader(current_working_dir + shaderPath + "VS.glsl", GL_VERTEX_SHADER);
	shader->AddShader(current_working_dir + shaderPath + "FS.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
}
