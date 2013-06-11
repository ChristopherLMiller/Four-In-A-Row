///////////////////////////////////////////////////////////////////////////////////////////////
//
// File: GameState.h
//
// Desc: declaration of a base GameState for further GameStates to derive from
//
// Programmer: Chris Miller
//
// Date: 7/13/08
//
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Global.h"

class GameState
{
protected:
	GameState(){};

public:

	virtual void clear() = 0;
	virtual void destroy(Connect4* pGame) = 0;
	virtual BOOL create(Connect4* pGame) = 0;

	virtual void checkInput(Connect4* pGame) = 0;
	virtual void update(Connect4* pGame) = 0;
	virtual void display(Connect4* pGame) = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void onLostDevice(Connect4 *pGame) = 0;
	virtual void onResetDevice(Connect4 *pGame, HWND) = 0;

	void changeState(Connect4* pGame, GameState *pState)
	{
		pGame->changeState(pState);
	}
};

#endif