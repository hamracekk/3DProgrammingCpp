///Output struct for vertex
struct VertexOut
{
    float3 color : COLOR;
    float4 position : SV_POSITION;
};

///Outputs the position -> 4d vector
///Input is 2D vector 
///Semantic names are used for position
///Outpu after build is .cso file (bytcode) and that file is runtime loaded to vertex shader

VertexOut main(float2 pos : SV_POSITION, float3 col : COLOR)
{
    VertexOut outStruct;
    outStruct.color = col;
    outStruct.position = float4(pos.x, pos.y, 0, 1.0f);
    return outStruct;
}