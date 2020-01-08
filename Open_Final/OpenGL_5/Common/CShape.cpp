#include "CShape.h"



CShape::CShape() {
	// ambient 預設為 0, diffuse, specular 的顏色都是灰色 0.5
	// Ka = 0 係數 , kd = 0.8 , ks = 0.2
	_material.ambient = vec4(vec3(0));
	_material.diffuse = vec4(vec3(0.5f));
	_material.specular = vec4(vec3(0.5f));
	_material.ka = 0; _material.kd = 0.8f; _material.ks = 0.2f;
	_material.shininess = 2.0f;

	_iMode = FLAT_SHADING;

	// 預設為 RGBA 為 (0.5,0.5,0.5,1.0) , 由這個灰階顏色來代表的物件顏色
	_Color[0] = 0.5f; _Color[1] = 0.5f; _Color[2] = 0.5f; _Color[3] = 1.0f;
	for (int i = 0; i < LIGHT_NUM; i++)
	{
		_iLighting[i] = 1; // 預設接受燈光的照明

	}
	

}

//收回空間
CShape::~CShape() {
	if (_pPoints != NULL)delete[] _pPoints;
	if (_pNormals != NULL)delete[] _pNormals;
	if (_pColors != NULL)delete[] _pColors;
	if (_pTex != NULL)delete[] _pTex;

	if (_pVXshader != NULL)delete[]_pVXshader;
	if (_pFShader != NULL)delete[] _pFShader;
}

void CShape::createBufferObject()
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Create and initialize a buffer object
	glGenBuffers(1, &_uiBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx + sizeof(vec4) * _iNumVtx, NULL, GL_STATIC_DRAW);
	//sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx + sizeof(vec4)*m_iNumVtx <- vertices, normal and color

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * _iNumVtx, _pPoints);  // vertices
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx, sizeof(vec3) * _iNumVtx, _pNormals); // // vertices' normal
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors); // vertcies' Color
}


void CShape::setShader(GLuint shaderHandle) {

	createBufferObject();

	if (shaderHandle == MAX_UNSIGNED_INT)_vbo = InitShader(_pVXshader, _pFShader);
	else _vbo = shaderHandle;
	GLuint vPosition = glGetAttribLocation(_vbo, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(_vbo, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4) * _iNumVtx));
	
	GLuint vColorVtx = glGetAttribLocation(_vbo, "vVtxColor");  // vertices' color 
	glEnableVertexAttribArray(vColorVtx);
	glVertexAttribPointer(vColorVtx, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx));

	_uiModelView = glGetUniformLocation(_vbo, "ModelView");
	// m_mxMVFinal , m_mxModelView 宣告時就是單位矩陣
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);

	_uiProjection = glGetUniformLocation(_vbo, "Projection");
	// m_mxProjection 宣告時就是單位矩陣
	glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);

	_uiColor = glGetUniformLocation(_vbo, "vObjectColor");
	glUniform4fv(_uiColor, 1, _Color);


	//lighting with GPU

	_uiShininess = glGetUniformLocation(_vbo, "fShininess");
	glUniform1f(_uiShininess, _material.shininess);

	
	_uiLightInView[0] = glGetUniformLocation(_vbo, "LightInView[0]");
	glUniform4fv(_uiLightInView[0], 1, _vLightInView[0]);

	_uiLightInView[1] = glGetUniformLocation(_vbo, "LightInView[1]");
	glUniform4fv(_uiLightInView[1], 1, _vLightInView[1]);

	_uiLightInView[2] = glGetUniformLocation(_vbo, "LightInView[2]");
	glUniform4fv(_uiLightInView[2], 1, _vLightInView[2]);

	_uiLightInView[3] = glGetUniformLocation(_vbo, "LightInView[3]");
	glUniform4fv(_uiLightInView[3], 1, _vLightInView[3]);



	_uiLightNUM = glGetUniformLocation(_vbo, "LightNUM");
	glUniform1i(_uiLightNUM, _vLightNUM);


		
	
	_uiAmbient[0] = glGetUniformLocation(_vbo, "AmbientProduct[0]");
	glUniform4fv(_uiAmbient[0], 1, _ambientProduct[0]);

	_uiAmbient[1] = glGetUniformLocation(_vbo, "AmbientProduct[1]");
	glUniform4fv(_uiAmbient[1], 1, _ambientProduct[1]);

	_uiAmbient[2] = glGetUniformLocation(_vbo, "AmbientProduct[2]");
	glUniform4fv(_uiAmbient[2], 1, _ambientProduct[2]);

	_uiAmbient[3] = glGetUniformLocation(_vbo, "AmbientProduct[3]");
	glUniform4fv(_uiAmbient[3], 1, _ambientProduct[3]);
		

	_uiDiffuseProduct[0] = glGetUniformLocation(_vbo, "DiffuseProduct[0]");
	glUniform4fv(_uiDiffuse[0], 1, _diffuseProduct[0]);
		
	_uiDiffuseProduct[1] = glGetUniformLocation(_vbo, "DiffuseProduct[1]");
	glUniform4fv(_uiDiffuse[1], 1, _diffuseProduct[1]);

	_uiDiffuseProduct[2] = glGetUniformLocation(_vbo, "DiffuseProduct[2]");
	glUniform4fv(_uiDiffuse[2], 1, _diffuseProduct[2]);

	_uiDiffuseProduct[3] = glGetUniformLocation(_vbo, "DiffuseProduct[3]");
	glUniform4fv(_uiDiffuse[3], 1, _diffuseProduct[3]);
		
	_uiSpecular[0] = glGetUniformLocation(_vbo, "SpecularProduct[0]");
	glUniform4fv(_uiSpecular[0], 1, _specularProduct[0]);

	_uiSpecular[1] = glGetUniformLocation(_vbo, "SpecularProduct[1]");
	glUniform4fv(_uiSpecular[1], 1, _specularProduct[1]);
		
	_uiSpecular[2] = glGetUniformLocation(_vbo, "SpecularProduct[2]");
	glUniform4fv(_uiSpecular[2], 1, _specularProduct[2]);

	_uiSpecular[3] = glGetUniformLocation(_vbo, "SpecularProduct[3]");
	glUniform4fv(_uiSpecular[3], 1, _specularProduct[3]);
		
	_uiLighting[0] = glGetUniformLocation(_vbo, "iLighting[0]");
	glUniform1i(_uiLighting[0], _iLighting[0]);

	_uiLighting[1] = glGetUniformLocation(_vbo, "iLighting[1]");
	glUniform1i(_uiLighting[1], _iLighting[1]);
		
	_uiLighting[2] = glGetUniformLocation(_vbo, "iLighting[2]");
	glUniform1i(_uiLighting[2], _iLighting[2]);

	_uiLighting[3] = glGetUniformLocation(_vbo, "iLighting[3]");
	glUniform1i(_uiLighting[3], _iLighting[3]);
	
	_uicutoff[0] = glGetUniformLocation(_vbo, "Cutoff[0]");
	glUniform1f(_uicutoff[0], _cutoff[0]);

	_uicutoff[1] = glGetUniformLocation(_vbo, "Cutoff[1]");
	glUniform1f(_uicutoff[1], _cutoff[1]);
		
	_uicutoff[2] = glGetUniformLocation(_vbo, "Cutoff[2]");
	glUniform1f(_uicutoff[2], _cutoff[2]);

	_uicutoff[3] = glGetUniformLocation(_vbo, "Cutoff[3]");
	glUniform1f(_uicutoff[3], _cutoff[3]);


	_uiLightDir[0] = glGetUniformLocation(_vbo, "vlightDir[0]");
	glUniform4fv(_uiLightDir[0], 1, _vLightDir[0]);

	_uiLightDir[1] = glGetUniformLocation(_vbo, "vlightDir[1]");
	glUniform4fv(_uiLightDir[0], 1, _vLightDir[1]);

	_uiLightDir[2] = glGetUniformLocation(_vbo, "vlightDir[2]");
	glUniform4fv(_uiLightDir[0], 1, _vLightDir[2]);

	_uiLightDir[3] = glGetUniformLocation(_vbo, "vlightDir[3]");
	glUniform4fv(_uiLightDir[0], 1, _vLightDir[3]);
		
		


	glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void CShape::drawingSetShader() {

	glUseProgram(_vbo);
	glBindVertexArray(_vao);
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);
	if (_viewUpdated || _TRSUpdated) { // Model View 的相關矩陣內容有更動
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);
	
	if (_projUpdated) {
		glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);
		_projUpdated = false;
	}

	glUniform4fv(_uiColor, 1, _Color);
	for (int i = 0; i < LIGHT_NUM; i++)
	{
		glUniform4fv(_uiLightInView[i], 1, _vLightInView[i]);
		glUniform4fv(_uiAmbient[i], 1, _ambientProduct[i]);
		glUniform4fv(_uiDiffuseProduct[i], 1, _diffuseProduct[i]);
		glUniform4fv(_uiSpecular[i], 1, _specularProduct[i]);
		glUniform1i(_uiLighting[i], _iLighting[i]);
		glUniform1f(_uicutoff[i], _cutoff[i]);
	}
	
	
	
	glUniform1f(_uiShininess, _material.shininess);
	

}

// 此處預設使用前一個描繪物件所使用的 Shader
void CShape::drawingWithoutSetShader()
{
	glBindVertexArray(_vao);
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);

	if (_viewUpdated || _TRSUpdated) { // Model View 的相關矩陣內容有更動
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);

	if (_projUpdated) {
		glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);
		_projUpdated = false;
	}

	glUniform4fv(_uiColor, 1, _Color);
	for (int i = 0; i < LIGHT_NUM; i++)
	{
		glUniform4fv(_uiLightInView[i], 1, _vLightInView[i]);
		glUniform4fv(_uiAmbient[i], 1, _ambientProduct[i]);
		glUniform4fv(_uiDiffuseProduct[i], 1, _diffuseProduct[i]);
		glUniform4fv(_uiSpecular[i], 1, _specularProduct[i]);
		glUniform1i(_uiLighting[i], _iLighting[i]);
		glUniform1f(_uicutoff[i], _cutoff[i]);
	}
	
	
	
	glUniform1f(_uiShininess, _material.shininess);
	
}


//複製vxShader、fsShader名稱
void CShape::setShaderName(const char vxShader[], const char fsShader[])
{
	int len;
	len = strlen(vxShader);
	_pVXshader = new char[len + 1];
	memcpy(_pVXshader,vxShader,len+1);

	len = strlen(fsShader);
	_pFShader = new char[len + 1];
	memcpy(_pFShader, fsShader, len + 1);

}

void CShape::setViewMatrix(mat4& mat) {
	_mxView = mat;
	_viewUpdated = true;
}

void CShape::setTRSMatrix(mat4& mat) {
	_mxTRS = mat;
	_TRSUpdated = true;
}

void CShape::setProjectionMatrix(mat4& mat) {
	_mxProjection = mat;
	_projUpdated = true;

}

void CShape::setColor(vec4 vColor) {
	_Color[0] = vColor.x;
	_Color[1] = vColor.y;
	_Color[2] = vColor.z;
	_Color[3] = vColor.w;
	glUniform4fv(_uiColor, 1, _Color);
}

void CShape::setMaterials(color4 ambient, color4 diffuse, color4 specular) {

	_material.ambient = ambient;
	_material.diffuse = diffuse;
	_material.specular = specular;

}

void CShape::setKaKdKsShini(float ka, float kd, float ks, float shininess) // ka kd ks shininess
{
	_material.ka = ka;
	_material.kd = kd;
	_material.ks = ks;
	_material.shininess = shininess;
}


vec4 CShape::phongReflectionModel(vec4 vPoint, vec3 vNormal, vec4 vLightPos, color4 vLightI) {

	vec3 vN;
	vec4 vColor;
	vec4 vPosInView , vLightInView;

	// 1.將點頂轉到鏡頭座標系統
	vPosInView = _mxMVFinal * vPoint;

	// 2.將面的 Normal 轉到鏡頭座標系統，並轉成單位向量 有加跟沒加一樣
	//#ifdef GENERAL_CASE 
	//	//	  m_mxITMV 已經在呼叫 SetModelViewMatrix(mat4 &mat) 與 SetTRSMatrix(mat4 &mat) 就同時計算
	//	//    此處就不用再重新計算
	//	vN = m_mxITMV * vNormal;
	//#else
	//vN = m_mxMV3X3Final * vNormal;
	//#endif

	//vN = normalize(vN);

	// 3. 計算 Ambient color :  Ia = Ka * Material.ambient * La
	vColor = _material.ka * _material.ambient * vLightI;

	// 4. 計算 Light 在鏡頭座標系統的位置
//    此處假設 Light 的位置已經在世界座標的絕對位置
	vLightInView = _mxView * vLightPos;

	vec3 vLight; // 用 vec3 來宣告是為了節省計算, 如果要讓程式寫起來更方便，可改用 vec4 來宣告
	vLight.x = vLightInView.x - vPosInView.x;
	vLight.y = vLightInView.y - vPosInView.y;
	vLight.z = vLightInView.z - vPosInView.z;
	vLight = normalize(vLight); // normalize light vector

	// 5. 計算 L dot N
	GLfloat fLdotN = vLight.x * vN.x + vLight.y * vN.y + vLight.z * vN.z; //燈光與物品法向量內積
	if (fLdotN > 0) { // 該點被光源照到才需要計算
		// Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
		vColor += _material.kd * _material.diffuse * vLightI * fLdotN;

		// Specular color
		// Method 1: Phone Model
		//   計算 View Vector
		vec3 vView;
		vView.x = 0 - vPosInView.x;  // 目前已經以鏡頭座標為基礎, 所以 View 的位置就是 (0,0,0)
		vView.y = 0 - vPosInView.y;
		vView.z = 0 - vPosInView.z;
		vView = normalize(vView);

		//	 計算 Light 的 反射角 vRefL
		vec3 vRefL = 2.0f * (fLdotN)*vN - vLight;

		vRefL = normalize(vRefL);
		//   計算  vReflectedL dot View
		GLfloat RdotV = vRefL.x * vView.x + vRefL.y * vView.y + vRefL.z * vView.z;

		// Specular Color : Is = Ks * Ls * (R dot V)^Shininess;
		if (RdotV > 0) vColor += _material.ks * _material.specular * vLightI * powf(RdotV, _material.shininess);
	}

	vColor.w = 1; // Alpha 改設定成 1，預設都是不透明物體
	// Method 2: Modified Phone Model 

	// 將顏色儲存到  m_Colors 中，因為 Quad 是兩個共平面的三角面所構成, 所以設定兩個三角面都有相同的顏色
	// 也就是設定所有的頂點都是這個顏色
	return vColor;


}


vec4 CShape::phongReflectionModel(vec4 vPoint, vec3 vNormal, const LightSource& Lights)
{
	vec3 vN, vLDir;		// 用 vec3 來宣告是為了節省計算, 如果要讓程式寫起來更方便，可改用 vec4 來宣告
	vec4 vColor;	// 該頂點的顏色
	vec4 vPosInView, vLightInView;


	// 1. 將點頂轉到鏡頭座標系統，因為頂點會根據 TRS Matrix 移動，所以必須以 m_mxMVFinal 計算在世界座標的絕對位置
	vPosInView = _mxMVFinal * vPoint;

	// 2. 將面的 Normal 轉到鏡頭座標系統，並轉成單位向量
	//    只有物件的 rotation 會改變 Normal 的方向，乘上物件的 Final ModelView Matrix，同時轉換到鏡頭座標
	//    m_mxMV3X3Final 在 Update 處取自 m_mxMVFinal 的左上的 3X3 部分, 不需要位移量
#ifdef GENERAL_CASE
	//	  m_mxITMV 已經在呼叫 SetModelViewMatrix(mat4 &mat) 與 SetTRSMatrix(mat4 &mat) 就同時計算
	//    此處就不用再重新計算
	vN = m_mxITMV * vNormal;
#else
	vN = _mxMV3x3Final * vNormal;
#endif

	vN = normalize(vN);

	// 3. 計算 Ambient color :  Ia = Ka * Material.ambient * La
	vColor = _material.ka * _material.ambient * Lights.diffuse;

	// 4. 計算 Light 在鏡頭座標系統的位置
	//    此處假設 Light 的位置已經在世界座標的絕對位置
	vLightInView = _mxView * Lights.position;

	vec3 vLight; // 用 vec3 來宣告是為了節省計算, 如果要讓程式寫起來更方便，可改用 vec4 來宣告
	vLight.x = vLightInView.x - vPosInView.x;
	vLight.y = vLightInView.y - vPosInView.y;
	vLight.z = vLightInView.z - vPosInView.z;
	vLight = normalize(vLight); // normalize light vector

	vec3 vLightDir = _mxMV3x3Final * Lights.spotDirection;

	// 5. 計算照明的點是否落在 spotCutoff 之內
	GLfloat fLdotLDir = -(vLight.x * vLightDir.x + vLight.y * vLightDir.y + vLight.z * vLightDir.z); //多一負號因為兩向量不為同方向，因此其夾角大於90度

	if (fLdotLDir >= Lights.spotCosCutoff) //需位於spotlight 範圍內
	{
		// 5. 計算 L dot N
		GLfloat fLdotN = vLight.x * vN.x + vLight.y * vN.y + vLight.z * vN.z;
		color4 fLightI = Lights.diffuse * powf(fLdotLDir, Lights.spotExponent);

		// Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
		vColor += _material.kd * _material.diffuse * fLightI * fLdotN;

		// Specular color
		// Method 1: Phone Model
		//   計算 View Vector
		vec3 vView;
		vView.x = 0 - vPosInView.x;  // 目前已經以鏡頭座標為基礎, 所以 View 的位置就是 (0,0,0)
		vView.y = 0 - vPosInView.y;
		vView.z = 0 - vPosInView.z;
		vView = normalize(vView);

		//	 計算 Light 的 反射角 vRefL
		vec3 vRefL = 2.0f * (fLdotN)*vN - vLight;

		vRefL = normalize(vRefL);
		//   計算  vReflectedL dot View
		GLfloat RdotV = vRefL.x * vView.x + vRefL.y * vView.y + vRefL.z * vView.z;

		// Specular Color : Is = Ks * Ls * (R dot V)^Shininess;
		if (RdotV > 0) vColor += _material.ks * _material.specular * fLightI * powf(RdotV, _material.shininess);
	}

	vColor.w = 1; // Alpha 改設定成 1，預設都是不透明物體
	// Method 2: Modified Phone Model 

	// 將顏色儲存到  m_Colors 中，因為 Quad 是兩個共平面的三角面所構成, 所以設定兩個三角面都有相同的顏色
	// 也就是設定所有的頂點都是這個顏色
	return vColor;
}