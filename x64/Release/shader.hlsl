struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float3 Direction;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Reflect;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix WorldViewProjection;
	matrix World;
	matrix WorldInverseTranspose;
	DirectionalLight gDirLight;
	Material gMaterial;
	float3 gEyeW;
};

struct VSInput
{
	float3 position	: POSITION;
	float3 normal : NORMAL;
};
struct PSInput
{
	float4 position : SV_POSITION;
	float3 worldPos : POSITION;
	float3 normal : NORMAL;
};

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a directional light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
float4 ComputeDirectionalLight(Material mat, DirectionalLight L, float3 normal, float3 toEye)
{
	// Initialize outputs.
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = L.Direction;

	// Add ambient term.
	ambient = mat.Ambient * L.Ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// if we can see
	if (diffuseFactor > 0.0f)
	{
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
	}
	return ambient + diffuse;
}


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PSInput VS(VSInput input)
{
	PSInput output;

	float4 Pos = float4(input.position, 1.0f);
	output.position = mul(float4(input.position, 1.0f), WorldViewProjection);
	output.worldPos = mul(float4(input.position, 1.0f), World).xyz;
	output.normal = mul(float4(input.normal, 1.0f), WorldInverseTranspose).xyz;

	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PSInput input) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
	input.normal = normalize(input.normal);

	float3 toEyeW = normalize(gEyeW - input.worldPos);

	return ComputeDirectionalLight(gMaterial, gDirLight, input.normal, toEyeW);
	//return float4(0.0f, 0.0f, 0.0f, 1.0f);
}
