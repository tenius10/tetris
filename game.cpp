#include "game.h"
#include "console/console.h"

Game::Game(){
    // 초반 블록 초기화
    curTetro=getRandomTetro();
    nextTetro=getRandomTetro();

    // 블록 시작 위치 초기화
    cur_x=(BOARD_WIDTH / 2)-(curTetro.size()/2);
    cur_y=1;  // y:0 에는 box 테두리가 있으니까 1부터

    // board 초기화
    for(int i=0;i<BOARD_WIDTH;i++){
        for(int j=0;j<BOARD_HEIGHT;j++){
            board_[i][j]=false;
        }
    }

    delay=0;
    line=0;

    gameover=false;
}

// 게임의 한 프레임을 처리한다.
void Game::update(){
    delay++;
    if(delay==DROP_DELAY){
        // 딜레이 초기화
        delay=0;

        // 게임 로직 처리
        if(canMoveDown()){
            // 아직 밑으로 이동할 수 있으면 블록 한 칸 내리기
            cur_y++;
        }
        else{
            // 바닥에 도착했으면 고정시키기
            fixTetro();

            // 새로운 테트로미노 가져오기
            curTetro=nextTetro;
            nextTetro=getRandomTetro();

            // 테트로미노 위치 변경
            cur_x=(BOARD_WIDTH / 2)-(curTetro.size()/2);
            cur_y=1;

            // 새로운 블록을 생성했는데 canMoveDown이 false이면 gameover
            if(!canMoveDown()){
                gameover=true;
            }
        }
    }
}

// 게임 화면을 그린다.
void Game::draw(){
    // 1. 게임 테두리 그리기
    console::drawBox(0, 0, BOARD_WIDTH-1, BOARD_HEIGHT-1);

    // 2. 위에서 떨어지는 테트로미노 그리기
    curTetro.drawAt("", cur_x, cur_y);

    // 3. 이미 쌓인 블럭 그리기
    for(int i=0;i<BOARD_HEIGHT;i++){
        for(int j=0;j<BOARD_WIDTH;j++){
            if(board_[j][i]){
                console::draw(j, i, BLOCK_STRING);
            }
        }
    }
}

// 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit(){
    // LINES 만큼의 줄을 지우면 게임 클리어
    if(line==LINES){
        gameClear();
        return true;
    }
    
    // 더 이상 블록을 생성할 공간이 없으면 게임 오버
    // => 생성하자마자 canMoveDown이 false이면 gameover
    if(gameover){
        gameOver();
        return true;
    }

    return false;
}

// curTetro를 한 줄 아래로 내릴 수 있는지 여부를 반환한다.
bool Game::canMoveDown(){
    // 테트로미노를 한 칸 아래로 내렸을 때, board_와 겹치는지 검사
    int size=curTetro.size();
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(curTetro.check(i, j)){
                // 한 칸 아래에 이미 고정된 블록이 있거나 인덱스 범위를 넘으면 false 반환
                if(cur_y + j + 1 > BOARD_HEIGHT - 2 || board_[cur_x+i][cur_y+j+1]){
                    return false;
                }
            }
        }
    }
    return true;
}

void Game::fixTetro(){
    int size=curTetro.size();
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            // 블록을 한 칸 내린 위치에 이미 고정된 블록이 있으면 false 반환
            if(curTetro.check(i, j)){
                board_[cur_x+i][cur_y+j]=true;
            }
        }
    }
}

// 랜덤으로 테트로미노를 생성한다.
Tetromino Game::getRandomTetro(){
    int idx = rand() % TETRO_COUNT;
    // I, O, T, S, Z, J, L
    switch(idx){
        case 0: return Tetromino("I", 4, "XXXXOOOOXXXXXXXX");
        case 1: return Tetromino("O", 2, "OOOO");
        case 2: return Tetromino("T", 3, "XOXOOOXXX");
        case 3: return Tetromino("S", 3, "XOOOOXXXX");
        case 4: return Tetromino("Z", 3, "OOXXOOXXX");
        case 5: return Tetromino("J", 3, "OXXOOOXXX");
        default: return Tetromino("L", 3, "XXOOOOXXX");
    }
}

void Game::gameClear(){

}

void Game::gameOver(){

}