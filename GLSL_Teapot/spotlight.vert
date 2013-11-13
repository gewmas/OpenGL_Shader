/* -------------------------------------------------------
 
 This shader implements a spotlight per pixel using  the
 diffuse, specular, and ambient terms acoording to "Mathematics of Lighthing"
 as found in the book "OpenGL Programming Guide" (aka the Red Book)
 
 Antonio Ramires Fernandes
 
 --------------------------------------------------------- */

varying vec3 normal;
varying vec3 vertex_to_light_vector;
varying vec3 vertex_to_eye_vector;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    
    normal = gl_NormalMatrix * gl_Normal;
    vec4 vertex_in_modelView_space = gl_ModelViewMatrix * gl_Vertex;
    vertex_to_light_vector =  vec3(gl_LightSource[0].position - vertex_in_modelView_space);
    
    vertex_to_eye_vector = vec3(-vertex_in_modelView_space);
	
	
	
	
} 
