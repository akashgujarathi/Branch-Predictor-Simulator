#define pragma once 
#include "main.h"

int main(int argc, char *argv[])
{
	start = clock();	
	int noOfPcBits;			//m

	// TODO make it in switch case
	if((argc == 4) && (!strcmp(argv[1],"bimodal")) )	//Checking the simulator type
	{		
			std::cout << "COMMAND " << std::endl;
			std::cout << argv[0] << " ";
			std::cout << argv[1] << " ";
			std::cout << argv[2] << " ";
			std::cout << argv[3] << std::endl;
			noOfPcBits = atoi(argv[2]);	// Taking number of PC bits
			simBimodal(noOfPcBits,argv[3],0,7);	// calling bimodal function with noOfPcBits & inputFilename		
	}
	else if((argc == 5) && (!strcmp(argv[1],"gshare")) )
	{
			std::cout << "COMMAND " << std::endl;
			std::cout << argv[0] << " ";
			std::cout << argv[1] << " ";
			std::cout << argv[2] << " ";
			std::cout << argv[3] << " ";
			std::cout << argv[4] << std::endl;

			noOfPcBits = atoi(argv[2]);	// Taking number of PC bits
			int	gbhr = atoi(argv[3]);		// Global branch history register

			simGshare(noOfPcBits,gbhr,argv[4],0,7);
	}
	else if((argc == 7) && (!strcmp(argv[1],"hybrid")))
	{
			std::cout << "COMMAND " << std::endl;
			std::cout << argv[0] << " ";
			std::cout << argv[1] << " ";
			std::cout << argv[2] << " ";
			std::cout << argv[3] << " ";
			std::cout << argv[4] << " ";
			std::cout << argv[5] << " ";
			std::cout << argv[6] << std::endl;

			int k = atoi(argv[2]);		// Taking number of PC bits
			int	M1 = atoi(argv[3]);		// Global branch history register
			int N = atoi(argv[4]);
			int M2 = atoi(argv[5]);

			simHybrid(k,M1,N,M2,argv[6],0,3);
	}
	else
	{
		std::cout << "Invalid Input..!!!"<<std::endl;
	}
	return 0;
}


void printTheOutput(int predicted,int misPredicted, const char * name)
{
	stop = clock();
	float ratio = ((float)misPredicted/(float)predicted)*100.00;
	std::cout << "OUTPUT\n";
	std::cout << "Number of Predictions: " << predicted<<std::endl;	
	std::cout << "Number of Mispredictions: " << misPredicted<<std::endl;
	std::cout << "Misprediction rate: " << std::setprecision(4)<< ratio <<"%"<<std::endl;
//	std::cout << "Program Execution Time:"<<(float)(stop - start)/CLOCKS_PER_SEC<<" sec."<<std::endl;

	std::cout << "FINAL " <<name<< " CONTENTS"<<std::endl;

	for(int i =0; i < pTable.size(); i++) { std::cout <<i <<"\t"<<pTable[i] <<std::endl; }
	
	return;
}


void simHybrid(int k, int M1, int N, int M2,char* inputFilename,int sStr, int sEnd)
{
	unsigned int address, noOfPredictions = 0, misPredicted = 0, branchNum = 0;
	char bStatus;
	std::vector<int> chooser;

	unsigned int noOfPredictions_1 = 0, branchNum_1 = 0;
	std::vector<int> pTable_1;

	unsigned int noOfPredictions_2 = 0, branchNum_2 = 0;
	int bRCnt_2 = 0;
	std::vector<int> pTable_2;

	// Initilize table for hybrid 
	for (int i = 0; i < pow(2,k); i++)
	{
		pTable.push_back(1);
	}
	pTable.resize(pow(2,k));

	// Initilize table for bimodal
	for (int i = 0; i < pow(2,M2); i++)
	{
		pTable_1.push_back(4);
	}
	pTable_1.resize(pow(2,M2));

	// Initilize table for gshare
	for (int i = 0; i < pow(2,M1); i++)
	{
		pTable_2.push_back(4);
	}
	pTable_2.resize(pow(2,M1)); 


	if(!freopen(inputFilename, "r", stdin)) { return; }

	while(scanf("%x %c", &address, &bStatus)!=EOF) 
	{
		bool flagBimodal = true;
		bool flagGshare = true;
		noOfPredictions ++;
		branchNum = 0, branchNum_1 = 0, branchNum_2 = 0;
		address >>= 2;
		
		// Step 1. 
		for(int i = 0; i < M2; i++)
		{
			branchNum_1 += (address&(1 << i));	// Using bits m+1 through 2 of the Program Counter BIMODAL
		}
		if( ( pTable_1[branchNum_1] > 3 && bStatus != 't' ) || ( pTable_1[branchNum_1] < 4 && bStatus == 't' ) ) 
		{ 
				flagBimodal = false; 
		}
		// Step 1.
		for(int i = 0; i < M1; i++)
		{
			branchNum_2 += (address&(1 << i));	// Using bits m+1 through 2 of the Program Counter GSHARE
		}
			branchNum_2 ^= (bRCnt_2);

		if( ( pTable_2[branchNum_2] > 3 && bStatus != 't' ) || ( pTable_2[branchNum_2] < 4 && bStatus == 't' ) ) 
		{ 
				flagGshare = false;
		}	

		// Step 2.
		for(int i = 0; i < k; i++)
		{
			branchNum += (address&(1 << i));	// Using bits m+1 through 2 of the Program Counter HYBRID
		}	
		if(pTable[branchNum] >= 2)
		{
			// Use prediction obtained by gshare
			if(flagGshare == false) { misPredicted++; }

			if(bStatus == 't')
			{
				if(pTable_2[branchNum_2] != 7) { pTable_2[branchNum_2]++; } 
		 			bRCnt_2 = (bRCnt_2 >> 1) | (1 << (N-1));
			}
			else
			{
				if(pTable_2[branchNum_2] != 0) { pTable_2[branchNum_2]--; }		
				bRCnt_2 >>= 1; 
			} 
		}
		else
		{
			// Use prediction obtained by bimodal
			if(flagBimodal == false) { misPredicted++; }

			if(bStatus == 't')
			{
				if(pTable_1[branchNum_1] != 7) { pTable_1[branchNum_1]++; }
				bRCnt_2 = (bRCnt_2 >> 1) | (1 << (N-1));
			}
			else
			{
				if(pTable_1[branchNum_1] != 0) { pTable_1[branchNum_1]--; }
				bRCnt_2 >>= 1;
			}
		}

		if( (flagBimodal == true && flagGshare == true) || (flagBimodal == false && flagGshare == false)  ){}
		else if( flagGshare == true && flagBimodal == false) 
		{
			if(pTable[branchNum] != sEnd) { pTable[branchNum]++; }
		}
		else if( flagBimodal == true && flagGshare == false) 
		{
			if(pTable[branchNum] != sStr) { pTable[branchNum]--; }
		}	
	}
	printTheOutput(noOfPredictions,misPredicted,"CHOOSER");
	std::cout << "FINAL GSHARE CONTENTS"<<std::endl;
	for(int i =0; i < pTable_2.size(); i++) { std::cout <<i <<"\t"<<pTable_2[i] <<std::endl; }
	std::cout << "FINAL BIMODAL CONTENTS"<<std::endl;
	for(int i =0; i < pTable_1.size(); i++) { std::cout <<i <<"\t"<<pTable_1[i] <<std::endl; }	
}

void simBimodal(int m, char* inputFilename,int sStr,int sEnd)
{

	unsigned int address, noOfPredictions = 0, misPredicted = 0, branchNum = 0;
	char bStatus;
	//1. intialize the predition table to 4
	
	for (int i = 0; i < pow(2,m); i++)
	{
		pTable.push_back(4);
	}
	pTable.resize(pow(2,m));

	//2. Read the file 

	if(!freopen(inputFilename, "r", stdin)) { return; }

	while(scanf("%x %c", &address, &bStatus)!=EOF) 
	{
		noOfPredictions ++;
		branchNum = 0;
		address >>= 2;	// Discarding the last two bits

		for(int i = 0; i < m; i++)
		{
			branchNum += (address&(1 << i));	// Using bits m+1 through 2 of the Program Counter
		}
		/*
			1. If the address is above 4 and the branch is not taken that is a misPrediction
			2. If the address is below 4 and the branch is taken that is a misPrediction
		*/

		if( ( pTable[branchNum] > 3 && bStatus != 't' ) || ( pTable[branchNum] < 4 && bStatus == 't' ) ) 
			{ 
				misPredicted++; 
			}
		
		/*
			7 & 0 are the saturation points:
			1.	If branch is 'taken' and branchCounter is less than 7 then counter is inc.
			2.  If the branch is 'not taken' and branchCounter is greater than 0 then counter is dec.
		*/	
		
		if(bStatus == 't')
		{
			if(pTable[branchNum] != sEnd) { pTable[branchNum]++; }
		}
		else
		{
			if(pTable[branchNum] != sStr) { pTable[branchNum]--; }
		}	
	}
		printTheOutput(noOfPredictions,misPredicted,"bimodal");
	return ;
}

void simGshare(int m, int n, char* inputFilename,int sStr, int sEnd)
{
	unsigned int address, noOfPredictions = 0, misPredicted = 0, branchNum = 0;
	char bStatus;
	int bRCnt = 0;

	//1. intialize the predition table to 4
	
	for (int i = 0; i < pow(2,m); i++)
	{
		pTable.push_back(4);
	}
	pTable.resize(pow(2,m)); 

	//2. Read the file 

	if(!freopen(inputFilename, "r", stdin)) { return; }
	//int q = 0;
	while(scanf("%x %c", &address, &bStatus)!=EOF) 
	{
		noOfPredictions ++;
		branchNum = 0;
		address >>= 2;	// Discarding the last two bits

		for(int i = 0; i < m; i++)
		{
			branchNum += (address&(1 << i));	// Using bits m+1 through 2 of the Program Counter
		}
			branchNum ^= (bRCnt);	// 1. branch history register is XORed with the lowermst bit of the index PC bits
	
		/*
			1. If the address is above 4 and the branch is not taken that is a misPrediction
			2. If the address is below 4 and the branch is taken that is a misPrediction
		*/
		if( ( pTable[branchNum] > 3 && bStatus != 't' ) || ( pTable[branchNum] < 4 && bStatus == 't' ) ) 
		{ 
			misPredicted++; 
		}
		
		//	4. Update the Global branch history register, shift the register right by 1 position.
		
		if(bStatus == 't')
		{
			if(pTable[branchNum] != sEnd) { pTable[branchNum]++; } 
		 		bRCnt = (bRCnt >> 1) | (1 << (n-1));
			}
		else
		{
			if(pTable[branchNum] != sStr) { pTable[branchNum]--; }		
				bRCnt >>= 1; 
		}
	}

	printTheOutput(noOfPredictions,misPredicted,"gshare");
	return;
}