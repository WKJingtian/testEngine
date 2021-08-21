#pragma once
#include "bgfx/bgfx.h"

namespace tengine
{

	struct PosColorVertex
	{
		float m_x;
		float m_y;
		float m_z;
		uint32_t m_abgr;

		//static void init()
		//{
		//	ms_layout
		//		.begin()
		//		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		//		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
		//		.end();
		//};
		//
		//static bgfx::VertexLayout ms_layout;
	};

	static PosColorVertex QuadVertex[] =
	{
		{-0.5f,  0.5f,  0, 0xFF0000FF },
		{ 0.5f,  0.5f,  0, 0x00FF00FF },
		{-0.5f, -0.5f,  0, 0x0000FFFF },
		{ 0.5f,  0.5f,  0, 0x00FF00FF },
		{ 0.5f, -0.5f,  0, 0xFFFFFFFF },
		{-0.5f, -0.5f,  0, 0x0000FFFF },
	};


	static const uint32_t QuadIndex[] =
	{
		//0, 2, 3,
		0, 2, 1,
		3, 5, 4,
		//4, 2, 3,
		//4, 3, 2,
	};
}