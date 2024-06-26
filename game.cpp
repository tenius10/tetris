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
    lines=LINES;

    existHold=false;
    canHold=true;

    gameclear=false;
    gameover=false;
    esc=false;

    startTime=clock();
}

// 문자열 s를 y 줄에 중앙 정렬로 출력한다.
void Game::drawCenter(std::string s, int y){
    int bm = BOARD_WIDTH / 2;
    int sm = s.size() / 2;
    int idx = 0;
    if(s.size() < BOARD_WIDTH) idx = bm - sm;
    console::draw(idx, y, s);
}

// 화면에 게임 클리어 문구를 보여준다.
void Game::gameClear(std::string clearTime){
    int y = BOARD_HEIGHT / 2;
    drawCenter("You Win", y);
    drawCenter(clearTime, y+1);
}

// 화면에 게임 오버 문구를 보여준다.
void Game::gameOver(){
    int y = BOARD_HEIGHT / 2;
    drawCenter("You Lost", y);
}

// 게임의 한 프레임을 처리한다.
void Game::update(){
    delay++;
    
    // 키 입력 확인

    // ESC 누르면 게임 즉시 종료
    if(console::key(console::K_ESC)){
        esc=true;
        return;
    }
    // 왼쪽으로 이동
    if(console::key(console::K_LEFT)){
        if(!isConflict(curTetro, cur_x-1, cur_y)){
            cur_x--;
        }
    }
    // 오른쪽으로 이동
    if(console::key(console::K_RIGHT)){
        if(!isConflict(curTetro, cur_x+1, cur_y)){
            cur_x++;
        }
    }
    // 시계 방향 회전
    if(console::key(console::K_X)){
        Tetromino cwTetro=curTetro.rotatedCW();
        if(!isConflict(cwTetro, cur_x, cur_y)){
            curTetro=cwTetro;
        }
    }
    // 반시계 방향 회전
    if(console::key(console::K_Z)){
        Tetromino ccwTetro=curTetro.rotatedCCW();
        if(!isConflict(ccwTetro, cur_x, cur_y)){
            curTetro=ccwTetro;
        }
    }
    // 하드 드롭
    if(console::key(console::K_UP)){
        hardDrop();
    }
    // 소프트 드롭
    if(console::key(console::K_DOWN)){
        softDrop();
    }
    // 홀드
    if(console::key(console::K_SPACE)){
        if(canHold) hold();
    }
    
    // 프레임 딜레이만큼 기다렸으면 로직 실행
    if(delay >= DROP_DELAY){
        // 딜레이 초기화
        delay = 0;

        // 게임 로직 처리
        if(!isConflict(curTetro, cur_x, cur_y+1)){
            // 아직 밑으로 이동할 수 있으면 블록 한 칸 내리기
            cur_y++;
        }
        else{
            // 바닥에 도착했으면 고정시키기
            fixTetro();

            // 완성된 line이 있으면 터뜨리기
            checkAndRemoveLine();
            if(lines==0){
                gameclear=true;
                return;
            }

            // 새로운 테트로미노 가져오기
            curTetro=nextTetro;
            nextTetro=getRandomTetro();
            canHold=true;

            // 테트로미노 위치 변경
            cur_x=(BOARD_WIDTH / 2)-(curTetro.size()/2);
            cur_y=1;

            // 새로운 블록을 생성했는데 기존 블록과 부딪히면 gameover
            if(isConflict(curTetro, cur_x, cur_y)){
                gameover=true;
            }
        }
    }    
}

// 게임 화면을 그린다.
void Game::draw(){
    // 1. 게임 테두리 그리기
    console::drawBox(0, 0, BOARD_WIDTH-1, BOARD_HEIGHT-1);

    // 2. shadow 그리기 (내려오는 블록, 쌓인 블록보다 먼저 그려야 함)
    int dy = 0;
    while(!isConflict(curTetro, cur_x, cur_y+dy+1)) dy++;
    curTetro.drawAt(SHADOW_STRING, cur_x, cur_y+dy);

    // 3. 위에서 떨어지는 테트로미노 그리기
    curTetro.drawAt(BLOCK_STRING, cur_x, cur_y);

    // 4. 이미 쌓인 블럭 그리기
    for(int i=0;i<BOARD_HEIGHT;i++){
        for(int j=0;j<BOARD_WIDTH;j++){
            if(board_[j][i]){
                console::draw(j, i, BLOCK_STRING);
            }
        }
    }

    // 5. next 그리기
    console::drawBox(BOARD_WIDTH+2, 0, BOARD_WIDTH+7, 5);
    console::draw(BOARD_WIDTH+3, 0, "Next");
    int sm = nextTetro.size() / 2;
    nextTetro.original()->drawAt(BLOCK_STRING, BOARD_WIDTH+5-sm, 3-sm);
    
    // 6. hold 그리기
    console::drawBox(BOARD_WIDTH+9, 0, BOARD_WIDTH+14, 5);
    console::draw(BOARD_WIDTH+10, 0, "Hold");
    if(existHold){
        sm = holdTetro.size()/2;
        holdTetro.original()->drawAt(BLOCK_STRING, BOARD_WIDTH+12-sm, 3-sm);
    }
    
    // 7. 지워야 하는 line 개수 출력
    drawCenter(std::to_string(lines)+" lines left", BOARD_HEIGHT);

    // 8. 플레이 시간 출력
    std::string curTime=getCurTime();
    drawCenter(curTime, BOARD_HEIGHT+1);

    // 9. 게임 결과 출력
    if(gameclear) gameClear(curTime); 
    if(gameover) gameOver();
}

// 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit(){
    // LINES 만큼의 줄을 지우면 게임 클리어
    if(gameclear) return true;
    
    // 더 이상 블록을 생성할 공간이 없으면 게임 오버
    // => 생성한 블록이 기존 블록과 겹치거나, 아래로 내려갈 수 없는 경우 gameover
    if(gameover) return true;

    // esc 누르면 게임 종료
    if(esc) return true;

    return false;
}

// curTetro가 (x, y)에 있을 때, 테두리 혹은 기존 블록에 부딪히는지 여부를 반환한다.
bool Game::isConflict(Tetromino& tetro, int x, int y){
    int size=tetro.size();
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(tetro.check(i, j)){
                // 인덱스 검사
                if(x+j < 1 || y+i < 1 || x+j > BOARD_WIDTH-2 || y+i > BOARD_HEIGHT-2){
                    return true;
                }
                // 이미 쌓여있는 블록과 겹치는지 검사
                if(board_[x+j][y+i]) {
                    return true;
                }
            }
        }
    }
    return false;
}

// 현재 테트로미노를 현재 위치에 고정시킨다.
void Game::fixTetro(){
    int size=curTetro.size();
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            // 테트로미노를 board에 고정
            if(curTetro.check(i, j)){
                board_[cur_x+j][cur_y+i]=true;
            }
        }
    }
}

// 완성된 line을 제거한다.
void Game::checkAndRemoveLine(){
    // 아래에서부터 검사
    int y_cursor=BOARD_HEIGHT-2;
    int y_idx=BOARD_HEIGHT-2;
    while(y_idx>0){
        if(isFullLine(y_idx)){
            // 라인이 꽉 찼다면, 해당 라인을 y_cursor에 그리지 않고 지나가기 
            if(lines>0) lines--;
        }
        else{
            // 라인이 꽉 차지 않았다면, 해당 라인을 y_cursor에 그냥 그리기
            for(int i=0;i<BOARD_WIDTH;i++){
                board_[i][y_cursor]=board_[i][y_idx];
            }
            y_cursor--;
        }
        y_idx--;
    }
    // 위에 빈 공간은 false로 채워넣기
    while(y_cursor>0){
        for(int i=0;i<BOARD_WIDTH;i++){
            board_[i][y_cursor]=false;
        }
        y_cursor--;
    }
}

// BOARD의 y번째 줄이 꽊 찼는지 여부를 반환한다.
bool Game::isFullLine(int y){
    for(int i=1;i<BOARD_WIDTH-1;i++){
        if(!board_[i][y]) return false;
    }
    return true;
}

// 현재 테트로미노를 보관한다.
void Game::hold(){
    if(!canHold) return;

    if(existHold){
        // 홀드된 게 있다면, curTetro와 holdTetro를 swap
        Tetromino tmp=curTetro;
        curTetro=holdTetro;
        holdTetro=tmp;

        // 홀드했던 걸 가져오는 상황에서는 curTetro의 모양을 original의 모양과 일치시켜야 함.
        // => Tetromino에 setter 메서드가 없으니까 새로 객체를 만들어야 함
        curTetro=getTetro(curTetro.original()->name());
    }
    else{
        // 홀드된 게 없다면 hold
        holdTetro=curTetro;
        curTetro=nextTetro;
        nextTetro=getRandomTetro();
    }

    // 블록 좌표 이동
    cur_x=(BOARD_WIDTH / 2)-(curTetro.size()/2);
    cur_y=1;
    
    existHold=true;
    canHold=false;
}

// 현재 테트로미노를 바로 바닥에 고정시킨다 (하드 드롭)
void Game::hardDrop(){
    while(!isConflict(curTetro, cur_x, cur_y+1)){
        cur_y++;
    }
}

// 현재 테트로미노를 빠르게 떨어뜨린다 (소프트 드롭)
void Game::softDrop(){
    if(!isConflict(curTetro, cur_x, cur_y+1)){
        cur_y++;
    }
}


// 랜덤으로 테트로미노를 생성한다.
Tetromino Game::getRandomTetro(){
    int idx = rand() % TETRO_COUNT;
    // I, O, T, S, Z, J, L
    switch(idx){
        case 0: return getTetro("I");
        case 1: return getTetro("O");
        case 2: return getTetro("T");
        case 3: return getTetro("S");
        case 4: return getTetro("Z");
        case 5: return getTetro("J");
        default: return getTetro("L");
    }
}

// name 모양의 테트로미노를 생성한다.
Tetromino Game::getTetro(std::string name){
    if(name=="I") return Tetromino("I", 4, "XXXXOOOOXXXXXXXX");
    if(name=="O") return Tetromino("O", 2, "OOOO");
    if(name=="T") return Tetromino("T", 3, "XOXOOOXXX");
    if(name=="S") return Tetromino("S", 3, "XOOOOXXXX");
    if(name=="Z") return Tetromino("Z", 3, "OOXXOOXXX");
    if(name=="J") return Tetromino("J", 3, "OXXOOOXXX");
    if(name=="L") return Tetromino("L", 3, "XXOOOOXXX");
}

// 현재 시간(분/초/밀리초)을 --:--.-- 형태의 string으로 반환한다.
std::string Game::getCurTime(){
    endTime=clock();
    double intervalSecond=(double)(endTime-startTime)/CLOCKS_PER_SEC;
    int minute=(int)(intervalSecond/60);
    int second=(int)intervalSecond%60;
    int milliSecond=(intervalSecond-(int)intervalSecond)*1000;  // 0 ~ 999
    milliSecond/=10;  // 0 ~ 99

    std::string curTime="";
    curTime+=(minute<10)?"0":"";
    curTime+=std::to_string(minute)+":";
    curTime+=(second<10)?"0":"";
    curTime+=std::to_string(second)+".";
    curTime+=(milliSecond<10)?"0":"";
    curTime+=std::to_string(milliSecond);
    
    return curTime;
}