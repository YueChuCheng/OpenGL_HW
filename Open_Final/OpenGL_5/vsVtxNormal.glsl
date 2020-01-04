#version 130
in vec4 vPosition;
in vec3 vNormal;
out vec4 color;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 vObjectColor;

void main()
{
    gl_Position = Projection * ModelView * vPosition;
	if( vObjectColor.x == -1.0f && vObjectColor.y == -1.0f && vObjectColor.z == -1.0f ) {
		if( vNormal.x > 0 || vNormal.y > 0 || vNormal.z > 0) color = vec4(vNormal, 1.0);
		else color = vec4(-vNormal*0.5, 1.0);
	}
	else color = vObjectColor;
}

