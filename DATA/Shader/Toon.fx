
float lineSize = 1.0;
float bodyAlpha = 1.0;

//------------------------------------------------------
//		���_�t�H�[�}�b�g
//------------------------------------------------------

struct VS_TOON
{
    float4 Pos    : POSITION;
    float4 Color  : COLOR0;
    float2 Tex	  : TEXCOORD0;
	float  Bright : TEXCOORD1;

};

//------------------------------------------------------
//		���_�V�F�[�_�[	
//------------------------------------------------------

VS_BASIC VS_BodyLine( VS_INPUTL In )
{
	VS_BASIC Out = (VS_BASIC)0;

	In.Normal = normalize( In.Normal );
	In.Pos.xyz	 += In.Normal * lineSize;

	Out.Pos   = mul(In.Pos, Projection);

	return Out;
}

VS_TOON VS_Toon( VS_INPUTL In )
{
	VS_TOON Out = (VS_TOON)0;

	float3x3 mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	N = normalize(N);

	Out.Pos   = mul(In.Pos, Projection);
	Out.Color.rgb = DirLight( LightDir, N ) + HemiLight( N );
	Out.Color.a = 1.0f;
	Out.Tex   = In.Tex;

	float3 L  = normalize( LightDir );
	Out.Bright = -dot( L, N );
	Out.Bright = saturate( Out.Bright );

	return Out;
}

//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------

float4 PS_BodyLine( VS_BASIC In) : COLOR
{
	//��
	return float4(0,0,0,bodyAlpha);
}

float4 PS_Toon( VS_TOON In) : COLOR
{   
	float4	Out;
	//�s�N�Z���F����
	Out = In.Color * tex2D( DecaleSamp, In.Tex );
	Out.a = bodyAlpha;

	if( In.Bright < 0.6f ){
		Out.rgb *= 0.75f;
	}else if( In.Bright < 0.8f ){
		Out.rgb *= 0.85f;	//�@�e�̔Z��
	}

	return Out;
}

float3 bodyColor;
float4 PS_EnemyToon( VS_TOON In) : COLOR
{   
	float4	Out;
	//�s�N�Z���F����
	Out = float4(bodyColor, bodyAlpha);

	if( In.Bright < 0.6f ){
		Out.rgb *= 0.75f;
	}else if( In.Bright < 0.8f ){
		Out.rgb *= 0.85f;	//�@�e�̔Z��
	}

	Out = ColorRate(Out);

	return Out;
}

//------------------------------------------------------
//		�ʏ�`��e�N�j�b�N
//------------------------------------------------------

technique toon
{
	pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		CullMode         = CW;
		ZEnable          = true;

		VertexShader = compile vs_2_0 VS_BodyLine();
		PixelShader  = compile ps_2_0 PS_BodyLine();
    }

    pass P1
    {
		CullMode         = CCW;

		VertexShader = compile vs_3_0 VS_Toon();
		PixelShader  = compile ps_3_0 PS_Toon();
    }
}

technique enemyToon
{
	pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		CullMode         = CW;
		ZEnable          = true;

		VertexShader = compile vs_2_0 VS_BodyLine();
		PixelShader  = compile ps_2_0 PS_BodyLine();
    }

    pass P1
    {
		CullMode         = CCW;

		VertexShader = compile vs_3_0 VS_Toon();
		PixelShader  = compile ps_3_0 PS_EnemyToon();
    }
}

technique enemyEye
{
	pass P0
    {
		VertexShader = compile vs_2_0 VS_BodyLine();
		PixelShader  = compile ps_2_0 PS_BodyLine();
    }
}