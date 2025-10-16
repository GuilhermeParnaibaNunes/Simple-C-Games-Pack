#include <stdio.h>
#include <stdlib.h>
#ifndef TTTTable_H_
#define TTTTable_H_

/*Cria nova mesa de jogo*/
    // - Campo X: descrição.

void PrintLineDiv(){
  printf("---+---+---\n");
}

void PrintGTable(char (*gTable)[3]){
  for(int i = 0; i < 3; i++){
    printf(" %c | %c | %c \n", gTable[i][0], gTable[i][1], gTable[i][2]);
    PrintLineDiv();
  }
}

void PrintEmptyGTable(){
  char emptyGTable[3][3];
  for(int i = 0; i < 3; i++){
    emptyGTable[i][0] = '1';
    emptyGTable[i][1] = '1';
    emptyGTable[i][2] = '1';
  }
  PrintGTable(emptyGTable);
}

void PrintNumberedGTable(){
  char numberedGTable[3][3];
  int numb = 1;
  for(int i = 0; i < 3; i++){
    numberedGTable[i][0] = numb;
    numberedGTable[i][1] = ++numb;
    numberedGTable[i][2] = ++numb;
  }
  PrintGTable(numberedGTable);

}

#endif // _TTTTable_H_
