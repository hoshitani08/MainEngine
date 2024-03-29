cbuffer cbuff0 : register(b0)
{
	float4 color; // 色(RGBA)
	float brightnessColor; // 明度
	matrix mat; //ビュープロジェクション行列
	float timer;
};

//頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION;  //システム用頂点座標
	float2 uv    : TEXCOORD;  //uv値
};