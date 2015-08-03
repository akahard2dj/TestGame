#ifndef SETCORE_H
#define SETCORE_H

#include <time.h>
#include <memory>

#include "CRandom.h"

#define SEL_CARD	3

class CSetCore
{
public:
	CSetCore();
	CSetCore(int _numItems);
	~CSetCore();

	static CSetCore* createSetGame(int _numItems);

protected:
	void designBoardGame();
	void getSetBits();

private:
	void printArray();

public:
	Random* pRandom;

protected:
	int numItems;
	int rowNum;
	int colNum;
	int numTiles;
	int* pBoardArray;
	int** pBitCards;
};


#endif
