#include "graph.h"

/***************************************************************************************************************************
Lookup tables for gates
****************************************************************************************************************************/

int andGate[5][5] = {
//o   1 xv  d  db	
  {0, 0, 0, 0, 0},
  {0, 1, 2, 3, 4},
  {0, 2, 2, 2, 2},
  {0, 3, 2, 3, 0},
  {0, 4, 2, 0, 4}
};

int orGate[5][5] = {
//o   1 xv  d  db	
  {0, 1, 2, 3, 4},
  {1, 1, 1, 1, 1},
  {2, 1, 2, 2, 2},
  {3, 1, 2, 3, 1},
  {4, 1, 2, 1, 4}
};

int notGate[5] = {1, 0 , 2, 4, 3};
int lineNumber = 0;

/***************************************************************************************************
Count the Total Number of Primary inputs and outputs
***************************************************************************************************/
void CountPri(NODE *Node,int Max,int *Npi,int *Npo)
{
int i,j,k;

i=j=k=0;
for(i=1;i<=Max;i++){ 
  if(Node[i].Type!=0){
    if(Node[i].Nfi==0){      j++; }
    if(Node[i].Nfo==0){      k++; } }}
*Npi=j; *Npo=k;
return;
}//end of CountPri
/***************************************************************************************************
Convert (char *) type read to (int)     
***************************************************************************************************/


/***************************************************************************************************/
/***************************************************************************************************
 Function to read the test patterns(.pattern files)
***************************************************************************************************/
int ReadPattern(FILE * pattern, int patternList[Mpt][Mpi], int nPi){

	char line[Mlin];
	char input[5] = "";

	while (fgets(line, sizeof(line), pattern) && lineNumber < Mpt) {
		int index = 0;

		while(index != nPi){
			sprintf(input, "%.*s\n", 1 , &line[index]);
            if(input[0] == 'x'){
                patternList[lineNumber][index] = 2;
            }else{
			    patternList[lineNumber][index] = atoi(input);
            }
			index ++;
		}
		lineNumber++;
	}

}

//end of ReadPattern 
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to simulate logic
***************************************************************************************************/
 void simulateLogic(NODE * Node, int patternList[Mpt][Mpi], int Max, int Npi, FILE * fres){
	int testNum;

	//iterating through test cases
	for(testNum = 0; testNum < lineNumber; testNum++){
		int itr ;
		int curPatternIndex = 0;
        //printing patterns
        int inputNum;
        for(inputNum = 0; inputNum < Npi; inputNum++ ){
			if(patternList[testNum][inputNum] == 2){
				fprintf(fres, "x");
			}else{
			fprintf(fres, "%d", patternList[testNum][inputNum]);
			}
		}
        fprintf(fres, " ");

		for(itr = 0; itr <= Max; itr++){ 
			if(Node[itr].Type != 0){  //not an invalid gate
				switch(Node[itr].Type){
					case INPT: // type input
						Node[itr].Cval = patternList[testNum][curPatternIndex];
						curPatternIndex ++;
						break;
					case FROM: // type  from
						Node[itr].Cval = Node[Node[itr].Fin->id].Cval; 
						break;
					case BUFF: // type  buff
						Node[itr].Cval = Node[Node[itr].Fin->id].Cval;
						break;
					case NOT: // type not
						Node[itr].Cval = notGate[Node[Node[itr].Fin->id].Cval];
						break;
					case AND: //type and
						Node[itr].Cval = andOperation(Node, (Node[itr].Fin));
						break;
					case NAND: //type Nand
						Node[itr].Cval = notGate[andOperation(Node, (Node[itr].Fin))];
						break;
					case OR: // type or
						Node[itr].Cval = orOperation(Node, (Node[itr].Fin));
						break;
					case NOR: // type Nor
						Node[itr].Cval = notGate[orOperation(Node, (Node[itr].Fin))];
						break;
					case XOR: // type xor
						Node[itr].Cval = xorOperation(Node, (Node[itr].Fin));
						break;
					case XNOR: // type xNor
						Node[itr].Cval = notGate[xorOperation(Node, (Node[itr].Fin))];
						break;
				}
				if(Node[itr].Nfo == 0){
					if(Node[itr].Cval == 2){
						fprintf(fres, "x");
					}else{
					fprintf(fres, "%d" , Node[itr].Cval);
					}
				}
			}

		}
		fprintf(fres, "\n");
	}
 }

//end of simulateLogic
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to andOperation
***************************************************************************************************/
int andOperation(NODE * Node, LIST *Cur){
	LIST *tmp=Cur;
	int output=1;

	while(tmp!=NULL){  
		output = andGate[output][Node[tmp->id].Cval]; // check output by two by two
		if(output == 0){ // if one pair give out as 0 then stop
			break;
		}
		tmp = tmp->next; 
	} 
	return output;
}
//end of andOperation
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to orOperation
***************************************************************************************************/
int orOperation(NODE * Node, LIST *Cur){
	LIST *tmp=Cur;
	int output=0;


	while(tmp!=NULL){   
		output = orGate[output][Node[tmp->id].Cval]; // check output by two by two
		if(output == 1){ // if one pair give out as 1 then stop
			break;
		}
		tmp = tmp->next; 
	} 
	return output;
}
//end of orOperation
/****************************************************************************************************************************/


/***************************************************************************************************
 Function to xorOperation
***************************************************************************************************/
int xorOperation(NODE * Node, LIST *Cur){
	LIST *tmp=Cur;
	int output=1;
 
	if (Node[tmp->next->id].Cval == Node[tmp->id].Cval){ // check input value is equal or not
		output = 0;
	}
		
	return output;
}
//end of xorOperation
/****************************************************************************************************************************/

