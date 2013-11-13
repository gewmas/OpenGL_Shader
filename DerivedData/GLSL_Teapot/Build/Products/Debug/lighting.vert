/*
 attribute float myAttrib;
 
 Vertex attributes are used to communicate from "outside" to the vertex shader. Unlike uniform variables, values are provided per vertex (and not globally for all vertices). There are built-in vertex attributes like the normal or the position, or you can specify your own vertex attribute like a tangent or another custom value. Attributes can't be defined in the fragment shader.
 
 Built-in Vertex Attribute
 
 gl_Vertex	Position (vec4)
 gl_Normal	Normal (vec4)
 gl_Color	Primary color of vertex (vec4)
 gl_MultiTexCoord0	Texture coordinate of texture unit 0 (vec4)
 gl_MultiTexCoord1	Texture coordinate of texture unit 1 (vec4)
 gl_MultiTexCoord2	Texture coordinate of texture unit 2 (vec4)
 gl_MultiTexCoord3	Texture coordinate of texture unit 3 (vec4)
 gl_MultiTexCoord4	Texture coordinate of texture unit 4 (vec4)
 gl_MultiTexCoord5	Texture coordinate of texture unit 5 (vec4)
 gl_MultiTexCoord6
 Texture coordinate of texture unit 6 (vec4)
 gl_MultiTexCoord7	Texture coordinate of texture unit 7 (vec4)
 gl_FogCoord	Fog Coord (float)
 
 */

/*
 Varying Variables
 Varying variables provide an interface between Vertex and Fragment Shader. Vertex Shaders compute values per vertex and fragment shaders compute values per fragment. If you define a varying variable in a vertex shader, its value will be interpolated (perspective-correct) over the primitive being rendered and you can access the interpolated value in the fragment shader.
 Varying can be used only with the data types float, vec2, vec3, vec4, mat2, mat3, mat4. (arrays of them too.)
 */

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