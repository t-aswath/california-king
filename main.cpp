#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>

#define initfen "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

static bool turn = true;
static bool wcastling = true;
static bool bcastling = true;


class Pieces{
    public:
        sf::Sprite piece;
        bool hasPiece;
        int val;
        bool white;
        Pieces() : hasPiece(false), val(0), white(false){}
        Pieces(sf::Sprite s,bool hp,int v,bool w){
            this->hasPiece=hp;
            this->val=v;
            this->piece=s;
            this->white=w;
        }
};

Pieces board[8][8];

std::vector<std::pair<int,int>>moves;

void pieceLoader(int white,int type,int r,int c){
    sf::Sprite piece;
    piece.setTextureRect(sf::IntRect(type*45,45*white,45,45));
    piece.setPosition(sf::Vector2f((100*r)+5,(100*c)+5));
    piece.setScale(sf::Vector2f(2,2));
    board[r][c] = Pieces(piece,true,type,white);
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

void king(Pieces& p,int x,int y){
    int move[][2]={{x+1,y},{x,y+1},{x-1,y},{x,y-1},{x+1,y+1},{x+1,y-1},{x-1,y+1},{x-1,y-1}};
    for(int i=0;i<8;i++){
        if(move[i][0]>=0&&move[i][0]<8&&move[i][1]>=0&&move[i][1]<8){
            if(board[move[i][0]][move[i][1]].hasPiece&&p.white!=board[move[i][0]][move[i][1]].white||!board[move[i][0]][move[i][1]].hasPiece){
                moves.push_back(std::make_pair(move[i][0],move[i][1]));
            }
        }
    }
}

void rook(Pieces& p,int x,int y){
    for(int i=x+1;i<8;i++){
        if(board[i][y].hasPiece){
            if(board[i][y].white!=p.white){
                moves.push_back(std::make_pair(i,y));
                break;
            }
            else{
                break;
            }
        }
        else{
            moves.push_back(std::make_pair(i,y));
        }
    }
    for(int i=y+1;i<8;i++){
        if(board[x][i].hasPiece){
            if(board[x][i].white!=p.white){
                moves.push_back(std::make_pair(x,i));
                break;
            }
            else{
                break;
            }
        }
        else{
            moves.push_back(std::make_pair(x,i));
        }
    }
    for(int i=x-1;i>=0;i--){
        if(board[i][y].hasPiece){
            if(board[i][y].white!=p.white){
                moves.push_back(std::make_pair(i,y));
                break;
            }
            else{
                break;
            }
        }
        else{
            moves.push_back(std::make_pair(i,y));
        }
    }
    for(int i=y-1;i>=0;i--){
        if(board[x][i].hasPiece){
            if(board[x][i].white!=p.white){
                moves.push_back(std::make_pair(x,i));
                break;
            }
            else{
                break;
            }
        }
        else{
            moves.push_back(std::make_pair(x,i));
        }
    }
}

void bishop(Pieces& p,int x,int y){
    int tx=x+1,ty=y+1;
    while(tx<8&&ty<8){
        if(board[tx][ty].hasPiece){
            if(board[tx][ty].white!=p.white){
                moves.push_back(std::make_pair(tx,ty));
                break;
            }
            else{
                break;
            }
        }
        else{
            moves.push_back(std::make_pair(tx,ty));
            tx++;ty++;
        }
    }
    tx=x-1;ty=y-1;
    while(tx>=0&&ty>=0){
        if(board[tx][ty].hasPiece){
            if(board[tx][ty].white!=p.white){
                moves.push_back(std::make_pair(tx,ty));
                break;
            }
            else{
                break;
            }
        }
        else{
            moves.push_back(std::make_pair(tx,ty));
            tx--;ty--;
        }
    }
    tx=x-1;ty=y+1;
    while(tx>=0&&ty<8){
        if(board[tx][ty].hasPiece){
            if(board[tx][ty].white!=p.white){
                moves.push_back(std::make_pair(tx,ty));
                break;
            }
            else{
                break;
            }
        }
        else{
            moves.push_back(std::make_pair(tx,ty));
            tx--;ty++;
        }
    }
    tx=x+1;ty=y-1;
    while(tx<8&&ty>=0){
        if(board[tx][ty].hasPiece){
            if(board[tx][ty].white!=p.white){
                moves.push_back(std::make_pair(tx,ty));
                break;
            }
            else{
                break;
            }
        }
        else{
            moves.push_back(std::make_pair(tx,ty));
            tx++;ty--;
        }
    }
}

void knight(Pieces& p,int x,int y){
    int move[][2]={{x+2,y+1},{x+2,y-1},{x-2,y-1},{x-2,y+1},{x-1,y+2},{x-1,y-2},{x+1,y+2},{x+1,y-2}};
    for(int i=0;i<8;i++){
        if(move[i][0]>=0&&move[i][0]<8&&move[i][1]>=0&&move[i][1]<8){
            if(board[move[i][0]][move[i][1]].hasPiece&&p.white!=board[move[i][0]][move[i][1]].white||!board[move[i][0]][move[i][1]].hasPiece){
                moves.push_back(std::make_pair(move[i][0],move[i][1]));
            }
        }
    }
}

void pawn(Pieces& p,int x,int y){
    if(!p.white){
        if(y-1>=0&&x+1<8&&board[x+1][y-1].hasPiece&&board[x+1][y-1].white!=p.white){
            moves.push_back(std::make_pair(x+1,y-1));
        }
        if(y-1>=0&&x-1<8&&board[x-1][y-1].hasPiece&&board[x-1][y-1].white!=p.white){
            moves.push_back(std::make_pair(x-1,y-1));
        }
        if(y-1>=0&&!board[x][y-1].hasPiece){
            moves.push_back(std::make_pair(x,y-1));
        }
        else{
            return;
        }
        if(y==6&&!board[x][y-2].hasPiece){
            moves.push_back(std::make_pair(x,y-2));
        }
    }
    else{
        if(y+1>=0&&x+1<8&&board[x+1][y+1].hasPiece&&board[x+1][y+1].white!=p.white){
            moves.push_back(std::make_pair(x+1,y+1));
        }
        if(y+1>=0&&x-1<8&&board[x-1][y+1].hasPiece&&board[x-1][y+1].white!=p.white){
            moves.push_back(std::make_pair(x-1,y+1));
        }
        if(y+1<8&&!board[x][y+1].hasPiece){
            moves.push_back(std::make_pair(x,y+1));
        }
        else{
            return;
        }
        if(y==1&&!board[x][y+2].hasPiece){
            moves.push_back(std::make_pair(x,y+2));
        }
    }
}

void movehint(sf::RenderWindow& window,Pieces p,int x,int y){
    x=x/100;
    y=y/100;
    moves.push_back(std::make_pair(x,y));
    switch(p.val){

        case 0:
            king(p,x,y);
            break;
        case 1:
            rook(p,x,y);
            bishop(p,x,y);
            break;
        case 2:
            bishop(p,x,y);
            break;
        case 3:
            knight(p,x,y);
            break;
        case 4:
            rook(p,x,y);
            break;
        case 5:
            pawn(p,x,y);
            break;
    }
    for(std::pair<int,int>m:moves){
            if(m.first==x&&m.second==y)continue;
            sf::CircleShape hint(10);
            hint.setOutlineThickness(2.f);
            hint.setOutlineColor(sf::Color(255,255,255));
            hint.setFillColor(sf::Color(108,111,125,20));
            hint.setPosition(sf::Vector2f((100*m.first)+40,(100*m.second)+40));
            window.draw(hint);
    }
}

int movepiece(int x,int y,int i,int j){
    int nx = x/100;
    int ny = y/100;
    int cap =1;
    if(nx==i&&ny==j){
        board[i][j].piece.setPosition(sf::Vector2f((100*i)+5,(100*j)+5));
        moves.clear();
        return 0;
    }
    else{
        if(std::find(moves.begin(),moves.end(),std::make_pair(nx,ny))!=moves.end()){
            Pieces temp = board[i][j];
            if(temp.hasPiece)cap+=1;
            board[i][j]=Pieces();
            board[nx][ny]= temp;
            board[nx][ny].piece.setPosition(sf::Vector2f((100*nx)+5,(100*ny)+5));
            turn = !turn;
            moves.clear();
            return cap;
        }
        else{
            board[i][j].piece.setPosition(sf::Vector2f((100*i)+5,(100*j)+5));
            moves.clear();
            return 0;
        }
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

    //loading sounds
    sf::SoundBuffer movebuffer;
    sf::SoundBuffer capbuffer;
    if(!movebuffer.loadFromFile("./move-self.wav")){
        std::cout<<"unable to load audio";
    }
    if(!capbuffer.loadFromFile("./capture.wav")){
        std::cout<<"unable to load audio";
    }
    sf::Sound moveaudio;
    sf::Sound capaudio;
    moveaudio.setBuffer(movebuffer);
    capaudio.setBuffer(capbuffer);

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
                if(event.mouseButton.button == sf::Mouse::Left&&!move){
                    for(int i=0;i<8;i++){
                        for(int j=0;j<8;j++){
                            if(board[i][j].piece.getGlobalBounds().contains(mpos.x,mpos.y)){
                                if(!board[i][j].white&&turn||board[i][j].white&&!turn){
                                    move=true;
                                    mx=mpos.x;my=mpos.y;
                                    x=i;y=j;
                                }
                            }
                        }
                    }
                }
            }

            if(event.type == sf::Event::MouseButtonReleased){
                if(event.mouseButton.button == sf::Mouse::Left&&move){
                    move=false;
                    int mp = movepiece(mpos.x,mpos.y,x,y);
                    if(mp==1){
                        moveaudio.play(); 
                    }
                    if(mp==2){
                        capaudio.play();
                    }
                    // for(int i=0;i<8;i++){
                    //     for(int j=0;j<8;j++){
                    //         std::cout<<board[i][j].hasPiece<<" ";
                    //     }
                    //     std::cout<<"\n";
                    // }      
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
