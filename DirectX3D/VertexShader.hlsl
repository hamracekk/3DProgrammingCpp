///Output struct for vertex
struct VertexOut
{
    float3 color : COLOR;
    float4 position : SV_POSITION;
};

// constant buffer
cbuffer ConstBuffer
{
    matrix transform;
};

///Outputs the position -> 4d vector
///Input is 2D vector 
///Semantic names are used for position
///Outpu after build is .cso file (bytcode) and that file is runtime loaded to vertex shader

VertexOut main(float3 pos : POSITION, float3 col : COLOR)
{
    VertexOut outStruct;
    outStruct.color = col;
    outStruct.position = mul(float4(pos, 1.0f), transform);
    return outStruct;
}