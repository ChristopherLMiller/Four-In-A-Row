#ifndef MAINMENU_STATE_H
#define MAINMENU_STATE_H

#include "GameState.h"

class MainMenuState : public GameState
{
private:
	MainMenuState() {clear();};
	static MainMenuState		m_MainMenuState;

	IDirect3DTexture9		*m_pMainMenuTexture;
	ID3DXSprite*		mSprite;
	ID3DXFont*			mFont;

	D3DXVECTOR3			mBkgrdCntr;		// center of the background texture
	RECT				mBkgrdRect;		// source rectangle for drawing background

	bool m_keyEnterPressed;
	bool m_keyEscPressed;
	bool m_keyUpPressed;
	bool m_keyDownPressed;
	
	D3DXCOLOR		m_playColor;
	RECT			m_playRect;
	D3DXCOLOR		m_CreditsColor;
	RECT			m_CreditsRect;
	D3DXCOLOR		m_QuitColor;
	RECT			m_QuitRect;
	int				m_currentSelected;
	enum choice {PLAY =1, CREDITS, QUIT};

public:
	void clear();
	void destroy(Connect4* pGame);
	BOOL create(Connect4* pGame);

	void checkInput(Connect4* pGame);
	void update(Connect4* pGame);
	void display(Connect4* pGame);

	void pause(){};
	void resume(){};

	void onResetDevice(Connect4* pGame, HWND mhMainWnd);
	void onLostDevice(Connect4* pGame);
	static MainMenuState*	Instance(){return &m_MainMenuState;}
};

#endif