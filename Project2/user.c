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
    int patternsCount = 0;

	while (fgets(line, sizeof(line), pattern) && lineNumber < Mpt) {
		int index = 0;
        patternsCount++;
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

    return patternsCount;

}

//end of ReadPattern 
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to simulate logic
***************************************************************************************************/
 void simulateLogic(NODE * Node, int patternIndex, int patternList[Mpt][Mpi], int fnode, int fault, int Max, int Npi,int Npo, FILE * fres){
	
		int itr ;
		int curInputIndex = 0;
        char foutputs[256] = "";
        char coutputs[256] = "";
          
		for(itr = 0; itr <= Max; itr++){ 
			if(Node[itr].Type != 0){  //not an invalid gate
                    switch(Node[itr].Type){
                        case INPT: // type input
                            Node[itr].Cval = patternList[patternIndex][curInputIndex];
                            
                            //printing patterns
                            if(patternList[patternIndex][curInputIndex] == 2){
                                fprintf(fres, "x");
                            }else{
                                fprintf(fres, "%d", patternList[patternIndex][curInputIndex]);
                            }
                            if(curInputIndex == Npi-1){
                            fprintf(fres, " ");
                            }
                            curInputIndex ++;
                            
                            itr==fnode ? (Node[itr].Fval = faultInjection(Node, itr, fault)) : (Node[itr].Fval = Node[itr].Cval);
                            
                            break;
                        case FROM: // type  from
                            Node[itr].Cval = Node[Node[itr].Fin->id].Cval; 
                            itr==fnode ? (Node[itr].Fval = faultInjection(Node, itr, fault)) : (Node[itr].Fval = Node[Node[itr].Fin->id].Fval);
                            
                            break;
                        case BUFF: // type  buff
                            Node[itr].Cval = Node[Node[itr].Fin->id].Cval;
                            itr==fnode ? (Node[itr].Fval = faultInjection(Node, itr, fault)) : (Node[itr].Fval = Node[Node[itr].Fin->id].Fval);
                            break;
                        case NOT: // type not
                            Node[itr].Cval = notGate[Node[Node[itr].Fin->id].Cval];
                            itr==fnode ? (Node[itr].Fval = faultInjection(Node, itr, fault)) : (Node[itr].Fval = notGate[Node[Node[itr].Fin->id].Fval]);
                            break;
                        case AND: //type and
                            Node[itr].Cval = andOperation(Node, (Node[itr].Fin),1);
                            itr==fnode ? (Node[itr].Fval = faultInjection(Node, itr, fault)) : (Node[itr].Fval = andOperation(Node, (Node[itr].Fin), 0));
                            break;
                        case NAND: //type Nand
                            Node[itr].Cval = notGate[andOperation(Node, (Node[itr].Fin),1)];
                            itr==fnode ? (Node[itr].Fval = faultInjection(Node, itr, fault)) : (Node[itr].Fval = notGate[andOperation(Node, (Node[itr].Fin), 0)]);
                            break;
                        case OR: // type or
                            Node[itr].Cval = orOperation(Node, (Node[itr].Fin),1);
                            itr==fnode ? (Node[itr].Fval = faultInjection(Node, itr, fault)) : (Node[itr].Fval = orOperation(Node, (Node[itr].Fin),0));
                            break;
                        case NOR: // type Nor
                            Node[itr].Cval = notGate[orOperation(Node, (Node[itr].Fin),1)];
                            itr==fnode ? (Node[itr].Fval = faultInjection(Node, itr, fault)) : (Node[itr].Fval = notGate[orOperation(Node, (Node[itr].Fin),0)]);
                            break;
                        case XOR: // type xor
                            Node[itr].Cval = xorOperation(Node, (Node[itr].Fin));
                            break;
                        case XNOR: // type xNor
                            Node[itr].Cval = notGate[xorOperation(Node, (Node[itr].Fin))];
                            break;
                    }
                    
                if(Node[itr].Nfo == 0){
                    char temp[50];
                    sprintf(temp, "%d", Node[itr].Cval);
                    strcat(coutputs, temp);
                    
                    char temp2[50];
                    sprintf(temp2, "%d", Node[itr].Fval);
                    strcat(foutputs, temp2);

                }

            }

		}
		printOutputs(foutputs, coutputs, fres, Npo);
	}
 

//end of simulateLogic
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to andOperation
***************************************************************************************************/
int andOperation(NODE * Node, LIST *Cur, int ff){
	LIST *tmp=Cur;
	int output=1;

	while(tmp!=NULL){ 
        if(ff){ 
		output = andGate[output][Node[tmp->id].Cval]; // check output by two by two
        }else{
            output = andGate[output][Node[tmp->id].Fval]; // check output by two by two
        }
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
int orOperation(NODE * Node, LIST *Cur, int ff){
	LIST *tmp=Cur;
	int output=0;


	while(tmp!=NULL){ 
        if(ff){  
		output = orGate[output][Node[tmp->id].Cval]; // check output by two by two
        }else{
            output = orGate[output][Node[tmp->id].Fval]; // check output by two by two
        }
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

void runSimulator(NODE * graph, int patternList [Mpt][Mpi], int totalPatterns, FAULT * faultList, int totalFaults, int Max, int Npi, int Npo, FILE * fres){
    
    int i,j;
    for(i = 0; i < totalFaults; i++) {
        fprintf(fres, "%d/%d\n", faultList[i].Snod, faultList[i].Sval);
        // Access fault list element as faultList[i]
        for(j = 0; j < totalPatterns; j++) {
            //run fault free and fault simulation
            fprintf(fres, "\t ");
            simulateLogic(graph, j, patternList, faultList[i].Snod, faultList[i].Sval, Max, Npi,Npo, fres);
            fprintf(fres, " ");            
           
        }

        fprintf(fres, "\n");
    }

}

int faultInjection(NODE* Node,int fnode, int fault){
    if(Node[fnode].Cval != fault){
        if(Node[fnode].Cval == 1){
            Node[fnode].Fval = 3; // d
        }else if(Node[fnode].Cval == 0){
            Node[fnode].Fval = 4; // dbar
        }else{
            Node[fnode].Fval = fault; // x
        }
    }else{
        Node[fnode].Fval = fault;
    }
    return Node[fnode].Fval;
}

void printOutputs(char* foutputs, char* coutputs, FILE* fres,int Npo){
    int i;
    int detected = 0;
    for(i = 0; i < Npo; i++){     
        fprintf(fres, "%c", coutputs[i]==2 ? 'x' : coutputs[i]);
    }
    fprintf(fres, " ");
    for(i = 0; i < Npo; i++){     
        fprintf(fres, "%c", foutputs[i]==2 ? 'x' : foutputs[i]);
        if(foutputs[i] == '3' || foutputs[i] == '4'){
            detected = 1;
        }
    }
    fprintf(fres, " ");

    if(strcmp(coutputs, foutputs) != 0){
        if(detected){ // check for d or dbar
            fprintf(fres, " DETECTED\n");
        } else {
            fprintf(fres, " UNDECTDED\n");
        }
    }else{
        fprintf(fres, " UNDECTDED\n");
    }
}

