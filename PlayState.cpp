#include <d3dx9.h>
#include <d3d9.h>
#include "MainMenuState.h"
#include "DirectInput.h"
#include "PlayState.h"

const int NO_ONE = 0;
const int TIE = 3;

PlayState PlayState::m_PlayState;

void PlayState::clear()
{
	mTexture = NULL;
}

void PlayState::destroy(Connect4 *pGame)
{
	ReleaseCOM(mTexture);
	ReleaseCOM(mSprite);
	ReleaseCOM(mFont);
}

BOOL PlayState::create(Connect4 *pGame)
{
	HR(D3DXCreateTextureFromFile(gd3dDevice, "Background.tga", &mTexture));
	HR(D3DXCreateSprite(gd3dDevice, &mSprite));

	mKeyEscPressed = false;
	mKeyEnterPressed = false;
	mKey1Pressed = false;
	mKey2Pressed = false;
	mKey3Pressed = false;
	mKey4Pressed = false;
	mKey5Pressed = false;
	mKey6Pressed = false;
	mKey7Pressed = false;

	mBkgrdCntr = D3DXVECTOR3(353.0f, 303.0f, 1.0f);
	mBlackCntr = D3DXVECTOR3(50.0f, 50.0f, 0.0f);
	mRedCntr = D3DXVECTOR3(50.0, 50.0f, 0.0f);
	SetRect(&mBkgrdRect,   0,   0,  707, 606);
	SetRect(&mBlackRect, 924, 100, 1024, 199);
	SetRect(&mRedRect,   924,   0, 1024,  99);

	D3DXFONT_DESC fontDesc;
	fontDesc.Height          = 40;
    fontDesc.Width           = 30;
    fontDesc.Weight          = 0;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
    strcpy_s(fontDesc.FaceName, _T("Times New Roman"));
	
	HR(D3DXCreateFontIndirect(gd3dDevice, &fontDesc, &mFont));

	// player is always red and goes first
	mPlayer = 1;
	mComputer = 2;
	mTurn = mPlayer;
	mWinner = NO_ONE;

	// clear the board for play
	clearBoard();

	return TRUE;
}

void PlayState::checkInput(Connect4 *pGame)
{
	gDInput->poll();
	// see if the user is pressing the button
	if (gDInput->keyDown(DIK_ESCAPE) && !mKeyEscPressed)
	{
		mKeyEscPressed = true;
	}

	// now see if the release the button if so, then change the state
	if (!gDInput->keyDown(DIK_ESCAPE) && mKeyEscPressed)
	{
		mKeyEscPressed = false;
		pGame->changeState(MainMenuState::Instance());
	}

	// see if someone has one, if so then pause and wait to go back to main menu and lock out all other keys
	if (mWinner == 1 || mWinner == 2 || mWinner == 3)
	{
		mKey1Pressed = false;
		mKey2Pressed = false;
		mKey3Pressed = false;
		mKey4Pressed = false;
		mKey5Pressed = false;
		mKey6Pressed = false;
		mKey7Pressed = false;
		if (gDInput->keyDown(DIK_RETURN) && !mKeyEnterPressed)
		{
			mKeyEnterPressed = true;
		}

		if (!gDInput->keyDown(DIK_RETURN) && mKeyEnterPressed)
		{
			mKeyEnterPressed = false;
			pGame->changeState(MainMenuState::Instance());
		}
	}
}

void PlayState::update(Connect4 *pGame)
{
	mWinner = winner();

	// see if anybody won, if so got to announce winner
	if (mWinner != NO_ONE)
	{
		mTurn = mPlayer;
		announceWinner();
		
	}

	// see who's turn it is and get input if player turn

	if (mTurn == mPlayer)
	{
		humanMove();
	}
	else
	{
		computerMove();
	}

	mWinner = winner();

	// see if anybody won, if so got to announce winner
	if (mWinner != NO_ONE)
	{
		mTurn = mPlayer;
		announceWinner();
	}
}

void PlayState::display(Connect4 *pGame)
{
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(46,54,153), 1.0f, 0));  //white
	HR(gd3dDevice->BeginScene());
	HR(mSprite->Begin(D3DXSPRITE_OBJECTSPACE | D3DXSPRITE_DONOTMODIFY_RENDERSTATE));

	// pieces must be drawn first otherwise the board will look funny
	drawPieces();
	drawBackground();

	// display if somebody has won
	if (mWinner != 0)
	{
			RECT R = {0,50, 1024, 768};

		if (mWinner == mPlayer)
		{
			mFont->DrawText(0, _T("You have Beaten the computer!!!!!"),  -1, &R, DT_NOCLIP | DT_CENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)); 
		}
		if (mWinner == mComputer)
		{
			mFont->DrawText(0, _T("The computer has beaten you!!!"), -1, &R, DT_NOCLIP | DT_CENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}
		if (mWinner == 3)
		{
			mFont->DrawText(0, _T("You have tied the Computer"), -1, &R, DT_NOCLIP | DT_CENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}


	HR(mSprite->End());
	HR(gd3dDevice->EndScene());
	HR(gd3dDevice->Present(NULL, NULL, NULL, NULL));
}

void PlayState::drawPieces()
{
	// variable to keep track of the positon
	D3DXVECTOR3 temp;

	// loop and draw board
	for (int col = 0; col <= 6; ++col)
	{
		for (int row = 0; row <= 5; ++row)
		{
			temp = D3DXVECTOR3((float)((col*101)-302), (float)((row*101)-252), 0.0f);

			// check and see who is where, 1 is red, 2 is black, 0 is blank
			if (mBoard[row][col] == 1)
			{
				HR(mSprite->Draw(mTexture, &mRedRect, &mRedCntr, &temp, D3DCOLOR_XRGB(255,255,255)));
			}
			else if(mBoard[row][col] == 2)
			{
				HR(mSprite->Draw(mTexture, &mBlackRect, &mBlackCntr, &temp, D3DCOLOR_XRGB(255,255,255)));
			}
		}
	}
	HR(mSprite->Flush());
}

void PlayState::drawBackground()
{
	// Turn on the alpha state
	HR(gd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true));

	// draw the background sprite
	HR(mSprite->Draw(mTexture, &mBkgrdRect, &mBkgrdCntr, 0, D3DCOLOR_XRGB(255,255,255)));
	HR(mSprite->Flush());

	// Turn off the alpha state
	HR(gd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false));
}

void PlayState::clearBoard()
{
	// initiaze the board
	for(int i = 0; i < NUM_ROWS; i++)
	{
		for (int j = 0; j < NUM_COLS; j++)
		{
			mBoard[i][j] = 0;
		}
	}
}

void PlayState::humanMove()
{
	if (gDInput->keyDown(DIK_1) && !mKey1Pressed)
	{
		mKey1Pressed = true;
	}
	if (gDInput->keyDown(DIK_2) && !mKey2Pressed)
	{
		mKey2Pressed = true;
	}
	if (gDInput->keyDown(DIK_3) && !mKey3Pressed)
	{
		mKey3Pressed = true;
	}
	if (gDInput->keyDown(DIK_4) && !mKey4Pressed)
	{
		mKey4Pressed = true;
	}
	if (gDInput->keyDown(DIK_5) && !mKey5Pressed)
	{
		mKey5Pressed = true;
	}
	if (gDInput->keyDown(DIK_6) && !mKey6Pressed)
	{
		mKey6Pressed = true;
	}
	if (gDInput->keyDown(DIK_7) && !mKey7Pressed)
	{
		mKey7Pressed = true;
	}

	if (!gDInput->keyDown(DIK_1) && mKey1Pressed)
	{
		mKey1Pressed = false;

		// see if top spot empty
		if (mBoard[5][0] == 0)
		{
			for (int i = 0; i < NUM_ROWS; i++)
			{
				if (mBoard[i][0] == 0)
				{
					mBoard[i][0] = mPlayer;
					mTurn = opponent(mTurn);
					return;
				}
			}
		}
		else
		{
			// top spot wasn't empty, let the player move again
			mTurn = mPlayer;
		}
	}
	if (!gDInput->keyDown(DIK_2) && mKey2Pressed)
	{
		mKey2Pressed = false;

		// see if top spot empty
		if (mBoard[5][1] == 0)
		{
			for (int i = 0; i < NUM_ROWS; i++)
			{
				if (mBoard[i][1] == 0)
				{
					mBoard[i][1] = mPlayer;
					mTurn = opponent(mTurn);
					return;
				}
			}
		}
		else
		{
			// top spot wasn't empty, let the player move again
			mTurn = mPlayer;
		}
	}
	if (!gDInput->keyDown(DIK_3) && mKey3Pressed)
	{
		mKey3Pressed = false;

		// see if top spot empty
		if (mBoard[5][2] == 0)
		{
			for (int i = 0; i < NUM_ROWS; i++)
			{
				if (mBoard[i][2] == 0)
				{
					mBoard[i][2] = mPlayer;
					mTurn = opponent(mTurn);
					return;
				}
			}
		}
				else
		{
			// top spot wasn't empty, let the player move again
			mTurn = mPlayer;
		}
	}
	if (!gDInput->keyDown(DIK_4) && mKey4Pressed)
	{
		mKey4Pressed = false;

		// see if top spot empty
		if (mBoard[5][3] == 0)
		{
			for (int i = 0; i < NUM_ROWS; i++)
			{
				if (mBoard[i][3] == 0)
				{
					mBoard[i][3] = mPlayer;
					mTurn = opponent(mTurn);
					return;
				}
			}
		}
				else
		{
			// top spot wasn't empty, let the player move again
			mTurn = mPlayer;
		}
	}
	if (!gDInput->keyDown(DIK_5) && mKey5Pressed)
	{
		mKey5Pressed = false;

		// see if top spot empty
		if (mBoard[5][4] == 0)
		{
			for (int i = 0; i < NUM_ROWS; i++)
			{
				if (mBoard[i][4] == 0)
				{
					mBoard[i][4] = mPlayer;
					mTurn = opponent(mTurn);
					return;
				}
			}
		}
				else
		{
			// top spot wasn't empty, let the player move again
			mTurn = mPlayer;
		}
	}
	if (!gDInput->keyDown(DIK_6) && mKey6Pressed)
	{
		mKey6Pressed = false;

		// see if top spot empty
		if (mBoard[5][5] == 0)
		{
			for (int i = 0; i < NUM_ROWS; i++)
			{
				if (mBoard[i][5] == 0)
				{
					mBoard[i][5] = mPlayer;
					mTurn = opponent(mTurn);
					return;
				}
			}
		}
				else
		{
			// top spot wasn't empty, let the player move again
			mTurn = mPlayer;
		}
	}
	if (!gDInput->keyDown(DIK_7) && mKey7Pressed)
	{
		mKey7Pressed = false;

		// see if top spot empty
		if (mBoard[5][6] == 0)
		{
			for (int i = 0; i < NUM_ROWS; i++)
			{
				if (mBoard[i][6] == 0)
				{
					mBoard[i][6] = mPlayer;
					mTurn = opponent(mTurn);
					return;
				}
			}
		}
				else
		{
			// top spot wasn't empty, let the player move again
			mTurn = mPlayer;
		}
	}
}

void PlayState::computerMove()
{
	srand((unsigned)time(0));
	int temp;		// store who wins for computer move
	int tempSpot;	// store the current spot to safe proof the board

	// loop and see if the computer can win this turn
	for (int i = 0; i < NUM_ROWS; i++)
	{
		for (int j = 0; j < NUM_COLS; j++)
		{
			if (mBoard[i][j] == 0)
			{
				tempSpot = mBoard[i][j];

				// assign computer to location
				mBoard[i][j] = mComputer;
				temp = winner();
				if (temp == mComputer)
				{
					if (mBoard[i-1][j] != 0)
					{
						mBoard[i][j] = mComputer;
						mTurn = opponent(mTurn);
						return;
					}
				}
				mBoard[i][j] = tempSpot;
			}
		}
	}

		// loop and see if the player can win this turn
	for (int i = 0; i < NUM_ROWS; i++)
	{
		for (int j = 0; j < NUM_COLS; j++)
		{
			if (mBoard[i][j] == 0)
			{
				tempSpot = mBoard[i][j];

				// assign computer to location
				mBoard[i][j] = mPlayer;
				temp = winner();
				if (temp == mPlayer)
				{
					if (mBoard[i-1][j] != 0)
					{
						mBoard[i][j] = mComputer;
						mTurn = opponent(mTurn);
						return;
					}
				}
				mBoard[i][j] = tempSpot;
			}
		}
	}

	// neither can win, try and choose a good location
	srand((unsigned)time(0));

	int direction = rand() %12;

	for (int i = 0; i < NUM_ROWS; i++)
	{
		for (int j = 0; j < NUM_COLS; j++)
		{
			// see if the current space is occupied by the computer
			if (mBoard[i][j] == mComputer)
			{
				switch(direction)
				{
				case 1: // left of current space
					{
						// see if current spot in first column if so not doable
						if (j != 0)
						{
							// see if space to left is empty, if so take it
							if (mBoard[i][j-1] == 0)
							{
								if (mBoard[i-1][j-1] != 0)
								{
									mBoard[i][j-1] = mComputer;
									mTurn = opponent(mTurn);
									return;
								}
							}
						}
					}
				case 2: // right of space
					{
						// see if current spot is last columnn if so not doable
						if (j != 6)
						{
							// see if space to right is empty, if so take it
							if (mBoard[i][j+1] == 0)
							{
								if (mBoard[i-1][j+1] != 0)
								{
									mBoard[i][j+1] = mComputer;
									mTurn = opponent(mTurn);
									return;
								}
							}
						}
					}
				case 3:	// top of space
					{
						// see if current space is top of column, if so not doable
						if (i != 5)
						{
							// see if space on top is empty, if so take it
							if (mBoard[i+1][j] == 0)
							{
								if (mBoard[i-1][j] != 0)
								{
									mBoard[i+1][j] = mComputer;
									mTurn = opponent(mTurn);
									return;
								}
							}
						}
					}
				case 4:	// down and left of space
					{
						// see if current space is left and bottom of column
						if ((i != 0) && ( j!= 0))
						{
							// see if space left and down is empty, if so take it
							if (mBoard[i-1][j-1] == 0)
							{
								if (mBoard[i-2][j-1] != 0)
								{
									mBoard[i-1][j-1] = mComputer;
									mTurn = opponent(mTurn);
									return;
								}
							}
						}
					}
				case 5:	// up and left
					{
						// see if current space is left of column and top of rows
						if ((i != 5) && (j != 0))
						{
							// see if space left and up is empty, if so take it
							if (mBoard[i+1][j-1] == 0)
							{
								if (mBoard[i][j-1] != 0)
								{
									mBoard[i+1][j-1] = mComputer;
									mTurn = opponent(mTurn);
									return;
								}
							}
						}
					}
				case 6:	// up and right
					{
						// see if current space is not at right col and top row
						if ((i != 5) && (j != 6))
						{
							// see if space right and up is empty, if so take it
							if (mBoard[i+1][j+1] == 0)
							{
								if (mBoard[i][j+1] != 0)
								{
									mBoard[i+1][j+1] = mComputer;
									mTurn = opponent(mTurn);
									return;
								}
							}
						}
					}
				case 7:
					{
						// see if current space is not at right and bottom
						if ((i != 0) && (j != 6))
						{
							// see if space right and down is empty, if so take it
							if (mBoard[i-1][j+1] == 0)
							{
								if (mBoard[i-2][j+1] != 0)
								{
									mBoard[i-1][j+1] = mComputer;
									mTurn = opponent(mTurn);
									return;
								}
							}
						}
					}
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
					{
						randomPlace();
						return;
					}
				}
			}
		}
	}

	randomPlace();
	return;
}

void PlayState::randomPlace()
{
	// local variables
	int computerCol;		// column computer moves int
	bool inCol = false;		// whether pick was in a free column
	bool madeMove = false;	// whether computer made move, don't let exit till done
	vector<int> posColumns;	// possible locations to move

	// push all possible locations for a win
	for (int i = 0; i < NUM_COLS; i++)
	{
		if (mBoard[5][i] == 0)
		{
			// if there is a column with a free space, add to list of possible columns
			posColumns.push_back(i);
		}
	}

	do
	{
		// pick a random column
		computerCol = rand()%7;

		// see if the column is in the list, if not loop again
		for (unsigned int i = 0; i < posColumns.size(); i++)
		{
			if (posColumns[i] == computerCol)
			{
				inCol = true;
				break;
			}
		}
	} while (inCol == false);

	// make a move, column is know good, just find the spot
	do
	{
		for (int i = 0; i < NUM_ROWS; i++)
		{
			if (mBoard[i][computerCol] == 0)
			{
				mBoard[i][computerCol] = mComputer;
				mTurn = opponent(mTurn);
				madeMove = true;
				return;
			}
		}
	} while (madeMove == false);
}

int PlayState::opponent(int turn)
{
	if (turn == 1)
		return 2;
	else
		return 1;
}

void PlayState::announceWinner()
{
}

int PlayState::winner()
{
	const int winPos[69][4][2] = {	{ {5,0}, {5,1}, {5,2}, {5,3} }, // Top row
									{ {5,1}, {5,2}, {5,3}, {5,4} }, // Top row
									{ {5,2}, {5,3}, {5,4}, {5,5} }, // Top row
									{ {5,3}, {5,4}, {5,5}, {5,6} }, // Top row
									{ {4,0}, {4,1}, {4,2}, {4,3} }, // Second from top row
							  		{ {4,1}, {4,2}, {4,3}, {4,4} }, // Second from top row
									{ {4,2}, {4,3}, {4,4}, {4,5} }, // Second from top row
									{ {4,3}, {4,4}, {4,5}, {4,6} }, // Second from top row
									{ {3,0}, {3,1}, {3,2}, {3,3} }, // Third from top row
									{ {3,1}, {3,2}, {3,3}, {3,4} }, // Third from top row
									{ {3,2}, {3,3}, {3,4}, {3,5} }, // Third from top row
									{ {3,3}, {3,4}, {3,5}, {3,6} }, // Third from top row
							  		{ {2,0}, {2,1}, {2,2}, {2,3} }, // Fourth from top row
									{ {2,1}, {2,2}, {2,3}, {2,4} }, // Fourth from top row
									{ {2,2}, {2,3}, {2,4}, {2,5} }, // Fourth from top row
									{ {2,3}, {2,4}, {2,5}, {2,6} }, // Fourth from top row
									{ {1,0}, {1,1}, {1,2}, {1,3} }, // Fifth from top row
									{ {1,1}, {1,2}, {1,3}, {1,4} }, // Fifth from top row
									{ {1,2}, {1,3}, {1,4}, {1,5} }, // Fifth from top row
									{ {1,3}, {1,4}, {1,5}, {1,6} }, // Fifth from top row
									{ {0,0}, {0,1}, {0,2}, {0,3} }, // Bottom row
									{ {0,1}, {0,2}, {0,3}, {0,4} }, // Bottom row
									{ {0,2}, {0,3}, {0,4}, {0,5} }, // Bottom row
									{ {0,3}, {0,4}, {0,5}, {0,6} }, // Bottom row
									{ {0,0}, {1,0}, {2,0}, {3,0} }, // left Col
									{ {1,0}, {2,0}, {3,0}, {4,0} }, // Left Col
									{ {2,0}, {3,0}, {4,0}, {5,0} }, // Left Col
									{ {0,1}, {1,1}, {2,1}, {3,1} }, // second left col
									{ {1,1}, {2,1}, {3,1}, {4,1} }, // second left col
									{ {2,1}, {3,1}, {4,1}, {5,1} }, // second left col
									{ {0,2}, {1,2}, {2,2}, {3,2} }, // third left col
									{ {1,2}, {2,2}, {3,2}, {4,2} }, // third left col
									{ {2,2}, {3,2}, {4,2}, {5,2} }, // third left col
									{ {0,3}, {1,3}, {2,3}, {3,3} }, // fourth left col
									{ {1,3}, {2,3}, {3,3}, {4,3} }, // fourth left col
									{ {2,3}, {3,3}, {4,3}, {5,3} }, // fourth left col
									{ {0,4}, {1,4}, {2,4}, {3,4} }, // Fifth left col
									{ {1,4}, {2,4}, {3,4}, {4,4} }, // fifth left col
									{ {2,4}, {3,4}, {4,4}, {5,4} }, // fifth left col
									{ {0,5}, {1,5}, {2,5}, {3,5} }, // sixth left col
									{ {1,5}, {2,5}, {3,5}, {4,5} }, // sixth left col
									{ {2,5}, {3,5}, {4,5}, {5,5} }, // sixth left col
									{ {0,6}, {1,6}, {2,6}, {3,6} }, // right col
									{ {1,6}, {2,6}, {3,6}, {4,6} }, // right col
									{ {2,6}, {3,6}, {4,6}, {5,6} }, // right col
									{ {0,0}, {1,1}, {2,2}, {3,3} },
									{ {1,0}, {2,1}, {3,2}, {4,3} },
									{ {2,0}, {3,1}, {4,2}, {5,3} },
									{ {0,1}, {1,2}, {2,3}, {3,4} },
									{ {1,1}, {2,2}, {3,3}, {4,4} },
									{ {2,1}, {3,2}, {4,3}, {5,4} },
									{ {0,2}, {1,3}, {2,4}, {3,5} },
									{ {1,2}, {2,3}, {3,4}, {4,5} },
									{ {2,2}, {3,3}, {4,4}, {5,5} },
									{ {0,3}, {1,4}, {2,5}, {3,6} },
									{ {1,3}, {2,4}, {3,5}, {4,6} },
									{ {2,3}, {3,4}, {4,5}, {5,6} },
									{ {3,0}, {2,1}, {1,2}, {0,3} },
							  		{ {4,0}, {3,1}, {2,2}, {1,3} },
									{ {5,0}, {4,1}, {3,2}, {2,3} },
									{ {3,1}, {2,2}, {1,3}, {0,4} },
									{ {4,1}, {3,2}, {2,3}, {1,4} },
									{ {5,1}, {4,2}, {3,3}, {2,4} },
									{ {3,2}, {2,3}, {1,4}, {0,5} },
									{ {4,2}, {3,3}, {2,4}, {1,5} },
									{ {5,2}, {4,3}, {3,4}, {2,5} },
									{ {3,3}, {2,4}, {1,5}, {0,6} },
									{ {4,3}, {3,4}, {2,5}, {1,6} },
									{ {5,3}, {4,4}, {3,5}, {2,6} } };

	// check for a human win
	 for (int i = 0; i < 69; i++)
	 {
		 if (mBoard[ winPos[i][0][0] ][ winPos[i][0][1] ] == mPlayer && 
			 mBoard[ winPos[i][1][0] ][ winPos[i][1][1] ] == mPlayer &&
			 mBoard[ winPos[i][2][0] ][ winPos[i][2][1] ] == mPlayer &&
			 mBoard[ winPos[i][3][0] ][ winPos[i][3][1] ] == mPlayer)
		{
			// human won
			mWinner = mPlayer;
			return mPlayer;
		}

		 if (mBoard[ winPos[i][0][0] ][ winPos[i][0][1] ] == mComputer && 
			 mBoard[ winPos[i][1][0] ][ winPos[i][1][1] ] == mComputer &&
			 mBoard[ winPos[i][2][0] ][ winPos[i][2][1] ] == mComputer &&
			 mBoard[ winPos[i][3][0] ][ winPos[i][3][1] ] == mComputer)
		{
			// computer won
			mWinner = mComputer;
			return mComputer;
		}
	 }

	 // neither player won, check for a tie
	 if ((mBoard[5][0] != 0) && (mBoard[5][1] != 0) && (mBoard[5][2] != 0) && (mBoard[5][3] != 0) && (mBoard[5][4] != 0) && (mBoard[5][5] != 0) && (mBoard[5][6] != 0))
	 {
		 // all places are filled, tell tie
		 return mWinner = TIE;
	 }
	 else
	 {
		 // there was still a free space, game continues.
		 return NO_ONE;
	 }
}

void PlayState::onResetDevice(Connect4* pGame, HWND mhMainWnd)
{
	// Call the onResetDevice of other objects.
	mSprite->OnResetDevice();

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

void PlayState::onLostDevice(Connect4* pGame)
{
	mSprite->OnLostDevice();
}