//////////////////////////////////////////////////////////////////////////////////////
//
// File: Connect4.cpp
// Desc: main application file for the connect 4 game
// Author: Chris Miller
// Date: 6/12/08
//
//////////////////////////////////////////////////////////////////////////////////////

#include "Global.h"
#include "IntroState.h"
#define DEBUG

using std::vector;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	Connect4 app(hInstance, "Four In a Row - v.1.0", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gd3dApp = &app;

	DirectInput di(DISCL_NONEXCLUSIVE|DISCL_FOREGROUND, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	gDInput = &di;

	return gd3dApp->run();
}

Connect4::Connect4(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP)
:D3DApp(hInstance, winCaption, devType, requestedVP)
{
	if (!checkDeviceCaps())
	{
		logError("checkDeviceCaps - FAILED");
		return;
	}

	changeState(IntroState::Instance());
	enableFullScreenMode(true);
	onResetDevice();

}
Connect4::~Connect4()
{
	// release resources
}

bool Connect4::checkDeviceCaps()
{
	// Nothing to check
	return true;
}

void Connect4::onLostDevice()
{
	// reset any device that needs to be reset
	m_States.back()->onLostDevice(this);
}

void Connect4::onResetDevice()
{
	// Call the onResetDevice of other objects.
	m_States.back()->onResetDevice(this, mhMainWnd);

	// Sets up the camera 1000 units back looking at the origin.
	D3DXMATRIX V;
	D3DXVECTOR3 pos(0.0f, 0.0f, -1000.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);
	HR(gd3dDevice->SetTransform(D3DTS_VIEW, &V));

	// The following code defines the volume of space the camera sees.
	D3DXMATRIX P;
	RECT R;
	GetClientRect(mhMainWnd, &R);
	float width  = (float)R.right;
	float height = (float)R.bottom;
	D3DXMatrixPerspectiveFovLH(&P, D3DX_PI*0.25f, width/height, 1.0f, 5000.0f);
	HR(gd3dDevice->SetTransform(D3DTS_PROJECTION, &P));

	// This code sets texture filters, which helps to smooth out distortions
	// when you scale a texture.  
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR));
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR));
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR));

	// This line of code disables Direct3D lighting.
	HR(gd3dDevice->SetRenderState(D3DRS_LIGHTING, false));
	
	// The following code specifies an alpha test and reference value.
	HR(gd3dDevice->SetRenderState(D3DRS_ALPHAREF, 10));
	HR(gd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER));

	// The following code is used to setup alpha blending.
	HR(gd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE));
	HR(gd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1));
	HR(gd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
	HR(gd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA));

	// Indicates that we are using 2D texture coordinates.
	HR(gd3dDevice->SetTextureStageState(
		0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2));
}

void Connect4::updateScene(float dt)
{
	m_States.back()->update(this);
}

void Connect4::drawScene()
{
	m_States.back()->display(this);
}

void Connect4::changeState(GameState* pState)
{
	// cleanup the current state
	if ( !m_States.empty() )
	{
		m_States.back()->destroy(this);
		m_States.pop_back();
	}

	// store and init the new state
	m_States.push_back(pState);
	m_States.back()->create(this);
}

void Connect4::pushState(GameState* pState)
{
	// pause current state
	if ( !m_States.empty() ) {
		m_States.back()->pause();
	}

	// store and init the new state
	m_States.push_back(pState);
	m_States.back()->create(this);
}

void Connect4::popState()
{
	// cleanup the current state
	if ( !m_States.empty() ) {
		m_States.back()->destroy(this);
		m_States.pop_back();
	}

	// resume previous state
	if ( !m_States.empty() ) {
		m_States.back()->resume();
	}
}

void Connect4::checkInput()
{
	m_States.back()->checkInput(this);
}