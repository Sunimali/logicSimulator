#include "graph.h"
/***************************************************************************************************************************
 Main Function(Serial and Parallel Fault Simulation)
****************************************************************************************************************************/
void main(int argc,char **argv)
{
FILE *fisc,*fvec,*ffau,*fres;             //file pointers used for .isc file, .vec file, .faults file and resultfile
int Max,Opt,Npi,Npo,Total,Tfs;              //maxnode id,option,tot no of PIs,tot no of Pos,Tot no of input patterns& faults in.vec in.faults

NODE graph[Mnod];                         //structure used to store the ckt information in .isc file 
PATTERN vector[Mpt];                      //structure used to store the input vectors information in .vec file 
FAULT stuck[Mft];                      //structure used to store the faults information in .faults file
int a;                             //random variables


//Read the .isc file and store the information in graph structure
fisc=fopen(argv[1],"r");                           //file pointer to open .isc file 
Max=0; 
Max=ReadIsc(fisc,graph);                 //read .isc file and return index of last node in graph formed
fclose(fisc);                            //close file pointer for .isc file
// PrintCircuit(graph,Max);                 //print all members of graph structure
CountPri(graph, Max, &Npi, &Npo); 

//Read the .vec file and store the information in  vector structure
fvec=fopen(argv[2],"r");                           //file pointer to open .vec file
Total=0; 
int patternList[Mpt][Mpi];
Total=ReadPattern(fvec,patternList, Npi);                   //read .vec file and store in vector structure and return Total number of patterns  
// printf("\nTotal No. of Pattern: %d",Total);             //print total number of patterns in .vec file
fclose(fvec);                                      //close file pointer for .vec file
 
// printf("\nIndex\tInputVector\n");
// for(a=0;a<Total;a++){  printf("%d\t%s",a,vector[a].piv); } //print all members of vector structure

// Opt=0; 
// Opt=atoi(argv[3]);                      //getting the option from terminal for xval

//Read the .faults file 
ffau=fopen(argv[3],"r");  
Tfs = ReadFaults(ffau,stuck);                 //read .faults file and store in stuck structure and return Total number of faults
// printf("\nTotal No. of Faults: %d",Tfs);             //print total number of faults in .faults file
fclose(ffau);				

fres=fopen(argv[4],"w");                           //file pointer to open .out file for printing results
//Perform Logic Simulation for each Input vector and print the Pos .val in output file 
runSimulator(graph, patternList, Total, stuck, Tfs, Max, Npi, Npo, fres);

fclose(fres); 					   //close file pointer for .out file
 
                                                 
ClearCircuit(graph,Mnod);                                      //clear memeory for all members of graph
for(a=0;a<Total;a++){ bzero(vector[a].piv,Mpi); }                //clear memeory for all members of vector
return;
}//end of main
/****************************************************************************************************************************/

