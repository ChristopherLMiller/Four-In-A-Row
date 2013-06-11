#ifndef INTRO_STATE_H
#define INTRO_STATE_H

#include "GameState.h"

class IntroState : public GameState
{
private:
	IntroState() {clear();};
	static IntroState		m_IntroState;

	IDirect3DTexture9		*m_pIntroTexture;
	ID3DXSprite*		mSprite;

	D3DXVECTOR3			mBkgrdCntr;		// center of the background texture
	RECT				mBkgrdRect;		// source rectangle for drawing background

	bool m_keyEnterPressed;
	float delay;

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
	void onLostDevice(Connect4 *pGame);

	static IntroState*	Instance(){return &m_IntroState;}
};

#endif