#include <string>
#include <cstdlib>
#include "tetromino.h"
#include "console/console.h"


Tetromino Tetromino::I("I", 4, "XXXXOOOOXXXXXXXX");
Tetromino Tetromino::O("O", 2, "OOOO");
Tetromino Tetromino::T("T", 3, "XOXOOOXXX");
Tetromino Tetromino::S("S", 3, "XOOOOXXXX");
Tetromino Tetromino::Z("Z", 3, "OOXXOOXXX");
Tetromino Tetromino::J("J", 3, "OXXOOOXXX");
Tetromino Tetromino::L("L", 3, "XXOOOOXXX");

Tetromino::Tetromino(){

}

Tetromino::Tetromino(std::string name, int size, std::string shape){
    name_=name;
    size_=size;
    for(int i=0;i<size*size;i++){
        shape_[i/size][i%size]=shape[i]=='O';
    }
    
    if(name=="I") original_=&I;
    if(name=="O") original_=&O;
    if(name=="T") original_=&T;
    if(name=="S") original_=&S;
    if(name=="Z") original_=&Z;
    if(name=="J") original_=&J;
    if(name=="L") original_=&L;
}

// 시계 방향으로 회전한 모습의 테트로미노 객체를 반환한다.
Tetromino Tetromino::rotatedCW(){
    // 시계 방향 회전 아이디어 : (size-1, 0)을 (0, 0)로 보정
    // => (size-1, 0)과 좌표를 비교(절댓값 cmp)해본 후, r과 c를 스왑
    Tetromino copy=Tetromino(*this);
    for(int i=0;i<size_;i++){
        for(int j=0;j<size_;j++){
            copy.shape_[j][size_-1-i]=shape_[i][j];
        }
    }
    return copy;
}

// 반시계 방향으로 회전한 모습의 테트로미노 객체를 반환한다.
Tetromino Tetromino::rotatedCCW(){
    // 반시계 방향 회전 아이디어 : (0, size-1)을 (0, 0)로 보정
    // => (size-1, 0)과 좌표를 비교(절댓값 cmp)해본 후, r과 c를 스왑
    Tetromino copy=Tetromino(*this);
    for(int i=0;i<size_;i++){
        for(int j=0;j<size_;j++){
            copy.shape_[size_-1-j][i]=shape_[i][j];
        }
    }
    return copy;
}

// 화면의 x, y 위치에 s 문자열로  테트로미노를 그린다
void Tetromino::drawAt(std::string s, int x, int y){
    // 매개변수 s는 디버깅 용도로, 실제 구현에는 사용되지 않음.
    for(int i=0;i<size_;i++){
        for(int j=0;j<size_;j++){
            if(shape_[i][j]){
                console::draw(x+i, y+j, BLOCK_STRING);
            }
        }
    }
}