#include "header/Angel.h"
#include "Common/TypeDefine.h"
#include "Common/CQuad.h"
#include "Common/CSmoothQuad.h"
#include "Common/C2DBTN.h"
#include "Common/CSoildCube.h"
#include "Common/test.h"
#include "Common/CCamera.h"


#define SPACE_KEY 32
#define SCREEN_SIZE 600
#define HALF_SIZE SCREEN_SIZE /2 
#define VP_HALFWIDTH  20.0f
#define VP_HALFHEIGHT 20.0f
#define GRID_SIZE 40 // must be an even number


CSolidCube* CCube[3];

CQuad* CQ_leftWall, * CQ_rightWall;
CQuad* CQ_frontWall, * CQ_backWall;


CSmoothQuad* CFloor;
CSmoothQuad* CQ_ceiling;

ModelPool* ObjModel;


//button
C2DBTN* CBtn[4];

GLfloat g_fRadius = 40.0; //視覺範圍
GLfloat g_fTheta = 60.0f * DegreesToRadians;
GLfloat g_fPhi = 45.0f * DegreesToRadians;
GLfloat g_fCameraMoveX = 0.f;				// for camera movment
GLfloat g_fCameraMoveY = 7.0f;				// for camera movment
GLfloat g_fCameraMoveZ = 0.f;				// for camera movment
mat4	g_matMoveDir;		// 鏡頭移動方向
point4  g_MoveDir;
point4  g_at;				// 鏡頭觀看方向
point4  g_eye;				// 鏡頭位置



//camera
point4  eye(g_fRadius* sin(g_fTheta)* sin(g_fPhi), g_fRadius* cos(g_fTheta), g_fRadius* sin(g_fTheta)* cos(g_fPhi), 1.0f);
point4  at(0.0f, 0.0f, 0.0f, 1.0f); 
point4 g_vUp(0.0, 1.0, 0.0, 0.0);
point4 front;
point4 right;

// Part 2 : for single light source
bool g_bAutoRotating = false;
float _fLightDelta = 0; //燈光旋轉，經過時間
float _fLightRadius = 6;//燈光位置
float _fLightTheta = 0;//燈光旋轉角度

float g_fLightR = 0.7f;
float g_fLightG = 0.7f;
float g_fLightB = 0.7f;

//light
vec4 vlight_Center(0.0f, 10.0f, 0.0f, 1.0f);
vec4 vlight_Center_Color(g_fLightR, g_fLightG, g_fLightB, 1.0f);



LightSource  Light_center = {

	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(_fLightRadius, 15.0f, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),		  //spotTarget
	vec3(_fLightRadius, 0.0f, 0.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	0.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};



LightSource  _Light1 = {
	color4(0.0, 0.0, 0.0, 1.0f), // ambient 
	color4(1.0, 0.0, 0.0, 1.0f), // diffuse
	color4(0.0, 0.0, 0.0, 1.0f), // specular
	point4(0.0, 17.0, -10.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(10.0f, 0.0f, 10.0f),		  //spotTarget
	vec3(0.0, 0.0, -10.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	0.95f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource  _Light2 = {

	color4(0.0, 0.0, 0.0, 1.0f), // ambient 
	color4(0.0, 1.0, 0.0, 1.0f), // diffuse
	color4(0.0, 0.0, 0.0, 1.0f), // specular
	point4(-10.4, 17.0, 6.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(10.0f, 0.0f, 10.0f),		  //spotTarget
	vec3(-10.4, 0.0, 6.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	0.95f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource  _Light3 = {

	color4(0.0, 0.0, 0.0, 1.0f), // ambient 
	color4(0.0, 0.0, 1.0, 1.0f), // diffuse
	color4(0.0, 0.0, 0.0, 1.0f), // specular
	point4(10.4, 17.0, 6.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(10.0f, 0.0f, 10.0f),		  //spotTarget
	vec3(10.4, 0.0, 6.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	0.95f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource Light_resulte[LIGHT_NUM] = { //最後燈光結果

	Light_center, _Light1 ,  _Light2 ,  _Light3
};


// 函式的原型宣告
extern void IdleProcess();

void init( void )
{


	mat4 mxT , mxS;
	vec4 vT, vColor;
	// 產生所需之 Model View 與 Projection Matrix

	
	
	auto camera = CCamera::create();
	camera->updateViewLookAt(eye, at);
	camera->updatePerspective(60.0, (GLfloat)SCREEN_SIZE / (GLfloat)SCREEN_SIZE, 1.0, 1000.0);
	

	CFloor = new CSmoothQuad(GRID_SIZE);
	CFloor->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CFloor->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CFloor->setShadingMode(GOURAUD_SHADING);
	CFloor->setShader();
	
	vec3 vceilingNormal =vec3(0.0f, -1.0f, 0.0f);
	CQ_ceiling = new CSmoothQuad(GRID_SIZE,20.0f);
	CQ_ceiling->setNormal(vceilingNormal);
	CQ_ceiling->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQ_ceiling->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CQ_ceiling->setShadingMode(GOURAUD_SHADING);
	CQ_ceiling->setShader();
	
	





	vT.x = -20.0f; vT.y = 20.0f; vT.z = 0;
	mxT = Translate(vT);
	CQ_leftWall = new CQuad;
	//CQ_leftWall->setNormal(vec3(-1.0, 0.0, 0.0));
	CQ_leftWall->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQ_leftWall->setShadingMode(GOURAUD_SHADING);
	CQ_leftWall->setLightNUM(1);
	CQ_leftWall->setShader();
	CQ_leftWall->setColor(vec4(0.6f));
	CQ_leftWall->setTRSMatrix(mxT * RotateZ(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQ_leftWall->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	

	vT.x = 20.0f; vT.y = 20.0f; vT.z = 0;
	mxT = Translate(vT);
	CQ_rightWall = new CQuad;
	//CQ_rightWall->setNormal(vec3(1.0, 0.0, 0.0));
	CQ_rightWall->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQ_rightWall->setShadingMode(GOURAUD_SHADING);
	CQ_rightWall->setLightNUM(1);
	CQ_rightWall->setShader();
	CQ_rightWall->setColor(vec4(0.6f));
	CQ_rightWall->setTRSMatrix(mxT * RotateZ(90.0f) * Scale(40.0f, 1, 40.0f));
	CQ_rightWall->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	

	vT.x = 0.0f; vT.y = 20.0f; vT.z = 20.0f;
	mxT = Translate(vT);
	CQ_frontWall = new CQuad;
	//CQ_frontWall->setNormal(vec3(0.0, 0.0, 1.0));
	CQ_frontWall->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQ_frontWall->setShadingMode(GOURAUD_SHADING);
	CQ_frontWall->setLightNUM(1);
	CQ_frontWall->setShader();
	CQ_frontWall->setColor(vec4(0.6f));
	CQ_frontWall->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQ_frontWall->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	

	vT.x = 0.0f; vT.y = 20.0f; vT.z = -20.0f;
	mxT = Translate(vT);
	CQ_backWall = new CQuad;
	//CQ_backWall->setNormal(vec3(0.0, 0.0, -1.0));
	CQ_backWall->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQ_backWall->setShadingMode(GOURAUD_SHADING);
	CQ_backWall->setLightNUM(1);
	CQ_backWall->setShader();
	CQ_backWall->setColor(vec4(0.6f));
	CQ_backWall->setTRSMatrix(mxT * RotateX(90.0f) * Scale(40.0f, 1, 40.0f));
	CQ_backWall->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	

	

	CBtn[0] = new C2DBTN;
	vColor.x = 1; vColor.y = 0; vColor.z = 0;
	CBtn[0]->setDefaultColor(vColor);
	CBtn[0]->setShader();
	mxS = Scale(0.1f, 0.1f, 0.1f);
	mxT = Translate(0.9f, -0.9f, 0);
	CBtn[0]->setTRSMatrix(mxT * mxS);

	CBtn[1] = new C2DBTN;
	vColor.x = 0; vColor.y = 1; vColor.z = 0;
	CBtn[1]->setDefaultColor(vColor);
	CBtn[1]->setShader();
	mxS = Scale(0.1f, 0.1f, 0.1f);
	mxT = Translate(0.75f, -0.9f, 0);
	CBtn[1]->setTRSMatrix(mxT * mxS);

	CBtn[2] = new C2DBTN;
	vColor.x = 0; vColor.y = 0; vColor.z = 1;
	CBtn[2]->setDefaultColor(vColor);
	CBtn[2]->setShader();
	mxS = Scale(0.1f, 0.1f, 0.1f);
	mxT = Translate(0.6f, -0.9f, 0);
	CBtn[2]->setTRSMatrix(mxT * mxS);

	CBtn[3] = new C2DBTN;
	vColor.x = 1; vColor.y = 1; vColor.z = 1;
	CBtn[3]->setDefaultColor(vColor);
	CBtn[3]->setShader();
	mxS = Scale(0.1f, 0.1f, 0.1f);
	mxT = Translate(0.45f, -0.9f, 0);
	CBtn[3]->setTRSMatrix(mxT* mxS);


	CCube[0] = new CSolidCube;
	//set material
	CCube[0]->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CCube[0]->setKaKdKsShini(0.15f, 0.8f, 0.2f, 2);
	CCube[0]->setShader();
	vT.x = 0.0; vT.y = 1.0; vT.z = -10.0f;
	mxT = Translate(vT);
	CCube[0]->setTRSMatrix(mxT);
	CCube[0]->setShadingMode(GOURAUD_SHADING);



	CCube[1] = new CSolidCube;
	//set material
	CCube[1]->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CCube[1]->setKaKdKsShini(0.15f, 0.8f, 0.2f, 2);
	CCube[1]->setShader();
	vT.x = -10.4; vT.y = 1.0; vT.z = 6.0f;
	mxT = Translate(vT);
	CCube[1]->setTRSMatrix(mxT);
	CCube[1]->setShadingMode(GOURAUD_SHADING);


	CCube[2] = new CSolidCube;
	//set material
	CCube[2]->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CCube[2]->setKaKdKsShini(0.15f, 0.8f, 0.2f, 2);
	CCube[2]->setShader();
	vT.x = 10.4; vT.y = 1.0; vT.z = 6.0f;
	mxT = Translate(vT);
	CCube[2]->setTRSMatrix(mxT);
	CCube[2]->setShadingMode(GOURAUD_SHADING);



	vT.x = 0.0; vT.y = 1.0; vT.z = 0.0f;
	ObjModel = new ModelPool("Model/TeaPot.obj");
	ObjModel->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	ObjModel->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	ObjModel->setLightNUM(1);
	ObjModel->setShader();
	mxT = Translate(vT);
	ObjModel->setTRSMatrix(mxT* Scale(2.0f, 2.0f, 2.0f));
	ObjModel->setShadingMode(GOURAUD_SHADING);

	
	bool bPDirty;
	mat4 mpx = camera->getProjectionMatrix(bPDirty);
	
	CQ_leftWall->setProjectionMatrix(mpx);
	CQ_rightWall->setProjectionMatrix(mpx);
	CQ_frontWall->setProjectionMatrix(mpx);
	CQ_backWall->setProjectionMatrix(mpx);
	CFloor->setProjectionMatrix(mpx);
	CQ_ceiling->setProjectionMatrix(mpx);

	for (int i = 0; i < 3; i++)
	{
		CCube[i]->setProjectionMatrix(mpx);
	}
	
	ObjModel->setProjectionMatrix(mpx);


}

//----------------------------------------------------------------------------
void GL_Display( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window
	CFloor->draw();
	CQ_ceiling->draw();
	CQ_leftWall->draw();
	CQ_rightWall->draw();
	CQ_frontWall->draw();
	CQ_backWall->draw();
	
	for (int i = 0; i < 4; i++)
	{
		CBtn[i]->draw();
	}

	for (int i = 0; i < 3; i++)
	{
		CCube[i]->draw();
	}
	ObjModel->draw();
	
	
	glutSwapBuffers();	// 交換 Frame Buffer
}

//----------------------------------------------------------------------------
// Part 2 : for single light source
void UpdateLightPosition(float dt)
{
	mat4 mxT;
	_fLightDelta += dt;
	_fLightTheta = _fLightDelta * (float)M_PI_2 ; //每一秒轉180度
	if (_fLightTheta>=(float)M_PI*2.0f) {
		_fLightTheta -= (float)M_PI * 2.0f;
		_fLightDelta -= 4.0f;
	}
	Light_resulte[0].position.x = _fLightRadius * cosf(_fLightTheta);
	Light_resulte[0].position.z = _fLightRadius * sinf(_fLightTheta);
	
	
}
//----------------------------------------------------------------------------

void onFrameMove(float delta)
{

	// for camera
	g_at = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi) + g_fCameraMoveX,
		g_fRadius * cos(g_fTheta) + g_fCameraMoveY,
		g_fRadius * sin(g_fTheta) * cos(g_fPhi) + g_fCameraMoveZ,
		1.0f);
	g_eye = vec4(g_fCameraMoveX, g_fCameraMoveY, g_fCameraMoveZ, 1.0f);	//第一人稱視角
	auto camera = CCamera::getInstance();
	camera->updateViewLookAt(g_eye, g_at);



	mat4 mvx;	// view matrix & projection matrix
	bool bVDirty;	// view 與 projection matrix 是否需要更新給物件
	
	mvx = camera->getViewMatrix(bVDirty);
	if (bVDirty) {
		CFloor->setViewMatrix(mvx);
		CQ_leftWall->setViewMatrix(mvx);
		CQ_rightWall->setViewMatrix(mvx);
		CQ_frontWall->setViewMatrix(mvx);
		CQ_backWall->setViewMatrix(mvx);
		CQ_ceiling->setViewMatrix(mvx);

		for (int i = 0; i < 3; i++)
		{
			CCube[i]->setViewMatrix(mvx);
		}
		ObjModel->setViewMatrix(mvx);
	
	}

	
	if (g_bAutoRotating) { // Part 2 : 重新計算 Light 的位置
		UpdateLightPosition(delta);
	}
	// 如果需要重新計算時，在這邊計算每一個物件的顏色


	
	CFloor->update(delta, Light_resulte);
	CQ_ceiling->update(delta, Light_resulte);
	CQ_leftWall->update(delta, Light_resulte);
	CQ_rightWall->update(delta, Light_resulte);
	CQ_frontWall->update(delta, Light_resulte);
	CQ_backWall->update(delta, Light_resulte);
	for (int i = 0; i < 3; i++)
	{
		CCube[i]->update(delta, Light_resulte);
	}
	ObjModel->update(delta, Light_resulte);


	GL_Display();
}

//----------------------------------------------------------------------------

void Win_Keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
	/*case 65://A
	case 97://a
		//燈光旋轉
		g_bAutoRotating = !g_bAutoRotating;
		break;
		*/
	case 82: //R
		if (Light_resulte[0].diffuse.x < 1.0f)
			Light_resulte[0].diffuse.x += 0.05f;
		else
			Light_resulte[0].diffuse.x = 0.95f;
		break;
	case 114: //r
		if (Light_resulte[0].diffuse.x > 0.0f)
			Light_resulte[0].diffuse.x -= 0.05f;
		else
			Light_resulte[0].diffuse.x = 0.0f;
		break;

	case 71: //G
		if (Light_resulte[0].diffuse.y < 1.0f)
			Light_resulte[0].diffuse.y += 0.05f;
		else
			Light_resulte[0].diffuse.y = 0.95f;
		break;

	case 103: //g
		if (Light_resulte[0].diffuse.y > 0.0f)
			Light_resulte[0].diffuse.y -= 0.05f;
		else
			Light_resulte[0].diffuse.y = 0.0f;
		break;

	case 66: //B
		if (Light_resulte[0].diffuse.z < 1.0f)
			Light_resulte[0].diffuse.z += 0.05f;
		else
			Light_resulte[0].diffuse.z = 0.95f;
		break;

	case 98: //b
		if (Light_resulte[0].diffuse.z > 0.0f)
			Light_resulte[0].diffuse.z -= 0.05f;
		else
			Light_resulte[0].diffuse.z = 0.0f;
		break;


	case 'W':
	case 'w':
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);
		if (g_fCameraMoveX <= 18.5f && g_fCameraMoveX >= -18.5f && g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f) {	//限制空間
			g_fCameraMoveX += (g_matMoveDir._m[0][3] * 0.2f);
			g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 0.2f);
		}
		else {	// 修正卡牆
			if (g_fCameraMoveX > 18.5) g_fCameraMoveX = 18.5f;
			else if (g_fCameraMoveX < -18.5) g_fCameraMoveX = -18.5f;
			if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
			else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;
		}
		break;
	case 'S':
	case 's':
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);
		if (g_fCameraMoveX <= 18.5f && g_fCameraMoveX >= -18.5f && g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f) {	//限制空間
			g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 0.2f);
			g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 0.2f);
		}
		else {	// 修正卡牆
			if (g_fCameraMoveX > 18.5) g_fCameraMoveX = 18.5f;
			else if (g_fCameraMoveX < -18.5) g_fCameraMoveX = -18.5f;
			if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
			else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;
		}
		break;
	case 'A':
	case 'a':
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);
		if (g_fCameraMoveX <= 18.5f && g_fCameraMoveX >= -18.5f && g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f) {	//限制空間
			g_fCameraMoveX += (g_matMoveDir._m[0][3] * 0.2f);
			g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 0.2f);
		}
		else {	// 修正卡牆
			if (g_fCameraMoveX > 18.5) g_fCameraMoveX = 18.5f;
			else if (g_fCameraMoveX < -18.5) g_fCameraMoveX = -18.5f;
			if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
			else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;
		}
		break;
	case 'D':
	case 'd':
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);
		if (g_fCameraMoveX <= 18.5f && g_fCameraMoveX >= -18.5f && g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f) {	//限制空間
			g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 0.2f);
			g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 0.2f);
		}
		else {	// 修正卡牆
			if (g_fCameraMoveX > 18.5) g_fCameraMoveX = 18.5f;
			else if (g_fCameraMoveX < -18.5) g_fCameraMoveX = -18.5f;
			if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
			else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;
		}
		break;


	case 80://P
	case 112://p
		printf("( %f , %f , %f )\n", eye.x, eye.y, eye.z);
		break;

    case 033:
		delete CFloor;
		delete CQ_ceiling;
		delete CQ_leftWall;
		delete CQ_rightWall;
		delete CQ_frontWall;
		delete CQ_backWall;
		for (int i = 0; i < 4; i++)
		{
			delete CBtn[i];
		}

		for (int i = 0; i < 3; i++)
		{
			delete CCube[i];
		}
		delete ObjModel;
		
		
		CCamera::getInstance()->destroyInstance();
        exit( EXIT_SUCCESS );
        break;
    }
}

inline void ScreenToUICoordinate(int x, int y, vec2& pt) {

	pt.x = 2.0f * (float)x / SCREEN_SIZE - 1.0f;
	pt.y = -2.0f * (float)y / SCREEN_SIZE + 1.0f;
}


//----------------------------------------------------------------------------
void Win_Mouse(int button, int state, int x, int y) {
	vec2 pt;
	if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
		ScreenToUICoordinate(x, y, pt);

		//spot light 開關鍵
		for (int i = 0; i < 3; i++) 
		{
			if (CBtn[i]->onTouch(pt)) //關閉開關
			{
				if(Light_resulte[i + 1].spotCutoff==1.0f)
					Light_resulte[i+1].spotCutoff = 0.95f;
				else
					Light_resulte[i + 1].spotCutoff = 1.0f;
			}
			

		}

		//rotate center light
		if (CBtn[3]->onTouch(pt)) //關閉開關
		{
			g_bAutoRotating = !g_bAutoRotating;
		}


		
	}



}
//----------------------------------------------------------------------------
void Win_SpecialKeyboard(int key, int x, int y) {
	
	front = normalize(at-eye);
	right = normalize(cross(front, g_vUp));
	switch (key)
	{
	case GLUT_KEY_UP:
		if (eye.x+ front.x<18.0f && eye.x + front.x > -18.0f)
		{
			eye += point4(front.x, 0.0f, 0.0f, 0.0f) * 3.0f;
			at += point4(front.x, 0.0f, 0.0f, 0.0f) * 3.0f;
		}

		if ( eye.z + front.z < 18.0f && eye.z + front.z > -18.0f)
		{
			eye += point4(0.0f, 0.0f, front.z, 0.0f) * 3.0f;
			at += point4(0.0f, 0.0f, front.z, 0.0f) * 3.0f;
		}
		
		
		break;
	case GLUT_KEY_DOWN:
		if (eye.x - front.x<18.0f && eye.x - front.x > -18.0f)
		{
			eye -= point4(front.x, 0.0f, 0.0f, 0.0f) * 3.0f;
			at -= point4(front.x, 0.0f, 0.0f, 0.0f) * 3.0f;
		}

		if (eye.z - front.z < 18.0f && eye.z - front.z > -18.0f)
		{
			eye -= point4(0.0f, 0.0f, front.z, 0.0f) * 3.0f;
			at -= point4(0.0f, 0.0f, front.z, 0.0f) * 3.0f;
		}
		break;
	case GLUT_KEY_LEFT:		// 目前按下的是向左方向鍵
		
		if (eye.x - right.x<18.0f && eye.x - right.x > -18.0f)
		{
			eye -= point4(right.x, 0.0f, 0.0f, 0.0f) * 3.0f;
			at -= point4(right.x, 0.0f, 0.0f, 0.0f) * 3.0f;
		}

		if (eye.z - right.z < 18.0f && eye.z - right.z > -18.0f)
		{
			eye -= point4(0.0f, 0.0f, right.z, 0.0f) * 3.0f;
			at -= point4(0.0f, 0.0f, right.z, 0.0f) * 3.0f;
		}
		
		break;
	case GLUT_KEY_RIGHT:	// 目前按下的是向右方向鍵
		if (eye.x + right.x<18.0f && eye.x + right.x > -18.0f)
		{
			eye += point4(right.x, 0.0f, 0.0f, 0.0f) * 3.0f;
			at += point4(right.x, 0.0f, 0.0f, 0.0f) * 3.0f;
		}

		if (eye.z + right.z < 18.0f && eye.z + right.z > -18.0f)
		{
			eye += point4(0.0f, 0.0f, right.z, 0.0f) * 3.0f;
			at += point4(0.0f, 0.0f, right.z, 0.0f) * 3.0f;
		}
		break;

	default:
		break;
	}

	if (eye.x > 18.0f)
		eye.x = 17.9;

	if (eye.x < -18.0f)
		eye.x = -17.9;

	if (eye.z > 18.0f)
		eye.z = 17.9;

	if (eye.z < -18.0f)
		eye.z = -17.9;

	auto camera = CCamera::getInstance();
	camera->updateViewLookAt(eye, at);
}

//----------------------------------------------------------------------------
// The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.
void Win_PassiveMotion(int x, int y) {
	g_fPhi = (float)-M_PI * (x - HALF_SIZE) / (HALF_SIZE); // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	g_fTheta = (float)M_PI * (float)y / SCREEN_SIZE;
	at = point4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), g_fRadius * cos(g_fTheta), g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.0f);

	auto camera = CCamera::getInstance();
	camera->updateLookAt(at);
	
}

// The motion callback for a window is called when the mouse moves within the window while one or more mouse buttons are pressed.
void Win_MouseMotion(int x, int y) {
	g_fPhi = (float)-M_PI * (x - HALF_SIZE) / (HALF_SIZE);  // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	g_fTheta = (float)M_PI * (float)y / SCREEN_SIZE;
	at = point4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), g_fRadius * cos(g_fTheta), g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.0f);

	auto camera = CCamera::getInstance();
	camera->updateLookAt(at);
}
//----------------------------------------------------------------------------
void GL_Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background
	glEnable(GL_DEPTH_TEST);
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(SCREEN_SIZE,SCREEN_SIZE);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("ShowRoom");

	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutMouseFunc(Win_Mouse);
	glutMotionFunc(Win_MouseMotion);
	glutPassiveMotionFunc(Win_PassiveMotion);
	glutKeyboardFunc(Win_Keyboard);	// 處理 ASCI 按鍵如 A、a、ESC 鍵...等等
	glutSpecialFunc(Win_SpecialKeyboard);	// 處理 NON-ASCI 按鍵如 F1、Home、方向鍵...等等
	glutDisplayFunc(GL_Display);
	glutReshapeFunc(GL_Reshape);
	glutIdleFunc(IdleProcess);

	glutMainLoop();
	return 0;

	
}