//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D		g_texture		: register(t0);	//テクスチャー
Texture2D		g_normalTexture : register(t2); //ノーマルマップ
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
	float4      g_customColor;		//プログラム側で色を変える場合の変数
	float		g_shininess;		//ハイライトの強さ
	bool		g_isTexture;		// テクスチャ貼ってあるかどうか
	bool		g_isNormal;  //プログラム側で変える色
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
	float4 col		: COLOR0;		//カラー
	float4 fog		: COLOR1;		//フォグ
	float2 uv		: TEXCOORD3;	//UV座標
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData;
	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, g_matWVP);

	//視線ベクトル
	float4 wPos = mul(pos, g_matW);
	float4 wCameraPos = mul(g_cameraPosition, g_matW);
	outData.eyeVector = normalize(wPos - wCameraPos);

	//法線
	outData.normal = mul(normal, g_matNormal);
	outData.normal.w = 0;

	tangent.w = 0;
	//法線と接線を使って従法線を作る
	//従法線
	float3 biNormal = cross(normal, tangent);
	
	//法線を回転
	normal = mul(normal, g_matNormal);
	normal = normalize(normal);

	//従法線を回転
	biNormal = mul(biNormal, g_matNormal);
	biNormal = normalize(biNormal);
	
	//接線を回転
	tangent = mul(tangent, g_matNormal);
	tangent = normalize(tangent);

	//ライトの向きをライトと各ベクトルで求める
	float4 light = float4(0, -1, 0, 0);
	//outData.light = light;
	outData.light.x = dot(light, tangent);
	outData.light.y = dot(light, biNormal);
	outData.light.z = dot(light, normal);
	outData.light.w	 = 0;


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
	inData.normal = normalize(inData.normal);
	//ライトベクトル
	float4 light = normalize(inData.light);

	float4 normal = g_normalTexture.Sample(g_sampler, inData.uv);// +inData.normal;
	normal = normalize(normal);
	normal.w = 0;
	//拡散反射光(ディフューズ)
	
	//法線とライトの方向の内積
	if (g_isNormal==false)
	{
		//normal = inData.normal;
	}
	float4 shade = saturate(dot(normal, -light));
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
		float4 vecReflect = reflect(inData.light, normal);
		speculer = float4(1,1,1,0) * pow(saturate(dot(vecReflect, inData.eyeVector)), g_shininess) * g_speculer;
	}
	float4 outColor;
	outColor = diffuse * shade + diffuse * ambient + speculer;
	return outColor;
}


