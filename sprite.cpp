
#include "main.h"
#include "renderer.h"
#include "sprite.h"
#include "bg.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************



//=============================================================================
// 頂点データ設定
//=============================================================================
void SetSprite(ID3D11Buffer* buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = Width * 0.5f;		// コンピューターは割り算が苦手
	hh = Height * 0.5f;		// 掛け算の方が処理が速い

	// 指定された座標を中心に設定するプログラム

	// 頂点０番（左上の頂点）
	vertex[0].Position = XMFLOAT3(X - hw, Y - hh, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(U, V);

	// 頂点１番（右上の頂点）
	vertex[1].Position = XMFLOAT3(X + hw, Y - hh, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);

	// 頂点２番（左下の頂点）
	vertex[2].Position = XMFLOAT3(X - hw, Y + hh, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);

	// 頂点３番（右下の頂点）
	vertex[3].Position = XMFLOAT3(X + hw, Y + hh, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);

	



	GetDeviceContext()->Unmap(buf, 0);

}


void SetSpriteLeftTop(ID3D11Buffer* buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// 左上を原点として設定するプログラム
	vertex[0].Position = XMFLOAT3(X, Y, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(U, V);

	vertex[1].Position = XMFLOAT3(X + Width, Y, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);

	vertex[2].Position = XMFLOAT3(X, Y + Height, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);

	vertex[3].Position = XMFLOAT3(X + Width, Y + Height, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);



	GetDeviceContext()->Unmap(buf, 0);

}


// 指定座標を左上として描画する
void SetSpriteLTColor(ID3D11Buffer* buf,
	float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 color)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// 左上を原点として設定するプログラム
	vertex[0].Position = XMFLOAT3(X, Y, 0.0f);
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = XMFLOAT2(U, V);

	vertex[1].Position = XMFLOAT3(X + Width, Y, 0.0f);
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);

	vertex[2].Position = XMFLOAT3(X, Y + Height, 0.0f);
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);

	vertex[3].Position = XMFLOAT3(X + Width, Y + Height, 0.0f);
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);

	GetDeviceContext()->Unmap(buf, 0);

}


void SetSpriteColor(ID3D11Buffer* buf, float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 color)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = Width * 0.5f;		// コンピューターは割り算が苦手
	hh = Height * 0.5f;		// 掛け算の方が処理が速い

	// 指定された座標を中心に設定するプログラム

	// 頂点０番（左上の頂点）
	vertex[0].Position = XMFLOAT3(X - hw, Y - hh, 0.0f);
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = XMFLOAT2(U, V);

	// 頂点１番（右上の頂点）
	vertex[1].Position = XMFLOAT3(X + hw, Y - hh, 0.0f);
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);

	// 頂点２番（左下の頂点）
	vertex[2].Position = XMFLOAT3(X - hw, Y + hh, 0.0f);
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);

	// 頂点３番（右下の頂点）
	vertex[3].Position = XMFLOAT3(X + hw, Y + hh, 0.0f);
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);

	GetDeviceContext()->Unmap(buf, 0);

}


void SetSpriteColorRotation(ID3D11Buffer* buf, float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color, float Rot)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = Width * 0.5f;		// コンピューターは割り算が苦手
	hh = Height * 0.5f;		// 掛け算の方が処理が速い

	// 指定された座標を中心に回転するプログラム
	float BaseAngle = atan2f(hh, hw);			// 中心点から頂点に対する角度
	XMVECTOR temp = { hw, hh, 0.0f, 0.0f };
	temp = XMVector2Length(temp);				// 中心点から頂点に対する距離
	float Radius = 0.0f;
	XMStoreFloat(&Radius, temp);

	// ここでアフィン変換（sincosのやつ）を使って4頂点を回転させる
	float x = X - cosf(BaseAngle + Rot) * Radius;
	float y = Y - sinf(BaseAngle + Rot) * Radius;
	vertex[0].Position = XMFLOAT3(x, y, 0.0f);

	x = X + cosf(BaseAngle - Rot) * Radius;
	y = Y - sinf(BaseAngle - Rot) * Radius;
	vertex[1].Position = XMFLOAT3(x, y, 0.0f);

	x = X - cosf(BaseAngle - Rot) * Radius;
	y = Y + sinf(BaseAngle - Rot) * Radius;
	vertex[2].Position = XMFLOAT3(x, y, 0.0f);

	x = X + cosf(BaseAngle + Rot) * Radius;
	y = Y + sinf(BaseAngle + Rot) * Radius;
	vertex[3].Position = XMFLOAT3(x, y, 0.0f);



	vertex[0].Diffuse = Color;
	vertex[1].Diffuse = Color;
	vertex[2].Diffuse = Color;
	vertex[3].Diffuse = Color;

	vertex[0].TexCoord = XMFLOAT2(U, V);
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);

	GetDeviceContext()->Unmap(buf, 0);

}

void SetSpriteColorRotationBottomUp(ID3D11Buffer* buf, float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color, float Rot)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float hw;
	hw = Width * 0.5f;		// コンピューターは割り算が苦手
	//hh = Height * 0.5f;		// 掛け算の方が処理が速い

	// 指定された座標を中心に回転するプログラム
	float BaseAngle = atan2f(Height, hw);			// 中心点から頂点に対する角度
	XMVECTOR temp = { hw, Height, 0.0f, 0.0f };
	temp = XMVector2Length(temp);				// 中心点から頂点に対する距離
	float Radius = 0.0f;
	XMStoreFloat(&Radius, temp);

	// ここでアフィン変換（sincosのやつ）を使って4頂点を回転させる
	float x = X - cosf(BaseAngle + Rot) * Radius;
	float y = Y - sinf(BaseAngle + Rot) * Radius;
	vertex[0].Position = XMFLOAT3(x, y, 0.0f);

	x = X + cosf(BaseAngle - Rot) * Radius;
	y = Y - sinf(BaseAngle - Rot) * Radius;
	vertex[1].Position = XMFLOAT3(x, y, 0.0f);

	x = X - cosf(Rot) * hw;
	y = Y - sinf(Rot) * hw;
	vertex[2].Position = XMFLOAT3(x, y, 0.0f);

	x = X + cosf(Rot) * hw;
	y = Y + sinf(Rot) * hw;
	vertex[3].Position = XMFLOAT3(x, y, 0.0f);



	vertex[0].Diffuse = Color;
	vertex[1].Diffuse = Color;
	vertex[2].Diffuse = Color;
	vertex[3].Diffuse = Color;

	vertex[0].TexCoord = XMFLOAT2(U, V);
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);

	GetDeviceContext()->Unmap(buf, 0);

}
void SetSpriteColorRotIso(ID3D11Buffer* buf, float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color, float Rot,
	float ratio)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = Width * 0.5f;		// コンピューターは割り算が苦手
	hh = Height * 0.5f;		// 掛け算の方が処理が速い

	// 指定された座標を中心に回転するプログラム
	float BaseAngle = atan2f(hh, hw);			// 中心点から頂点に対する角度
	XMVECTOR temp = { hw, hh, 0.0f, 0.0f };
	temp = XMVector2Length(temp);				// 中心点から頂点に対する距離
	float Radius = 0.0f;
	XMStoreFloat(&Radius, temp);

	// ここでアフィン変換（sincosのやつ）を使って4頂点を回転させる
	float x = X - cosf(BaseAngle + Rot) * Radius;
	float y = Y - ratio * sinf(BaseAngle + Rot) * Radius;
	vertex[0].Position = XMFLOAT3(x, y, 0.0f);

	x = X + cosf(BaseAngle - Rot) * Radius;
	y = Y - sinf(BaseAngle - Rot) * Radius;
	vertex[1].Position = XMFLOAT3(x, y, 0.0f);

	x = X - cosf(BaseAngle - Rot) * Radius;
	y = Y + ratio * sinf(BaseAngle - Rot) * Radius;
	vertex[2].Position = XMFLOAT3(x, y, 0.0f);

	x = X + cosf(BaseAngle + Rot) * Radius;
	y = Y + sinf(BaseAngle + Rot) * Radius;
	vertex[3].Position = XMFLOAT3(x, y, 0.0f);

	XMVECTOR V0pos = XMLoadFloat3(&vertex[0].Position);
	XMVECTOR V3pos = XMLoadFloat3(&vertex[3].Position);
	XMVECTOR v3 = LinearInterpolation(V0pos, V3pos, ratio);
	XMVECTOR v0 = LinearInterpolation(V3pos, V0pos, ratio);
	XMStoreFloat3(&vertex[0].Position, v0);
	XMStoreFloat3(&vertex[3].Position, v3);

	vertex[0].Diffuse = Color;
	vertex[1].Diffuse = Color;
	vertex[2].Diffuse = Color;
	vertex[3].Diffuse = Color;

	vertex[0].TexCoord = XMFLOAT2(U, V);
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);

	GetDeviceContext()->Unmap(buf, 0);

}



void SetSpriteIsoAndDrawLayer2(ID3D11Buffer* buf, float X, float Y, float TexSize, float Polisize,
	float U, float V, float UW, float VH,
	ID3D11ShaderResourceView** Texture,
	float* MappChipList,
	int height, int width)
{
	PLAYER* player = GetPlayer();
	int px = (int)player[0].sprite.pos.x;
	int py = (int)player[0].sprite.pos.y;
	float adjust;
	BOOL shade = FALSE;//階段がプレイヤーの前にいるか？
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			adjust = (MappChipList[x * height + y]) - (int)(MappChipList[x * height + y]);
			if (((px + 1) == x) && (py == y) || ((px == x) && (py + 1 == y)) || ((px == x - 1) && (py == y - 1)) || ((px == x - 1) && (py == y - 2)))
			{

				shade = TRUE;
			}

			if (shade)
			{
				if (!GetStair())
				{
					GetDeviceContext()->PSSetShaderResources(0, 1, &Texture[(int)(10 * adjust) + 3]);
					SetSprite(buf, X - TexSize * 0.5f * x + TexSize * 0.5f * y, Y + TexSize * 0.25f * x + TexSize * 0.25f * y - TexSize * 0.5f, Polisize, Polisize, U, V, UW, VH);
					GetDeviceContext()->Draw(4, 0);

				}
			}





		}
	}




}
void SetSpriteWithVertexsColor(ID3D11Buffer* buf, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT3 v3, XMFLOAT3 v4,
	float U, float V, float UW, float VH,
	XMFLOAT4 color)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;


	// 指定された座標を中心に設定するプログラム

	// 頂点０番（左上の頂点）
	vertex[0].Position = v1;
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = XMFLOAT2(U, V);

	// 頂点１番（右上の頂点）
	vertex[1].Position = v2;
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);

	// 頂点２番（左下の頂点）
	vertex[2].Position = v3;
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);

	// 頂点３番（右下の頂点）
	vertex[3].Position = v4;
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);

	GetDeviceContext()->Unmap(buf, 0);


}

//地面を透視投影で描画する
//s1　ポリゴン近辺と投影面の理論深度差
//s2　投影面と観測点の理論深度差
//h   観測点の高さ（地面ポリゴンとの理論距離）
void SetSpriteColorPerspetiveFloorandDraw(ID3D11Buffer* buf, float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color,
	float s1, float s2, float h)
{
	//ポリゴン中心からモニター中心だでのX軸距離

	float hw;
	hw = Width * 0.5f;			// コンピューターは割り算が苦手
	//hh = Height * 0.5f;		// 掛け算の方が処理が速い

	//近辺/遠辺と観測点の理論深度差
	float depthnear = s1 + s2;
	float depthfar = depthnear + Height;
	float angle1 = atan2f(h - Y, s2 + s1);			//ポリゴンの近辺から観測点の角度	
	float angle2 = atan2f(h - Y, s2 + s1 + Height);	//ポリゴンの遠辺から観測点の角度

	float h1 = tanf(angle1) * s1;
	float h2 = tanf(angle2) * (s1 + Height);
	float tate = h2 - h1;

	XMVECTOR lt = { X - hw , Y , 0.0f ,  0.0f };
	XMVECTOR rt = { X + hw , Y , 0.0f ,  0.0f };
	XMVECTOR lb = { X - hw , Y , 0.0f ,  0.0f };
	XMVECTOR rb = { X + hw , Y , 0.0f ,  0.0f };
	XMVECTOR viewpoint = { SCREEN_CENTER_X , h , 0.0f , 0.0f };

	XMVECTOR p0 = viewpoint + (lt - viewpoint) / depthfar * s2;
	XMVECTOR p1 = viewpoint + (rt - viewpoint) / depthfar * s2;
	XMVECTOR p2 = viewpoint + (lb - viewpoint) / depthnear * s2;
	XMVECTOR p3 = viewpoint + (rb - viewpoint) / depthnear * s2;
	p0.m128_f32[1] = SCREEN_HEIGHT - p0.m128_f32[1];
	p1.m128_f32[1] = SCREEN_HEIGHT - p1.m128_f32[1];
	p2.m128_f32[1] = SCREEN_HEIGHT - p2.m128_f32[1];
	p3.m128_f32[1] = SCREEN_HEIGHT - p3.m128_f32[1];

	XMVECTOR tmep;
	float stride = 1 / tate;

	//視界外のポリゴンを非描画
	if (p1.m128_f32[0] < 0.0f)
		return;
	if (p0.m128_f32[0] > SCREEN_WIDTH)
		return;


	//長方形ポリゴンを透視投影処理して得た台形の描画
	for (int count = 0; count < tate + 1; count++)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
		//差分法で行ポリゴンの頂点を計算
		tmep = p0 + (p2 - p0) / tate * (float)count;
		XMStoreFloat3(&vertex[0].Position, tmep);
		XMStoreFloat3(&vertex[2].Position, tmep);
		vertex[2].Position.y++;

		tmep = p1 + (p3 - p1) / tate * (float)count;
		XMStoreFloat3(&vertex[1].Position, tmep);
		XMStoreFloat3(&vertex[3].Position, tmep);
		vertex[3].Position.y++;

		vertex[0].Diffuse = Color;
		vertex[1].Diffuse = Color;
		vertex[2].Diffuse = Color;
		vertex[3].Diffuse = Color;

		vertex[0].TexCoord = XMFLOAT2(U, V + stride * count);
		vertex[1].TexCoord = XMFLOAT2(U + UW, V + stride * count);
		vertex[2].TexCoord = XMFLOAT2(U, V + stride * count + stride);
		vertex[3].TexCoord = XMFLOAT2(U + UW, V + stride * count + stride);

		GetDeviceContext()->Unmap(buf, 0);

		GetDeviceContext()->Draw(4, 0);
	}

}

//ポリゴンを直交射影で描画する
//v1,v2,v3,v4 はポリゴン４頂点の座標
//Campos　カメラ座標
//Camat   カメラ注視点
void SetSpriteColorOrthoRot(ID3D11Buffer* buf, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT3 v3, XMFLOAT3 v4,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color,
	XMFLOAT3 Campos,
	XMFLOAT3 Camat)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	XMVECTOR temp = XMLoadFloat3(&Camat);
	temp.m128_f32[2]++;
	XMVECTOR campos = XMLoadFloat3(&Campos);
	XMVECTOR camat = XMLoadFloat3(&Camat);
	XMVECTOR normal = XMVector3Normalize(camat - campos);
	XMVECTOR vectorTpProject = temp - campos;
	XMVECTOR projection = XMVector3Dot(vectorTpProject, normal) * normal;
	temp = temp - projection;
	XMVECTOR tangent = temp - campos;
	XMMATRIX transformMatrix;
	transformMatrix.r[0] = tangent;
	transformMatrix.r[1] = XMVector3Cross(normal, tangent);
	transformMatrix.r[2] = normal;
	transformMatrix.r[3] = XMVectorSetW(campos, 1.0f);

	XMVECTOR V1 = OrthoProJection(v1, Campos, Camat) - campos;
	XMVECTOR V2 = OrthoProJection(v2, Campos, Camat) - campos;
	XMVECTOR V3 = OrthoProJection(v3, Campos, Camat) - campos;
	XMVECTOR V4 = OrthoProJection(v4, Campos, Camat) - campos;

	V1 = XMVector4Transform(V1, transformMatrix);
	V2 = XMVector4Transform(V2, transformMatrix);
	V3 = XMVector4Transform(V3, transformMatrix);
	V4 = XMVector4Transform(V4, transformMatrix);

	// 指定された座標を中心に設定するプログラム

	// 頂点０番（左上の頂点）
	XMStoreFloat3(&vertex[0].Position, V1);
	vertex[0].Diffuse = Color;
	vertex[0].TexCoord = XMFLOAT2(U, V);

	// 頂点１番（右上の頂点）
	XMStoreFloat3(&vertex[1].Position, V2);
	vertex[1].Diffuse = Color;
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);

	// 頂点２番（左下の頂点）
	XMStoreFloat3(&vertex[2].Position, V3);
	vertex[2].Diffuse = Color;
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);

	// 頂点３番（右下の頂点）
	XMStoreFloat3(&vertex[3].Position, V4);
	vertex[3].Diffuse = Color;
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);

	GetDeviceContext()->Unmap(buf, 0);

}

XMVECTOR OrthoProJection(XMFLOAT3 Vertexin, XMFLOAT3 Campos, XMFLOAT3 Camat)
{
	XMVECTOR temp = XMLoadFloat3(&Vertexin);
	XMVECTOR campos = XMLoadFloat3(&Campos);
	XMVECTOR camat = XMLoadFloat3(&Camat);
	XMVECTOR normal = XMVector3Normalize(camat - campos);
	XMVECTOR vectorTpProject = temp - campos;
	XMVECTOR projection = XMVector3Dot(vectorTpProject, normal) * normal;
	temp = temp - projection;
	return temp;
}


XMVECTOR LinearInterpolation(XMVECTOR p0, XMVECTOR  p1, float s)
{
	return (p0 * (1 - s) + p1 * s);
}
