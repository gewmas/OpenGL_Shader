uniform sampler2DRect texture;

void main(){
    float width = 4.0;
    
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    
    for (float i = -width; i <= width; i++) {
        for (float j = -width; j <= width; j++) {
            color += texture2DRect(texture, gl_TexCoord[0].st + vec2(i, j));
        }
    }
    
    color /= pow(2.0*width+1.0, 2.0);
    
    gl_FragColor = color;
}