attribute vec4 position;


void main(void)
{
    gl_Position = vec4(position, 1.0f);
}