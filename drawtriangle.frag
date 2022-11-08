#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture1;

void main(void)
{
    color = mix(texture2D(ourTexture, TexCoord), texture2D(ourTexture1, TexCoord), 0.2);
}
