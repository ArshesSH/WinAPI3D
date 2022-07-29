#pragma once

#include "cVector3.h"
#include "cMatrix.h"
#include "framework.h"
#include <vector>

class cMainGame
{
public:
	class World
	{
	public:
		World()
		{
			worldMat = cMatrix::Identity( 4 );
		}
		
	public:
		cMatrix	worldMat;

		std::vector<cVector3> lineVertex;
		cVector3 axisXTextPos;
		cVector3 axisYTextPos;
	};

	class Camera
	{
	public:
		Camera()
			:
			eyePos( 0, 0, -5 ),
			vLookAt( 0, 0, 0 ),
			vUp( 0, 1, 0 ),
			distance(5.0f),
			isLbuttonDown(false),
			rotAngle(0,0,0),
			scale(1.0f)
		{
			prevMouse.x = 0;
			prevMouse.y = 0;
			viewMat = cMatrix::Identity( 4 );
			projMat = cMatrix::Identity( 4 );
			viewportMat = cMatrix::Identity( 4 );
		}

	public:
		cVector3 eyePos;
		cVector3 vLookAt;
		cVector3 vUp;
		cMatrix viewportMat;
		cMatrix	viewMat;
		cMatrix	projMat;

		POINT prevMouse;
		bool isLbuttonDown;
		float distance;
		cVector3 rotAngle;
		float scale;
	};

	class Object
	{
	public:
		Object()
			:
			pos(0,0,0),
			rotY(0.0f),
			dir(0,0,1)
		{
			vertices.emplace_back( -1.0f, -1.0f, -1.0f );	// 0
			vertices.emplace_back( -1.0f, 1.0f, -1.0f );	// 1
			vertices.emplace_back( 1.0f, 1.0f, -1.0f );	// 2
			vertices.emplace_back( 1.0f, -1.0f, -1.0f );	// 3

			vertices.emplace_back( -1.0f, -1.0f, 1.0f );	// 4
			vertices.emplace_back( -1.0f, 1.0f, 1.0f );	// 5
			vertices.emplace_back( 1.0f, 1.0f, 1.0f );		// 6
			vertices.emplace_back( 1.0f, -1.0f, 1.0f );	// 7

			indices.push_back( 0 );
			indices.push_back( 1 );
			indices.push_back( 2 );

			indices.push_back( 0 );
			indices.push_back( 2 );
			indices.push_back( 3 );

			indices.push_back( 4 );
			indices.push_back( 6 );
			indices.push_back( 5 );

			indices.push_back( 4 );
			indices.push_back( 7 );
			indices.push_back( 6 );

			indices.push_back( 4 );
			indices.push_back( 5 );
			indices.push_back( 1 );

			indices.push_back( 4 );
			indices.push_back( 1 );
			indices.push_back( 0 );

			indices.push_back( 3 );
			indices.push_back( 2 );
			indices.push_back( 6 );

			indices.push_back( 3 );
			indices.push_back( 6 );
			indices.push_back( 7 );

			indices.push_back( 1 );
			indices.push_back( 5 );
			indices.push_back( 6 );

			indices.push_back( 1 );
			indices.push_back( 6 );
			indices.push_back( 2 );

			indices.push_back( 4 );
			indices.push_back( 0 );
			indices.push_back( 3 );

			indices.push_back( 4 );
			indices.push_back( 3 );
			indices.push_back( 7 );
		}
	public:
		std::vector<cVector3>	vertices;
		std::vector<DWORD>		indices;
		cVector3 pos;
		cVector3 dir;
		float rotY;
	};

public:
	cMainGame()
		:
		hBitmap( nullptr ),
		cam(),
		cube()
	{

	}

	~cMainGame()
	{
		SelectObject( memDC, hOldBitmap );
		DeleteObject( hBitmap );
		DeleteDC( memDC );
	}

	void SetUp()
	{
		HDC hdc = GetDC( g_hWnd );
		memDC = CreateCompatibleDC( hdc );

		RECT rc;
		GetClientRect( g_hWnd, &rc );

		hBitmap = CreateCompatibleBitmap( hdc, rc.right, rc.bottom );
		hOldBitmap = (HBITMAP)SelectObject( memDC, hBitmap );
		ReleaseDC( g_hWnd, hdc );

	}
	void Update()
	{
		RECT rc;
		GetClientRect( g_hWnd, &rc );

		cam.vLookAt = { 0.0f, 0.0f, 0.0f };
		cam.eyePos = { 0.0f, 5.0f, -5.0f };

		cMatrix matR = cMatrix::RotationY( cube.rotY );
		auto matT= cMatrix::Translation( cube.pos );
		world.worldMat = matR * matT;
		cam.viewMat = cMatrix::View( cam.eyePos, cam.vLookAt, cam.vUp );
		cam.projMat = cMatrix::Projection( PI / 4.0f, ((float)rc.right / (float)rc.bottom), 1.0f, 1000.0f );
		cam.viewportMat = cMatrix::Viewport( 0, 0, rc.right, rc.bottom, 0, 1 );

	}
	void Render( HDC hdc )
	{
		RECT rc;
		GetClientRect( g_hWnd, &rc );

		PatBlt( memDC, rc.left, rc.top, rc.right, rc.bottom, WHITENESS );

		cMatrix matWVP = world.worldMat * cam.viewMat * cam.projMat;

		for ( size_t i = 0; i < cube.indices.size(); i += 3 )
		{
			cVector3 v0 = cube.vertices[cube.indices[i]];
			cVector3 v1 = cube.vertices[cube.indices[i + 1]];
			cVector3 v2 = cube.vertices[cube.indices[i + 2]];

			v0 = cVector3::TransformCoord( v0, matWVP );
			v1 = cVector3::TransformCoord( v1, matWVP );
			v2 = cVector3::TransformCoord( v2, matWVP );

			if ( IsBackFace( v0, v1, v2 ) )
			{
				continue;
			}

			v0 = cVector3::TransformCoord( v0, cam.viewportMat );
			v1 = cVector3::TransformCoord( v1, cam.viewportMat );
			v2 = cVector3::TransformCoord( v2, cam.viewportMat );

			MoveToEx( memDC, v0.x, v0.y, NULL );
			LineTo( memDC, v1.x, v1.y );
			LineTo( memDC, v2.x, v2.y );
			LineTo( memDC, v0.x, v0.y );
		}

		BitBlt( hdc, 0, 0, rc.right, rc.bottom, memDC, 0, 0, SRCCOPY );

	}

	void WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{}

	void SetGrid();
	void DrawGrid();
	bool IsBackFace( const cVector3& v0, const cVector3& v1, const cVector3& v2 ) const
	{
		cVector3 v01 = v1 - v0;
		cVector3 v02 = v2 - v0;
		cVector3 n = cVector3::Cross( v01, v02 );
		cVector3 look = { 0.0f, 0.0f, 1.0f };
		return (cVector3::Dot( n, look ) > 0);
	}
	void Update_Rotation();
	void Update_Move();
	void Update_Scale();

private:
	HDC						memDC;
	HBITMAP					hOldBitmap, hBitmap;

	Camera cam;
	Object cube;
	World world;
};

