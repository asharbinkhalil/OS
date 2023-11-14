#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include<pthread.h>
#include <sched.h>
#include<string>
#include <X11/Xlib.h>
using namespace std;
int BOARD_SIZE = 0; // size of the game board
int BLOCK_SIZE = 0; // size of each block on the game board
const int BOARD_START_X = 5; // starting x coordinate of the game board
const int BOARD_START_Y = 5; // starting y coordinate of the game board
const int SCORES_START_X = 800; // starting x coordinate of the scores
const int SCORES_START_Y = 100; // starting y coordinate of the scores
const int Bsize = 650;
bool flag = false;
class player_thread
{
public:
    int index;
    sf::Texture texture;
    pthread_t id;
    std::string name;
    sf::Sprite sp;
    int score;
    int x, y;
    player_thread()
    {
        index=0;
        x = y = 0;
        id = 0;
        name = "";
        score = 0;
    }
};
struct Block_info {
    int color;
    int reward;
};
struct game_info {
    Block_info** board;
    int boardSize;
    player_thread* playerinfo;
    player_thread* oppinfo;
};
void UpdatePriority(vector<player_thread>& players)
{
    // update priorities of threads
    for (int i = 0; i < players.size(); i++) {
        struct sched_param param;
        param.sched_priority = players.size()-i;
        int ret = pthread_setschedparam(players[i].id, SCHED_RR, &param);
        if (ret != 0) {
            // error handling
        }
    }
}
// Function to initialize the game board with zeros and ones
void initBoard(Block_info** board) {
    int rewards = BOARD_SIZE * BOARD_SIZE * 0.2;
    for(int i=0;i<BOARD_SIZE;i++)
    {
        for(int j=0;j<BOARD_SIZE;j++)
        {
            board[i][j].color= (i+j)%2;
            board[i][j].reward=0;
        }
    }
    for(int i =0;i<rewards;i++)
    {
        int x= rand()% (BOARD_SIZE -2) + 1;
        int y= rand()% (BOARD_SIZE -2) + 1;
        int scores []= {1,1,1,1,1,2,2,2,5,5};
        if(board[x][y].reward!=0)
        {
            i--;
            continue;
        }
        board[x][y].reward = scores[rand()% (sizeof(scores) / sizeof(int))];
    }
}
int GenerateRandomNumber()
{
    // roll number ---> 20k-1724
    int num = (rand() % 90 + 10) * 4;  // creating a random number and multiplying it with the last digit of the roll number
    num = (1724 / num) % 25;
    if (num < 10)
        num += 15;
    return num;

}
void* ManagePlayerGamePlay(void* gPtr)
{
    game_info* ptr = (game_info*)(gPtr);

    while(1)
    {
    // Get a random sleep time between 1 and 2 seconds
    std::srand(std::time(nullptr));
    string out ="";
    // Get a random direction to move
    int newX = ptr->playerinfo->x;
    int newY = ptr->playerinfo->y;

        if (ptr->playerinfo->index == 0) {
        // Use arrow keys
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            newY--;
            out="Player has moved up";
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            newY++;
            out="Player has moved down";
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            newX--;
            out="Player has moved left";
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            newX++;
            out="Player has moved right";
        }
    } else {
        // Use WASD keys
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            newY--;
            out="Player has moved up";
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            newY++;
            out="Player has moved down";
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            newX--;
            out="Player has moved left";
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            newX++;
            out="Player has moved right";
        }
    }
    // Check if the new position is within the board boundaries
        if (newX >= 0 && newX < ptr->boardSize && newY >= 0 && newY < ptr->boardSize && (ptr->oppinfo->x!= newX || ptr->oppinfo->y!= newY)) {
            
        // Update the player's position
        ptr->playerinfo->x = newX;
        ptr->playerinfo->y = newY;
        ptr->playerinfo->score+=ptr->board[newY][newX].reward;
        ptr->board[newY][newX].reward =0;
        cout<<ptr->playerinfo->name<<"  "<<out<<endl;
        sf::sleep(sf::milliseconds(200));
        }
    }
    return NULL;
}
bool compareByScore(const player_thread& a, const player_thread& b) {
    return a.score > b.score;
}
void CreatePlayerThreads(vector<player_thread>& players, Block_info** board, const int boardSize)
{

    for (int i =0;i<players.size();i++)
    {
        game_info *ginfo= new game_info;
        ginfo->board = board;
        ginfo->boardSize = boardSize;
        ginfo->playerinfo = &players[i];
        ginfo->oppinfo = &players[(i+1)%2];
        pthread_create(&players[i].id, NULL, ManagePlayerGamePlay, (void*)ginfo);
    }
    int counter = players.size();
    UpdatePriority(players);
}
void inputPlayerinfo(vector<player_thread>& players)
{
    int size = 2;
    players.resize(size);
    for (auto& i : players)
    {
        cout << "Enter player name : ";
        cin >> i.name;
    }
}
void SetPlayerPositions(vector<player_thread>& players)
{
    int x[] = { 0,BOARD_SIZE - 1,0,BOARD_SIZE - 1 };
    int y[] = { 0,BOARD_SIZE - 1,BOARD_SIZE - 1,0};
    for (int i = 0; i < players.size(); i++)
    {
        players[i].index = i%2;
        players[i].x = x[i];
        players[i].y = y[i];
    }
}
void drawPlayers(sf::RenderWindow& window,vector<player_thread> & players)
{
    for(auto &player:players)
    {
        player.sp.setPosition(sf::Vector2f(player.x*BLOCK_SIZE + (BLOCK_SIZE/5), player.y*BLOCK_SIZE + (BLOCK_SIZE/5)));
        window.draw(player.sp);
    }
}
int main()
{
    bool GameOver = false;
    srand(time(NULL));
    XInitThreads();
    vector<player_thread> players;
    inputPlayerinfo(players);
    sf::RenderWindow window(sf::VideoMode(1000, 700), "20k-1724-Ashar-Khalil");
    window.setFramerateLimit(30);
    // Calculate the block size dynamically based on the board size
    BOARD_SIZE = GenerateRandomNumber();
    BLOCK_SIZE = Bsize / BOARD_SIZE;
    SetPlayerPositions(players);
    // Create the game board rectangle shape
    sf::RectangleShape bg(sf::Vector2f(BOARD_SIZE * BLOCK_SIZE + 4, BOARD_SIZE * BLOCK_SIZE + 4));
    bg.setFillColor(sf::Color::Black);
    bg.setOutlineThickness(5.f);
    bg.setOutlineColor(sf::Color::White);
    bg.setPosition(sf::Vector2f(BOARD_START_X - 2, BOARD_START_Y - 2));
    window.draw(bg);
    bg.setPosition(sf::Vector2f(0, 0));
    sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    // Dynamically allocate a 2D array to represent the game board
    sf::Texture tx[3];
    sf::Sprite spr[3];
    for(int i=0;i<3;i++)
    {
        tx[i].loadFromFile("sprites/reward_"+to_string(i+1)+".png");
        spr[i].setTexture(tx[i]);
        spr[i].setScale(sf::Vector2f(BLOCK_SIZE*0.8 / spr[i].getGlobalBounds().width, BLOCK_SIZE*0.8 / spr[i].getGlobalBounds().height));
    }
    Block_info** board = new Block_info * [BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[i] = new Block_info[BOARD_SIZE];
    }

    // Initialize the game board with zeros and ones
    initBoard(board);
    for(int i=0;i<players.size();i++)
    {
        
        if(!players[i].texture.loadFromFile("sprites/ch3_"+to_string(i+1)+".png"));
        players[i].sp.setTexture(players[i].texture);
        players[i].sp.setScale(sf::Vector2f(BLOCK_SIZE*1.0 / players[i].sp.getGlobalBounds().width, BLOCK_SIZE*1.0 / players[i].sp.getGlobalBounds().height));
    }
        //Creating PlayerThreads
    CreatePlayerThreads(players, board, BOARD_SIZE);

    // Create a vector of player scores and add some sample scores
    
    // Sort the scores in descending order
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        
        if ( !GameOver)
        {
        window.clear();
        window.draw(bg);
   
        // Draw the game board using the dynamically allocated 2D array
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                if (board[row][col].color == 0) {
                    block.setFillColor(sf::Color::White);
                }
                else {
                    block.setFillColor(sf::Color::Black);
                }
                block.setPosition(sf::Vector2f(BOARD_START_X + col * BLOCK_SIZE, BOARD_START_Y + row * BLOCK_SIZE));
                window.draw(block);
                if(board[row][col].reward==1)
                {
                    spr[0].setPosition(sf::Vector2f(BOARD_START_X + col * BLOCK_SIZE, BOARD_START_Y + row * BLOCK_SIZE));
                    window.draw(spr[0]);
                }
                if(board[row][col].reward==2)
                {
                    spr[1].setPosition(sf::Vector2f(BOARD_START_X + col * BLOCK_SIZE, BOARD_START_Y + row * BLOCK_SIZE));
                    window.draw(spr[1]);
                }
                if(board[row][col].reward==5)
                {
                    spr[2].setPosition(sf::Vector2f(BOARD_START_X + col * BLOCK_SIZE, BOARD_START_Y + row * BLOCK_SIZE));
                    window.draw(spr[2]);
                }
            }
        }
        sf::Text text;
        sf::Font font;
        if (!font.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/arial.ttf")) {
            std::cout << "Error loading font" << std::endl;
            return 1;
        }
        
        sf::Text Title("Uhd ka Maidaan", font, 40);
        Title.setFillColor(sf::Color::White);
        Title.setPosition(sf::Vector2f(680, 20));
        window.draw(Title);
        std::sort(players.begin(), players.end(), compareByScore);
        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(SCORES_START_X, SCORES_START_Y));
        for (int i = 0; i < players.size(); i++) {
            text.setString(players[i].name + " : " + std::to_string(players[i].score));
            text.setFillColor(sf::Color::Red);
            window.draw(text);
            text.move(sf::Vector2f(0, 50)); // move the position for the next score
        }
        
        // sf::Texture chest;
        // chest.loadFromFile("sprites/dec.jpg");
        // sf::Sprite ch(chest);
        // ch.setScale(300/ch.getLocalBounds().width, 300/ch.getLocalBounds().height );
        // ch.setPosition(sf::Vector2f(680, 300));
        // window.draw(ch);
        
         drawPlayers(window,players);
        if ( (players[0].score >= 20) || (players[1].score >= 20))
        {
        	GameOver = true;
        }
     }    
        if ( GameOver )
    {
    	window.clear(sf::Color::Black);
      
      if (players[0].score >= 20) 
      {
        sf::Texture w1;
        w1.loadFromFile("sprites/win1.png");
        sf::Sprite win1(w1);
        win1.setScale(1000/win1.getLocalBounds().width, 700/win1.getLocalBounds().height );
        window.draw(win1);
        window.display();
      }
      
      else if (players[1].score >= 20) 
      {
        sf::Texture w2;
        w2.loadFromFile("sprites/win2.png");
        sf::Sprite win2(w2);
        win2.setScale(1000/win2.getLocalBounds().width, 700/win2.getLocalBounds().height );
        window.draw(win2);
        window.display();
      }
      
    }
    
       
        window.display();
        //UpdatePriority(players);
    }
    
    
   	
    // Free the dynamically allocated memory
    for (int i = 0; i < BOARD_SIZE; i++) {
        delete[] board[i];
    }
    delete[] board;
    return 0;
}

