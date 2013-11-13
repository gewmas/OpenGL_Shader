varying vec3 normal;
varying vec3 vertex_to_light_vector;
varying vec3 vertex_to_eye_vector;

void main()
{
	const vec4 AmbientColor = vec4(0.7, 0.1, 0.1, 1.0);
    const vec4 DiffuseColor = vec4(0.0, 0.1, 0.3, 1.0);
    const vec4 SpecularColor = vec4(1.0, 1.1, 0.0, 1.0);
    
    vec3 normalized_normal = normalize(normal);
    vec3 normalized_vertex_to_light_vector = normalize(vertex_to_light_vector);
    vec3 normalized_vertex_to_eye_vector = normalize(vertex_to_eye_vector);
    
    vec3 bisector = normalize(vertex_to_light_vector + vertex_to_eye_vector);
    
    float DiffuseTerm = clamp(max(0.0, dot(normalized_normal, normalized_vertex_to_light_vector)), 0.0, 1.0);
    float SpecularTerm = clamp(max(0.0, dot(normalized_normal, bisector)), 0.0, 1.0);
    
    gl_FragColor = AmbientColor + DiffuseColor * DiffuseTerm + SpecularColor * pow(SpecularTerm, 80.0);
}
