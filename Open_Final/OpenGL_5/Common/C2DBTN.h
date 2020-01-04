#ifndef C2DBTN_H
#define C2DBTN_H

#include "../Header/Angel.h"
#include "CShape.h"

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

#define POINT_BTN_NUM 6

class C2DBTN :public CShape
{
public:
	C2DBTN();
	~C2DBTN();

	vec4 _originalLL, _originalTR; // sprite �b local �y�Ъ����U�P�k�W��ӳ��I�y��
	vec4 _LL, _TR;  // sprite �b�@�ɮy�Ъ����W�P�k�U��ӳ��I�y��
	vec4 _defaultColor; 
	bool _bPushed; //�O�_��������

	void setTRSMatrix(mat4& mat);
	void update(float dt);
	void update(float dt, point4 vLightPos, color4 vLight) {};
	void update(float dt, const LightSource& lights) {};
	void update(float dt, const LightSource* lights) {};

	void setDefaultColor(vec4 vColor);
	bool getButtonStatus() { return _bPushed; }
	bool onTouch(const vec2& tp);

	GLuint getShaderHandle() { return _vbo; }

	void draw();
	void drawW();




	

private:

};



#endif // !C2DBTN
