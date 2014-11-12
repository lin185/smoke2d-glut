#include "Fluid.h"
#include <string.h>
#include <iostream>
#include <fstream>

int
loadShader(const char* filename, std::string& text)
{
	std::ifstream ifs;
	ifs.open(filename, std::ios::in);

	std::string line;
	while (ifs.good()) {
        getline(ifs, line);
		text += line + "\n";

	}
	//std::cout << "Reading file: " << filename << std::endl;
	//std::cout << text << std::endl;
	return 0;
}

GLuint CreateProgram(const char* vsKey, const char* gsKey, const char* fsKey)
{
	printf("Creating Progarm: %s\t%s\t%s\n", vsKey, gsKey, fsKey);
	GLuint programHandle = glCreateProgram();

	if(vsKey) {
		GLuint _vertexShader = glCreateShader(GL_VERTEX_SHADER); 
		std::string vertexShaderSource;
		loadShader(vsKey, vertexShaderSource);
		const char* vertex_shader_source = vertexShaderSource.c_str();
		glShaderSource(_vertexShader, 1, &vertex_shader_source, NULL);
		glCompileShader(_vertexShader);
		glAttachShader(programHandle, _vertexShader);

		// Check vertexShader compile error
		{
			GLint length;
			glGetShaderiv(_vertexShader,GL_INFO_LOG_LENGTH, &length);
			GLchar* log = new GLchar[length + 1];
			glGetShaderInfoLog(_vertexShader,length,NULL,log);
			if(length>1) { 
				printf("Vertex shader compile error: \n");
				printf("%s\n",log);
			}
		}

	}

	if(gsKey != 0) {
		GLuint _geoShader = glCreateShader(GL_GEOMETRY_SHADER); 
		std::string geoShaderSource;
		loadShader(gsKey, geoShaderSource);
		const char* geo_shader_source = geoShaderSource.c_str();
		glShaderSource(_geoShader, 1, &geo_shader_source, NULL);
		glCompileShader(_geoShader);
		glAttachShader(programHandle, _geoShader);

		// Check vertexShader compile error
		{
			GLint length;
			glGetShaderiv(_geoShader,GL_INFO_LOG_LENGTH, &length);
			GLchar* log = new GLchar[length + 1];
			glGetShaderInfoLog(_geoShader,length,NULL,log);
			if(length>1) { 
				printf("Geometry shader compile error: \n");
				printf("%s\n",log);
			}
		}

	}

		
	if(fsKey) {
		GLuint _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); 
		std::string fragmentShaderSource;
		loadShader(fsKey, fragmentShaderSource);
		const char* fragment_shader_source = fragmentShaderSource.c_str();
		glShaderSource(_fragmentShader, 1, &fragment_shader_source, NULL);
		glCompileShader(_fragmentShader); 
		glAttachShader(programHandle, _fragmentShader);

		// Check vertexShader compile error
		{
			GLint length;
			glGetShaderiv(_fragmentShader,GL_INFO_LOG_LENGTH, &length);
			GLchar* log = new GLchar[length + 1];
			glGetShaderInfoLog(_fragmentShader,length,NULL,log);
			if(length>1) { 
				printf("Fragment shader compile error: \n");
				printf("%s\n",log);
			}
		}

	}

	// Link the shader into a complete GLSL program.
    glLinkProgram(programHandle);

	// Check program
	{
		int infologLength = 0;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength > 1) {
			char *infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(programHandle, infologLength, NULL, infoLog);
			printf("Check Program error: \n");
			printf("%s\n",infoLog);
			free(infoLog);
		}
	}

   /* const char* vsSource = glswGetShader(vsKey);
    const char* gsSource = glswGetShader(gsKey);
    const char* fsSource = glswGetShader(fsKey);

    const char* msg = "Can't find %s shader: '%s'.\n";
   // PezCheckCondition(vsSource != 0, msg, "vertex", vsKey);
   // PezCheckCondition(gsKey == 0 || gsSource != 0, msg, "geometry", gsKey);
   // PezCheckCondition(fsKey == 0 || fsSource != 0, msg, "fragment", fsKey);
    
    GLint compileSuccess;
    GLchar compilerSpew[256];
    GLuint programHandle = glCreateProgram();

    GLuint vsHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsHandle, 1, &vsSource, 0);
    glCompileShader(vsHandle);
    glGetShaderiv(vsHandle, GL_COMPILE_STATUS, &compileSuccess);
    glGetShaderInfoLog(vsHandle, sizeof(compilerSpew), 0, compilerSpew);
   // PezCheckCondition(compileSuccess, "Can't compile %s:\n%s", vsKey, compilerSpew);
    glAttachShader(programHandle, vsHandle);
	 
    GLuint gsHandle;
    if (gsKey) {
        gsHandle = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gsHandle, 1, &gsSource, 0);
        glCompileShader(gsHandle);
        glGetShaderiv(gsHandle, GL_COMPILE_STATUS, &compileSuccess);
        glGetShaderInfoLog(gsHandle, sizeof(compilerSpew), 0, compilerSpew);
     //   PezCheckCondition(compileSuccess, "Can't compile %s:\n%s", gsKey, compilerSpew);
        glAttachShader(programHandle, gsHandle);
    }
    
    GLuint fsHandle;
    if (fsKey) {
        fsHandle = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fsHandle, 1, &fsSource, 0);
        glCompileShader(fsHandle);
        glGetShaderiv(fsHandle, GL_COMPILE_STATUS, &compileSuccess);
        glGetShaderInfoLog(fsHandle, sizeof(compilerSpew), 0, compilerSpew);
       // PezCheckCondition(compileSuccess, "Can't compile %s:\n%s", fsKey, compilerSpew);
        glAttachShader(programHandle, fsHandle);
    }

    glLinkProgram(programHandle);
    
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    glGetProgramInfoLog(programHandle, sizeof(compilerSpew), 0, compilerSpew);

    if (!linkSuccess) {
        //PezDebugString("Link error.\n");
       // if (vsKey) PezDebugString("Vertex Shader: %s\n", vsKey);
      //  if (gsKey) PezDebugString("Geometry Shader: %s\n", gsKey);
      //  if (fsKey) PezDebugString("Fragment Shader: %s\n", fsKey);
      //  PezDebugString("%s\n", compilerSpew);
    }
    
    return programHandle;*/
	return programHandle;
}
