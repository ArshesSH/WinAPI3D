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
		cVector3 axisZTextPos;
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
			isRbuttonDown( false ),
			rotAngle(0,0,0)
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
		bool isRbuttonDown;
		float distance;
		cVector3 rotAngle;
	};

	class Object
	{
	public:
		Object()
			:
			pos(0,0,0),
			rotY(0.0f),
			dir(0,0,1),
			scale( 1.0f )
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
		float scale;
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

		SetGrid();
	}
	void Update()
	{
		Update_Rotation();
		Update_Move();
		Update_Scale();

		RECT rc;
		GetClientRect( g_hWnd, &rc );

		cMatrix matRX = cMatrix::RotationX( cam.rotAngle.x );
		cMatrix matRY = cMatrix::RotationY( cam.rotAngle.y );
		cMatrix camMatR = matRX * matRY;

		cam.vLookAt = cube.pos;
		cam.eyePos = { 0.0f, cam.distance, cam.distance };
		cam.eyePos = cVector3::TransformCoord( cam.eyePos, camMatR );
		cam.eyePos = cam.eyePos + cube.pos;

		cMatrix matS = cMatrix::Scale( cube.scale );
		cMatrix matR = cMatrix::RotationY( cube.rotY );
		auto matT= cMatrix::Translation( cube.pos );

		cube.dir = { 0.0f, 0.0f, 1.0f };
		cube.dir = cVector3::TransformCoord( cube.dir, matR );

		world.worldMat = matS * matR * matT;
		//world.worldMat = matT * matR * matS;
		cam.viewMat = cMatrix::View( cam.eyePos, cam.vLookAt, cam.vUp );
		cam.projMat = cMatrix::Projection( PI / 4.0f, ((float)rc.right / (float)rc.bottom), 1.0f, 1000.0f );
		cam.viewportMat = cMatrix::Viewport( 0, 0, rc.right, rc.bottom, 0, 1 );

	}
	void Render( HDC hdc )
	{

		RECT rc;
		GetClientRect( g_hWnd, &rc );

		PatBlt( memDC, rc.left, rc.top, rc.right, rc.bottom, WHITENESS );

		DrawGrid();

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
	{
		switch ( message )
		{
		case WM_MOUSEWHEEL:
			cam.distance -= (GET_WHEEL_DELTA_WPARAM(wParam) / 1000.0f);
			if ( cam.distance < 1.0f )
			{
				cam.distance = 1.0f;
			}
			break;

		case WM_RBUTTONDOWN:
			{
				cam.prevMouse.x = LOWORD( lParam );
				cam.prevMouse.y = HIWORD( lParam );
				cam.isRbuttonDown = true;
			}
			break;

		case WM_RBUTTONUP:
			cam.isRbuttonDown = false;
			break;

		case WM_MOUSEMOVE:
			{
				if ( cam.isRbuttonDown )
				{
					POINT ptCurMouse;
					ptCurMouse.x = LOWORD( lParam );
					ptCurMouse.y = HIWORD( lParam );

					float dx = (float)ptCurMouse.x - (float)cam.prevMouse.x;
					float dy = (float)ptCurMouse.y - (float)cam.prevMouse.y;

					cam.rotAngle.y += (dx / 100.0f);
					cam.rotAngle.x += (dy / 100.0f);

					const float minAngle = -PI / 2.0f + EPSILON;
					const float maxAngle = -minAngle;
					if ( cam.rotAngle.x < minAngle )
					{
						cam.rotAngle.x = minAngle;
					}
					if ( cam.rotAngle.x > maxAngle )
					{
						cam.rotAngle.x = maxAngle;
					}

					cam.prevMouse = ptCurMouse;
				}
			}
			break;
		}



	}

	void SetGrid()
	{
		int halfTileCnt = 5;
		float interval = 1.0f;
		float max = halfTileCnt * interval;
		float min = -halfTileCnt * interval;

		for ( int i = 1; i <= halfTileCnt; ++i )
		{
			world.lineVertex.emplace_back( min, 0, i * interval );
			world.lineVertex.emplace_back( max, 0, i * interval );
			world.lineVertex.emplace_back( min, 0, -i * interval );
			world.lineVertex.emplace_back( max, 0, -i * interval );

			world.lineVertex.emplace_back( i * interval, 0, min );
			world.lineVertex.emplace_back( i * interval, 0, max );
			world.lineVertex.emplace_back( -i * interval, 0, min );
			world.lineVertex.emplace_back( -i * interval, 0, max );
		}

		world.lineVertex.emplace_back( 0, 0, min );
		world.lineVertex.emplace_back( 0, 0, max );
		world.lineVertex.emplace_back( min, 0, 0 );
		world.lineVertex.emplace_back( max, 0, 0 );

		world.axisXTextPos = { max, 0, 0 };
		world.axisZTextPos = { 0,0,max };
	}
	void DrawGrid()
	{
		cMatrix mat = cam.viewMat * cam.projMat * cam.viewportMat;
		for ( size_t i = 0; i < world.lineVertex.size(); i += 2 )
		{
			cVector3 v0 = world.lineVertex[i];
			cVector3 v1 = world.lineVertex[i + 1];

			v0 = cVector3::TransformCoord( v0, mat );
			v1 = cVector3::TransformCoord( v1, mat );

			MoveToEx( memDC, v0.x, v0.y, NULL );
			LineTo( memDC, v1.x, v1.y );
		}
		cVector3 v = world.axisXTextPos;
		v = cVector3::TransformCoord( v, mat );
		TextOut( memDC, v.x, v.y, (LPCWSTR)"X", 1 );

		v = world.axisZTextPos;
		v = cVector3::TransformCoord( v, mat );
		TextOut( memDC, v.x, v.y, (LPCWSTR)"Z", 1 );
	}
	bool IsBackFace( const cVector3& v0, const cVector3& v1, const cVector3& v2 ) const
	{
		cVector3 v01 = v1 - v0;
		cVector3 v02 = v2 - v0;
		cVector3 n = cVector3::Cross( v01, v02 );
		cVector3 look = { 0.0f, 0.0f, 1.0f };
		return (cVector3::Dot( n, look ) > 0);
	}
	void Update_Rotation()
	{
		if ( GetKeyState( 'A' ) & 0x8000 )
		{
			cube.rotY -= 0.1f;
		}
		if ( GetKeyState( 'D' ) & 0x8000 )
		{
			cube.rotY += 0.1f;
		}
	}
	void Update_Move()
	{
		if ( GetKeyState( 'W' ) & 0x8000 )
		{
			auto dir = cube.dir * 0.1f;
			cube.pos = cube.pos + dir;
		}
		if ( GetKeyState( 'S' ) & 0x8000 )
		{
			auto dir = cube.dir * 0.1f;
			cube.pos = cube.pos - dir;
		}
	}
	void Update_Scale()
	{
		if ( GetKeyState( 'R' ) & 0x8000 )
		{
			cube.scale += 0.1f;
			if ( cube.scale > 3.0f )
			{
				cube.scale = 3.0f;
			}
		}
		if ( GetKeyState( 'F' ) & 0x8000 )
		{
			cube.scale -= 0.1f;
			if ( cube.scale < 0.1f )
			{
				cube.scale = 0.1f;
			}
		}
	}
private:
	HDC						memDC;
	HBITMAP					hOldBitmap, hBitmap;

	Camera cam;
	Object cube;
	World world;
};

