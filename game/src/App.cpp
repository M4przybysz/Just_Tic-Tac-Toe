// Local Headers
#include "../include/App.hpp"

SDL_Renderer* App::renderer = nullptr;

App::App() { 
    std::clog << "App created!\n";
}

App::~App() { 
    SDL_FreeSurface(playerInfoSurface_);
    SDL_FreeSurface(turnNumSurface_);
    SDL_FreeSurface(winnerInfoSurface_ );
    std::clog << "Surfaces freed...\n";

    SDL_DestroyTexture(playerInfoTexture_);
    SDL_DestroyTexture(turnNumTexture_);
    SDL_DestroyTexture(winnerInfoTexture_);
    std::clog << "Textures destroyed...\n";

    TTF_CloseFont(font_);
    std::clog << "Font closed...\n";

    SDL_DestroyWindow(window_);
    std::clog << "Window destroyed...\n";
    SDL_DestroyRenderer(renderer);
    std::clog << "Renderer destroyed...\n";
    SDL_Quit();
    TTF_Quit();
    std::clog << "SDL quitted...\n";
    std::clog << "App destroyed!\n"; 
}

void App::init(const std::string& title, const int& x, const int& y, const int& width, const int& height, const unsigned int& flags) {
    // Init all SDL shit or it won't work
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() == 0) { // App needs SDL initialized to do anything
        std::clog << "SDL initialized...\n";
        window_ = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);

        font_ = TTF_OpenFont("./fonts/Helvetica-Bold.ttf", 20);

        if(window_) { // App needs a window_ to create renderer
            std::clog << "Window created...\n"; 
            renderer = SDL_CreateRenderer(window_, 1, 0);

            if(renderer) { // App needs a renderer to display anything
                std::clog << "Renderer created...\n";
                SDL_SetRenderDrawColor(renderer, 50, 0, 100, 0);
                isRunning_ = true;
                std::clog << "TTT is running!!!\n";
            }
            else { std::cerr << "Couldn't create a renderer!\n"; }
        }
        else { std::cerr << "Couldn't create a window!\n"; }
    }
    else {
        std::cerr << "Couldn't initialize SDL!\n";
        isRunning_ = false;
    }
}

void App::handleEvents() {
    SDL_Event event; // variable to store events

    while(SDL_PollEvent(&event)) { // get new event
        switch(event.type) {
            case SDL_QUIT:
                isRunning_ = false;
                std::clog << "Window closed...\n";
                break;
            
            // >>>Add more events to be handled here<<<
            case SDL_MOUSEBUTTONDOWN:
                // std::clog << (int)event.button.button << " " << event.button.x << " " << event.button.y << '\n';
                if((int)event.button.button == 1 && event.button.y >= 40 && winner == '.' && player_ == 1) { 
                    if(updateBoard(event.button.x, event.button.y) == 0) {
                        player_ = !player_;
                        turnCounter++;
                    } 
                }
                else if((int)event.button.button == 1 && winner != '.') {
                    resetGame();
                }
                break;

            default:
                break;
        }
    }
}

void App::update(const double& deltaTime) {
    if(winner == '.' && player_ == 0 && turnCounter < 10) {
        updateBot();
    }
    if(turnCounter >= 10) {
        int win = checkWin();
        winner = (win == 1)? 'X': (win == 0)? 'O' : (win == 2)? 'N' : '.';
    }
}

void App::render() {
    SDL_RenderClear(renderer);      // Clear renderer to show new things on screen

    // >>> Add stuff to render here <<<
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    drawBoard();
    drawXO();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0 ,0);   // invisible color to stop renderer from drawing
    SDL_RenderPresent(renderer);                    // Show everything on screen
}

void App::drawBoard() {
    if(winner == 'X' || winner == 'O' || winner == 'N') {
        winnerInfo_ = (winner == 'N')? "Tie (Click to restart)" : (winner == 'X')? "Player X won! (Click to restart)" : "Player O won! (Click to restart)";
        winnerInfoSurface_ = TTF_RenderText_Solid(font_, winnerInfo_.c_str(), textColor_);
        winnerInfoTexture_ = SDL_CreateTextureFromSurface(renderer, winnerInfoSurface_);
        SDL_Rect rect = {(600-winnerInfoSurface_->w)/2, (640-winnerInfoSurface_->h)/2, winnerInfoSurface_->w, winnerInfoSurface_->h};
        SDL_RenderCopy(renderer, winnerInfoTexture_, NULL, &rect);
    }
    else if(winner == '.') {
        // Draw board
        SDL_RenderDrawLine(renderer, 300, 0, 300, 40);
        SDL_RenderDrawLine(renderer, 0, 40, 600, 40);
        SDL_RenderDrawLine(renderer, 200, 40, 200, 640);
        SDL_RenderDrawLine(renderer, 400, 40, 400, 640);
        SDL_RenderDrawLine(renderer, 0, 240, 600, 240);
        SDL_RenderDrawLine(renderer, 0, 440, 600, 440);

        // Display text
        playerInfo_ = (player_)? ("Player: X") : ("Player: O");
        turnNum_ = "Turn: " + std::to_string(turnCounter);

        playerInfoSurface_ = TTF_RenderText_Solid(font_, playerInfo_.c_str(), textColor_);
        playerInfoTexture_ = SDL_CreateTextureFromSurface(renderer, playerInfoSurface_);
        turnNumSurface_ = TTF_RenderText_Solid(font_, turnNum_.c_str(), textColor_);
        turnNumTexture_ = SDL_CreateTextureFromSurface(renderer, turnNumSurface_);
        
        SDL_Rect rect1 = {(300-playerInfoSurface_->w)/2, 10, playerInfoSurface_->w, playerInfoSurface_->h};
        SDL_Rect rect2 = {300+(300-turnNumSurface_->w)/2, 10, turnNumSurface_->w, turnNumSurface_->h};

        SDL_RenderCopy(renderer, playerInfoTexture_, NULL, &rect1);
        SDL_RenderCopy(renderer, turnNumTexture_, NULL, &rect2);
    }
}

void App::drawXO() {
    if(winner == '.') {
        for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(board_[j][i] == 'X') { drawX(renderer, i*200+100, j*200+140); }
            if(board_[j][i] == 'O') { drawO(renderer, i*200+100, j*200+140, 80); }
        }
    }
    }
}

void App::resetGame() {
    player_ = 1;
    turnCounter = 1;
    board_ = {{{'.', '.', '.'}, {'.', '.', '.'}, {'.', '.', '.'}}};
    winner = '.';
}

int App::checkWin() {
    if((board_[0][0] == 'X' && board_[0][1] == 'X' && board_[0][2] == 'X') || (board_[1][0] == 'X' && board_[1][1] == 'X' && board_[1][2] == 'X') || 
       (board_[2][0] == 'X' && board_[2][1] == 'X' && board_[2][2] == 'X') || (board_[0][0] == 'X' && board_[1][0] == 'X' && board_[2][0] == 'X') || 
       (board_[0][1] == 'X' && board_[1][1] == 'X' && board_[2][1] == 'X') || (board_[0][2] == 'X' && board_[1][2] == 'X' && board_[2][2] == 'X') ||
       (board_[0][0] == 'X' && board_[1][1] == 'X' && board_[2][2] == 'X') || (board_[0][2] == 'X' && board_[1][1] == 'X' && board_[2][0] == 'X')) 
    {
        return 1;
    }
    if((board_[0][0] == 'O' && board_[0][1] == 'O' && board_[0][2] == 'O') || (board_[1][0] == 'O' && board_[1][1] == 'O' && board_[1][2] == 'O') || 
       (board_[2][0] == 'O' && board_[2][1] == 'O' && board_[2][2] == 'O') || (board_[0][0] == 'O' && board_[1][0] == 'O' && board_[2][0] == 'O') || 
       (board_[0][1] == 'O' && board_[1][1] == 'O' && board_[2][1] == 'O') || (board_[0][2] == 'O' && board_[1][2] == 'O' && board_[2][2] == 'O') ||
       (board_[0][0] == 'O' && board_[1][1] == 'O' && board_[2][2] == 'O') || (board_[0][2] == 'O' && board_[1][1] == 'O' && board_[2][0] == 'O')) 
    {
        return 0;
    }
    if(turnCounter >= 10) {
        return 2;
    }
    
    return -1;
}

int App::updateBoard(int mouseX, int mouseY) {
    short x = (mouseX >= 0 && mouseX < 200)? 0 : (mouseX < 400)? 1 : (mouseX <= 600)? 2 : -1;
    short y = (mouseY >= 40 && mouseY < 240)? 0 : (mouseY < 440)? 1 : (mouseY <= 640)? 2 : -1;

    if(winner == '.' && x != -1 && y != -1 && board_[y][x] == '.') {
        board_[y][x] = (player_)? 'X' : 'O';
        // std::clog << x << ' ' << y << ' ' << board[y][x] << '\n';

        std::clog << board_[0][0] << board_[0][1] << board_[0][2] << '\n' << board_[1][0] << board_[1][1] << board_[1][2] << '\n' << board_[2][0] << board_[2][1] << board_[2][2] << '\n';

        int win = checkWin();
        std::clog << win << '\n';

        winner = (win == 1)? 'X': (win == 0)? 'O' : (win == 2)? 'N' : '.';
        std::clog << winner << '\n';

        return 0;
    }
    return -1;
}

std::pair<short, short> App::botSearch(char p) {
    short x = -1, y = -1;

    for(short i = 0; i < 3; i++) {      // board_ y
        for(short j = 0; j < 3; j++) {  // board_ x
            if(board_[i][j] == p) {
                // check column
                if(i == 0) {
                    if(board_[i+1][j] == p && board_[i+2][j] == '.') { y = i+2; x = j; break; }
                    if(board_[i+2][j] == p && board_[i+1][j] == '.') { y = i+1; x = j; break; }
                }
                else if(i == 1) {
                    if(board_[i+1][j] == p && board_[i-1][j] == '.') { y = i-1; x = j; break; }
                    if(board_[i-1][j] == p && board_[i+1][j] == '.') { y = i+1; x = j; break; }
                }
                else if(i == 2) {
                    if(board_[i-1][j] == p && board_[i-2][j] == '.') { y = i-2; x = j; break; }
                    if(board_[i-2][j] == p && board_[i-1][j] == '.') { y = i-1; x = j; break; }
                }

                // check row
                if(j == 0) {
                    if(board_[i][j+1] == p && board_[i][j+2] == '.') { y = i; x = j+2; break; }
                    if(board_[i][j+2] == p && board_[i][j+1] == '.') { y = i; x = j+1; break; }
                }
                else if(j == 1) {
                    if(board_[i][j+1] == p && board_[i][j-1] == '.') { y = i; x = j-1; break; }
                    if(board_[i][j-1] == p && board_[i][j+1] == '.') { y = i; x = j+1; break; }
                }
                else if(j == 2) {
                    if(board_[i][j-1] == p && board_[i][j-2] == '.') { y = i; x = j-2; break; }
                    if(board_[i][j-2] == p && board_[i][j-1] == '.') { y = i; x = j-1; break; }
                }

                // check diagonal
                if(i == 0 && j == 0) {
                    if(board_[i+1][j+1] == p && board_[i+2][j+2] == '.') { y = i+2; x = j+2; break; }
                    if(board_[i+2][j+2] == p && board_[i+1][j+1] == '.') { y = i+1; x = j+1; break; }
                }
                else if(i == 1 && j == 1) {
                    if(board_[i-1][j-1] == p && board_[i+1][j+1] == '.') { y = i+1; x = j+1; break; }
                    if(board_[i+1][j+1] == p && board_[i-1][j-1] == '.') { y = i-1; x = j-1; break; }
                    if(board_[i+1][j-1] == p && board_[i-1][j+1] == '.') { y = i-1; x = j+1; break; }
                    if(board_[i-1][j+1] == p && board_[i+1][j-1] == '.') { y = i+1; x = j-1; break; }
                }
                else if(i == 2 && j == 2) {
                    if(board_[i-1][j-1] == p && board_[i-2][j-2] == '.') { y = i-2; x = j-2; break; }
                    if(board_[i-2][j-2] == p && board_[i-1][j-1] == '.') { y = i-1; x = j-1; break; }
                }
                else if(i == 0 && j == 2) {
                    if(board_[i+1][j-1] == p && board_[i+2][j-2] == '.') { y = i+2; x = j-2; break; }
                    if(board_[i+2][j-2] == p && board_[i+1][j-1] == '.') { y = i-1; x = j+1; break; }
                }
                else if(i == 2 && j == 0) {
                    if(board_[i-1][j+1] == p && board_[i-2][j+2] == '.') { y = i-2; x = j+2; break; }
                    if(board_[i-2][j+2] == p && board_[i-1][j+1] == '.') { y = i+1; x = j-1; break; }
                }
            }
        }
    }

    return {x, y};
}

void App::updateBot() {
    short x, y;
    std::pair<short, short> botTarget = {-1, -1};
    bool lock = false;

    // Search for 'O'
    if(!lock) {
        botTarget = botSearch('O');
        if(botTarget.first != -1 && botTarget.second != -1) {
            x = botTarget.first;
            y = botTarget.second;
            lock = true;
        }
    }

    // Search for player win in 1 turn
    if(!lock) {
        botTarget = botSearch('X');
        if(botTarget.first != -1 && botTarget.second != -1) {
            x = botTarget.first;
            y = botTarget.second;
            lock = true;
        }
    }

    // Choose random
    if(!lock) {
        do {
            x = rand() % 4;
            y = rand() % 4;
        } while(board_[y][x] != '.');
    }

    board_[y][x] = 'O';
    player_ = !player_;
    turnCounter++;

    int win = checkWin();
    winner = (win == 1)? 'X': (win == 0)? 'O' : (win == 2)? 'N' : '.';
}

void drawX(SDL_Renderer* renderer, int centerX, int centerY) {
    SDL_RenderDrawLine(renderer, centerX-80, centerY-80, centerX+80, centerY+80);
    SDL_RenderDrawLine(renderer, centerX-80, centerY+80, centerX+80, centerY-80);
}

// SDL implementation of midpoint circle algorithm
void drawO(SDL_Renderer* renderer, int centreX, int centreY, int radius)
{
    const int diameter = (radius * 2);

    int x = (radius - 1);
    int y = 0;
    int tx = 1;
    int ty = 1;
    int error = (tx - diameter);

    while(x >= y) {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
   }
}
