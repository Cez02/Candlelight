struct VertexPos
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VertexShaderOutput
{
    float4 position : SV_Position;
    float4 color    : COLOR;
};

VertexShaderOutput main(VertexPos IN)
{
    VertexShaderOutput OUT;

    OUT.color = IN.color;
    OUT.position = float4(IN.position, 1.0f);

    return OUT;
}