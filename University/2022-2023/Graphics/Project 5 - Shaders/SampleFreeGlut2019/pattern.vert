#version 330 compatibility

uniform float	uTime;		// "Time", from Animate( )
out vec2  	vST;		// texture coords

out  vec3  vN;		// normal vector
out  vec3  vL;		// vector from point to light
out  vec3  vE;		// vector from point to eye

uniform bool vertPattern;

vec3 LightPosition = vec3(  5., 5., 0. );

const float PI = 	3.14159265;
const float AMP = 	0.2;		// amplitude
const float W = 	2.;		// frequency

void
main( )
{ 

	vST = gl_MultiTexCoord0.st;
	vec3 vert = gl_Vertex.xyz;
	
	if(vertPattern){
		vert.x *= sin(uTime * 10 * PI * vert.y);
	}
	

	//<< change vert to perform vertex distortion >>
	vec4 ECposition = gl_ModelViewMatrix * vec4( vert, gl_Vertex.w );
	vL = LightPosition - ECposition.xyz;		// vector from the point
							// to the light position


	vN = normalize( gl_NormalMatrix * gl_Normal );	// normal vector

	vE = vec3( 0., 0., 0. ) - ECposition.xyz;	// vector from the point
							// to the eye position 

	gl_Position = gl_ModelViewProjectionMatrix * vec4( vert, gl_Vertex.w );
}
