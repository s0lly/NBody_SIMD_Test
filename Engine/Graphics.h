/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
/*******************************************************************************************
*	Code and amendments by s0lly														   *
*	https://www.youtube.com/c/s0lly							                               *
*	https://s0lly.itch.io/																   *
*	https://www.instagram.com/s0lly.gaming/												   *
********************************************************************************************/

#pragma once
#include "ChiliWin.h"
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"
#include "nmmintrin.h"

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x,y,z;		// position
		float u,v;			// texcoords
	};
public:
	Graphics( class HWNDKey& key );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	void EndFrame();
	void BeginFrame();
	void PutPixel( int x,int y,int r,int g,int b )
	{
		PutPixel( x,y,{ unsigned char( r ),unsigned char( g ),unsigned char( b ) } );
	}
	void PutPixel( int x,int y,Color c );
	void DrawRect(float locX, float locY, int length, int height, Color c, float alpha = 1.0f)
	{
		for (int j = (int)locY; j < (int)locY + height; j++)
		{
			if (j >= 0 && j < ScreenHeight)
			{
				for (int i = (int)locX; i < (int)locX + length; i++)
				{
					if (i >= 0 && i < ScreenWidth)
					{
						PutPixel(i, j, c);
					}
				}
			}
		}
	}
	void DrawCircleSIMD(float locX, float locY, float radius, Color c) // Doesn't work yet!!!
	{
		int left = (int)(locX - radius + 0.5f);
		left = left < 0 ? 0 : left;

		int right = (int)(locX + radius + 0.5f);
		right = right >= ScreenWidth ? ScreenWidth : right;

		int top = (int)(locY - radius + 0.5f);
		top = top < 0 ? 0 : top;

		int bottom = (int)(locY + radius + 0.5f);
		bottom = bottom >= ScreenHeight ? ScreenHeight : bottom;

		float radiusSqrd = radius * radius;

		__m256 colorVec = _mm256_set1_ps(*(float*)&c);

		int size = sizeof(Color);

		__m256 xVec = _mm256_set1_ps(locX);
		__m256 yVec = _mm256_set1_ps(locY);

		for (int j = top; j < bottom; j++)
		{
			float xSqrd = radiusSqrd - ((int)locY - j) * ((int)locY - j);

			float xBegin = locX - sqrt(xSqrd) + 0.5f;
			float xEnd = locX + sqrt(xSqrd) + 0.5f;

			if (xBegin > xEnd)
			{
				std::swap(xBegin, xEnd);
			}

			xBegin = xBegin < left ? left : xBegin;
			xEnd = xEnd >= right ? right : xEnd;

			int i = (int)xBegin;

			for (; i < (int)xEnd - 7; i += 8)
			{
				memcpy(&pSysBuffer[Graphics::ScreenWidth * j + i], &colorVec, sizeof(colorVec));
			}

			for (; i < (int)xEnd; i++)
			{
				memcpy(&pSysBuffer[Graphics::ScreenWidth * j + i], &c, sizeof(c));
			}
		}
	}

	~Graphics();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Color*                                              pSysBuffer = nullptr;
public:
	static constexpr int ScreenWidth = 1600;
	static constexpr int ScreenHeight = 900;
};