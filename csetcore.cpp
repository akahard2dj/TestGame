#include "CSetCore.h"

CSetCore* CSetCore::createSetGame(int _numItems)
{
	CSetCore* p = new CSetCore(_numItems);

	return p;
}

CSetCore* CSetCore::createSetGame(int _numItems, int _nSolution)
{
	CSetCore* p = new CSetCore(_numItems, _nSolution);

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
	
	for (int i = 0; i < nComb; i++) {
		delete[] pCheckList[i];
	}
	delete[] pCheckList;
}

CSetCore::CSetCore(int _numItems)
{
	unsigned long initCode[4] = { (unsigned)time(NULL), (unsigned)time(NULL), (unsigned)time(NULL), (unsigned)time(NULL) };
	pRandom = Random::createRandom(initCode, 4);

	numItems = _numItems;
	rowNum = numItems;
	colNum = numItems;
	numTiles = rowNum * colNum;

	std::string bitmask(SEL_CARD, 1);
	bitmask.resize(numItems*numItems, 0);
	nComb = nCk(numItems*numItems, SEL_CARD);	
	
	pCheckList = new int*[nComb];
	for (int i = 0; i < nComb; i++) {
		pCheckList[i] = new int[SEL_CARD];
		memset(pCheckList[i], 0, sizeof(int)*SEL_CARD);
	}
	int idx = 0;

	do {
		int cnt = 0;
		for (int i = 0; i < numItems*numItems; ++i) {
			if (bitmask[i]) {
				//printf("%d ", i);
				pCheckList[idx][cnt] = i;
				cnt = cnt + 1;
			}
		}
		idx = idx + 1;
		//printf(" << %d\n",idx);
	} while (std::prev_permutation(bitmask.begin(), bitmask.end()));

	designBoardGame();
	while (!(vSolution.size())) {
		shuffleBoardGame();
	}
	initCharSet();
	printArray();

}

CSetCore::CSetCore(int _numItems, int _nSolution)
{	
	unsigned long initCode[4] = { (unsigned)time(NULL), (unsigned)time(NULL), (unsigned)time(NULL), (unsigned)time(NULL) };
	pRandom = Random::createRandom(initCode, 4);

	numItems = _numItems;
	rowNum = numItems;
	colNum = numItems;
	numTiles = rowNum * colNum;

	std::string bitmask(SEL_CARD, 1);
	bitmask.resize(numItems*numItems, 0);
	nComb = nCk(numItems*numItems, SEL_CARD);

	pCheckList = new int*[nComb];
	for (int i = 0; i < nComb; i++) {
		pCheckList[i] = new int[SEL_CARD];
		memset(pCheckList[i], 0, sizeof(int)*SEL_CARD);
	}
	int idx = 0;

	do {
		int cnt = 0;
		for (int i = 0; i < numItems*numItems; ++i) {
			if (bitmask[i]) {
				//printf("%d ", i);
				pCheckList[idx][cnt] = i;
				cnt = cnt + 1;
			}
		}
		idx = idx + 1;
		//printf(" << %d\n",idx);
	} while (std::prev_permutation(bitmask.begin(), bitmask.end()));

	designBoardGame();
	while ( ((int)vSolution.size()>_nSolution) || (int)vSolution.size() == 0) {
		rearrangeBoardGame();
	}
	initCharSet();
	printArray();

}

void CSetCore::initCharSet()
{
	cSetTable[0][0] = 83;	cSetTable[0][1] = 67;	cSetTable[0][2] = 84;
	cSetTable[1][0] = 82;	cSetTable[1][1] = 89;	cSetTable[1][2] = 66;
	cSetTable[2][0] = 70;	cSetTable[2][1] = 83;	cSetTable[2][2] = 84;
	cSetTable[3][0] = 66;	cSetTable[3][1] = 71;	cSetTable[3][2] = 87;
	cSetTable[4][0] = 67;	cSetTable[4][1] = 83;	cSetTable[4][2] = 79;

	makeImageName();
}

void CSetCore::makeImageName()
{
	char str[] = "CFS.png";
	std::string target = str;
	vImageName.push_back(target);
}

void CSetCore::designBoardGame()
{
	pBoardArray = new int[numTiles];
	memset(pBoardArray, 0, sizeof(int)*numTiles);

	pBitCards = new int*[numTiles];
	for (int i = 0; i < numTiles; i++) {
		pBitCards[i] = new int[numItems];
		memset(pBitCards[i], 0, sizeof(int)*numItems);
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
	setBits();
	isSetGame();
}

void CSetCore::rearrangeBoardGame()
{
	vSolution.clear();
	vImageName.clear();

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
	setBits();
	isSetGame();
}

void CSetCore::shuffleBoardGame()
{
	rearrangeBoardGame();

	while ( (int)vSolution.size() == 0) {
		rearrangeBoardGame();
	}
}

void CSetCore::shuffleBoardGame(int _nSolution)
{
	rearrangeBoardGame();

	while (((int)vSolution.size()>_nSolution) || (int)vSolution.size() == 0) {
		rearrangeBoardGame();
	}
}

void CSetCore::setBits()
{
	int remainder, nextNum;
	for (int i = 0; i < numTiles; i++) {
		nextNum = pBoardArray[i] / SEL_CARD;
		remainder = pBoardArray[i] % SEL_CARD;
		pBitCards[i][0] = remainder;

		for (int j = 1; j < numItems; j++) {
			remainder = nextNum % SEL_CARD;
			nextNum = nextNum / SEL_CARD;
			pBitCards[i][j] = remainder;
		}
	}
}

bool CSetCore::isSet(int* answer)
{
	//pBoardArray = new int[numTiles];

	int* sumBit;
	sumBit = new int[numItems];
	for (int i = 0; i < numItems; i++)
		sumBit[i] = pBitCards[answer[0]][i] + pBitCards[answer[1]][i] + pBitCards[answer[2]][i];

	//printf("isSet>> sumbit : %d%d%d\n", sumBit[0], sumBit[1], sumBit[2]);
	bool flag;
	switch (numItems)
	{
	case 3:
		//printf("isSet>> %d\n", numItems);
		flag = (sumBit[0] % 3 == 0) && (sumBit[1] % 3 == 0) && (sumBit[2] % 3 == 0);
		break;
	case 4:
		//printf("isSet>> %d\n", numItems);
		flag = (sumBit[0] % 3 == 0) && (sumBit[1] % 3 == 0) && (sumBit[2] % 3 == 0) && (sumBit[3] % 3 == 0);
		break;
	case 5:
		//printf("isSet>> %d\n", numItems);
		flag = (sumBit[0] % 3 == 0) && (sumBit[1] % 3 == 0) && (sumBit[2] % 3 == 0) && (sumBit[3] % 3 == 0) && (sumBit[4] % 3 == 0);
		break;
	default:
		flag = false;
		break;
	}
	delete[] sumBit;


	if (flag) {
		return true;
	}
	else {
		return false;
	}
}

bool CSetCore::isSetGame()
{	
	std::string bitmask(SEL_CARD, 1);
	bitmask.resize(numItems*numItems, 0);
	int nComb = nCk(numItems*numItems, SEL_CARD);
	//printf("isSetGame>> nComb = %d\n", nComb);
	int** checkList;
	checkList = new int*[nComb];
	for (int i = 0; i < nComb; i++) {
		checkList[i] = new int[SEL_CARD];
		memset(checkList[i], 0, sizeof(int)*SEL_CARD);
	}
	int idx = 0;

	do {
		int cnt = 0;
		for (int i = 0; i < numItems*numItems; ++i) {
			if (bitmask[i]) {
				//printf("%d ", i);
				checkList[idx][cnt] = i;
				cnt = cnt + 1;
			}
		}
		idx = idx + 1;
		//printf(" << %d\n",idx);
	} while (std::prev_permutation(bitmask.begin(), bitmask.end()));

	for (int i = 0; i < nComb; i++) {
		bool bCheck = isSet(pCheckList[i]);
		if (bCheck) {
			vSolution.push_back(i);
			//printf("Solution: %d %d %d\n", pCheckList[i][0], pCheckList[i][1], pCheckList[i][2]);
		}
	}
	//printf("## Total Solution : %d \n", vSolution.size());

	if (vSolution.size()) {
		return true;
	}
	else {
		return false;
	}
}

int CSetCore::nCk(int _n, int _k)
{
	int k = _k;
	int n = _n;
	
	if (k > n)
		return 0;
	int r = 1;
	for (int d = 1; d <= k; ++d ) {
		r *= n--;
		r /= d;
	}

	return r;
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
			for (int k = 0; k < numItems; k++) {
				printf("%d", pBitCards[idx][k]);
			}
			printf("\t");
		}
		printf("\n");
	}
	printf("\n");
}
