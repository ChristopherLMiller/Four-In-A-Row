#include <d3dx9.h>
#include <d3d9.h>
#include "MainMenuState.h"
#include "DirectInput.h"
#include "CreditsState.h"
#include "PlayState.h"

#pragma warning(disable: 4309)

MainMenuState MainMenuState::m_MainMenuState;

void MainMenuState::clear()
{
	m_pMainMenuTexture = NULL;
	mSprite = NULL;
}

void MainMenuState::destroy(Connect4 *pGame)
{
	ReleaseCOM(m_pMainMenuTexture);
	ReleaseCOM(mSprite);
}

BOOL MainMenuState::create(Connect4 *pGame)
{
	// create the texture and sprite device
	HR(D3DXCreateTextureFromFile(gd3dDevice, "MainMenu.jpg", &m_pMainMenuTexture));
	HR(D3DXCreateSprite(gd3dDevice, &mSprite));

	// set the texture variables
	mBkgrdCntr = D3DXVECTOR3(512.0f,384.0f,1.0f);
	SetRect(&mBkgrdRect,   0,   256,  1024, 1024); 

	// clear keyboard input
	m_keyEnterPressed = false;
	m_keyEscPressed = false;
	m_keyUpPressed = false;
	m_keyDownPressed = false;

	// set up the fonts
	m_playColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_CreditsColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_QuitColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// set the font rect
	SetRect(&m_playRect, 0, 225, 1024, 768);
	SetRect(&m_CreditsRect, 0, 325, 1024, 768);
	SetRect(&m_QuitRect, 0, 425, 1024, 768);

	m_currentSelected = PLAY;

	D3DXFONT_DESC fontDesc;
	fontDesc.Height          = 35;
    fontDesc.Width           = 20;
    fontDesc.Weight          = 0;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
    strcpy_s(fontDesc.FaceName, _T("Times New Roman"));
	
	HR(D3DXCreateFontIndirect(gd3dDevice, &fontDesc, &mFont));

	return TRUE;
}

void MainMenuState::checkInput(Connect4 *pGame)
{
	gDInput->poll();

	// see if the user is pressing the button
	if (gDInput->keyDown(DIK_RETURN) && !m_keyEnterPressed)
	{
		m_keyEnterPressed = true;
	}
	if (gDInput->keyDown(DIK_ESCAPE) && !m_keyEscPressed)
	{
		m_keyEscPressed = true;
	}
	if (gDInput->keyDown(DIK_UP) && !m_keyUpPressed)
	{
		m_keyUpPressed = true;
	}
	if (gDInput->keyDown(DIK_DOWN) && !m_keyDownPressed)
	{
		m_keyDownPressed = true;
	}
	// now see if the release the button if so, then change the state
	if (!gDInput->keyDown(DIK_RETURN) && m_keyEnterPressed)
	{
		m_keyEnterPressed = false;

		switch(m_currentSelected)
		{
		case PLAY:
			pGame->changeState(PlayState::Instance());
			break;
		case CREDITS:
			pGame->changeState(CreditsState::Instance());
			break;
		case QUIT:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
	}
	if (!gDInput->keyDown(DIK_ESCAPE) && m_keyEscPressed)
	{
		m_keyEscPressed = false;
		PostQuitMessage(0);
	}	
	if (!gDInput->keyDown(DIK_UP) && m_keyUpPressed)
	{
		// see if we are already at top, if so jump to bottom
		m_keyUpPressed = false;
		if (m_currentSelected == PLAY)
		{
			m_currentSelected = QUIT;
		}
		else
		{
			m_currentSelected--;
		}
	}
	if (!gDInput->keyDown(DIK_DOWN) && m_keyDownPressed)
	{
		m_keyDownPressed = false;
		if (m_currentSelected == QUIT)
		{
			m_currentSelected = PLAY;
		}
		else
		{
			m_currentSelected ++;
		}
	}
}

void MainMenuState::update(Connect4 *pGame)
{
	// change font colors based on selected item
	if (m_currentSelected == PLAY)
	{
		m_playColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		m_playColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	if (m_currentSelected == CREDITS)
	{
		m_CreditsColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		m_CreditsColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	if (m_currentSelected == QUIT)
	{
		m_QuitColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		m_QuitColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

}

void MainMenuState::display(Connect4 *pGame)
{
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(46,54,153), 1.0f, 0));  //blue
	HR(gd3dDevice->BeginScene());
	HR(mSprite->Begin(D3DXSPRITE_OBJECTSPACE | D3DXSPRITE_DONOTMODIFY_RENDERSTATE));
	
	// Turn on the alpha state
	HR(gd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true));

	// draw the background sprite
	HR(mSprite->Draw(m_pMainMenuTexture, &mBkgrdRect, &mBkgrdCntr, 0, D3DCOLOR_XRGB(255,255,255)));
	HR(mSprite->Flush());

	// Turn off the alpha state
	HR(gd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false));

	HR(mSprite->End());

	// render the fonts
	mFont->DrawText(0, _T("Play Game"), -1, &m_playRect, DT_NOCLIP | DT_CENTER, m_playColor);
	mFont->DrawText(0, _T("Credits"), -1, &m_CreditsRect, DT_NOCLIP | DT_CENTER, m_CreditsColor);
	mFont->DrawText(0, _T("Quit"), -1, &m_QuitRect, DT_NOCLIP | DT_CENTER, m_QuitColor);

	HR(gd3dDevice->EndScene());
	HR(gd3dDevice->Present(NULL, NULL, NULL, NULL));
}

void MainMenuState::onResetDevice(Connect4* pGame, HWND mhMainWnd)
{
	// Call the onResetDevice of other objects.
	mSprite->OnResetDevice();
	mFont->OnResetDevice();

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

void MainMenuState::onLostDevice(Connect4* pGame)
{
	mSprite->OnLostDevice();
	mFont->OnLostDevice();
}