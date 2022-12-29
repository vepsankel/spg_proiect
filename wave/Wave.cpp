#include "Wave.h"

using namespace m2;

m2::Wave::Wave(int precision) {
	bufferOk = false;
	textOk = false;
	shaderOK = false;

	generator.Calculate();

	LoadText();
	LoadFramebuffer();
	LoadShader();

	if (precision == 0)
		this->precision = generator.GetPeriod();
	else
		this->precision = precision;
}

void Wave::SetFourier(Fourier f)
{
	this->generator = f;
}

void Wave::FillTexture() {
	int retVal;

	shader->Use();

	float vertices[] = {
	  -1.0f, -1.0f,  // Vertex 1 position
	  1.0f, -1.0f,  // Vertex 2 position
	  1.0f,  1.0f,  // Vertex 3 position
	  -1.0f,  1.0f   // Vertex 4 position
	};

	float texCoords[] = {
	  0.0f, 1.0f,  // Vertex 1 tex coord
	  1.0f, 1.0f,  // Vertex 2 tex coord
	  1.0f, 0.0f,  // Vertex 3 tex coord
	  0.0f, 0.0f   // Vertex 4 tex coord
	};

	glViewport(0, 0, TEXT_SIZE, TEXT_SIZE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// Create the VBOs
	GLuint vboPos, vboTexCoord;
	glGenBuffers(1, &vboPos);
	glGenBuffers(1, &vboTexCoord);

	// Bind the VBOs and fill them with the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vboPos);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboTexCoord);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), texCoords, GL_STATIC_DRAW);

	// Create the VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Enable the vertex attrib arrays and specify the format and stride of the data
	glEnableVertexAttribArray(0);  // Vertex position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);  // Vertex tex coord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	// Bind the VBOs to the VAO
	glBindBuffer(GL_ARRAY_BUFFER, vboPos);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, vboTexCoord);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	// Send frequency uniforms
	int loc = glGetUniformLocation(shader->program, "frequencies");
	glUniform1fv(loc, precision, (const GLfloat *) &generator.GetAngFreqs()[0]);

	//loc = glGetUniformLocation(shader->program, "amplitudes_re");
	loc = shader->GetUniformLocation("amplitudes_re");
	glUniform1fv(loc, precision, (const GLfloat *) &generator.GetAmpsRe()[0]);
	
	loc = glGetUniformLocation(shader->program, "amplitudes_im");
	glUniform1fv(loc, precision, (const GLfloat *) &generator.GetAmpsIm()[0]);

	loc = glGetUniformLocation(shader->program, "scale");
	glUniform3fv(loc, 1, &generator.GetScale(100, precision)[0]);

	std::cout << "Scale" << generator.GetScale(100, precision)[0] << ", "
		<< generator.GetScale(100, precision)[1] << ", "
		<< generator.GetScale(100, precision)[2] << "\n";

	loc = glGetUniformLocation(shader->program, "prec");
	glUniform1i(loc, precision);

	// Draw the quad using glDrawArrays
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	// Unbind the VAO
	glBindVertexArray(0);
}

void Wave::LoadShader() {
	if (shaderOK)
		return;

	std::string shaderPath = "/src/lab_m2/spg_proiect/wave/DrawToText_";

	char buff[200]; //create string buffer to hold path
	GetCurrentDir(buff, 200);
	std::string current_working_dir(buff);
	std::cout << current_working_dir;

	shader = std::make_unique<Shader>("DrawToText");
	shader->AddShader(current_working_dir + shaderPath + "VS.glsl", GL_VERTEX_SHADER);
	shader->AddShader(current_working_dir + shaderPath + "FS.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();

	shaderOK = true;
}

void Wave::LoadText() {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D ,texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXT_SIZE, TEXT_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
}

void Wave::LoadFramebuffer() {
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
}

GLuint m2::Wave::GetText()
{
	return texture;
}

std::shared_ptr<Wave> m2::SlowUpFastDownWave(int precision)
{
	std::shared_ptr<Wave> v = std::make_shared<Wave>(precision);

	std::complex<float> sample1(std::complex<float>(0, 0));
	std::complex<float> sample2(std::complex<float>(0.1, 0));
	std::complex<float> sample3(std::complex<float>(0.2, 0));
	std::complex<float> sample4(std::complex<float>(0.4, 0));
	std::complex<float> sample5(std::complex<float>(0.7, 0));
	std::complex<float> sample6(std::complex<float>(1.0, 0));
	std::complex<float> sample7(std::complex<float>(1.5, 0));
	std::complex<float> sample8(std::complex<float>(1.4, 0));
	std::complex<float> sample9(std::complex<float>(1.2, 0));
	std::complex<float> sample10(std::complex<float>(0.95, 0));
	std::complex<float> sample11(std::complex<float>(0.7, 0));
	std::complex<float> sample12(std::complex<float>(0.3, 0));
	std::complex<float> sample13(std::complex<float>(0.1, 0));
	std::complex<float> sample14(std::complex<float>(0.05, 0));
	std::complex<float> sample15(std::complex<float>(0.01, 0));

	Fourier f1;
	std::vector<std::complex<float>> vec;
	vec.push_back(sample1);
	vec.push_back(sample2);
	vec.push_back(sample3);
	vec.push_back(sample4);
	vec.push_back(sample5);
	vec.push_back(sample6);
	vec.push_back(sample7);
	vec.push_back(sample8);
	vec.push_back(sample9);
	vec.push_back(sample10);
	vec.push_back(sample11);
	vec.push_back(sample12);
	vec.push_back(sample13);
	vec.push_back(sample14);
	vec.push_back(sample15);

	f1.SetSample(vec);

	std::cout << f1;

	v->SetFourier(f1);
	v->FillTexture();

	return v;
}

std::shared_ptr<Wave> m2::WavePreset(int preset, int precision) {
	switch (preset)
			case 0: return SlowUpFastDownWave(precision);
}