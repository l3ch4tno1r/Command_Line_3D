#pragma once

#define NOMINMAX
#include <Windows.h>

#include <string>
#include <thread>

#include "Geometry\Geometry3D\Transform3D.h"
#include "Geometry\Geometry3D\HVector3D.h"
#include "Geometry\Geometry2D\HVector2D.h"

using namespace LCNMath::Geometry::Dim2;
using namespace LCNMath::Geometry::Dim3;

class Console
{
private:
	const float cm_ScreenDist = 0.1f;

private:
	//static Console console;

	UINT32 m_Width;
	UINT32 m_Height;

	char*  m_Screen;
	HANDLE m_HConsole;
	DWORD  m_DwBytesWritten;

	std::thread m_MainThread;

	float       m_Focal;
	Transform3D m_R0ToCam;

	Console();

	~Console();

	Console(const Console&) = delete;
	Console& operator=(const Console&) = delete;

	void MainThread();

public:
	static Console& Get();

	void Clear();

	inline Transform3D& R0ToCam()
	{
		return m_R0ToCam;
	}

	inline float& Focal()
	{
		return m_Focal;
	}

	static uint ClipEdge(const HVector3D& v1, const HVector3D& v2, // Edge
		                 const HVector3D& n,  const HVector3D& p,  // Plane parameters
		                       HVector3D& o1,       HVector3D& o2);
private:
	void DrawPoint(int x, int y, char c = '#');

	bool LineInSight(HVector2D& OA, HVector2D& OB);

	void DrawLine(int x1, int y1, int x2, int y2, char c = '#');

	void DrawLine(const HVector2D& v1, const HVector2D& v2, char c = '#');

	void DisplayMessage(const std::string& msg);

	void HeartBeat();

	void Render();
};