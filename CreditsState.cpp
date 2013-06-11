#include <d3d9.h>
#include <d3dx9.h>
#include "MainMenuState.h"
#include "CreditsState.h"

CreditsState CreditsState::m_CreditsState;

void CreditsState::clear()
{
	m_pCreditTexture = NULL;
}

void CreditsState::destroy(Connect4* pGame)
{
	ReleaseCOM(m_pCreditTexture);
}

BOOL CreditsState::create(Connect4 *pGame)
{
	m_keyEnterPressed = false;
	m_keyEscPressed = false;

	HR(D3DXCreateTextureFromFile(gd3dDevice, "Credits.jpg", &m_pCreditTexture));
	HR(D3DXCreateSprite(gd3dDevice, &mSprite));

	m_CreditCntr = D3DXVECTOR3(512.0f,384.0f,1.0f);
	SetRect(&m_CreditRect, 0,   256,  1024, 1024);

	return TRUE;
}

void CreditsState::checkInput(Connect4 *pGame)
{
	gDInput->poll();

	if (gDInput->keyDown(DIK_ESCAPE) && !m_keyEscPressed)
	{
		m_keyEscPressed = true;
	}
	if (gDInput->keyDown(DIK_RETURN) && !m_keyEnterPressed)
	{
		m_keyEnterPressed = true;
	}

	if (!gDInput->keyDown(DIK_ESCAPE) && m_keyEscPressed)
	{
		m_keyEscPressed = false;
		pGame->changeState(MainMenuState::Instance());
	}

	if (!gDInput->keyDown(DIK_RETURN) && m_keyEnterPressed)
	{
		m_keyEnterPressed = false;
		pGame->changeState(MainMenuState::Instance());
	}
}

void CreditsState::update(Connect4* pGame)
{
}

void CreditsState::display(Connect4 *pGame)
{
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(46,54,153), 1.0f, 0));  //blue
	HR(gd3dDevice->BeginScene());
	HR(mSprite->Begin(D3DXSPRITE_OBJECTSPACE | D3DXSPRITE_DONOTMODIFY_RENDERSTATE));

	// Turn on the alpha state
	HR(gd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true));

	// draw the background sprite
	HR(mSprite->Draw(m_pCreditTexture, &m_CreditRect, &m_CreditCntr, 0, D3DCOLOR_XRGB(255,255,255)));
	HR(mSprite->Flush());

	// Turn off the alpha state
	HR(gd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false));

	HR(mSprite->End());
	HR(gd3dDevice->EndScene());
	HR(gd3dDevice->Present(NULL, NULL, NULL, NULL));
}

void CreditsState::onLostDevice(Connect4 *pGame)
{
	mSprite->OnLostDevice();
}

void CreditsState::onResetDevice(Connect4 *pGame, HWND mhMainWnd)
{
	// reset the other objects

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

void CreditsState::resume()
{
}

void CreditsState::pause()
{
}

