cbuffer cbPerFrame
{
	float4x4 u_ViewProjection;
};

cbuffer cbPerObject
{
	float4x4 u_Model;
};

Texture2D u_Main : register(t0);

SamplerState u_MainSS
{
	Filter = MIN_MAG_MIP_POINT;
};

struct vin {
	float3 Position : POSITION0;
	float2 Uv : TEXCOORD0;
};

struct v2p {
	float4 Position : SV_POSITION;
	float2 Uv : TEXCOORD0;
};

v2p vMain(vin v)
{
	v2p o;
	o.Position = mul(u_ViewProjection, mul(u_Model, float4(v.Position, 1.0)));
	o.Uv = v.Uv;
	return o;
}
			
float4 pMain(v2p p) : SV_TARGET
{
	return u_Main.Sample(u_MainSS, p.Uv);
}