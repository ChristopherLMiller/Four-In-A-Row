/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: Connect4.h
// Desc: declaration of the game class
// Author: Chris Miller
// Date: 6/12/08
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CONNECT4_H
#define __CONNECT4_H

#include "Global.h"

using namespace std;

class GameState;

class Connect4 : public D3DApp
{
private:
	vector<GameState*>  m_States;		// vector of game states

public:
	Connect4(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP);
	~Connect4();

	bool checkDeviceCaps();
	void onLostDevice();
	void onResetDevice();
	void checkInput();
	void updateScene(float dt);
	void drawScene();

	// states
	void changeState(GameState *pState);
	void pushState(GameState *pState);
	void popState();
};

#endif