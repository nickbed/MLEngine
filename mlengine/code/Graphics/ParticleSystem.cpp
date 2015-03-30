#include "ParticleSystem.h"
#include <math.h>

ParticleSystem::ParticleSystem()
{
	Init();
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Init()
{
	particletime = 0.0f;
	deltaT = 0.0f;
	graphicdeltaT = 0.0f;
	oldGraphicTime = 0.0f;
	drawBuf = 1;

	//Load in the particle texture
	particleTexture = mauvefileresource::ResourceManager::GetResource<ImageTexture>("data\\images\\waterdrop.png");
	particleGPUTexture = new GPUTexture();
	particleGPUTexture->SetTexture(particleTexture);
	particleGPUTexture->UploadData();
	glBindTexture(GL_TEXTURE_2D, particleGPUTexture->GetDataLocation());
	glEnable(GL_POINT_SPRITE);

	//Load in the particle shader
	particleShader = mauvefileresource::ResourceManager::GetResource<Shader>("data\\shaders\\particles");
	particleRenderSub = glGetSubroutineIndex(particleShader->GetShaderID(), GL_VERTEX_SHADER, "render");
	particleUpdateSub = glGetSubroutineIndex(particleShader->GetShaderID(), GL_VERTEX_SHADER, "update");
	particleShader->UseShader();
	//particleShader->SendUniform1i("ParticleTex", particleGPUTexture->GetDataLocation());
	particleShader->SendUniform1f("ParticleLifetime", 1.5f);
	particleShader->SendUniformVec3("Accel", glm::vec3(0.0f, -0.4f, 0.0f));

	InitParticleBuffers(false);

	canDraw = true;

	currentParticlePosition = glm::vec3(0.0);

	glm::mat4 currMat(1.0f);
	float rotang =rand()%360;
	currMat = glm::translate(currMat, glm::vec3(0.5f, 0.5f, 0.0f));
	currMat = glm::rotate(currMat,rotang , glm::vec3(0.0f, 0.0f, 1.0f));
	currMat = glm::translate(currMat, glm::vec3(-0.5f, -0.5f, 0.0f));
	particleShader->SendUniformMat4("rotMatrix",currMat);

}

void ParticleSystem::InitParticleBuffers(bool test)
{
	particleShader->UseShader();
	nParticles = 4000;

	// Generate the buffers
	glGenBuffers(2, posBuf);    // position buffers
	glGenBuffers(2, velBuf);    // velocity buffers
	glGenBuffers(2, startTime); // Start time buffers
	glGenBuffers(1, &initVel);  // Initial velocity buffer (never changes, only need one)
	glGenBuffers(1, &initPos);  // Initial velocity buffer (never changes, only need one)

	// Allocate space for all buffers
	int size = nParticles * 3 * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, initPos);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, startTime[1]);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);

	// Fill the first position buffer with zeroes
	posArray = new GLfloat[nParticles * 3];
	GLfloat *data = new GLfloat[nParticles * 3];
	float xLength = 5.0f;
	if (test) xLength = 10.0f;
	float step = xLength / nParticles;
	float currentXpos = 0.0f;
	int isX = 0;
	for (int i = 0; i < nParticles * 3; i++)
	{
		if (isX == 0)
		{
			posArray[i] = currentXpos;
			currentXpos += step;
		}
		else
		{
			if (isX >= 2) isX = -1;
			if (test)posArray[i] = 10.0f;
			else
			{
				posArray[i] = 0.0f;
			}
		}
		++isX;
	}
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, posArray);
	glBindBuffer(GL_ARRAY_BUFFER, initPos);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, posArray);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	for (int i = 0; i < nParticles; i++) {
		
		theta = glm::mix(0.0f, (float)glm::pi<float>() / 15.f, randFloat());
		phi = glm::mix(0.0f, (float)glm::pi<float>()*2.0f, randFloat());

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(1.25f, 1.5f, randFloat());
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = -v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	// Fill the first start time buffer
	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.003f;
	for (int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	// Create vertex arrays for each set of buffers
	glGenVertexArrays(2, particleArray);

	// Set up particle array 0
	glBindVertexArray(particleArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
	glVertexAttribPointer(2, 1, GL_FLOAT, FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(3, 3, GL_FLOAT, FALSE, 0, NULL);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, initPos);
	glVertexAttribPointer(4, 3, GL_FLOAT, FALSE, 0, NULL);
	glEnableVertexAttribArray(4);

	// Set up particle array 1
	glBindVertexArray(particleArray[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, startTime[1]);
	glVertexAttribPointer(2, 1, GL_FLOAT, FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(3, 3, GL_FLOAT, FALSE, 0, NULL);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, initPos);
	glVertexAttribPointer(4, 3, GL_FLOAT, FALSE, 0, NULL);
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, startTime[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, startTime[1]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

void ParticleSystem::SetPosition(glm::vec3 pos)
{
	currentParticlePosition = pos;
	 //Allocate space for all buffers
	int size = nParticles * 3 * sizeof(GLfloat);

	// Fill the first position buffer with zeroes
	float xLength = 5.0f;
	float step = xLength / nParticles;
	float currentXpos = 0.0f;
	int arraypos = 0;
	for (int i = 0; i < nParticles * 3; i++)
	{
		posArray[i] = pos[arraypos++];
		if (arraypos > 2) arraypos = 0;
	}
	//glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, size, posArray);
	glBindBuffer(GL_ARRAY_BUFFER, initPos);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, posArray);

	// Set up particle array 0
	glBindVertexArray(posArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	//glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, 0, NULL);
	//glEnableVertexAttribArray(0);
}

float ParticleSystem::randFloat()
{
	return ((float)rand() / RAND_MAX);
}


void ParticleSystem::Draw(glm::mat4 VP, glm::vec3 cameraPos)
{
	graphicdeltaT = (float)glfwGetTime() - oldGraphicTime;
	//SetPosition(currentParticlePosition);
	oldGraphicTime = (float)glfwGetTime();
	particletime += graphicdeltaT;

	if (!canDraw) return;
	glBindTexture(GL_TEXTURE_2D, particleGPUTexture->GetDataLocation());

	float dist = glm::length(glm::vec3(0.0) - cameraPos);
	float pointSize = 10.0f;
	/*if (dist > 15.0f) pointSize = 5.0f;
	else
	{
		pointSize = 10.0f;
	}*/
	glPointSize(pointSize);
	glEnable(GL_BLEND);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//glBlendColor(1.0, 0.0, 0.0, 1.0);
	particleShader->UseShader();
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &particleUpdateSub);

	particleShader->SendUniform1f("H", graphicdeltaT);
	particleShader->SendUniform1f("Time", particletime);

	glEnable(GL_RASTERIZER_DISCARD);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
	
	glBeginTransformFeedback(GL_POINTS);
	glBindVertexArray(particleArray[1 - drawBuf]);
	glDrawArrays(GL_POINTS, 0, nParticles);
	glEndTransformFeedback();

	glDisable(GL_RASTERIZER_DISCARD);

	////////////////////////////////////////////////////////////////////////////////////////

	// Render pass
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &particleRenderSub);


	particleShader->SendUniformMat4("MVP", VP);


	glBindVertexArray(particleArray[drawBuf]);

	glDrawTransformFeedback(GL_POINTS, feedback[drawBuf]);


	// Swap buffers
	drawBuf = 1 - drawBuf;
	//canDraw = false;
}


