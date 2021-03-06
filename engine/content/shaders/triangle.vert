#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out dvec2 dTexCoords;
out vec3 pos;
uniform mat4 model;
uniform mat4 fullMatrix;
uniform sampler2D normals;
void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * texture(normals,vec2(aTexCoords)).xyz;  
    TexCoords = vec2(aTexCoords);
    dTexCoords = aTexCoords;
    gl_Position = fullMatrix * vec4(FragPos,1.0);
    pos = vec3(gl_Position);
} 