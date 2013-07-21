// Copyright (c) 2013, Hannes Würfel <hannes.wuerfel@student.hpi.uni-potsdam.de>
// Computer Graphics Systems Group at the Hasso-Plattner-Institute, Germany
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//   * Neither the name of the Computer Graphics Systems Group at the
//     Hasso-Plattner-Institute (HPI), Germany nor the names of its
//     contributors may be used to endorse or promote products derived from
//     this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.


cbuffer cb
{
	float4x4 gWorldViewProj; 
};

Texture2D tex;

SamplerState anisotropicSampler
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct Vin
{
	float3 PosL  : POSITION;
    float4 Color : COLOR;
    float2 Tex   : TEXCOORD;
};

struct Vout
{
	float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
    float2 Tex   : TEXCOORD;
};

Vout vertexShader(Vin vin)
{
	Vout vout;
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
    vout.Color = vin.Color;
    vout.Tex = vin.Tex;
    
    return vout;
}

float4 pixelShader(Vout pin, uniform bool useTexture) : SV_Target
{
    if(useTexture)
    {
        // simple sepia
		float4 color = tex.Sample(anisotropicSampler, pin.Tex);
		//float r = clamp(color.r * 0.393f + color.g * 0.769f + color.b * 0.189f, 0.0f, 1.0f);
		//float g = clamp(color.r * 0.349f + color.g * 0.686f + color.b * 0.168f, 0.0f, 1.0f);
		//float b = clamp(color.r * 0.272f + color.g * 0.534f + color.b * 0.131f, 0.0f, 1.0f);

		float gray = clamp(color.r * 0.2126f + color.g * 0.7152f + color.b * 0.0722f, 0.0f, 1.0f);
		return float4(gray, gray, gray, color.a);
		//return float4(r, g, b, color.a);
    }
    else
    {
        return pin.Color;
    }
}

technique11 SimpleTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, vertexShader()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixelShader(false)));
    }
}

technique11 SimpleTechniqueTextured
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, vertexShader()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixelShader(true)));
    }
}
