//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D		g_texture		: register(t0);	//テクスチャー
Texture2D		g_normalTexture : register(t1); //ノーマルマップ
SamplerState	g_sampler		: register(s0);	//サンプラー

//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
	float4x4	g_matWVP;			// ワールド・ビュー・プロジェクションの合成行列
	float4x4	g_matW;				//ワールド行列
	float4x4    g_matNormal;		//法線変形行列(回転行列と拡大行列の逆行列)
	float4		g_diffuseColor;		// ディフューズカラー（マテリアルの色）
	float4		g_ambient;			//アンビエント
	float4		g_speculer;			//スペキュラー
	float4		g_lightDirection;	//ライトの向き
	float4		g_cameraPosition;	//カメラの位置
	float		g_shininess;		//ハイライトの強さ
	bool		g_isTexture;		// テクスチャ貼ってあるかどうか
	bool		g_isUseCustomClor;  //プログラム側で変える色
	float4      g_customColor;		//プログラム側で色を変える場合の変数
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
	float4 pos		: SV_POSITION;	//位置
	float4 normal	: TEXCOORD0;	//法線
	float4 light	: TEXCOORD1;	//ライト
	float4 eyeVector: TEXCOORD2;	//視線
	float4 col		: COLOR;		//カラー
	float2 uv		: TEXCOORD3;	//UV座標
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData;
	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, g_matWVP);
	//視線ベクトル
	float4 cameraPos = mul(pos, g_matW);
	outData.eyeVector = normalize(g_cameraPosition - cameraPos);

	//法線
	normal.w = 0;
	outData.normal = mul(normal, g_matNormal);

	//UV
	outData.uv = uv;

	//まとめて出力
	return outData;
}
//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
	//PS_IN outData;
	inData.normal = normalize(inData.normal);
//ライトベクトル
float4 light = float4(0, -1, 0, 0);
	light = normalize(light);
	//拡散反射光(ディフューズ)
	//法線とライトの方向の内積
	float4 shade = saturate(dot(inData.normal, -light));
	shade.a = 1;
	//テクスチャ
	float4 diffuse;
	if (g_isTexture == true)
	{
		diffuse = g_texture.Sample(g_sampler, inData.uv);
	}
	else
	{
		diffuse = g_diffuseColor;
	}
	//環境光(アンビエント)
	float4 ambient = float4(0.2, 0.2, 0.2, 1.0f);// g_ambient;
	ambient.a = 1;
	//鏡面反射光(スペキュラー)
	float4 speculer = float4(0, 0, 0, 0);
	if (g_speculer.a != 0)
	{
		float4 vecReflect = reflect(light, inData.normal);
		speculer = float4(1,1,1,0) * pow(saturate(dot(vecReflect, inData.eyeVector)), g_shininess) * g_speculer;
	}
	float4 outColor;
	outColor = diffuse * shade + diffuse * ambient + speculer;
	return outColor;
}

///////////normalMap///////////////////////////////////
////───────────────────────────────────────
//// 頂点シェーダ
////───────────────────────────────────────
//VS_OUT VS(float4 pos : POSITION,float4 uv : TEXCOORD,float4 normal : NORMAL, float4 tangent : TANGENT)
//{
//	//ピクセルシェーダーへ渡す情報
//	VS_OUT outData;
//
//	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
//	//スクリーン座標に変換し、ピクセルシェーダーへ
//	outData.pos = mul(pos, g_matWVP);
//	
//	//UV
//	outData.uv = uv;
//
//	//従法線ベクトル
//	float3 biNormal = cross(normal, tangent);
//	biNormal = mul(biNormal, g_matNormal);
//	biNormal = normalize(biNormal);
//	
//	//接線の回転
//	tangent = mul(tangent, g_matNormal);
//	tangent = normalize(tangent);
//	
//	//法線ベクトルの回転
//	normal.w = 0;
//	normal = mul(normal, g_matNormal);
//	normal = normalize(normal);
//
//
//	//視線ベクトル
//	//float4 eye = mul(pos, g_matW);
//	float4 eye = normalize(mul(pos,g_matW)-g_cameraPosition);
//	outData.eyeVector.x = dot(eye, tangent);
//	outData.eyeVector.y = dot(eye, biNormal);
//	outData.eyeVector.z = dot(eye, normal);
//	outData.eyeVector.w = 0;
//
//	float4 light = float4(0, -1, 0, 0);
//	light = normalize(light);
//	outData.light.x = dot(light, tangent);
//	outData.light.y = dot(light, biNormal);
//	outData.light.z = dot(light, normal);
//	outData.light.w = 0;
//	outData.light = normalize(outData.light);
//
//	outData.col = clamp(dot(normal, outData.light), 0, 1);
//	outData.normal = normal;
//
//	//まとめて出力
//	return outData;
//}
//
////───────────────────────────────────────
//// ピクセルシェーダ
////───────────────────────────────────────
//float4 PS(VS_OUT inData) : SV_Target
//{
//	float4 diffuse;
//	float4 ambient = float4(0.2, 0.2, 0.2, 1.0f);// g_ambient;
//	float4 speculer = float4(0, 0, 0, 0);
//	
//	float4 normal = g_normalTexture.Sample(g_sampler, inData.uv) * 2 - 1;
//
//	inData.normal = normalize(inData.normal);
//	normal.w = 0;
//	normal = normalize(normal);
//
//	////ライトベクトル
//	//float4 light = float4(0, -1, 0, 0);
//	//light = normalize(light);
//
//
//
//	//拡散反射光(ディフューズ)
//	//法線とライトの方向の内積
//	float4 shade = saturate(dot(normal, -inData.light));
//	shade.a = 1;
//
//	//テクスチャ
//	if (g_isTexture == true)
//	{
//		diffuse = g_texture.Sample(g_sampler, inData.uv)*shade;
//		ambient = g_texture.Sample(g_sampler, inData.uv) * g_ambient;
//	}
//	else
//	{
//		diffuse = g_diffuseColor * shade;
//		ambient = g_diffuseColor * g_ambient;
//	}
//
//	//環境光(アンビエント)
//	ambient.a = 1;
//
//	//鏡面反射光(スペキュラー)
//	float4 vecReflect = reflect(inData.light, inData.normal);
//	speculer = pow(saturate(dot(vecReflect, inData.eyeVector)), g_shininess) * g_speculer;
//	float4 outColor;
//	outColor = diffuse + diffuse * ambient + speculer;
//	return outColor;
//}