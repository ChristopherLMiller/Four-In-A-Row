#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "GameState.h"

class PlayState : public GameState
{
private:
	PlayState() {clear();};
	static PlayState	m_PlayState;

	IDirect3DTexture9*	mTexture;
	ID3DXSprite*		mSprite;
	ID3DXFont*			mFont;

	D3DXVECTOR3			mBkgrdCntr;		// center of the background texture
	RECT				mBkgrdRect;		// source rectangle for drawing background
	D3DXVECTOR3			mBlackCntr;		// center of the black piece
	RECT				mBlackRect;		// source rectangle for drawing black piece
	D3DXVECTOR3			mRedCntr;		// center of the red piece
	RECT				mRedRect;		// source rectangle for drawing red piece
	int					mBoard[6][7];	// game board
	static const int	NUM_ROWS = 6;	// number of rows
	static const int	NUM_COLS = 7;	// number of columns
	int					mPlayer;		// number that represents the player
	int					mComputer;		// number to represent the computer
	int					mTurn;			// current persons turn
	int					mWinner;		// who won

	bool				mKey1Pressed;	// key 1 pressed
	bool				mKey2Pressed;	// key 2 pressed
	bool				mKey3Pressed;	// key 3 pressed
	bool				mKey4Pressed;	// key 4 pressed
	bool				mKey5Pressed;	// key 5 pressed
	bool				mKey6Pressed;	// key 6 pressed
	bool				mKey7Pressed;	// key 7 pressed
	bool				mKeyEscPressed;	// escape key pressed
	bool				mKeyEnterPressed;

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

	static PlayState*	Instance() { return &m_PlayState; }

	// gameplay related functions
	void drawPieces();
	void drawBackground();

	// game functions
	void humanMove();
	void computerMove();
	int opponent(int turn);
	void announceWinner();
	int winner();

	// AI functions
	void randomPlace();

	// function to clear the board
	void clearBoard();
};

#endif