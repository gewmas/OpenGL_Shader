varying vec3 normal;
varying vec3 vertex_to_light_vector;
varying vec3 vertex_to_eye_vector;
varying vec2 texture_coordinate;

void main()
{
    gl_Position  = gl_ModelViewProjectionMatrix * gl_Vertex;
    
    normal = gl_NormalMatrix * gl_Normal;
    
    vec4 vertex_in_modelView_space = gl_ModelViewMatrix * gl_Vertex;
    
    vertex_to_light_vector = vec3(gl_LightSource[0].position - vertex_in_modelView_space);
    
    vertex_to_eye_vector = vec3(-vertex_in_modelView_space);
    
    texture_coordinate = vec2(gl_MultiTexCoord0);
    
}