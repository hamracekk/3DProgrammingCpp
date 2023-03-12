// constant buffer
cbuffer ConstBuffer
{
    matrix transform;
};

///Outputs the position -> 4d vector
///Input is 2D vector 
///Semantic names are used for position
///Outpu after build is .cso file (bytcode) and that file is runtime loaded to vertex shader

float4 main(float3 pos : POSITION) : SV_POSITION
{
    return mul(float4(pos, 1.0f), transform);
}