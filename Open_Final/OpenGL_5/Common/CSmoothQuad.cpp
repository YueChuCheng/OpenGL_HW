#include"CSmoothQuad.h"

CSmoothQuad::~CSmoothQuad()
{
	delete[] _pSquares;
	delete[] _pfSquaresT;
}

CSmoothQuad::CSmoothQuad(int iSize, float fYPos)
{
	_iGridSize = iSize;
	
	_pSquares = new CQuad[_iGridSize * _iGridSize];
	_pfSquaresT = new float[_iGridSize * _iGridSize * 3];
	_fYPos = fYPos;


}

//�]�w�C�@�檺��V�覡
void CSmoothQuad::setShadingMode(int iMode) {
	int idx = 0;
	for (int i = 0; i < _iGridSize; i++)
	{
		for (int j = 0; j < _iGridSize; j++)
		{
			_pSquares[idx].setShadingMode(iMode);
			idx++;
		}
	}

}


void CSmoothQuad::setShader() {

	mat4 mxT;
	int idx = 0;
	vec4 vT, vColor;
	for (int i = 0; i < _iGridSize; i++)
	{
		for (int j = 0; j < _iGridSize; j++)
		{
			_pfSquaresT[idx * 3 + 2] = -_iGridSize / 2 + i + 0.5f;
			_pfSquaresT[idx * 3 + 0] = -_iGridSize / 2 + j + 0.5f;
			_pfSquaresT[idx * 3 + 1] = _fYPos;
			
			if (i + j == 0) { // �Ĥ@�ӡA �ݭn���w Shader
				_pSquares[idx].setShader();
				_shaderHandle = _pSquares[idx].getShaderHandle();
				vColor.x = 0.3f; vColor.y = 0.6f; vColor.z = 0.6f; vColor.w = 1.0f;
				_pSquares[idx].setColor(vColor);
			}
			else { // ��L�� 
				
				_pSquares[idx].setShader(_shaderHandle);
				/*if ((i + j) % 2) {
					vColor.x = 0.2f; vColor.y = 0.2f; vColor.z = 0.2f; vColor.w = 1.0f;
					_pSquares[idx].setColor(vColor);
				}
				else {
					vColor.x = 0.3f; vColor.y = 0.6f; vColor.z = 0.6f; vColor.w = 1.0f;
					_pSquares[idx].setColor(vColor);
				}*/
			}
			mxT = Translate(_pfSquaresT[idx * 3 + 0], _pfSquaresT[idx * 3 + 1], _pfSquaresT[idx * 3 + 2]);
			_pSquares[idx].setTRSMatrix(mxT);
			idx++;
		}

	}


}


void CSmoothQuad::setProjectionMatrix(mat4& mat) {
	for (int i = 0; i < _iGridSize * _iGridSize; i++) {
		_pSquares[i].setProjectionMatrix(mat);
	}
}
void CSmoothQuad::setTRSMatrix(mat4& mat) {
	mat4 mxT;
	// �����[�W�C�@�Ӥ�楻�����첾
	for (int i = 0; i < _iGridSize* _iGridSize; i++)
	{
		mxT = Translate(_pfSquaresT[i * 3 + 0], _pfSquaresT[i * 3 + 1], _pfSquaresT[i * 3 + 2]);
		_pSquares[i].setTRSMatrix(mat * mxT);
	}
}

void CSmoothQuad::draw() {
	
	_pSquares[0].draw();
	for (int i = 1; i < _iGridSize * _iGridSize; i++) {
		
		_pSquares[i].drawW();
		
	}
}

void CSmoothQuad::setViewMatrix(mat4 &mat) {
	for (int i = 0; i < _iGridSize * _iGridSize; i++) {
		_pSquares[i].setViewMatrix(mat);
	}
}

void CSmoothQuad::update(float dt, point4 vLightPos, color4 vLightI) {
	for (int i = 0; i < _iGridSize * _iGridSize; i++) {
		_pSquares[i].update(dt, vLightPos, vLightI);
	}
}
void CSmoothQuad::update(float dt, const LightSource& Lights) {
	for (int i = 0; i < _iGridSize * _iGridSize; i++) {
		_pSquares[i].update(dt, Lights);
	}
}

void CSmoothQuad::setMaterials(color4 ambient, color4 diffuse, color4 specular) {
	for (int i = 0; i < _iGridSize * _iGridSize; i++) {
		_pSquares[i].setMaterials(ambient, diffuse, specular);
	}
}
void CSmoothQuad::setKaKdKsShini(float ka, float kd, float ks, float shininess) {
	for (int i = 0; i < _iGridSize * _iGridSize; i++) {
		_pSquares[i].setKaKdKsShini(ka, kd, ks, shininess);
	}
}