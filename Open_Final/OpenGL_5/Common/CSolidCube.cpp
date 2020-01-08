#include "CSoildCube.h"

CSolidCube::CSolidCube()
{
	_iNumVtx  = POINT_CUBE_NUM;
	_pPoints  = new vec4[_iNumVtx];
	_pNormals = new vec3[_iNumVtx];
	_pColors  = new vec4[_iNumVtx];
	_pTex     = new vec2[_iNumVtx];

	_vertices[0] = point4(-0.5, -0.5, 0.5, 1.0);
	_vertices[1] = point4(-0.5, 0.5, 0.5, 1.0);
	_vertices[2] = point4(0.5, 0.5, 0.5, 1.0);
	_vertices[3] = point4(0.5, -0.5, 0.5, 1.0);
	_vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
	_vertices[5] = point4(-0.5, 0.5, -0.5, 1.0);
	_vertices[6] = point4(0.5, 0.5, -0.5, 1.0);
	_vertices[7] = point4(0.5, -0.5,-0.5, 1.0);

	_iIndex = 0;

	Quad(1, 0, 3, 2);
	Quad(2, 3, 7, 6);
	Quad(3, 0, 4, 7);
	Quad(6, 5, 1, 2);
	Quad(4, 5, 6, 7);
	Quad(5, 4, 0, 1);

	//預設將所有的面都設定成灰色
	for (int i = 0; i < _iNumVtx; i++)
	{
		_pColors[i] = vec4(-1.0f, -1.0f, -1.0f, 1.0f);
		setShaderName("vsPerPixelLighting.glsl", "fsPerPixelLighting.glsl");
	}

	setMaterials(vec4(0), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	setKaKdKsShini(0, 0.8f, 0.2f, 1);
	

}

CSolidCube::~CSolidCube()
{
}



void CSolidCube::Quad(int a, int b, int c, int d)
{
	//計算法向量，因同一面所以法向量會相同
	vec4 u = _vertices[b] - _vertices[a];
	vec4 v = _vertices[c] - _vertices[b];
	vec3 normal = normalize(cross(u , v));

	//將法向量的結果放入變數中 ， 將四點的資料依照三角形的成形順序依序放入變數
	_pNormals[_iIndex] = normal; _pPoints[_iIndex] = _vertices[a] ;_iIndex++;
	_pNormals[_iIndex] = normal; _pPoints[_iIndex] = _vertices[b] ;_iIndex++;
	_pNormals[_iIndex] = normal; _pPoints[_iIndex] = _vertices[c] ;_iIndex++;
	_pNormals[_iIndex] = normal; _pPoints[_iIndex] = _vertices[a] ;_iIndex++;
	_pNormals[_iIndex] = normal; _pPoints[_iIndex] = _vertices[c] ;_iIndex++;
	_pNormals[_iIndex] = normal; _pPoints[_iIndex] = _vertices[d] ;_iIndex++;



}


void CSolidCube::draw() {
	drawingSetShader();
	glDrawArrays(GL_TRIANGLES, 0, POINT_CUBE_NUM);
}


void CSolidCube::drawW() {
	drawingWithoutSetShader();
	glDrawArrays(GL_TRIANGLES, 0, POINT_CUBE_NUM);
}




void CSolidCube::update(float dt, point4 vLightPos, color4 vLightI)
{

	/*if (_viewUpdated || _TRSUpdated) {
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}


	for (int i = 0; i < LIGHT_NUM; i++)
	{
		_vLightInView[i] = _mxView * vLightPos;		// 將 Light 轉換到鏡頭座標再傳入
	// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
		_ambientProduct[i] = _material.ka * _material.ambient * vLightI;
		_diffuseProduct[i] = _material.kd * _material.diffuse * vLightI;
		_specularProduct[i] = _material.ks * _material.specular * vLightI;
	}
	*/

}


void CSolidCube::update(float dt, const LightSource& Lights)
{

	/*if (_viewUpdated || _TRSUpdated) {
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}
	for (int i = 0; i < LIGHT_NUM; i++)
	{
		_vLightInView[i] = _mxView * Lights;		// 將 Light 轉換到鏡頭座標再傳入
	// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
		_ambientProduct[i] = _material.ka * _material.ambient * vLightI;
		_diffuseProduct[i] = _material.kd * _material.diffuse * vLightI;
		_specularProduct[i] = _material.ks * _material.specular * vLightI;
	}*/

}


void CSolidCube::update(float dt, const LightSource* Lights)
{

	if (_viewUpdated || _TRSUpdated) {
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}
	for (int i = 0; i < LIGHT_NUM; i++)
	{
		_vLightDir[i] = _mxView * Lights[i].spotDirection;
		_vLightInView[i] = _mxView * Lights[i].position;		// 將 Light 轉換到鏡頭座標再傳入
	// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
		_ambientProduct[i] = _material.ka * _material.ambient * Lights[i].ambient;
		_diffuseProduct[i] = _material.kd * _material.diffuse * Lights[i].diffuse;
		_specularProduct[i] = _material.ks * _material.specular * Lights[i].specular;
		_cutoff[i] = Lights[i].spotCutoff;

	}

}



void CSolidCube::update(float dt)
{
	if (_viewUpdated || _TRSUpdated) {
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}
}