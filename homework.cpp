#include <bits/stdc++.h>
#define ll long long
using namespace std;

ll GAME_OVER = 1e10;
int maxDepth;
int maxBranchFactor;

struct penteData{
    char playerColor;
    char opponentColor;
    double playTime;
    ll playerCaptures; // captured by player
    ll opponentCaptures; // captured by opponent
    vector< vector<char> > board;
    vector < pair<int,int> > playerCoins;
    vector< pair<int,int> > oppCoins;
    int playerPtr, oppPtr;
    ll movesPlayed;
};

bool isValidCoord(int i, int j){
     return i>=0 && j>= 0 && i<19 && j<19;   
}

struct boardScore{
     
     ll halfOpenOne, openOne; // 1s 
     ll halfOpenTwo, openTwo, stretchTwo, stretch2CellTwo; // 2s
     ll halfOpenThree, stretchThree, openThree; // 3s
     ll halfOpenFour, splitFour, stretchFour, openFour ; // 4s
     ll fiver; // 5s
     ll captures; // default weight of openTwo;
     ll fukumi;
     ll pawnbroker;
     // TODO Shapes;
    
     boardScore(){
           halfOpenOne = openOne = 0; // 1s 
           halfOpenTwo = openTwo = stretchTwo = stretch2CellTwo = 0; // 2s
           halfOpenThree = stretchThree = openThree = 0; // 3s
           halfOpenFour = splitFour =  stretchFour = openFour = 0; // 4s
           fiver = 0; // 5s
           captures = 0;
           pawnbroker = 0;
           fukumi = 0;
     }
     void updateScoreBasedOnK(int k, bool openPossible){
        switch(k){
                  case 1: halfOpenOne++; if(openPossible){openOne++;} break;
                  case 2: halfOpenTwo++; if(openPossible){openTwo++;} break;
                  case 3: halfOpenThree++; if(openPossible){openThree++;} break;
                  case 4: halfOpenFour++; if(openPossible){openFour++;} break;
                  case 5: fiver++; break;
                }

     }
     // O(30)
     void updateScoreForRow(penteData &st, int i, int j, char currColor){
          // Row Calculation
          int row = 0;
          bool halfOpenPossible = false, halfOpen = false, openPossible = false;
          if(isValidCoord(i,j-1) && st.board[i][j-1] == '.'){
              halfOpenPossible = true;
              halfOpen = true;
          }
          int k;
          for(k=0;k<=5 && isValidCoord(i,j+k);k++){
            if(st.board[i][j+k] == currColor){row++;}
            else if(st.board[i][j+k] == '.') {
                openPossible = halfOpenPossible;
                halfOpenPossible = true;
                break;
            }
            else {
              openPossible = false;
              break;
            }
          }
          if(halfOpenPossible || k >= 5){
              updateScoreBasedOnK(k,openPossible);
          }
          // pawnbroker 
          if(isValidCoord(i-2,j+1) && st.board[i-1][j+1] == currColor  && isValidCoord(i,j+2) && st.board[i][j+2] == currColor
                && st.board[i][j+1] == '.' && st.board[i-1][j+1] == '.'){
                  pawnbroker++;
          }
          // stretch2CellTwo
          if(isValidCoord(i,j+3) && st.board[i][j+3] == currColor 
               && st.board[i][j+1] == '.' && st.board[i][j+2] == '.'){
              if(halfOpen || (isValidCoord(i,j+4) && st.board[i][j+4] == '.')){
                stretch2CellTwo++;
              }
          }
          // stretchTwo
          if(isValidCoord(i,j+2) && st.board[i][j+2] == currColor && st.board[i][j+1] == '.'){
              if(halfOpen || (isValidCoord(i,j+3) && st.board[i][j+3] == '.')){
                stretchTwo++;
              }
          }
          // stretchThree && splitFour
          if(isValidCoord(i,j+3) && st.board[i][j+3] == currColor 
                && st.board[i][j+1] == currColor
                && st.board[i][j+2] == '.'){
              if(halfOpen || (isValidCoord(i,j+4) && st.board[i][j+4] == '.')){
                stretchThree++;
              }
              if(isValidCoord(i,j+4) && st.board[i][j+4] == currColor){
                  if(halfOpen || (isValidCoord(i,j+5) && st.board[i][j+5] == '.') ){
                    splitFour++;
                  }
              }
          }
          // stretchFour
          if(k == 3 && isValidCoord(i, j+4) && st.board[i][j+4] == currColor
              && st.board[i][j+3] == '.' ){
              if(halfOpen || (isValidCoord(i,j+5) && st.board[i][j+5] == '.')){
                  stretchFour++;
              }
            }
     }
     

     void updateScoreForCol(penteData &st, int i, int j, char currColor){

          // Column Calculation
          int col = 0;
          bool halfOpenPossible = false , halfOpen = false, openPossible = false;
          if(isValidCoord(i-1,j) && st.board[i-1][j] == '.'){
              halfOpenPossible = true;
              halfOpen =true;
          }
          int k;
          for(k=0;k<=5 && isValidCoord(i+k,j);k++){
            if(st.board[i+k][j] == currColor){col++;}
            else if(st.board[i+k][j] == '.') {
                openPossible = halfOpenPossible;
                halfOpenPossible = true;
                break;
            }
            else {
              openPossible = false;
              break;
            }
          }
          if(halfOpenPossible || k >= 5){
              updateScoreBasedOnK(k,openPossible);
          }
          // pawnbroker 
          if(isValidCoord(i+1,j+2) && st.board[i+1][j+2] == currColor  && isValidCoord(i+2,j) && st.board[i+2][j] == currColor
                && st.board[i+1][j] == '.' && st.board[i+1][j+1] == '.'){
                  pawnbroker++;
          }
          // stretch2CellTwo
          if(isValidCoord(i+3,j) && st.board[i+3][j] == currColor 
                 && st.board[i+1][j] == '.' && st.board[i+2][j] == '.'){
              if(halfOpen || (isValidCoord(i+4,j) && st.board[i+4][j] == '.')){
                stretch2CellTwo++;
              }
          }
          // stretchTwo
          if(isValidCoord(i+2,j) && st.board[i+2][j] == currColor && st.board[i+1][j] == '.'){
              if(halfOpen || (isValidCoord(i+3,j) && st.board[i+3][j] == '.')){
                stretchTwo++;
              }
          }
          // stretchThree && splitFour
          if(isValidCoord(i+3,j) && st.board[i+1][j] == currColor 
                && st.board[i+3][j] == currColor
                && st.board[i+2][j] == '.'){
              if(halfOpen || (isValidCoord(i+4,j) && st.board[i+4][j] == '.')){
                stretchThree++;
              }
              if(isValidCoord(i+4,j) && st.board[i+4][j] == currColor){
                  if(halfOpen || (isValidCoord(i+5,j) && st.board[i+5][j] == '.') ){
                    splitFour++;
                  }
              }
          }
          // stretchFour
          if(k == 3 && isValidCoord(i+4, j) && st.board[i+3][j] == '.' 
               && st.board[i+4][j] == currColor ){
              if(halfOpen || (isValidCoord(i+5,j) && st.board[i+5][j] == '.')){
                  stretchFour++;
              }
          }
     }

     void updateScoreForTLBR(penteData &st, int i, int j, char currColor){
          // TLBR Calculation
          int tlbr = 0;
          bool halfOpenPossible = false, halfOpen = false, openPossible = false;
          if(isValidCoord(i-1,j-1) && st.board[i-1][j-1] == '.'){
              halfOpenPossible = true;
              halfOpen = true;
          }
          int k;
          for(k=0;k<=5 && isValidCoord(i+k,j+k);k++){
            if(st.board[i+k][j+k] == currColor){tlbr++;}
            else if(st.board[i+k][j+k] == '.') {
                openPossible = halfOpenPossible;
                halfOpenPossible = true;
                break;
            }
            else {
              openPossible = false;
              break;
            }
          }
          if(halfOpenPossible || k >= 5){
              updateScoreBasedOnK(k,openPossible);
          }
          // pawnbroker 
          if(isValidCoord(i+2,j+2) && st.board[i+2][j+2] == currColor  
                && st.board[i+1][j+1] == '.'){
                  if(isValidCoord(i,j+2) && st.board[i][j+2] == currColor){
                    pawnbroker++;
                  }
                  if(isValidCoord(i+2, j) && st.board[i+2][j] == currColor){
                    pawnbroker++;
                  }
          }
          // stretch2CellTwo
          if(isValidCoord(i+3,j+3) && st.board[i+3][j+3] == currColor 
                && st.board[i+1][j+1] == '.' && st.board[i+2][j+2] == '.'){
              if(halfOpen || (isValidCoord(i+4,j+4) && st.board[i+4][j+4] == '.')){
                stretch2CellTwo++;
              }
          }

          // stretchTwo
          if(isValidCoord(i+2,j+2) && st.board[i+2][j+2] == currColor && st.board[i+1][j+1] == '.'){
              if(halfOpen || (isValidCoord(i+3,j+3) && st.board[i+3][j+3] == '.')){
                stretchTwo++;
              }
          }
          // stretchThree && splitFour
          if(isValidCoord(i+3,j+3) && st.board[i+1][j+1] == currColor 
                && st.board[i+3][j+3] == currColor
                && st.board[i+2][j+2] == '.'){
              if(halfOpen || (isValidCoord(i+4,j+4) && st.board[i+4][j+4] == '.')){
                stretchThree++;
              }
              if(isValidCoord(i+4,j+4) && st.board[i+4][j+4] == currColor){
                  if(halfOpen || (isValidCoord(i+5,j+5) && st.board[i+5][j+5] == '.') ){
                    splitFour++;
                  }
              }
          }
          // stretchFour
          if(k == 3 && isValidCoord(i+4, j+4) && st.board[i+3][j+3] == '.' 
              && st.board[i+4][j+4] == currColor ){
              if(halfOpen || (isValidCoord(i+5,j+5) && st.board[i+5][j+5] == '.')){
                  stretchFour++;
              }
          }

     }

     void updateScoreForBLTR(penteData &st, int i, int j, char currColor){
        // BLTR Calculation
          int bltr = 0;
          bool halfOpenPossible = false, halfOpen = false, openPossible = false;
          if(isValidCoord(i-1,j+1) && st.board[i-1][j+1] == '.'){
              halfOpenPossible = true;
              halfOpen = true;
          }
          int k;
          for(k=0;k<=5 && isValidCoord(i+k,j-k);k++){
            if(st.board[i+k][j-k] == currColor){bltr++;}
            else if(st.board[i+k][j-k] == '.') {
                openPossible = halfOpenPossible;
                halfOpenPossible = true;
                break;
            }
            else {
              openPossible = false;
              break;
            }
          }
          if(halfOpenPossible || k >= 5){
              updateScoreBasedOnK(k,openPossible);
          }
          // pawnbroker 
          if(isValidCoord(i+2,j-2) && st.board[i+2][j-2] == currColor && st.board[i+1][j-1] == '.'){
                  if(isValidCoord(i+2,j) && st.board[i+2][j] == currColor){
                    pawnbroker++;
                  }
                  if(isValidCoord(i,j-2) && st.board[i][j-2] == currColor){
                    pawnbroker++;
                  }
          }

          // stretch2CellTwo
          if(isValidCoord(i+3,j-3) && st.board[i+3][j-3] == currColor 
                 && st.board[i+1][j-1] == '.' && st.board[i+2][j-2] == '.'){
              if(halfOpen || (isValidCoord(i+4,j-4) && st.board[i+4][j-4] == '.')){
                stretch2CellTwo++;
              }
          }

          // stretchTwo
          if(isValidCoord(i+2,j-2) && st.board[i+2][j-2] == currColor && st.board[i+1][j-1] == '.'){
              if(halfOpen || (isValidCoord(i+3,j-3) && st.board[i+3][j-3] == '.')){
                stretchTwo++;
              }
          }
          // stretchThree && splitFour
          if(isValidCoord(i+3,j-3) && st.board[i+1][j-1] == currColor 
                && st.board[i+3][j-3] == currColor
                && st.board[i+2][j-2] == '.'){
              if(halfOpen || (isValidCoord(i+4,j-4) && st.board[i+4][j-4] == '.')){
                stretchThree++;
              }
              if(isValidCoord(i+4,j-4) && st.board[i+4][j-4] == currColor){
                  if(halfOpen || (isValidCoord(i+5,j-5) && st.board[i+5][j-5] == '.') ){
                    splitFour++;
                  }
              }
          }
          // stretchFour
          if(k == 3 && isValidCoord(i+4, j-4) && st.board[i+3][j-3] == '.' 
              && st.board[i+4][j-4] == currColor ){
              if(halfOpen || (isValidCoord(i+5,j-5) && st.board[i+5][j-5] == '.')){
                  stretchFour++;
              }
          }
     }

     void updateFukumi(penteData &st, int i, int j, char currColor){
         if(isValidCoord(i, j+2) && st.board[i][j+2] == '.' && st.board[i][j+1] == '.'){
            if(isValidCoord(i,j-2) && st.board[i][j-2] == currColor && st.board[i][j-1] == currColor){
                if(isValidCoord(i-3,j-1) && st.board[i-3][j-1] == currColor && st.board[i-2][j] == currColor && st.board[i-1][j+1] == '.'){
                  fukumi++;
                }
                if(isValidCoord(i-2,j-1) && st.board[i-2][j-1] == currColor && st.board[i-1][j] == currColor){
                  fukumi++;
                }
                if(isValidCoord(i+3,j-1) && st.board[i+3][j-1] == currColor && st.board[i+2][j] == currColor && st.board[i+1][j+1] == '.'){
                  fukumi++;
                }
                if(isValidCoord(i+2,j-1) && st.board[i+2][j-1] == currColor && st.board[i+1][j] == currColor){
                  fukumi++;
                }
            }
         }
         if(isValidCoord(i, j-2) && st.board[i][j-2] == '.' && st.board[i][j-1] == '.'){
            if(isValidCoord(i,j+2) && st.board[i][j+2] == currColor && st.board[i][j+1] == currColor){
                if(isValidCoord(i-3,j+1) && st.board[i-3][j+1] == currColor && st.board[i-2][j] == currColor && st.board[i-1][j-1] == '.'){
                  fukumi++;
                }
                if(isValidCoord(i-2,j+1) && st.board[i-2][j+1] == currColor && st.board[i-1][j] == currColor){
                  fukumi++;
                }
                if(isValidCoord(i+3,j+1) && st.board[i+3][j+1] == currColor && st.board[i+2][j] == currColor && st.board[i+1][j-1] == '.'){
                  fukumi++;
                }
                if(isValidCoord(i+2,j+1) && st.board[i+2][j+1] == currColor && st.board[i+1][j] == currColor){
                  fukumi++;
                }

            }
         }

         if(isValidCoord(i-2, j) && st.board[i-2][j] == '.' && st.board[i-1][j] == '.'){
            if(isValidCoord(i+2,j) && st.board[i+2][j] == currColor && st.board[i+1][j] == currColor){
                if(isValidCoord(i+1,j-3) && st.board[i+1][j-3] == currColor && st.board[i][j-2] == currColor && st.board[i-1][j-1] == '.'){
                  fukumi++;
                }
                if(isValidCoord(i+1,j-2) && st.board[i+1][j-2] == currColor && st.board[i][j-1] == currColor){
                  fukumi++;
                }
                if(isValidCoord(i+1,j+3) && st.board[i+1][j+3] == currColor && st.board[i][j+2] == currColor && st.board[i-1][j+1] == '.'){
                  fukumi++;
                }
                if(isValidCoord(i+1,j+2) && st.board[i+1][j+2] == currColor && st.board[i][j+1] == currColor){
                  fukumi++;
                }

            }
         }

         if(isValidCoord(i+2, j) && st.board[i+2][j] == '.' && st.board[i+1][j] == '.'){
            if(isValidCoord(i-2,j) && st.board[i-2][j] == currColor && st.board[i-1][j] == currColor){
                if(isValidCoord(i-1,j-3) && st.board[i-1][j-3] == currColor && st.board[i][j-2] == currColor && st.board[i+1][j-1] == '.'){
                  fukumi++;
                }
                if(isValidCoord(i-1,j-2) && st.board[i-1][j-2] == currColor && st.board[i][j-1] == currColor){
                  fukumi++;
                }
                if(isValidCoord(i-1,j+3) && st.board[i-1][j+3] == currColor && st.board[i][j+2] == currColor && st.board[i+1][j+1] == '.'){
                  fukumi++;
                }
                if(isValidCoord(i-1,j+2) && st.board[i-1][j+2] == currColor && st.board[i][j+1] == currColor){
                  fukumi++;
                }
            }
         }


     }
    
     void updateScore(penteData &st, pair<int,int> curr, char currColor){

          int i = curr.first , j = curr.second;  
          if(st.board[i][j] != currColor){
            return;
          }
          updateScoreForRow(st, i,j, currColor);
          updateScoreForCol(st,i,j, currColor);
          updateScoreForBLTR(st, i,j, currColor);
          updateScoreForTLBR(st, i,j, currColor);   
          updateFukumi(st,i,j,currColor);
     }


     long long getCumulativeScore(){
          if(fiver > 0 || captures >= 10){
               // Game over;
               return GAME_OVER;
          }
          return 1 * halfOpenOne + 2 * openOne + 2 * stretch2CellTwo +
                 3 * halfOpenTwo + 4 * openTwo + 5 * stretchTwo  + 
                 6 * halfOpenThree + 7 * stretchThree +  + 8 * openThree + 
                 12 * halfOpenFour + 12 * splitFour  + 12 * stretchFour + 16 * openFour
                 + 8 * captures
                 + 3 * pawnbroker
                 + 9 * fukumi;
     }

     bool hasUnAnsweredFours(){
        return halfOpenFour > 0 || splitFour > 0 || stretchFour > 0 || openFour > 0;

     }
};

struct MaxComparator{
    
  inline bool operator() (const pair< pair<int,int>, ll> &a, const pair< pair<int,int>, ll> &b)
    {
        if(a.second != b.second){
          return (a.second > b.second);
        }
        return abs(a.first.first-9) + abs(a.first.second - 9) > abs(b.first.first - 9) + abs(b.first.second - 9); 

    }
    
};

struct MinComparator{
    
  inline bool operator() (const pair< pair<int,int>, ll> &a, const pair< pair<int,int>, ll> &b)
    {
        if(a.second != b.second){
           return (a.second < b.second);
        }
        return abs(a.first.first-9) + abs(a.first.second - 9) < abs(b.first.first - 9) + abs(b.first.second - 9); 
    }
    
};




penteData parseInput(){
    ifstream f("input.txt");
    penteData inputData;
    if(!f.is_open()){
        cout<<"UNABLE TO OPEN input.txt file, Aborting\n";
        exit(1);
    }
    string color;
    f >> color;
    if(color == "WHITE"){
        inputData.playerColor = 'w';
        inputData.opponentColor = 'b';
    }
    else{
        inputData.playerColor = 'b';
        inputData.opponentColor = 'w';   
    }
    f >> inputData.playTime;
    long long whiteCaptures; // captured by white
    long long blackCaptures; // captured by black
    string captures;
    f >> captures;
    long long num = 0;
    for(int i=0;i<captures.length();i++){
         if(captures[i] == ','){
             whiteCaptures = num;
             num = 0;
         }
         else{
              num = num*10 + captures[i] -'0';   
         }
    }
    blackCaptures = num;
    if(color == "WHITE"){
         inputData.playerCaptures =  whiteCaptures;
         inputData.opponentCaptures = blackCaptures;
    }
    else{
         inputData.playerCaptures = blackCaptures;
         inputData.opponentCaptures = whiteCaptures;
    }
    ll movesPlayed = inputData.opponentCaptures;
  
    for(int i=0;i<19;i++){
         string line;
         f >> line;
         vector<char> row;
         for(int j=0;j<19;j++){
              row.push_back(line[j]);
              if(line[j] == inputData.playerColor){
                  pair<int,int> coord(i,j);
                  inputData.playerCoins.push_back(coord);
              }
              else if(line[j] == inputData.opponentColor){
                  pair<int,int> coord(i,j);
                  inputData.oppCoins.push_back(coord);
              }
              if(inputData.playerColor == line[j]){
                   movesPlayed++;   
              }
         }
         inputData.board.push_back(row);
    }
    inputData.movesPlayed = movesPlayed;
    inputData.playerPtr = inputData.playerCoins.size();
    inputData.oppPtr = inputData.oppCoins.size();
    f.close();
    return inputData;
}

void writeToFile(int row, int col){
     ofstream f("output.txt");
     char colMap[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T'};
     int rowMap[] = {19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3 ,2, 1};
     if(!f.is_open()){
         cout<<"UNABLE TO OPEN output.txt file, Aborting\n";
         exit(1);    
     }
     // TODO : Convert the cell value based on Alphanumeric ordering
     //cout<<rowMap[row]<<colMap[col]<<endl;
     f<<rowMap[row]<<colMap[col]<<endl;
     f.close();
}


pair< pair<int,int>, long long> maxi(penteData &currState, long long alpha, long long beta, int currDepth, ll boardVal);
pair< pair<int,int>, long long> mini(penteData &currState, long long alpha, long long beta, int currDepth, ll boardVal);

long long utilityFunction(penteData &newState, char currPlayer){
    
    char playerColor = newState.playerColor;
    char oppColor = newState.opponentColor;
    boardScore player, opp; 
    player.captures = newState.playerCaptures;
    opp.captures = newState.opponentCaptures;

    for(int i=0;i<newState.playerPtr;i++){
      pair<int,int> pos = newState.playerCoins[i];
      player.updateScore(newState, pos, playerColor);
    }
    for(int i=0;i<newState.oppPtr;i++){
      pair<int,int> pos = newState.oppCoins[i];
      opp.updateScore(newState, pos, oppColor);
    }
     
    long long playerScore = player.getCumulativeScore();
    if(playerScore >= GAME_OVER){
         return GAME_OVER;   
    }
    long long oppScore = opp.getCumulativeScore();
    if(oppScore >= GAME_OVER){
         return -1* GAME_OVER;   
    }
    if(currPlayer == playerColor && opp.hasUnAnsweredFours()){
        return -oppScore; // Provide a large negative value to deprioritize this as it does not handle fours;
    }

    if(currPlayer == oppColor && player.hasUnAnsweredFours()){
        return playerScore; // Provide a large positive value to deprioritize this
    }


    
    return playerScore - oppScore;
}

bool isTerminalState(penteData &currState, long long boardVal){
    if(currState.playerCaptures >= 10 || currState.opponentCaptures >= 10){
        return true;   
    }
    if(abs(boardVal) == GAME_OVER){
        return true;
    }    
    return false;   
}

vector< pair<int,int> > getPossibleMoves(penteData &currState, int currDepth){
     vector< pair<int,int> > moves;
      if(maxDepth >= 4 && currDepth >= 3){
          vector< vector<int> > visited(19, vector<int>  (19, 0));
          for(int i=0;i<currState.playerPtr;i++){
              pair<int,int> pos = currState.playerCoins[i];
              if(currState.board[pos.first][pos.second] != currState.playerColor){
                continue;
              }
              for(int j=-1;j<=1;j++){
                for(int k=-1;k<=1 && isValidCoord(pos.first + j, pos.second + k);k++){
                  if(currState.board[pos.first +j][pos.second +k ] == '.'){
                      visited[pos.first+j][pos.second + k]++;
                  }
                }
              }
          }
          for(int i=0;i<currState.oppPtr;i++){
              pair<int,int> pos = currState.oppCoins[i];
              if(currState.board[pos.first][pos.second] != currState.opponentColor){
                continue;
              }
              for(int j=-1;j<=1;j++){
                for(int k=-1;k<=1 && isValidCoord(pos.first + j, pos.second + k);k++){
                  if(currState.board[pos.first +j][pos.second +k ] == '.'){
                      visited[pos.first+j][pos.second + k]++;
                  }
                }
              }
          }
          vector< pair< pair<int,int>, ll> > moveScoreList;
          for(int i=0;i<19;i++){
            for(int j=0;j<19;j++){
              if(visited[i][j] > 0){
                pair<int,int> move(i,j);
                pair< pair<int,int>, ll> moveScore(move,visited[i][j]);
                moveScoreList.push_back(moveScore);
              }
            }
          }
          sort(moveScoreList.begin(), moveScoreList.end(), MaxComparator());
          for(int i=0; i < moveScoreList.size();i++){
            moves.push_back(moveScoreList[i].first);
          }
          return moves;

      }
       for(int i=0;i<19;i++){
          for(int j=0;j<19;j++){
              if(currState.movesPlayed == 1 && currState.playerColor == 'w' && currState.opponentCaptures == 0){
                  if(abs(i-9) <3 && abs(j-9) < 3){
                    continue;
                  }
              }
              if(currState.board[i][j] == '.'){
                  pair<int,int> move(i,j);
                  moves.push_back(move);
              }
          }
       }
     
     return moves;
}



vector< pair<int,int> > capturePiecesIfPossible(penteData &currState, pair<int,int> currMove, char currColor, char oppColor){
    vector< pair<int,int> > capturedPos;
    int currX = currMove.first;
    int currY = currMove.second;

    // BL to the currMove
    if(isValidCoord(currX-3, currY-3) && currState.board[currX-3][currY-3] == currColor){
        if(currState.board[currX-2][currY-2] == oppColor && currState.board[currX-1][currY-1] == oppColor){
             pair<int,int> p1, p2;
             p1.first = currX-2; p1.second = currY-2;
             p2.first = currX-1; p2.second = currY-1;
             capturedPos.push_back(p1);
             capturedPos.push_back(p2);
        }  
    }

    // BS to the currMove
    if(isValidCoord(currX-3, currY) && currState.board[currX-3][currY] == currColor){
        if(currState.board[currX-2][currY] == oppColor && currState.board[currX-1][currY] == oppColor){
             pair<int,int> p1, p2;
             p1.first = currX-2; p1.second = currY;
             p2.first = currX-1; p2.second = currY;
             capturedPos.push_back(p1);
             capturedPos.push_back(p2);
        } 
    }
    // BR to the currMove
    if(isValidCoord(currX-3, currY + 3) && currState.board[currX-3][currY + 3] == currColor){
        if(currState.board[currX-2][currY+2] == oppColor && currState.board[currX-1][currY+1] == oppColor){
             pair<int,int> p1, p2;
             p1.first = currX-2; p1.second = currY+2;
             p2.first = currX-1; p2.second = currY+1;
             capturedPos.push_back(p1);
             capturedPos.push_back(p2);
        } 
    }
    // SL to the currMove 
    if(isValidCoord(currX, currY -3) && currState.board[currX][currY - 3] == currColor){
        if(currState.board[currX][currY-2] == oppColor && currState.board[currX][currY-1] == oppColor){
             pair<int,int> p1, p2;
             p1.first = currX; p1.second = currY-2;
             p2.first = currX; p2.second = currY-1;
             capturedPos.push_back(p1);
             capturedPos.push_back(p2);
        }
    }
    // SR to the currMove
    if(isValidCoord(currX, currY + 3) && currState.board[currX][currY + 3] == currColor){
        if(currState.board[currX][currY+2] == oppColor && currState.board[currX][currY+1] == oppColor){
             pair<int,int> p1, p2;
             p1.first = currX; p1.second = currY+2;
             p2.first = currX; p2.second = currY+1;
             capturedPos.push_back(p1);
             capturedPos.push_back(p2);
        } 
    }
    // TL to the currMove
    if(isValidCoord(currX+3, currY-3) && currState.board[currX+3][currY-3] == currColor){
        if(currState.board[currX+2][currY-2] == oppColor && currState.board[currX+1][currY-1] == oppColor){
             pair<int,int> p1, p2;
             p1.first = currX+2; p1.second = currY-2;
             p2.first = currX+1; p2.second = currY-1;
             capturedPos.push_back(p1);
             capturedPos.push_back(p2);
        }
    }
    // TS to the currMove
    if(isValidCoord(currX+3, currY) && currState.board[currX+3][currY] == currColor){
        if(currState.board[currX+2][currY] == oppColor && currState.board[currX+1][currY] == oppColor){
             pair<int,int> p1, p2;
             p1.first = currX+2; p1.second = currY;
             p2.first = currX+1; p2.second = currY;
             capturedPos.push_back(p1);
             capturedPos.push_back(p2);
        }
    }
    // TR to the currMove
    if(isValidCoord(currX+3, currY + 3) && currState.board[currX+3][currY + 3] == currColor){
        if(currState.board[currX+2][currY+2] == oppColor && currState.board[currX+1][currY+1] == oppColor){
             pair<int,int> p1, p2;
             p1.first = currX+2; p1.second = currY+2;
             p2.first = currX+1; p2.second = currY+1;
             capturedPos.push_back(p1);
             capturedPos.push_back(p2);
        }
    }
    for(int i=0;i<capturedPos.size();i++){
         pair<int,int> cord = capturedPos[i];
         currState.board[cord.first][cord.second] = '.';   
    }
    
    return capturedPos;

}


void uncapturePieces(penteData &currState, vector< pair<int,int> > &capturedPos, char otherPlayerChar){
    if(capturedPos.size() == 0){
         return;   
    }
    for(int i=0;i<capturedPos.size();i++){
         currState.board[capturedPos[i].first][capturedPos[i].second] = otherPlayerChar;
    }
}



pair< pair<int,int>, long long> maxi(penteData &currState, long long alpha, long long beta, int currDepth, ll boardVal){
        pair< pair<int,int>, long long> res;
        if(currDepth >= maxDepth || isTerminalState(currState, boardVal)){
              res.second = boardVal;
              res.first.first = -1, res.first.second = -1;
              return res;
        }
        vector< pair<int,int> > possibleMoves = getPossibleMoves(currState, currDepth);
        long long maxVal = -2e10;
        pair<int,int> bestMove;
        vector< pair< pair<int,int>, ll> > moveScoreList;
    
        for(int i=0;i<possibleMoves.size();i++){
             pair< pair<int,int>, ll> moveScorePair;
             pair<int,int> move = possibleMoves[i];

             // Set playerColor in this cell and perform relevant computations to get the new state
             currState.board[move.first][move.second] = currState.playerColor;
             vector< pair<int,int> > capturedPos = capturePiecesIfPossible(currState, move, currState.playerColor, currState.opponentColor);
             currState.playerCaptures += capturedPos.size();
             if(currState.playerPtr >= currState.playerCoins.size()){
                currState.playerCoins.push_back(move);
             }
             else{
                currState.playerCoins[currState.playerPtr] = move;
             }
             currState.playerPtr++;
             currState.movesPlayed++;
             
             moveScorePair.first = move;
             moveScorePair.second = utilityFunction(currState, currState.playerColor);
             moveScoreList.push_back(moveScorePair); 
            
             // Revert the change in this cell / and revert the computations done earlier
             uncapturePieces(currState,capturedPos, currState.opponentColor);
             currState.playerCaptures -= capturedPos.size();
             currState.movesPlayed--;
             currState.playerPtr--;
             currState.board[move.first][move.second] = '.';
        }
        // Sort based on Max value since we are maximizing
        sort(moveScoreList.begin(), moveScoreList.end(), MaxComparator());
        
        for(int i=0;i<min((int)moveScoreList.size(),maxBranchFactor);i++){
             pair<int,int> move = moveScoreList[i].first;

             // Set playerColor in this cell and perform relevant computations to get the new state 
             currState.board[move.first][move.second] = currState.playerColor;
             vector< pair<int,int> > capturedPos = capturePiecesIfPossible(currState, move, currState.playerColor, currState.opponentColor);
             currState.playerCaptures += capturedPos.size();
             if(currState.playerPtr >= currState.playerCoins.size()){
                currState.playerCoins.push_back(move);
             }
             else{
                currState.playerCoins[currState.playerPtr] = move;
             }
             currState.playerPtr++;
             currState.movesPlayed++;
             
             // Move to the next level 
             long long score = mini(currState,alpha, beta, currDepth + 1, moveScoreList[i].second).second;
            
             if(maxVal < score){
                   maxVal = score;
                   bestMove = move;
             }
             else if(maxVal == score && abs(score) != GAME_OVER){
                  if(abs(bestMove.first - 9) + abs(bestMove.second -9) > abs(move.first - 9) + abs(move.second - 9)){
                      bestMove = move;
                  }
             }
             if(alpha < maxVal){
                  alpha = maxVal;   
             }
             
             // Revert the change in this cell / and revert the computations done earlier
             uncapturePieces(currState,capturedPos, currState.opponentColor);
             currState.playerCaptures -= capturedPos.size();
             currState.movesPlayed--;
             currState.playerPtr--;
             currState.board[move.first][move.second] = '.';
            
             if(beta < maxVal){
                  break;   
             }
             
        }
        res.first = bestMove;
        res.second = maxVal;
        
        return res;
       
}


pair< pair<int,int>, long long> mini(penteData &currState, long long alpha, long long beta, int currDepth, ll boardVal){
        pair< pair<int,int>, long long> res;
        if(currDepth >= maxDepth || isTerminalState(currState, boardVal)){
              res.second = boardVal; 
              res.first.first = -1, res.first.second = -1;
              return res;
        }
        vector< pair<int,int> > possibleMoves = getPossibleMoves(currState, currDepth);
        long long minVal = 2e10;
        pair<int,int> bestMove;
        vector< pair< pair<int,int>, ll> > moveScoreList;
        for(int i=0;i<possibleMoves.size();i++){
             pair< pair<int,int> , ll> moveScorePair;
             pair<int,int> move = possibleMoves[i];
             // Set playerColor in this cell 
             currState.board[move.first][move.second] = currState.opponentColor;
            
             vector< pair<int,int> > capturedPos = capturePiecesIfPossible(currState ,move, currState.opponentColor, currState.playerColor);
             currState.opponentCaptures += capturedPos.size();
             if(currState.oppPtr >= currState.oppCoins.size()){
                currState.oppCoins.push_back(move);
             }
             else{
                currState.oppCoins[currState.oppPtr] = move;
             }
             currState.oppPtr++;

             
             moveScorePair.first = move;
             moveScorePair.second = utilityFunction(currState, currState.opponentColor);
             moveScoreList.push_back(moveScorePair);
            
             // Revert the change in this cell
             uncapturePieces(currState,capturedPos, currState.playerColor);
             currState.opponentCaptures -= capturedPos.size();
             currState.oppPtr--;
             currState.board[move.first][move.second] = '.';
        }
        sort(moveScoreList.begin(), moveScoreList.end(), MinComparator());
        for(int i=0;i<min((int)moveScoreList.size(),maxBranchFactor);i++){
             pair<int,int> move = moveScoreList[i].first;
             // Set playerColor in this cell 
             currState.board[move.first][move.second] = currState.opponentColor;
            
             vector< pair<int,int> > capturedPos = capturePiecesIfPossible(currState ,move, currState.opponentColor, currState.playerColor);
             currState.opponentCaptures += capturedPos.size();
             if(currState.oppPtr >= currState.oppCoins.size()){
                currState.oppCoins.push_back(move);
             }
             else{
                currState.oppCoins[currState.oppPtr] = move;
             }
             currState.oppPtr++;
             
             long long score = maxi(currState,alpha, beta, currDepth + 1, moveScoreList[i].second).second;
            
             if(minVal > score ){
                   minVal = score;
                   bestMove = move;
             }
             if(beta > minVal){
                  beta = minVal;   
             }
            
             // Revert the change in this cell
             uncapturePieces(currState,capturedPos, currState.playerColor);
             currState.opponentCaptures -= capturedPos.size();
             currState.oppPtr--;
             currState.board[move.first][move.second] = '.';
            
             if(alpha > minVal){
                  break;   
             }
             
        }
        res.first = bestMove;
        res.second = minVal;
        return res;
    
}




pair< pair<int,int>, long long> minmaxAlphaBetaPruning(penteData &currState){

    //Max Allowed Depth and based on which utility function gets called
    int currDepth = 0;
    maxDepth = 5;
    maxBranchFactor = 20;
    long long alpha = -1e10 , beta = 1e10;
    ll boardVal = utilityFunction(currState, currState.playerColor);
    if(currState.playerColor == 'w' && currState.movesPlayed == 0){
        pair< pair<int,int>, long long> firstMove;
        pair<int,int> center;
        center.first = 9; center.second = 9;
        firstMove.first = center;
        firstMove.second = 5;
        return firstMove;
    }
    if(currState.movesPlayed == 0 && currState.playerColor == 'b'){
      maxDepth = 1;
      maxBranchFactor = 400;
    }
    if(currState.movesPlayed > 5 && currState.movesPlayed < 10){
      maxDepth = 5;
      maxBranchFactor = 15;
    }
    if(currState.movesPlayed > 10){
      maxDepth = 4;
      maxBranchFactor = 20;
    }
    if(currState.playTime < 60.0 ){
        maxDepth = 3;
        maxBranchFactor = 30;
    }
    if(currState.playTime < 20.0){
        maxDepth = 2;
        maxBranchFactor = 40;
    }
    pair< pair<int,int>, long long> bestMove = maxi(currState,alpha, beta, currDepth, boardVal);
    return bestMove;
}


int main(){

    penteData input = parseInput();
    //cout<<input.playerColor<<" "<<input.playTime<<endl;
    //cout<<input.playerCaptures<<" "<<input.opponentCaptures<<endl;
    pair< pair<int,int>, long long> res = minmaxAlphaBetaPruning(input);
    //cout<<res.second<<endl;
    writeToFile(res.first.first, res.first.second);
    return 0;
}    

