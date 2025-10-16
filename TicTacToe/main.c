#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define EMPTY ' '
#define PLAYER_X 'X'
#define PLAYER_O 'O'

char * CreateSimulatedTable(char (*simulatedTable)[3], char (*gTable)[3]){
  for(int i = 0; i < 3; i++){
    simulatedTable[i][0] = gTable[i][0];
    simulatedTable[i][1] = gTable[i][1];
    simulatedTable[i][2] = gTable[i][2];
  }
  return simulatedTable;
}

void PrintGTable(char (*gTable)[3]){
  for(int i = 0; i < 3; i++){
    printf("\t %c | %c | %c \n", gTable[i][0], gTable[i][1], gTable[i][2]);
    if(i==2)
      break;
    printf("\t---+---+---\n");
  }
  printf("\n");
}

void EmptyTable(char (*gTable)[3]){
  for(int i = 0; i < 3; i++){
    gTable[i][0] = gTable[i][1] = gTable[i][2] = EMPTY;
  }
}

void PrintEmptyGTable(){
  char emptyGTable[3][3];
  EmptyTable(emptyGTable);
  PrintGTable(emptyGTable);
}

void PrintGTablePositions(){
  char numberedGTable[3][3];
  int numb = '1';
  for(int i = 0; i < 3; i++){
    numberedGTable[i][0] = numb;
    numberedGTable[i][1] = ++numb;
    numberedGTable[i][2] = ++numb;
    ++numb;
  }
  PrintGTable(numberedGTable);
}

int * FindXnY(int *XY, int pos){
  if(pos < 1 || pos > 9){
    printf("\n\t### POSICAO INVALIDA ###\n");
    XY[0] = -1;
    return XY;
  }
  int F = 0;
  while(F!=pos){
    F++;
    for(XY[1] = 0;XY[1] < 2; XY[1]++){
      if(F==pos)
        break;
      F++;
    }
    if(F==pos)
        break;
    XY[0]++;
  }
  return XY;
}

int IsPlayer(char player, char PoN){
    return PoN == player?1:0;
}

int IsEmpty(char EoN){
  return IsPlayer(EMPTY, EoN);
}

int * IsPlayerPos(char player, char (*gTable)[3]){
  int avaliabePos[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  for(int i = 0; i < 3; i++){
    if(IsPlayer(player, gTable[i][0]))
      avaliabePos[(i*3)] = (i*3)+1;
    if(IsPlayer(player, gTable[i][1]))
      avaliabePos[(i*3)+1] = (i*3)+2;
    if(IsPlayer(player, gTable[i][2]))
      avaliabePos[(i*3)+2] = (i*3)+3;
  }
//  for(int i = 0; i < 9; i++){
//    printf("\n\t### avaliablePos[%d] = %d ###", i, avaliabePos[i]);
//  }
  return avaliabePos;
}

int * IsAvailable(char (*gTable)[3]){
  return IsPlayerPos(EMPTY, gTable);
}

int MakeMove(int pos, char player, char (*gTable)[3]){
  int XY[2] = {0, 0};
  FindXnY(XY, pos);
  if(XY[0] == -1)
    return -1;
  if(!IsEmpty(gTable[XY[0]][XY[1]]))
    return 0; //pos ocuppied
  gTable[XY[0]][XY[1]] = player;
  return 1;
}

int RandMove(char player, char (*gTable)[3]){
  srand(time(NULL));
  int avPos[9], playPos = 0;
  avPos[9] = IsAvailable(gTable);
  while(!playPos){
    playPos = avPos[rand()%9];
  }
  return MakeMove(playPos, player, gTable);
}

int CPUlvIMove(char player, char (*gTable)[3]){
  return RandMove(player, gTable);
}

int VerifyWin(int pos,char currentPlayer,char(*gTable)[3]){
  int XY[2] = {0, 0};
  FindXnY(XY, pos);
  if(gTable[XY[0]][0] == gTable[XY[0]][1] && gTable[XY[0]][1] == gTable[XY[0]][2]) //Check pos row
    return 1;
  if(gTable[0][XY[1]] == gTable[1][XY[1]] && gTable[1][XY[1]] == gTable[2][XY[1]]) //Check pos column
    return 1;
  if(XY[0] == XY[1])
    if(gTable[0][0] == gTable[1][1] && gTable[1][1] == gTable[2][2]) //Check main diagonal
      return 1;
  if(XY[0]+XY[1] == 2)
    if(gTable[0][2] == gTable[1][1] && gTable[1][1] == gTable[2][0]) //Check alternative diagonal
      return 1;
  return 0;
}

int IsWinMove(char player, char (*gTable)[3]){
  int avPos[9], pos = 0;
  avPos[9] = IsAvailable(gTable);
  char sTable[3][3];
  CreateSimulatedTable(sTable, gTable); //Initialize a simulated table
  for(int i = 0; i < 9; i++){
    if(avPos[i]){ //If position avaliable
      MakeMove(i+1, player, sTable); //Make a test move
      if(VerifyWin(i+1, player, sTable)) //Verify if move wins game
        return i+1;
    }
  }
  return 0; //If none position wins game
}

int CPUlvIIMove(char player, char (*gTable)[3]){
  int winPosMove = IsWinMove(player, gTable);
  if(winPosMove) //Test for CPU WinGameMove
    return MakeMove(winPosMove, player, gTable);

  winPosMove = IsWinMove(player==PLAYER_X?PLAYER_O:PLAYER_X, gTable);
  if(winPosMove) //Test for Player WinGameMove
    return MakeMove(winPosMove, player, gTable);

  return RandMove(player, gTable);
}

int CPUlvIIIMove(char player, char (*gTable)[3], int round){
  srand(time(NULL));
  int corners[4] = {1, 3, 7, 9};
  if(!round) //If first round, put in random corner
    return MakeMove(corners[rand()%4], player, gTable);

  int winPosMove = IsWinMove(player, gTable);
  if(winPosMove) //Test for CPU WinGameMove
    return MakeMove(winPosMove, player, gTable);

  winPosMove = IsWinMove(player==PLAYER_X?PLAYER_O:PLAYER_X, gTable);
  if(winPosMove) //Test for Player WinGameMove
    return MakeMove(winPosMove, player, gTable);

  int avPos[9], pos = 0;
  avPos[9] = IsAvailable(gTable);
  if(avPos[4]) //Make a move in center, if possible
    return MakeMove(5, player, gTable);

  int CPUPos[9]; //Make a move in a
  CPUPos[9] = IsPlayerPos(player, gTable);
  if(CPUPos[0] && CPUPos[8])
    if(CPUPos[2])
      return MakeMove(3, player, gTable);
    else if(CPUPos[6])
      return MakeMove(7, player, gTable);
  else if(CPUPos[2] && CPUPos[6])
    if(CPUPos[0])
      return MakeMove(1, player, gTable);
    else if(CPUPos[8])
      return MakeMove(9, player, gTable);

  for(int i = 0; i < 4; i++){
    if(avPos[corners[i]-1]) //Make a move in a corner, if possible
      return MakeMove(corners[i], player, gTable);
  }
  return RandMove(player, gTable);
}

int main(){
  char gameTable[3][3], currentPlayer = PLAYER_X;
  int sel = 1, pos = 0, game = 1, changeTurn = 0, roundCount = 0;
  //game = 1 = still in game; changeTurn = 0 = still same player
  printf("\t******************************************\n"
         "\t### SEJA BEM-VINDO(A) AO JOGO DA VELHA ###\n"
         "\t******************************************\n");
  while(sel){
    EmptyTable(gameTable);
    game = 1;
    printf("\n\t************"
           "\n\t### MENU ###\n"
           "\t(1) - Iniciar novo jogo 1v1;\n"
           "\t(2) - Iniciar novo jogo vs CPU;\n"
           "\t(3) - Guia de jogo;\n"
           "\t(0) - Encerrar\n\n\t");
    scanf("%d", &sel);
    if(!sel)
      continue;
    switch(sel){
      case 1:
        printf("\t******************************\n"
               "\t### INICIANDO NOVO JOGO... ###\n\n");
        PrintGTable(gameTable);
        roundCount = 0;
        currentPlayer = PLAYER_X; //ADICIONAR A OPÇÃO DE ESCOLHER QUEM COMEÇA
        while(game){
          changeTurn = 0;
          while(!changeTurn){
            printf("\n\tJogador %c, escolha a posicao onde deseja jogar: ", currentPlayer);
            scanf("%d", &pos);
            printf("\n\t### %s ###\n\n"
            "", MakeMove(pos, currentPlayer, gameTable)==1?(changeTurn=1,"JOGADA RESGISTRADA"):"REFACA JOGADA");
          }
          roundCount++;
          PrintGTable(gameTable);
          if(roundCount>5){
            if(VerifyWin(pos, currentPlayer, gameTable)){
              printf("\t**************************\n"
                     "\t###   JOGO ENCERRADO   ###\n"
                     "\t### JOGADOR %c CAMPEAO! ###\n"
                     "\t**************************\n", currentPlayer);
              game = 0;
              break;
            }
            if(roundCount == 9){
              printf("\t**********************\n"
                     "\t### JOGO ENCERRADO ###\n"
                     "\t###     EMPATE     ###\n"
                     "\t**********************\n");
              game = 0;
            }
          }
          currentPlayer = (currentPlayer==PLAYER_X?PLAYER_O:PLAYER_X);
        }
        break;
      case 2:
        printf("\n\t************************"
           "\n\t### SELECIONE SEU OPONENTE ###\n"
           "\t(1) - CPU *;\n"
           "\t(2) - CPU **;\n"
           "\t(3) - CPU ***.\n\n\t");
        scanf("%d", &sel);
        if(sel < 1 || sel > 3)
          continue;

        printf("\t******************************\n"
               "\t### INICIANDO NOVO JOGO... ###\n\n");
        PrintGTable(gameTable);
        roundCount = 0;
        currentPlayer = PLAYER_X; //ADICIONAR A OPÇÃO DE ESCOLHER QUEM COMEÇA
        while(game){
          changeTurn = 0;
          while(!changeTurn){
            printf("\n\tJogador %c, escolha a posicao onde deseja jogar: ", currentPlayer);
            scanf("%d", &pos);
            printf("\n\t### %s ###\n\n"
            "", MakeMove(pos, currentPlayer, gameTable)==1?(changeTurn=1,"JOGADA RESGISTRADA"):"REFACA JOGADA");
          }
          roundCount++;
          PrintGTable(gameTable);
          if(roundCount>5){
            if(VerifyWin(pos, currentPlayer, gameTable)){
              printf("\t**************************\n"
                     "\t###   JOGO ENCERRADO   ###\n"
                     "\t### JOGADOR %c CAMPEAO! ###\n"
                     "\t**************************\n", currentPlayer);
              game = 0;
              break;
            }
            if(roundCount == 9){
              printf("\t**********************\n"
                     "\t### JOGO ENCERRADO ###\n"
                     "\t###     EMPATE     ###\n"
                     "\t**********************\n");
              game = 0;
            }
          }
          switch(sel){
            case 1:
              CPUlvIMove(PLAYER_O, gameTable);
              break;
            case 2:
              CPUlvIIMove(PLAYER_O, gameTable);
              break;
            case 3:
              CPUlvIIIMove(PLAYER_O, gameTable, roundCount);
              break;
          }
          roundCount++;
          PrintGTable(gameTable);
          if(roundCount>5){
            if(VerifyWin(pos, currentPlayer, gameTable)){
              printf("\t**************************\n"
                     "\t###   JOGO ENCERRADO   ###\n"
                     "\t### CPU (%c) CAMPEA! ###\n"
                     "\t**************************\n", currentPlayer);
              game = 0;
              break;
            }
            if(roundCount == 9){
              printf("\t**********************\n"
                     "\t### JOGO ENCERRADO ###\n"
                     "\t###     EMPATE     ###\n"
                     "\t**********************\n");
              game = 0;
            }
          }
        }
        break;
      case 3:
        printf("\n\t\t### To be continued... ###\n");
        break;
      default:
        printf("\n\t### SELECAO INVALIDA ###\n");
    }
  }
  return 0;
}
