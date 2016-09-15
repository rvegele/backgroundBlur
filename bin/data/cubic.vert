#version 120
//uniform vec2 screenSize;
varying vec2 texCoordVarying;

void main(void)
{
    texCoordVarying = gl_MultiTexCoord0.xy;
	gl_Position = ftransform();
}

//#version 120
//varying vec2 texCoordVarying;

//void main(void)
//{
//    texCoordVarying = gl_MultiTexCoord0.xy;
//    gl_Position = ftransform();
//}