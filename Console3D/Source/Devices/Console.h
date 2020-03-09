#pragma once

#define NOMINMAX
#include <Windows.h>

#include <string>
#include <thread>

#include "Geometry\Geometry3D\Transform3D.h"
#include "Geometry\Geometry3D\HVector3D.h"
#include "Geometry\Geometry2D\HVector2D.h"

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

private:
	enum FOVPostion
	{
		Behind = -1,
		Center =  1,
		Top    =  2,
		Bottom =  4,
		Left   =  8,
		Right  =  16
	};

	short PointInFOV(const HVector3D& vec) const;

	bool ScreenPlaneProjection(const HVector3D& a, const HVector3D& b, HVector3D& pa, HVector3D& pb) const;

	void DrawPoint(float x, float y, char c = '#');

	bool LineInSight(HVector2D& OA, HVector2D& OB);

	void DrawLine(HVector2D v1, HVector2D v2);

	void DisplayMessage(const std::string& msg);

	void HeartBeat();

	void Render();
};