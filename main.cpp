#include <stdio.h>
#include "CSetAlgorithm.h"
#include "CSetCore.h"

int main(int argc, char *argv[])
{

	CSetCore* pSetCore;
	pSetCore = CSetCore::createSetGame(5);

	printf("-- Stats --\n");
	printf(" Number of Solutions : %d\n", pSetCore->vSolution.size());
	printf(" Solution Lists\n");
	for (int i = 0; i < (int)pSetCore->vSolution.size(); i++) {
		int idx = pSetCore->vSolution[i];
		printf("%d : (%d)\t %d,%d,%d\n", i + 1, idx, pSetCore->pCheckList[idx][0], pSetCore->pCheckList[idx][1], pSetCore->pCheckList[idx][2]);
	}
	printf("%s", pSetCore->vImageName[0].c_str());

	return 0;
}
