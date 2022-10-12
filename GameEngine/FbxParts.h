//#pragma once
//#include <d3d11.h>
//#include <fbxsdk.h>
//#Include"Texture.h"
//#include<DirectXMath.h>
//#include"Transform.h"
//class FbxParts
//{
//
//	//マテリアル
////質感
//	struct MATERIAL
//	{
//		Texture* pTexture;    //テクスチャ
//		XMFLOAT4 diffuse;	  //ディフューズ(マテリアルの色)
//		XMFLOAT4 ambient;	  //アンビエント
//		XMFLOAT4 speculer;    //スペキュラー(ツルツルを表現する奴)
//		float shininess;	  //ハイライトの強さ
//
//	};
//
//	//シェーダーに送る情報をまとめた構造体
//	struct CONSTANT_BUFFER
//	{
//		XMMATRIX matWVP;			//ワールド、ビュー、プロジェクション行列の合成(頂点変換に使う)
//		XMMATRIX matW;				//ワールド行列
//		XMMATRIX matNormal;			//回転行列と拡大行列の合成(法線の変形に使う)
//		XMFLOAT4 diffuseColor;		//ディフューズ(マテリアルの色)
//		XMFLOAT4 ambient;			//アンビエント
//		XMFLOAT4 speculer;			//スペキュラー
//		XMFLOAT4 lightDirection;	//ライトの向き
//		XMFLOAT4 cameraPosition;	//カメラの位置
//		FLOAT	 shininess;			//ハイライトの強さ
//		BOOL	 isTexture;			//テクスチャ
//	};
//
//	//頂点に格納される情報
//	//mayaだとfbx出力する時に頂点法線の設定をしないと表示がおかしくなる
//	//blenderの場合は自動スムーズを切ると正しく表示される
//	struct VERTEX
//	{
//		XMVECTOR position;
//		XMVECTOR uv;
//		XMVECTOR normal;//blenderではスムーズシェードにした上でノーマルの自動スムーズを切らないと法線のデータがおかしくなって正しく表示されない
//	};
//
//	int vertexCount_;		//頂点数
//	int polygonCount_;		//ポリゴン数
//	int materialCount_;		//マテリアル数
//
//	ID3D11Buffer* pVertexBuffer_;
//	ID3D11Buffer** pIndexBuffer_;
//	ID3D11Buffer* pConstantBuffer_;
//	MATERIAL* pMaterialList_;
//	int* indexCount_;
//
//	int** ppIndex_;
//	VERTEX* pVertices_;
//
//public:
//
//	Fbx();
//	~Fbx();
//	HRESULT Load(std::string fileName);
//	HRESULT InitVertex(fbxsdk::FbxMesh* mesh);
//	HRESULT InitIndex(fbxsdk::FbxMesh* mesh);
//	HRESULT CreateConstantBuffer();
//	void InitMaterial(fbxsdk::FbxNode* pNode);
//	void RayCast(RayCastData& ray, Transform& transform);
//
//
//	//void ToPipeLine(Transform& transform);
//	//void bufferSet();
//	void    Draw(Transform& transform, SHADER_TYPE shaderType);
//	void    Release();
//};
//
