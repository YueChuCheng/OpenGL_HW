#include<cmath>
#include"CQuad.h"

CQuad::CQuad() {

	_iNumVtx = POINT_NUM;
	//_pPoints = NULL; _pNormals = NULL;
	_pTex = NULL;
	_pPoints = new vec4[_iNumVtx];
	_pNormals = new vec3[_iNumVtx];
	_pColors = new vec4[_iNumVtx];

	//點座標
	_pPoints[0] = vec4(-0.5f, 0.0f, 0.5f, 1.0f);
	_pPoints[1] = vec4(0.5f, 0.0f, 0.5f, 1.0f);
	_pPoints[2] = vec4(0.5f, 0.0f, -0.5f, 1.0f);
	_pPoints[3] = vec4(-0.5f, 0.0f, 0.5f, 1.0f);
	_pPoints[4] = vec4(0.5f, 0.0f, -0.5f, 1.0f);
	_pPoints[5] = vec4(-0.5f, 0.0f, -0.5f, 1.0f);

	//法向量
	_pNormals[0] = vec3(0, 1.0f, 0);
	_pNormals[1] = vec3(0, 1.0f, 0);
	_pNormals[2] = vec3(0, 1.0f, 0);
	_pNormals[3] = vec3(0, 1.0f, 0);
	_pNormals[4] = vec3(0, 1.0f, 0);
	_pNormals[5] = vec3(0, 1.0f, 0);

	//顏色
	for (int i = 0; i < _iNumVtx; i++) _pColors[i] = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	setShaderName("vsPerPixelLighting.glsl", "fsPerPixelLighting.glsl");

}

void CQuad::setNormal(vec3 vNormal) {
	_pNormals[0] = vNormal;
	_pNormals[1] = vNormal;
	_pNormals[2] = vNormal;
	_pNormals[3] = vNormal;
	_pNormals[4] = vNormal;
	_pNormals[5] = vNormal;
	
}


void CQuad::renderWithFlatShading(vec4 vLightPos, color4 vLight) {
	// 因為每一個平面的頂點的 Normal 都相同，所以此處並沒有計算此三個頂點的平均 Normal
	vec4 vCentroid;
	for (int i = 0; i < _iNumVtx; i+=3)
	{
		vCentroid = (_pPoints[i] + _pPoints[i + 1] + _pPoints[i + 2]) / 3.0f;
		_pColors[i] = _pColors[i + 1] = _pColors[i + 2] = phongReflectionModel(vCentroid,_pNormals[i],vLightPos,vLight);

	}
	glBindBuffer(GL_ARRAY_BUFFER,_vbo);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(vec4)*_iNumVtx+ sizeof(vec3) * _iNumVtx , sizeof(vec4) * _iNumVtx ,_pColors );
}

void CQuad::renderWithFlatShading(const LightSource& Lights)
{
	vec4 vCentroid;
	for (int i = 0; i < _iNumVtx; i += 3) {
		// 計算三角形的重心
		vCentroid = (_pPoints[i] + _pPoints[i + 1] + _pPoints[i + 2]) / 3.0f;
		_pColors[i] = _pColors[i + 1] = _pColors[i + 2] = phongReflectionModel(vCentroid, _pNormals[i], Lights.position, Lights.diffuse);
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors); // vertcies' Color
}

void CQuad::renderWithGouraudShading(vec4 vLightPos, color4 vLight) {

	_pColors[0] = _pColors[3] = phongReflectionModel(_pPoints[0],_pNormals[0],vLightPos,vLight);
	_pColors[2] = _pColors[4] = phongReflectionModel(_pPoints[2], _pNormals[2], vLightPos, vLight);
	_pColors[1] = phongReflectionModel(_pPoints[1], _pNormals[1], vLightPos, vLight);
	_pColors[5]  = phongReflectionModel(_pPoints[5], _pNormals[5], vLightPos, vLight);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors); // vertcies' Color


}


void CQuad::renderWithGouraudShading(const LightSource& Lights)
{
	
	_pColors[0] = _pColors[3] = phongReflectionModel(_pPoints[0], _pNormals[0], Lights);
	_pColors[2] = _pColors[4] = phongReflectionModel(_pPoints[2], _pNormals[2], Lights);
	_pColors[1] = phongReflectionModel(_pPoints[1], _pNormals[1], Lights);
	_pColors[5] = phongReflectionModel(_pPoints[5], _pNormals[5], Lights);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors); // vertcies' Color
}


// 此處所給的 vLightPos 必須是世界座標的確定絕對位置
void CQuad::update(float dt, vec4 vLightPos, color4 vLightI)
{

	/*if (_viewUpdated || _TRSUpdated) {
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}
	_vLightInView = _mxView * vLightPos;		// 將 Light 轉換到鏡頭座標再傳入
	// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
	_ambientProduct = _material.ka * _material.ambient * vLightI;
	_diffuseProduct = _material.kd * _material.diffuse * vLightI;
	_specularProduct = _material.ks * _material.specular * vLightI;*/


}

void CQuad::update(float dt, const LightSource* Lights)
{

	if (_viewUpdated || _TRSUpdated) {
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}
	/*vec3 vLightDir = _mxMVFinal * Lights[1].spotDirection;

	// 5. 計算照明的點是否落在 spotCutoff 之內
	GLfloat fLdotLDir = -(vLight.x * vLightDir.x + vLight.y * vLightDir.y + vLight.z * vLightDir.z);
	*/
	for (int i = 0; i < LIGHT_NUM; i++)
	{
		_vLightInView[i] = _mxView * Lights[i].position;		// 將 Light 轉換到鏡頭座標再傳入
		_vLightDir[i] = _mxView * Lights[i].spotDirection;
		// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
		_ambientProduct[i] = _material.ka * _material.ambient * Lights[i].ambient;
		_diffuseProduct[i] = _material.kd * _material.diffuse * Lights[i].diffuse ;
		_specularProduct[i] = _material.ks * _material.specular * Lights[i].specular;
		_cutoff[i] = Lights[i].spotCutoff;
		
	}
	

}

void CQuad::update(float dt, const LightSource& Lights)
{

	/*if (_viewUpdated || _TRSUpdated) {
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}
	_vLightInView = _mxView * Lights.position;		// 將 Light 轉換到鏡頭座標再傳入
	// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
	_ambientProduct = _material.ka * _material.ambient * Lights.ambient;
	_diffuseProduct = _material.kd * _material.diffuse * Lights.diffuse;
	_specularProduct = _material.ks * _material.specular * Lights.specular;*/

}

void CQuad::update(float dt) {
	if (_viewUpdated||_TRSUpdated) {
		_mxMVFinal = _mxView * _mxTRS;
		_mxITMV = InverseTransposeMatrix(_mxMVFinal);
		_viewUpdated = _TRSUpdated = false;

	}

}

void CQuad::setVtxColors(vec4 vLFColor, vec4 vLRColor, vec4 vTRColor, vec4 vTLColor) {
	_pColors[3] = _pColors[0] = vLFColor;
	_pColors[1] = vLRColor;
	_pColors[4] = _pColors[2] = vTRColor;
	_pColors[5] = vTLColor;

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors); // vertcies' Color

}

void CQuad::setVtxColors(vec4 vFColor, vec4 vSColor) {
	_pColors[0] = _pColors[1] = _pColors[2] = vFColor;
	_pColors[3] = _pColors[4] = _pColors[5] = vSColor;

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors); // vertcies' Color

}

void CQuad::draw() {
	drawingSetShader();
	glDrawArrays(GL_TRIANGLES, 0, POINT_NUM);

}
void CQuad::drawW() {
	drawingWithoutSetShader();
	glDrawArrays(GL_TRIANGLES, 0, POINT_NUM);

}



void CQuad::setLightNUM(int num) {

	_vLightNUM = num;

}
