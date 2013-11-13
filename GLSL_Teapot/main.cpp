//
//  main.cpp
//  shaderTest
//
//  Created by Joseph Spillane on 09/10/2012.
//  Copyright (c) 2012 Joseph Spillane. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>

enum shaderType{VERTEX_SHADER, FRAGMENT_SHADER};

GLuint programID;
GLuint vertexbuffer;

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){
    
    const GLubyte *errString;
    
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
    
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    printf("Compiling shader : %s\n", VertexShaderCode.c_str());
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);
    
    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
    }
    else {
        printf("Vertex shader compiled!\n\n\n");
    }
    
    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    printf("Compiling shader : %s\n", FragmentShaderCode.c_str());
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
    
    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
    }
    else {
        printf("Fragment shader compiled!\n\n\n");
    }
    
    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    
    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    
    return ProgramID;
}

float a = 0;
float lpos[4] = {1.0,1.0,1.0,0.0};
float cutOff = 100;

void renderScene(void) {
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glLoadIdentity();
	gluLookAt(0.0,5.0,5.0,
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
    
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, &cutOff);
	glRotatef(a,0,1,0);
    
    glColor3f(0,1,0);
	glutSolidTeapot(1);
//    glutSolidSphere(1, 20, 20);
	a+=0.1;
    
    // glFrontFace(GL_CW);
    // glBegin(GL_TRIANGLE_STRIP);
    // glVertex3f( -10.0f, -1.0f, -10.0f ); //vertex 1
    // glVertex3f( -10.0f, -1.0f, 10.0f ); //vertex 2
    // glVertex3f( 10.0f, -1.0f, -10.0f ); //vertex 3
    // glVertex3f( 10.0f, -1.0f, 10.0f ); //vertex 4
    // glEnd();
    
	glutSwapBuffers();
    
    // Clear the screen
    /*glClear( GL_COLOR_BUFFER_BIT );
     
     // Use our shader
     glUseProgram(programID);
     
     // 1rst attribute buffer : vertices
     glEnableVertexAttribArray(0);
     glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
     glVertexAttribPointer(
     0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
     3,                  // size
     GL_FLOAT,           // type
     GL_FALSE,           // normalized?
     0,                  // stride
     (void*)0            // array buffer offset
     );
     
     // Draw the triangle !
     glDrawArrays(GL_TRIANGLES, 0, 3); // From index 0 to 3 -> 1 triangle
     
     glDisableVertexAttribArray(0);
     
     // Swap buffers
     glutSwapBuffers();*/
    
    
}

void changeSize(int w, int h) {
    
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;
    
	float ratio = 1.0* w / h;
    
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    
	// Set the correct perspective.
	gluPerspective(45,ratio,1,100);
	glMatrixMode(GL_MODELVIEW);
    
    
}

GLuint LoadShader(const char * shader_file_path, shaderType shader_type, GLuint program_id = -1){
    
    GLuint shader_id;
    
    if(shader_type == VERTEX_SHADER)
        shader_id = glCreateShader(GL_VERTEX_SHADER);
    
    else
        shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    
    std::string ShaderCode;
    std::ifstream ShaderStream(shader_file_path, std::ios::in);
    if(ShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(ShaderStream, Line))
            ShaderCode += "\n" + Line;
        ShaderStream.close();
    }
    
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    // Compile Vertex Shader
    printf("Compiling shader : %s\n", shader_file_path);
    printf("Compiling shader : %s\n", ShaderCode.c_str());
    char const * SourcePointer = ShaderCode.c_str();
    glShaderSource(shader_id, 1, &SourcePointer , NULL);
    glCompileShader(shader_id);
    
    // Check Vertex Shader
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0) {
        std::vector<char> ShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(shader_id, InfoLogLength, NULL, &ShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &ShaderErrorMessage[0]);
    }
    else {
        printf("Shader compiled!\n\n\n");
    }
    
    // Link the program
    fprintf(stdout, "Linking program\n");
    if(program_id == -1)
        program_id = glCreateProgram();
    glAttachShader(program_id, shader_id);
    glLinkProgram(program_id);
    
    // Check the program
    glGetProgramiv(program_id, GL_LINK_STATUS, &Result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(program_id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
    
    glDeleteShader(shader_id);
    
    return program_id;
}

int main(int argc, char * argv[])
{
    glutInit(&argc, argv);
    //glutInitContextVersion(3,2);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("Teapot!");
    glClearColor(1, 1, 1, 0.0f);
    
    //GLuint VertexArrayID = 0;
    //glGenVertexArraysAPPLE(1, &VertexArrayID);
    //glBindVertexArrayAPPLE(VertexArrayID);GLenum errCode;
    const GLubyte* version = glGetString(GL_SHADING_LANGUAGE_VERSION);
    fprintf (stderr, "GLSL Version: %s\n", version);
    
    version = glGetString(GL_VERSION);
    fprintf (stderr, "openGL Version: %s\n", version);
    
    version = glGetString(GL_EXTENSIONS);
    fprintf (stderr, "openGL Extenstions: %s\n", version);
    
    
    
    
    // Create and compile our GLSL program from the shaders
//    programID = LoadShaders( "spotlight.vert", "spotlight.frag" );
//    programID = LoadShaders( "lighting.vert", "lighting.frag" );
    //Nov 13
    programID = LoadShader("lighting.vert", VERTEX_SHADER);
    programID = LoadShader("lighting.frag", FRAGMENT_SHADER, programID);
    //frame buffer object
//    fbo_program_id = LoadShader("blur.frag", FRAGMENT_SHADER);

    
    glUseProgram(programID);
    
    //static const GLfloat g_vertex_buffer_data[] = {
    //    -1.0f, -1.0f, 0.0f,
    //    1.0f, -1.0f, 0.0f,
    //    0.0f,  1.0f, 0.0f,
    //};
    
    // This will identify our vertex buffer
    
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    //glGenBuffers(1, &vertexbuffer);
    
    // The following commands will talk about our 'vertexbuffer' buffer
    //glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    
    // Give our vertices to OpenGL.
    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	//glutKeyboardFunc(processNormalKeys);
    
	glEnable(GL_DEPTH_TEST);
    glutMainLoop();
	glClearColor(1.0,1.0,1.0,1.0);
    
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}

