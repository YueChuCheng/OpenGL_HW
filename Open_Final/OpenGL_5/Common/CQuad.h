#ifndef CQUAD_H
#define CQUAD_H
#include "../Header/Angel.h"
#include"CShape.h"

#define POINT_NUM 6 //2 faces, 2 triangles/face 


class CQuad : public CShape
{
public:
	CQuad ();
	~CQuad() {};

	void update(float dt, const LightSource* lights);
	void update(float dt, vec4 vLightPos, color4 vLightI);
	void update(float dt, const LightSource &Lights);
	void update(float dt);
	void setLightNUM(int num);
	GLuint getShaderHandle() { return _vbo; }
	void renderWithFlatShading(vec4 vLightPos, color4 vLight);//  vLightI: Light Intensity
	void renderWithGouraudShading(vec4 vLightPos, color4 vLight);//  vLightI: Light Intensity
	void renderWithFlatShading(const LightSource& Lights);
	void renderWithGouraudShading(const LightSource& Lights);
	void setVtxColors(vec4 vLFColor , vec4 vLRColor, vec4 vTRColor, vec4 vTLColor );
	void setVtxColors(vec4 vFColor , vec4 vSColor);
	void setNormal(vec3 vNormal); //更改法向量

	void draw();
	void drawW();


private:

};
#endif
