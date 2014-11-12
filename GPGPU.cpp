#include "GPGPU.h"
#include "Fluid.h"
#include <iostream>
#include <fstream>

static GLuint QuadVao;
static GLuint VisualizeProgram;
static Slab Velocity, Density, Pressure, Temperature;
static Surface Divergence, Obstacles, HiresObstacles;


GPGPU::GPGPU(int w, int h) : _initialized(0), _width(w), _height(h)
{

	Velocity = CreateSlab(w, h, 2);
    Density = CreateSlab(w, h, 1);
    Pressure = CreateSlab(w, h, 1);
    Temperature = CreateSlab(w, h, 1);
    Divergence = CreateSurface(w, h, 3);
	InitSlabOps();
    VisualizeProgram = CreateProgram("./Shader/Fluid.Vertex", 0, "./Shader/Fluid.Visualize");

	Obstacles = CreateSurface(w, h, 3);
    CreateObstacles(Obstacles, w, h);

	w = ViewportWidth * 2;
    h = ViewportHeight * 2;
    HiresObstacles = CreateSurface(w, h, 1);
    CreateObstacles(HiresObstacles, w, h);
	
    QuadVao = CreateQuad();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ClearSurface(Temperature.Ping, AmbientTemperature);

	startTime = clock();
    // Create a texture to store the framebuffer
   /* glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGB, GL_FLOAT, 0);

    _programId = glCreateProgram();

	// Load fragment shader which will be used as computational kernel
	std::string edgeFragSource2;
	loadShader("fragment.glsl", edgeFragSource2);

    // Create the fragment program
    _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* source = edgeFragSource2.c_str();
    glShaderSource(_fragmentShader, 1, &source, NULL);
    glCompileShader(_fragmentShader);
    glAttachShader(_programId, _fragmentShader);

    // Link the shader into a complete GLSL program.
    glLinkProgram(_programId);

	// Check program
	{
		int infologLength = 0;
		glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength > 0) {
			char *infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(_programId, infologLength, NULL, infoLog);
			printf("%s\n",infoLog);
			free(infoLog);
		}
	}

    // Get location of the uniform variables
    _texUnitLoc = glGetUniformLocation(_programId, "texUnit");
	_initializedLoc = glGetUniformLocation(_programId, "initialized");*/
}

void GPGPU::restart()
{
	_initialized = 0;
}

void GPGPU::update()
{
	clock_t currentTime = clock();
	double elapsed_secs = double(currentTime - startTime) / CLOCKS_PER_SEC;
	unsigned int elapsedMicroseconds = elapsed_secs * 1000;
	startTime = currentTime;

	glViewport(0, 0, GridWidth, GridHeight);

    Advect(Velocity.Ping, Velocity.Ping, Obstacles, Velocity.Pong, VelocityDissipation);
    SwapSurfaces(&Velocity);

	
    Advect(Velocity.Ping, Temperature.Ping, Obstacles, Temperature.Pong, TemperatureDissipation);
    SwapSurfaces(&Temperature);

    Advect(Velocity.Ping, Density.Ping, Obstacles, Density.Pong, DensityDissipation);
    SwapSurfaces(&Density);

	ApplyBuoyancy(Velocity.Ping, Temperature.Ping, Density.Ping, Velocity.Pong);
    SwapSurfaces(&Velocity);

	ApplyImpulse(Temperature.Ping, ImpulsePosition, ImpulseTemperature);
    ApplyImpulse(Density.Ping, ImpulsePosition, ImpulseDensity);
	
	ComputeDivergence(Velocity.Ping, Obstacles, Divergence);
    ClearSurface(Pressure.Ping, 0);

	
	
	for (int i = 0; i < NumJacobiIterations; ++i) {
        Jacobi(Pressure.Ping, Divergence, Obstacles, Pressure.Pong);
        SwapSurfaces(&Pressure);
    }

    SubtractGradient(Velocity.Ping, Pressure.Ping, Obstacles, Velocity.Pong);
    SwapSurfaces(&Velocity);

	/*printf("hi");
    // Backup the viewport dimensions
    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    glViewport(0, 0, _width, _height);
        
    // Copy the results to the texture
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glUseProgram(_programId);
            
    // Set the uniform variables
    glUniform1i(_texUnitLoc, 0);
	glUniform1i(_initializedLoc, _initialized);
	_initialized = 1;
            
	// By drawing a quad, the fragment shader will be called for each pixel.
    glBegin(GL_QUADS);
    {            
        glTexCoord2f(0, 0); glVertex3f(-1, -1, -0.5f);
        glTexCoord2f(1, 0); glVertex3f( 1, -1, -0.5f);
        glTexCoord2f(1, 1); glVertex3f( 1,  1, -0.5f);
        glTexCoord2f(0, 1); glVertex3f(-1,  1, -0.5f);
    }
    glEnd();

    glUseProgram(0);
        
    // Copy the rendered image to the texture so that we can start from the current state in the next iteration
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, _width, _height);
        
    // Restore the stored viewport dimensions
	glViewport(vp[0], vp[1], vp[2], vp[3]);*/
}

void GPGPU::display()
{

	// Bind visualization shader and set up blend state:
    glUseProgram(VisualizeProgram);
    GLint fillColor = glGetUniformLocation(VisualizeProgram, "FillColor");
    GLint scale = glGetUniformLocation(VisualizeProgram, "Scale");
    glEnable(GL_BLEND);

    // Set render target to the backbuffer:
    glViewport(0, 0, ViewportWidth, ViewportHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw ink:
    glBindTexture(GL_TEXTURE_2D, Density.Ping.TextureHandle);
    glUniform3f(fillColor, 1, 1, 1);
    glUniform2f(scale, 1.0f / ViewportWidth, 1.0f / ViewportHeight);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    // Draw obstacles:
    glBindTexture(GL_TEXTURE_2D, HiresObstacles.TextureHandle);
    glUniform3f(fillColor, 0.125f, 0.4f, 0.75f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    // Disable blending:
    glDisable(GL_BLEND);


}
