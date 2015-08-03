#include "CSetCore.h"

CSetCore* CSetCore::createSetGame(int _numItems)
{
	CSetCore* p = new CSetCore(_numItems);

	return p;
}

CSetCore::CSetCore()
{

}

CSetCore::~CSetCore()
{
	delete[] pBoardArray;
	for (int i = 0; i < numTiles; i++) {
		delete[] pBitCards[i];
	}
	delete[] pBitCards;
}

CSetCore::CSetCore(int _numItems)
{
	unsigned long initCode[4] = { (unsigned)time(NULL), (unsigned)time(NULL), (unsigned)time(NULL), (unsigned)time(NULL) };
	pRandom = Random::createRandom(initCode, 4);

	numItems = _numItems;
	rowNum = numItems;
	colNum = numItems;
	numTiles = rowNum * colNum;

	designBoardGame();
	printArray();

}

void CSetCore::designBoardGame()
{
	pBoardArray = new int[numTiles];
	memset(pBoardArray, 0, sizeof(int)*numTiles);

	pBitCards = new int*[numTiles];
	for (int i = 0; i < numTiles; i++) {
		pBitCards[i] = new int[SEL_CARD];
		memset(pBitCards[i], 0, sizeof(int)*SEL_CARD);
	}	

	pBoardArray[0] = pRandom->getUniformRandomAB(0, numItems*numItems*numItems);
	int count = 1;
	while (count < rowNum*colNum) {
		int mFlag = 1;
		int number;
		number = pRandom->getUniformRandomAB(0, numItems*numItems*numItems - 1);
		for (int i = 0; i < count; i++)
			if (pBoardArray[i] == number)
				mFlag = 0;
		if (mFlag) {
			pBoardArray[count] = number;
			count++;
		}
	}
	getSetBits();
}

void CSetCore::getSetBits()
{
	int remainder, nextNum;
	for (int i = 0; i < numTiles; i++) {
		nextNum = pBoardArray[i] / numItems;
		remainder = pBoardArray[i] % numItems;
		pBitCards[i][0] = remainder;

		for (int j = 1; j < SEL_CARD; j++) {
			remainder = nextNum % numItems;
			nextNum = nextNum / numItems;
			pBitCards[i][j] = remainder;
		}
	}
}

void CSetCore::printArray()
{
	int idx;
	for (int i = 0; i < rowNum; i++) {
		for (int j = 0; j < colNum; j++) {
			idx = rowNum * i + j;
			printf("%d\t ", pBoardArray[idx]);
		}
		printf("\n");
	}
	printf("\n");

	for (int i = 0; i < rowNum; i++) {
		for (int j = 0; j < colNum; j++) {
			idx = rowNum * i + j;
			for (int k = 0; k < SEL_CARD; k++) {
				printf("%d", pBitCards[idx][k]);
			}
			printf("\t");
		}
		printf("\n");
	}
	printf("\n");
}
