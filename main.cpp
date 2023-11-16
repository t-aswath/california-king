#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <bits/stdc++.h>
#include <cctype>


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
                                x=i;y=j;
                            }
                        }
                    }
                }
            }

            if(event.type == sf::Event::MouseButtonReleased){
                if(event.mouseButton.button == sf::Mouse::Left){
                    move=false;
                }
            }
        }

        //piece movement
        if(move){
            board[x][y].piece.setPosition(mpos.x-45,mpos.y-45);
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
        window.display();
    }
    return 0;
}
