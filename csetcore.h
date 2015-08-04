#ifndef SETCORE_H
#define SETCORE_H

#include <time.h>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

#include "CRandom.h"

#define SEL_CARD	3

class CSetCore
{
public:
	CSetCore();
	CSetCore(int _numItems);
	CSetCore(int _numItems, int _nSolution);
	~CSetCore();

	static CSetCore* createSetGame(int _numItems);
	static CSetCore* createSetGame(int _numItems, int _nSolution);
	bool isSet(int* answer);
	bool isSetGame();
	void shuffleBoardGame();
	void shuffleBoardGame(int _nSolution);

protected:
	void designBoardGame();
	void rearrangeBoardGame();
	void setBits();
	int nCk(int _n, int _k);
	void initCharSet();
	void makeImageName();

private:
	void printArray();

public:
	int nComb;
	std::vector<int> vSolution;
	std::vector<std::string> vImageName;
	int** pCheckList;
	int* pBoardArray;
	int cSetTable[5][3];

protected:
	Random* pRandom;
	int numItems;
	int rowNum;
	int colNum;
	int numTiles;	
	int** pBitCards;	
};


#endif
