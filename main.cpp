#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>

#define initfen "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"


class Pieces{
    public:
        sf::Sprite piece;
        bool hasPiece;
        int val;
        bool white;
        void fenToBoard(std::string fen = initfen);
        void pieceLoader(int white,int type,int r,int c);
};

Pieces board[8][8];

void pieceLoader(int white,int type,int r,int c){
    sf::Sprite piece;
    piece.setTextureRect(sf::IntRect(type*45,45*white,45,45));
    piece.setPosition(sf::Vector2f((100*r)+5,(100*c)+5));
    piece.setScale(sf::Vector2f(2,2));
    board[r][c] = Pieces();
    board[r][c].white = white;
    board[r][c].val = type;
    board[r][c].hasPiece = true;
    board[r][c].piece = piece;
}

void fenToBoard(std::string fen = initfen){
    std::unordered_map<char, int>fmap ={{'k',0},{'q',1},{'b',2},{'n',3},{'r',4},{'p',5}};
    int r=0,c=0;
    for(char a:fen){
        if(a=='/'){
            r=0;
            c+=1;
        }
        else{
            if(std::isdigit(a)){
                r+=(a-'0');
            }
            else{
                int white = (std::islower(a))?1:0;
                int type = fmap[std::tolower(a)];
                pieceLoader(white,type,r,c);
                r++;
            }
        }
    }
}

void king(std::vector<std::pair<int,int>>& moves,Pieces& p,int x,int y){
    if(x+1<8)moves.push_back(std::make_pair(x+1,y));
    if(y+1<8)moves.push_back(std::make_pair(x,y+1));
    if(x-1>=0)moves.push_back(std::make_pair(x-1,y));
    if(y-1>=0)moves.push_back(std::make_pair(x,y-1));
    if(x+1<8&&y+1<8)moves.push_back(std::make_pair(x+1,y+1));
    if(x+1<8&&y-1>=0)moves.push_back(std::make_pair(x+1,y-1));
    if(x-1>=0&&y+1<8)moves.push_back(std::make_pair(x-1,y+1));
    if(x-1>=0&&y-1>=0)moves.push_back(std::make_pair(x-1,y-1));
}

void rook(std::vector<std::pair<int,int>>& moves,Pieces& p,int x,int y){
    for(int i=x+1;i<8;i++){
        moves.push_back(std::make_pair(i,y));
    }
    for(int i=y+1;i<8;i++){
        moves.push_back(std::make_pair(x,i));
    }
    for(int i=0;i<x;i++){
        moves.push_back(std::make_pair(i,y));
    }
    for(int i=0;i<y;i++){
        moves.push_back(std::make_pair(x,i));
    }
}

void bishop(std::vector<std::pair<int,int>>& moves,Pieces& p,int x,int y){
    int tx=x+1,ty=y+1;
    while(tx<8&&ty<8){
        moves.push_back(std::make_pair(tx,ty));
        tx++;ty++;
    }
    tx=x-1;ty=y-1;
    while(tx>=0&&ty>=0){
        moves.push_back(std::make_pair(tx,ty));
        tx--;ty--;
    }
    tx=x-1;ty=y+1;
    while(tx>=0&&ty<8){
        moves.push_back(std::make_pair(tx,ty));
        tx--;ty++;
    }
    tx=x+1;ty=y-1;
    while(tx<8&&ty>=0){
        moves.push_back(std::make_pair(tx,ty));
        tx++;ty--;
    }
}

void knight(std::vector<std::pair<int,int>>& moves,Pieces& p,int x,int y){
    if(x+2<8&&y+1<8)moves.push_back(std::make_pair(x+2,y+1));
    if(x+2<8&&y-1>=0)moves.push_back(std::make_pair(x+2,y-1));
    if(x-2>=0&&y-1>=0)moves.push_back(std::make_pair(x-2,y-1));
    if(x-2>=0&&y+1<8)moves.push_back(std::make_pair(x-2,y+1));
    if(x-1>=0&&y+2<8)moves.push_back(std::make_pair(x-1,y+2));
    if(x-1>=0&&y-2>=0)moves.push_back(std::make_pair(x-1,y-2));
    if(x+1<8&&y+2<8)moves.push_back(std::make_pair(x+1,y+2));
    if(x+1<8&&y-2>=0)moves.push_back(std::make_pair(x+1,y-2));
}

void pawn(std::vector<std::pair<int,int>>& moves,Pieces& p,int x,int y){
    if(!p.white){
        if(y==6){
            moves.push_back(std::make_pair(x,y-2));
        }
        if(y-1>=0){
            moves.push_back(std::make_pair(x,y-1));
        }
    }
    else{
        if(y==1){
            moves.push_back(std::make_pair(x,y+2));
        }
        if(y+1<8){
            moves.push_back(std::make_pair(x,y+1));
        }

    }

}

void movepiece(int x,int y,int i,int j){
    int nx = x/100;
    int ny = y/100;
    board[i][j].piece.setPosition(sf::Vector2f((100*nx)+5,(100*ny)+5));
}

void movehint(sf::RenderWindow& window,Pieces p,int x,int y){
    std::vector<std::pair<int,int>>moves;
    x=x/100;
    y=y/100;
    moves.push_back(std::make_pair(x,y));
    switch(p.val){

        case 0:
            king(moves,p,x,y);
            break;
        case 1:
            rook(moves,p,x,y);
            bishop(moves,p,x,y);
            break;
        case 2:
            bishop(moves,p,x,y);
            break;
        case 3:
            knight(moves,p,x,y);
            break;
        case 4:
            rook(moves,p,x,y);
            break;
        case 5:
            pawn(moves,p,x,y);
            break;
    }
    for(std::pair<int,int>m:moves){
            sf::CircleShape hint(10);
            hint.setFillColor(sf::Color::Red);
            hint.setPosition(sf::Vector2f((100*m.first)+40,(100*m.second)+40));
            window.draw(hint);
    }
}

int main(){

    // create the window
    sf::RenderWindow window(sf::VideoMode(1000, 800), "california king");

    // loading texture
    sf::Texture bTexture;
    sf::Texture pTexture;
    if(!bTexture.loadFromFile("./chesswath.png")){
        std::cout<<"unable to load board texture";
    }
    if(!pTexture.loadFromFile("./Sprite.png")){
        std::cout<<"unable to load piece texture";
    }
    bTexture.setSmooth(true);
    pTexture.setSmooth(true);
    sf::Sprite Sboard(bTexture);

    //piece & control
    bool move=false;
    int x,y;
    double mx,my;
    
    //init board && assign texture
    fenToBoard();
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(board[i][j].hasPiece){
                board[i][j].piece.setTexture(pTexture);
            }
        }
    }
    
    //window handling
    while (window.isOpen()){
        sf::Event event;
        sf::Vector2i mpos = sf::Mouse::getPosition(window);
        
        // events
        while (window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::MouseButtonPressed){
                if(event.mouseButton.button == sf::Mouse::Left){
                    for(int i=0;i<8;i++){
                        for(int j=0;j<8;j++){
                            if(board[i][j].piece.getGlobalBounds().contains(mpos.x,mpos.y)){
                                move=true;
                                mx=mpos.x;my=mpos.y;
                                x=i;y=j;
                            }
                        }
                    }
                }
            }

            if(event.type == sf::Event::MouseButtonReleased){
                if(event.mouseButton.button == sf::Mouse::Left){
                    move=false;
                    movepiece(mpos.x,mpos.y,x,y);
                }
            }
        }

        //window draw
        window.clear();
        window.draw(Sboard);
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                if(board[i][j].hasPiece){
                    window.draw(board[i][j].piece);
                }
            }
        }      

        //piece movement
        if(move){
            movehint(window,board[x][y],mx,my);
            board[x][y].piece.setPosition(sf::Vector2f(mpos.x-45,mpos.y-45));
        }  
        window.display();
    }
    return 0;
}
