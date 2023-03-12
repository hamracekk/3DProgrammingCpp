cbuffer constatBuffer
{
    float4 cube_faces[6];
};

///Pixel shader outputs collor (4D RGBA)
float4 main(uint id : SV_PrimitiveID) : SV_TARGET
{
    ///every cube gface has 2 triangles (id is id of the triangle)
    return cube_faces[id / 2];
}