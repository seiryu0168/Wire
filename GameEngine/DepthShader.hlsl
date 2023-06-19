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
	float4x4	g_matWVP;			//ワールド・ビュー・プロジェクションの合成行列
	float4x4	g_matW;				//ワールド行列
	float4x4	g_matWLP;			//ワールド、ライト、プロジェクションの合成行列
	float4x4	g_matWLPT;			//ワールド、ライト、プロジェクション、テクスチャ座標の合成行列
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
	float4 depth	: TEXCOORD0;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS_Depth(float4 pos : POSITION/*, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT */ )
{
	VS_OUT outData;
	
	outData.pos = mul(pos, g_matWVP);
	outData.depth = outData.pos;

	return outData;
}
//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS_Depth(VS_OUT inData) : SV_Target
{
	float4 color = float4(0,0,0,1);
	color = inData.depth.z/inData.depth.w;
	color.a = 1;
	return color;
}