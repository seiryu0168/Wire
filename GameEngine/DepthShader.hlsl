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
	float4 WVPpos = mul(pos, g_matWVP);
	outData.pos = WVPpos;
	outData.depth = WVPpos;

	return outData;
}
//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS_Depth(VS_OUT inData) : SV_Target
{
	float4 color = float4(0,0,0,1);
	color = (inData.pos.z*10.0f) / inData.pos.w;
	color.a = 1;
	return color;
}


