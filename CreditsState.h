#ifndef CREDITSSTATE_H
#define CREDITSSTATE_H

#include "GameState.h"

class CreditsState : public GameState
{
private:
	CreditsState() {clear();};
	static CreditsState		m_CreditsState;

	IDirect3DTexture9		*m_pCreditTexture;
	ID3DXSprite*			mSprite;

	D3DXVECTOR3				m_CreditCntr;
	RECT					m_CreditRect;

	bool m_keyEscPressed;
	bool m_keyEnterPressed;


public:

	virtual void clear();
	virtual void destroy(Connect4* pGame);
	virtual BOOL create(Connect4* pGame);

	virtual void checkInput(Connect4* pGame);
	virtual void update(Connect4* pGame);
	virtual void display(Connect4* pGame);

	virtual void pause();
	virtual void resume();

	virtual void onLostDevice(Connect4 *pGame);
	virtual void onResetDevice(Connect4 *pGame, HWND);

	static CreditsState* Instance() { return &m_CreditsState;}
};

#endif