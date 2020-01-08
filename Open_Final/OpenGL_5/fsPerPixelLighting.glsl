#version 130
// iTexLayer 的設定與判斷方式，假設此處可以支援到六張貼圖

#define LIGHT_NUM 4 //燈泡數量
#define NONE_MAP 0
#define DIFFUSE_MAP 1
#define LIGHT_MAP 2
#define NORMAL_MAP 4

in vec3 v3N;
in vec3 v3L[LIGHT_NUM];
in vec3 v3E;

in vec2 DiffuseMapUV; // 輸入 Diffuse Map 貼圖座標
in vec2 LightMapUV;   // 輸入 Light Map 貼圖座標

uniform int iTexLayer;
uniform float fElapsedTime;

// 以下為新增的部分
uniform vec4 LightInView[LIGHT_NUM];	 // Light's position in View Space
uniform vec4 AmbientProduct[LIGHT_NUM];  // light's ambient  與 Object's ambient  與 ka 的乘積
uniform vec4 DiffuseProduct[LIGHT_NUM];  // light's diffuse  與 Object's diffuse  與 kd 的乘積
uniform vec4 SpecularProduct[LIGHT_NUM]; // light's specular 與 Object's specular 與 ks 的乘積
uniform float fShininess;
uniform int iLighting[LIGHT_NUM];
uniform float Cutoff[LIGHT_NUM];
uniform vec4 vObjectColor; // 代表物件的單一顏色
uniform int LightNUM; // 有幾盞燈

// For Texture Sampler
uniform sampler2D diffuMap; // 貼圖的參數設定
uniform sampler2D lightMap; // 貼圖的參數設定

void main()
{
	// 先宣告 diffuse 與 specular
	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 LightingColor = vec4(0.0, 0.0, 0.0, 1.0);
	for (int i = 0; i < LightNUM; i++)
	{
		if (iLighting[i] != 1)
		{
			if (iTexLayer == NONE_MAP)
				gl_FragColor = vObjectColor;
			else
				gl_FragColor = vObjectColor * texture2D(diffuMap, DiffuseMapUV);
		}
		else
		{

			//V 1. 計算 Ambient color : Ia = AmbientProduct = Ka * Material.ambient * La =
			vec4 ambient = AmbientProduct[0]; // m_sMaterial.ka * m_sMaterial.ambient * vLightI;

			// 單位化傳入的 Normal Dir
			vec3 vN = normalize(v3N);

			//V 2. 單位化傳入的 Light Dir
			vec3 vL = normalize(v3L[i]); // normalize light vector

			//V 5. 計算 L dot N
			float fLdotN = vL.x * vN.x + vL.y * vN.y + vL.z * vN.z;
			if (fLdotN >= Cutoff[i])
			{ // 該點被光源照到才需要計算

				//V Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
				if(Cutoff[i] > 0.0f) //spot light
					diffuse += pow(fLdotN , 40) * DiffuseProduct[i];
				else
					diffuse += fLdotN * DiffuseProduct[i]  ;

				// Specular color
				// Method 1: Phone Model
				// 計算 View Vector
				// 單位化傳入的 v3E , View Direction
				vec3 vV = normalize(v3E);

				//計算 Light 的 反射角 vRefL = 2 * fLdotN * vN - L
				// 同時利用 normalize 轉成單位向量
				vec3 vRefL = normalize(2.0f * (fLdotN)*vN - vL);

				//   計算  vReflectedL dot View
				float RdotV = vRefL.x * vV.x + vRefL.y * vV.y + vRefL.z * vV.z;

				//V Specular Color : Is = Ks * Material.specular * Ls * (R dot V)^Shininess;
				if (RdotV > 0)
					specular += SpecularProduct[i] * pow(RdotV, fShininess);
			}
			

			LightingColor = ambient + diffuse + specular; // 計算顏色 ambient + diffuse + specular;
			LightingColor.w += DiffuseProduct[i].w;		  //V 設定為傳入材質的 alpha,	DiffuseProduct.w
														  //LightingColor.w = 1.0;	// 設定 alpha 為 1.0

			if (iTexLayer == NONE_MAP)
				gl_FragColor = LightingColor;
			else if (iTexLayer == DIFFUSE_MAP || iTexLayer == (DIFFUSE_MAP | NORMAL_MAP))
				gl_FragColor = LightingColor * texture2D(diffuMap, DiffuseMapUV);
			else if (iTexLayer == (DIFFUSE_MAP | LIGHT_MAP))
			{
				gl_FragColor = (0.65 * LightingColor * texture2D(diffuMap, DiffuseMapUV) + texture2D(diffuMap, DiffuseMapUV) *
																							   texture2D(lightMap, LightMapUV) * vec4(0.0f, 1.0f, 1.0f, 1.00));

				//			float t = 0.15 + 0.75 * abs(sin(fElapsedTime * 3.1415926 * 0.125));
				//			gl_FragColor =  ( 0.55 * LightingColor * texture2D(diffuMap, DiffuseMapUV)  + // texture2D(diffuMap, DiffuseMapUV) *
				//							texture2D(lightMap, LightMapUV) * vec4(0.0f, 1.0f, 1.0f, 1.0) * t);
			}
		}
	}
}
