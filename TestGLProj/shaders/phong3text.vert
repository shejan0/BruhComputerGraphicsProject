#version 400 core
out vec3 N;
out vec3 L;
out vec3 E;
out vec3 H;
out vec4 eyePosition;
out vec2 texCoordsInterpolated;
 
in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTextureCoordinates;

uniform vec4 lightPosition;
uniform mat4 Projection;
uniform mat4 ModelView;

 uniform vec4 lightDiffuse;
 uniform vec4 lightSpecular; 
 uniform vec4 lightAmbient;
 uniform vec4 surfaceDiffuse;
 uniform vec4 surfaceSpecular;
 uniform float shininess;
 uniform vec4 surfaceAmbient;
 uniform vec4  surfaceEmissive;
 uniform float useTexture;
void main()
{
	//if(useTexture > 0.0){
		texCoordsInterpolated = vertexTextureCoordinates;
	//}
    gl_Position = Projection * ModelView * vec4(vertexPosition, 1.0);

    eyePosition = ModelView * vec4(vertexPosition, 1.0);
    vec4 eyeLightPos = lightPosition;
	
	N = normalize(ModelView * vec4(vertexNormal,0.0)).xyz;
    L = normalize(eyeLightPos.xyz - eyePosition.xyz);
    E = -normalize(eyePosition.xyz);
    H = normalize(L + E);
}

