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

float rotation_degree = 0.0f; // The angle of rotation in degrees for our teapot

#pragma mark TextureMapping

// Define a 2 x 2 red and yellow checkered pattern using RGB colors.
#define green {0x00, 0x99, 0x22}
#define blue {0x55, 0x00, 0x44}
#define magenta {0xff, 0, 0xff}
GLubyte texture[][3] = {
    green, blue,
    blue, green,
};

// texture mapping
GLuint texName;

void textureMapping( void ){
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, current_image->getWidth(), current_image->getHeight(), 0, GL_RGB, GL_FLOAT, current_image->getImageDisplayArray());
    
}

#pragma mark Frame Buffer Object
GLuint fbo_program_id;
// create a framebuffer objects
unsigned int fbo; // The frame buffer object
unsigned int fbo_depth; // The depth buffer for the frame buffer object
unsigned int fbo_texture; // The texture object to write our frame buffer object to

int window_width = 500; // The width of our window
int window_height = 500; // The height of our window

float lpos[4] = {1.0,1.0,1.0,0.0};
float cutOff = 100;

void initFrameBufferDepthBuffer(void) {
    glGenRenderbuffersEXT(1, &fbo_depth); // Generate one render buffer and store the ID in fbo_depth
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo_depth); // Bind the fbo_depth render buffer
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, window_width, window_height); // Set the render buffer storage to be a depth component, with a width and height of the window
    
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth); // Set the render buffer of this buffer to the depth buffer
    
    
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0); // Unbind the render buffer
}

void initFrameBufferTexture(void) {
    glGenTextures(1, &fbo_texture); // Generate one texture
    
    glBindTexture(GL_TEXTURE_RECTANGLE_EXT, fbo_texture); // Bind the texture fbo_texture
    
    glTexImage2D(GL_TEXTURE_RECTANGLE_EXT, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // Create a standard texture with the width and height of our window
    
    // Setup the basic texture parameters
    
    glTexParameterf(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    // Unbind the texture
    glBindTexture(GL_TEXTURE_RECTANGLE_EXT, 0);
}

void initFrameBuffer(void) {
    
    initFrameBufferDepthBuffer(); // Initialize our frame buffer depth buffer
    
    initFrameBufferTexture(); // Initialize our frame buffer texture
    
    glGenFramebuffersEXT(1, &fbo); // Generate one frame buffer and store the ID in fbo
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); // Bind our frame buffer
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_RECTANGLE_EXT, fbo_texture, 0); // Attach the texture fbo_texture to the color buffer in our frame buffer
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth); // Attach the depth buffer fbo_depth to our frame buffer
    
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); // Check that status of our generated frame buffer
    
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT) // If the frame buffer does not report back as complete
    {
        std::cout << "Couldn't create frame buffer" << std::endl; // Make sure you include <iostream>
        exit(0); // Exit the application
    }
    
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // Unbind our frame buffer
}

void init(){
    glEnable(GL_TEXTURE_RECTANGLE_EXT); // Enable texturing so we can bind our frame buffer texture
    
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, &cutOff);
    
    initFrameBuffer(); // Create our frame buffer object
    
}

#pragma mark LoadShader
GLuint LoadShader(const char * shader_file_path, shaderType shader_type, GLuint program_id = -1){
    
    /* 
     Create Shader
     First you have to create an OpenGL "Shader Object" specifying what kind of shader it is (e.g. Vertex Shader, Geometry Shader, or Fragment Shader) . A shader object can be created using the OpenGL function glCreateShader with the arguments GL_VERTEX_SHADER or GL_FRAGMENT_SHADER (or GL_GEOMETRY_SHADER_EXT).
     */
    GLuint shader_id;
    if(shader_type == VERTEX_SHADER)
        shader_id = glCreateShader(GL_VERTEX_SHADER);
    
    else
        shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    
    
    /* 
     Loading Shader
     If the shaders are located in text files they must be loaded into memory first. This part has nothing to do with OpenGL, it is a simple ASCII file loader. If you write your shaders in Unicode (for the comments), you have to write your own loader. The actual program in memory should be in ASCII. You could also embed your shaders into your C++ code using a static char array. In other words: it doesn't matter how you get your shaders into memory. I recommend using ASCII files. This way you can change your shader code without recompiling your application.
     */
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
    glAttachShader(program_id, shader_id); //Append shader_id to existing program_id
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


#pragma mark Render
float a = 0;
void renderScene(void) {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); // Bind our frame buffer for rendering
    glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT); // Push our glEnable and glViewport states
    glViewport(0, 0, window_width, window_height); // Set the size of the frame buffer view port
    
    glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Set the clear colour
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the depth and colour buffers
    
    glLoadIdentity();
	gluLookAt(0.0,5.0,5.0,
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
    
    glBindTexture(GL_TEXTURE_2D, texName);
    glUseProgram(programID);
    glRotatef(a, 0, 1, 0);
    glutSolidTeapot(1.0);
    
    a += 0.1;
    
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopAttrib(); // Restore our glEnable and glViewport states
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // Unbind our texture
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //a+=1.0;
	//glutSwapBuffers();
}

void display (void) {
    
    renderScene();
    
    glUseProgram(fbo_program_id);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
    
    
    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(rotation_degree, 0, 1, 0);
    glBindTexture(GL_TEXTURE_RECTANGLE_EXT, fbo_texture);
    
    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); //front
    glTexCoord2f(0.0f, window_height); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(window_width, window_height); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(window_width, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); //right
    glTexCoord2f(0.0f, window_height); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(window_width, window_height); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(window_width, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); //top
    glTexCoord2f(0.0f, window_height); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(window_width, window_height); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(window_width, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); //bottom
    glTexCoord2f(0.0f, window_height); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(window_width, window_height); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(window_width, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); //left
    glTexCoord2f(0.0f, window_height); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(window_width, window_height); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(window_width, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); //back
    glTexCoord2f(0.0f, window_height); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(window_width, window_height); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(window_width, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    
    glEnd();
    
    rotation_degree+= 0.1;
    
    glBindTexture(GL_TEXTURE_RECTANGLE_EXT, 0);
    
    glUseProgram(0);
    glutSwapBuffers();
}

#pragma mark helper function
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

void keyboard( unsigned char key, int x, int y ) {
    switch(key) {
        case 27: // Escape key
            exit(0);
            break;
    }
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
//    programID = LoadShader("spotlight.vert", VERTEX_SHADER);
//    programID = LoadShader("spotlight.frag", FRAGMENT_SHADER, programID);

    //frame buffer object
    fbo_program_id = LoadShader("blur.frag", FRAGMENT_SHADER);

    
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
    init();
    textureMapping();
    
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc( keyboard );
    
	glEnable(GL_DEPTH_TEST);
    glutMainLoop();
	glClearColor(1.0,1.0,1.0,1.0);
    
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}

