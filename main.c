#include "raylib.h"
#include <math.h>

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum GameScreen
{
    LOGO = 0,
    TITLE,
    GAMEPLAY,
    GAMEOVER,
    ENDING
} GameScreen;

#define ANIMATION_SPEED 6
#define MAX_ENEMY 50
#define MAX_COIN 50
#define MAX_HEART 3
#define MAX_OBELISK 1
#define MAX_ATTACK_ORB 10
#define MAX_REQ_ATT_ORB 3
#define UNIT 20
#define FIRST_PHASE 25
#define SECOND_PHASE 50
#define THIRD_PHASE 100

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

const int screenWidth = 800;
const int screenHeight = 450;
const int _spritesWidth = 80; // 128
const int _spritesHeight = 80;
const int _eneT1SpritesWidth = 40;
const int _eneT1SpritesHeight = 32;
const int _eneT2SpritesWidth = 48;
const int _eneT2SpritesHeight = 64;
const float coinSpriteWidth = 16.67;
const float coinSpriteHeight = 18;
const int heartSpriteWidth = 16;
const int heartSpriteHeight = 17;
const int obeliskWidth = 200;
const int obelikHeight = 400;
const int runningFrames = 6;
const int attackFrames = 12;

float deltaTime;
float GRAVITY = -1920.0f; // Calculate and change accordingly to max height and elapsed time to that height
float dubJumpHeight = 0.0f;

int spriteIndex = 0;
int spriteToggle = 1;
int x = 0;
int y = screenHeight / 2 - 68;
int framesCounter = 0;
int gameSpeed = 0;
float phaseCounter = 0;
float enemyTimeCounter = 0;
float maxEnemyDiff = 1;
float coinTimeCounter = 0;
float maxCoinDiff = 2;
float heartTimeCounter = 0;
float maxHeartDiff = 15.5;
float obeliskTimeCounter = 0;
float maxObeliskTime = 20;
float attackOrbTimeCounter = 0;
float maxAttOrbTime = 6.5;
float maxAttackTime = 10;
int heartCount = 0;
int coinCount = 0;
bool key = true;

Rectangle startRect = {600, 100, screenWidth / 6, 30};
Rectangle exitRect = {600, 160, screenWidth / 6, 30};
Rectangle restartRect = {screenWidth / 2 - 4 * UNIT, screenHeight / 2 + 2 * UNIT, screenWidth / 6 + 2 * UNIT, 30};

Vector2 mousePos;

typedef struct player
{
    Rectangle rec;
    Color color;
    float vel;
    float health;
    float jumpTime;
    float attackTimeCount;
    int jumpCount;
    int framesCounter;
    int orbCount;
    bool isJumping;
    bool ULT;
    bool attackReady;
    bool attacking;
} Player;

typedef struct Enemy
{
    Rectangle rec;
    Vector2 speed;
    Color color;
    int type;
    int spriteIndex;
    int framesCounter;
    bool active;
    bool collisionChecked;
} Enemy;

typedef struct Coin
{
    Color color;
    Vector2 origin;
    float radius;
    int spriteIndex;
    int framesCounter;
    bool active;
} Coin;

typedef struct Heart
{
    Rectangle rec;
    Color color;
    int spriteIndex;
    int framesCounter;
    bool active;
} Heart;

typedef struct Obelisk
{
    Rectangle rec;
    Color color;
    int spriteIndex;
    int framesCounter;
    bool active;
    bool activated;
} Obelisk;

typedef struct AttackOrb
{
    Rectangle rec;
    Color color;
    int spriteIndex;
    int framesCounter;
    bool active;
} AttackOrb;

Player player = {0};
Enemy enemy[MAX_ENEMY] = {0};
Coin coin[MAX_COIN] = {0};
Heart heart[MAX_HEART] = {0};
Obelisk obelisk = {0};
AttackOrb attackOrb[MAX_ATTACK_ORB] = {0};

Texture2D playerSprite;
Texture2D enemyType1;
Texture2D enemyType2;
Texture2D coins_Hearts;
Texture2D obeliskTexture;

// Functions prototyping

// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);   // Initialize game
static void UpdateGame(void); // Update game (one frame)
static void DrawGame(void);   // Draw game (one frame)

GameScreen currentScreen = LOGO;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic screen manager");

    // TODO: Initialize all required variables and load all required data here!

    InitGame();

    playerSprite = LoadTexture("D:/GameDev(Raylib)/Assests/NightBorne/NightBorne.png");
    enemyType1 = LoadTexture("D:/GameDev(Raylib)/Assests/Dark VFX 1/DarkVFX(Rev).png");
    enemyType2 = LoadTexture("D:/GameDev(Raylib)/Assests/Dark VFX 2/Combined.png");
    coins_Hearts = LoadTexture("D:/GameDev(Raylib)/Assests/Coins/coins-chests-etc-2-0.png");
    obeliskTexture = LoadTexture("D:/GameDev(Raylib)/Assests/Obelisk/FlyingObelisk.png");

    // Background Textures

    Texture2D one = LoadTexture("D:/GameDev(Raylib)/Assests/Parallex Bgs/Clouds prallex Bg/Clouds 8/1.png");
    Texture2D two = LoadTexture("D:/GameDev(Raylib)/Assests/Parallex Bgs/Clouds prallex Bg/Clouds 8/2.png");
    Texture2D three = LoadTexture("D:/GameDev(Raylib)/Assests/Parallex Bgs/Clouds prallex Bg/Clouds 8/3.png");
    Texture2D four = LoadTexture("D:/GameDev(Raylib)/Assests/Parallex Bgs/Clouds prallex Bg/Clouds 8/4.png");
    Texture2D five = LoadTexture("D:/GameDev(Raylib)/Assests/Parallex Bgs/Clouds prallex Bg/Clouds 8/5.png");
    Texture2D six = LoadTexture("D:/GameDev(Raylib)/Assests/Parallex Bgs/Clouds prallex Bg/Clouds 8/6.png");

    // Initializing Music
    InitAudioDevice();

    Music musicTitle = LoadMusicStream("D:/GameDev(Raylib)/Assests/mp3/Ambient 5.mp3");
    Music musicGamePLay = LoadMusicStream("D:/GameDev(Raylib)/Assests/mp3/Action 5.mp3");

    PlayMusicStream(musicTitle);
    PlayMusicStream(musicGamePLay);

    float scrollingOne = 0.0f;
    float scrollingTwo = 0.0f;
    float scrollingThree = 0.0f;
    float scrollingFour = 0.0f;
    float scrollingFive = 0.0f;
    float scrollingSix = 0.0f;

    SetTargetFPS(60); // Set desired framerate (frames-per-second)
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        switch (currentScreen)
        {
        case LOGO:
        {
            // TODO: Update LOGO screen variables here!

            framesCounter++; // Count frames

            // Wait for 2 seconds (120 frames) before jumping to TITLE screen
            if (framesCounter > 120)
            {
                currentScreen = TITLE;
                framesCounter = 0;
            }
        }
        break;
        case TITLE:
        {
            // TODO: Update TITLE screen variables here!

            mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, startRect))
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    currentScreen = GAMEPLAY;
                    framesCounter = 0;
                }
            }
            if (CheckCollisionPointRec(mousePos, exitRect))
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    currentScreen = ENDING;
                    framesCounter = 0;
                }
            }
            framesCounter++;

            if (framesCounter >= ANIMATION_SPEED)
            {
                spriteIndex++;
                framesCounter = 0;
            }
            if (spriteIndex >= 9)
            {
                spriteIndex = 0;
            }

            // Music
            UpdateMusicStream(musicTitle);
        }
        break;
        case GAMEPLAY:
        {
            // TODO: Update GAMEPLAY screen variables here!

            // GAME
            deltaTime = GetFrameTime();

            // Changing Game Speed
            phaseCounter += deltaTime;
            if (phaseCounter <= FIRST_PHASE)
            {
                maxEnemyDiff = 1.0f;
                gameSpeed = 1;
            }
            else if (phaseCounter <= SECOND_PHASE)
            {
                maxEnemyDiff = 0.75f;
            }
            else if (phaseCounter <= THIRD_PHASE)
            {
                maxEnemyDiff = 0.50f;
                gameSpeed = 2;
            }

            UpdateGame();

            // Updating Bg
            scrollingOne -= 0.1f;
            scrollingTwo -= 0.25f;
            scrollingThree -= 0.5f;
            scrollingFour -= 0.75f;
            scrollingFive -= 1.0f;
            scrollingSix -= 0.1f;

            if (scrollingOne <= -one.width * 1.38)
                scrollingOne = 0;
            if (scrollingTwo <= -two.width * 1.38)
                scrollingTwo = 0;
            if (scrollingThree <= -three.width * 1.38)
                scrollingThree = 0;
            if (scrollingFour <= -one.width * 1.38)
                scrollingFour = 0;
            if (scrollingFive <= -two.width * 1.38)
                scrollingFive = 0;
            if (scrollingSix <= -three.width * 1.38)
                scrollingSix = 0;

            // Music
            UpdateMusicStream(musicGamePLay);
        }
        break;
        case GAMEOVER:
        {
            mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, restartRect))
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    InitGame();
                    phaseCounter = 0;
                    enemyTimeCounter = 0;
                    attackOrbTimeCounter = 0;
                    heartTimeCounter = 0;
                    coinTimeCounter = 0;
                    obeliskTimeCounter = 0;
                    scrollingOne = 0.0f;
                    scrollingTwo = 0.0f;
                    scrollingThree = 0.0f;
                    scrollingFour = 0.0f;
                    scrollingFive = 0.0f;
                    scrollingSix = 0.0f;
                    currentScreen = GAMEPLAY;
                    framesCounter = 0;
                }
            }
        }
        break;
        case ENDING:
        {
            // TODO: Update ENDING screen variables here!

            // Press enter to return to TITLE screen
            framesCounter++;
            if (framesCounter > 120)
            {

                CloseWindow();
                return 0;
            }
        }
        break;
        default:
            break;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        switch (currentScreen)
        {
        case LOGO:
        {
            // TODO: Draw LOGO screen here!
            DrawText("AMATERASU", screenWidth / 2 - 3 * 40, screenHeight / 2 - 40, 40, LIGHTGRAY);
            DrawText("Loading...", 280, 250, 20, GRAY);
        }
        break;
        case TITLE:
        {
            // TODO:

            DrawTextureEx(one, (Vector2){scrollingOne, 0}, 0.0f, 1.38f, WHITE);
            DrawTextureEx(one, (Vector2){one.width * 1.38 + scrollingOne, 0}, 0.0f, 1.38f, WHITE);

            // Draw midground image twice
            DrawTextureEx(two, (Vector2){scrollingTwo, 0}, 0.0f, 1.38f, WHITE);
            DrawTextureEx(two, (Vector2){two.width * 1.38 + scrollingTwo, 0}, 0.0f, 1.38f, WHITE);

            // Draw foreground image twice
            DrawTextureEx(three, (Vector2){scrollingThree, 0}, 0.0f, 1.38f, WHITE);
            DrawTextureEx(three, (Vector2){three.width * 1.38 + scrollingThree, 0}, 0.0f, 1.38f, WHITE);

            // Draw background image twice
            // NOTE: Texture is scaled twice its size
            // DrawTextureEx(four, (Vector2){ scrollingFour, 0 }, 0.0f, 1.38f, WHITE);
            // DrawTextureEx(four, (Vector2){ one.width*1.38 + scrollingFour, 0 }, 0.0f, 1.38f, WHITE);

            // Draw midground image twice
            DrawTextureEx(five, (Vector2){scrollingFive, 0}, 0.0f, 1.38f, WHITE);
            DrawTextureEx(five, (Vector2){two.width * 1.38 + scrollingFive, 0}, 0.0f, 1.38f, WHITE);

            // Draw foreground image twice
            DrawTextureEx(six, (Vector2){scrollingSix, 0}, 0.0f, 1.38f, WHITE);
            DrawTextureEx(six, (Vector2){three.width * 1.38 + scrollingSix, 0}, 0.0f, 1.38f, WHITE);

            DrawLineEx((Vector2){0, screenHeight / 2}, (Vector2){screenWidth, screenHeight / 2}, 2.0f, BLACK);

            // Options: Start, Exit
            DrawRectangle(600, 100, screenWidth / 6, 30, BLACK);
            DrawText("START", 615, 100, 30, WHITE);

            DrawRectangle(600, 160, screenWidth / 6, 30, BLACK);
            DrawText("EXIT", 630, 160, 30, WHITE);

            // DrawTexturePro(playerIdle, (Rectangle){idleIndex * _spritesWidth, 60, 128, 68}, (Rectangle){x, y, 128, 68}, (Vector2){0, 0}, 0, WHITE);
            DrawTexturePro(playerSprite, (Rectangle){spriteIndex * _spritesWidth, 0, _spritesWidth, _spritesHeight}, (Rectangle){x, y + UNIT / 4, _spritesWidth, _spritesHeight}, (Vector2){0, 0}, 0, WHITE);
        }
        break;
        case GAMEPLAY:
        {
            // TODO:
            // Draw Bg
            // Draw background image twice
            // NOTE: Texture is scaled 1.38 time its size
            DrawTextureEx(one, (Vector2){scrollingOne, 0}, 0.0f, 1.38f, WHITE);
            DrawTextureEx(one, (Vector2){one.width * 1.38 + scrollingOne, 0}, 0.0f, 1.38f, WHITE);

            // Draw midground image twice
            DrawTextureEx(two, (Vector2){scrollingTwo, 0}, 0.0f, 1.38f, WHITE);
            DrawTextureEx(two, (Vector2){two.width * 1.38 + scrollingTwo, 0}, 0.0f, 1.38f, WHITE);

            // Draw foreground image twice
            DrawTextureEx(three, (Vector2){scrollingThree, 0}, 0.0f, 1.38f, WHITE);
            DrawTextureEx(three, (Vector2){three.width * 1.38 + scrollingThree, 0}, 0.0f, 1.38f, WHITE);

            // Draw background image twice
            // NOTE: Texture is scaled twice its size
            // DrawTextureEx(four, (Vector2){ scrollingFour, 0 }, 0.0f, 1.38f, WHITE);
            // DrawTextureEx(four, (Vector2){ one.width*1.38 + scrollingFour, 0 }, 0.0f, 1.38f, WHITE);

            // Draw midground image twice
            DrawTextureEx(five, (Vector2){scrollingFive, 0}, 0.0f, 1.38f, WHITE);
            DrawTextureEx(five, (Vector2){two.width * 1.38 + scrollingFive, 0}, 0.0f, 1.38f, WHITE);

            // Draw foreground image twice
            DrawTextureEx(six, (Vector2){scrollingSix, 0}, 0.0f, 1.38f, WHITE);
            DrawTextureEx(six, (Vector2){three.width * 1.38 + scrollingSix, 0}, 0.0f, 1.38f, WHITE);

            // Draw GAMEPLAY screen here
            DrawGame();
        }
        break;
        case GAMEOVER:
        {
            DrawRectangle(restartRect.x, restartRect.y, restartRect.width, restartRect.height, BLACK);
            DrawText("RESTART", restartRect.x + UNIT / 2, restartRect.y, UNIT * 1.5, WHITE);
            DrawText("Game Over", screenWidth / 2 - UNIT * 5, screenHeight / 2 - UNIT * 2, UNIT * 2, BLACK);
        }
        break;
        case ENDING:
        {
            // TODO:

            DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
            DrawText("CLOSING THE GAME", 20, 20, 40, DARKBLUE);
        }
        break;
        default:
            break;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // TODO: Unload all loaded data (textures, fonts, audio) here!
    UnloadTexture(one);   // Unload background texture
    UnloadTexture(two);   // Unload midground texture
    UnloadTexture(three); // Unload foreground texture
    UnloadTexture(four);  // Unload background texture
    UnloadTexture(five);  // Unload midground texture
    UnloadTexture(six);
    UnloadTexture(playerSprite);   // Unload background texture
    UnloadTexture(coins_Hearts);   // Unload midground texture
    UnloadTexture(obeliskTexture); // Unload foreground texture
    UnloadTexture(enemyType1);     // Unload background texture
    UnloadTexture(enemyType2);     // Unload midground texture

    // Unload music
    UnloadMusicStream(musicTitle);

    CloseAudioDevice();

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void InitGame()
{

    coinCount = 0;
    // Initializing the player

    player.rec.x = 100;
    player.rec.y = (float)(screenHeight / 2 - 20);
    player.rec.width = UNIT;
    player.rec.height = UNIT + 1; // for collision with bottom enemies we need 1 extra px
    player.jumpCount = 0;
    player.isJumping = false;
    player.color = BLACK;
    player.vel = 480; // Calcualte and change accordingly to max height with elapsed time to that height
    player.jumpTime = 0.0f;
    player.framesCounter = 0;
    player.health = 3;
    player.orbCount = 0;
    player.ULT = false;
    player.attackReady = false;
    player.attacking = false;
    player.attackTimeCount = 0;

    // Initialize enemies
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        enemy[i].rec.width = 10;
        enemy[i].rec.height = UNIT;

        enemy[i].speed.x = 5;
        enemy[i].speed.y = 5;

        enemy[i].color = GRAY;
        enemy[i].collisionChecked = false;
        enemy[i].active = false;
    }

    // Initializing Coins
    for (int i = 0; i < MAX_COIN; ++i)
    {
        coin[i].radius = 7;
        coin[i].color = GREEN;
        coin[i].active = false;
    }

    // Initializing Hearts

    for (int i = 0; i < MAX_HEART; ++i)
    {
        heart[i].color = RED;
        heart[i].rec.width = UNIT;
        heart[i].rec.height = 1.5 * UNIT;
        heart[i].active = false;
    }

    // Initializing the obelisk

    obelisk.active = false;
    obelisk.activated = false;
    obelisk.rec.width = 2 * UNIT;
    obelisk.rec.height = 4 * UNIT;
    obelisk.rec.x = screenWidth + 2 * UNIT;
    obelisk.rec.y = screenHeight / 2 - 10 * UNIT;
    obelisk.color = BLACK;
    obelisk.spriteIndex = 0;

    // Initializing the attack Orb
    for (int i = 0; i < MAX_ATTACK_ORB; ++i)
    {
        attackOrb[i].active = false;
        attackOrb[i].rec.width = UNIT;
        attackOrb[i].rec.height = UNIT;
        attackOrb[i].color = PINK;
    }
}

void UpdateGame()
{

    if (player.health <= 0)
    {
        currentScreen = GAMEOVER;
    }

    if (player.isJumping)
    {

        if (IsKeyPressed(KEY_A) && player.jumpCount < 2)
        {
            if (player.jumpTime <= 1.0f)
            {
                player.jumpTime = 0;
                dubJumpHeight = 3 * UNIT;
            }
            else
            {
                dubJumpHeight = y;
                player.jumpTime = 0;
            }
            player.jumpCount++;

            // For drawing
            spriteIndex = 0;
            player.framesCounter = 0;
        }

        player.jumpTime += deltaTime;

        float y = (player.vel * player.jumpTime + 0.5f * GRAVITY * player.jumpTime * player.jumpTime) + dubJumpHeight;
        player.rec.y = (screenHeight / 2 - UNIT) - y;

        if (player.rec.y >= screenHeight / 2 - UNIT || IsKeyPressed(KEY_SPACE))
        {
            player.rec.y = screenHeight / 2 - UNIT;
            player.isJumping = false;
            player.jumpTime = 0.0f;
            player.jumpCount = 0;
            dubJumpHeight = 0;

            // For drawing
            if(player.attacking){
                spriteIndex = 9;
            }else
                spriteIndex = 0;
            player.framesCounter = 0;
        }
    }
    else
    {
        // The inverse movement & toggle
        if (IsKeyPressed(KEY_SPACE) && key)
        {
            player.rec.y = screenHeight / 2;
            key = !key;

            // For drawing
            if(player.attacking){
                spriteIndex = 15;
            }else{
                spriteIndex = 6;
            }
            player.framesCounter = 0;
        }
        else if (IsKeyPressed(KEY_SPACE) && !key)
        {
            player.rec.y = screenHeight / 2 - UNIT;
            key = !key;

            // For drawing
            if(player.attacking){
                spriteIndex = 9;
            }else
                spriteIndex = 0;
            player.framesCounter = 0;
        }
    }

    if (IsKeyPressed(KEY_A))
    {
        player.isJumping = true;
        player.jumpCount++;

        // For drawing
        spriteIndex = 0;
        player.framesCounter = 0;
    }

    // shield
    if (IsKeyPressed(KEY_S) && player.attackReady)
    {
        player.attackReady = false;
        player.attacking = true;
        player.attackTimeCount = 0;
        player.orbCount = 0;
        if(player.rec.y < screenHeight/2){
            spriteIndex = 9;
        }else{
            spriteIndex = 15;
        }
        spriteToggle = 0;
        player.framesCounter = 0;
    }

    // Enemies

    enemyTimeCounter += deltaTime;

    if (enemyTimeCounter >= maxEnemyDiff)
    {
        for (int i = 0; i < MAX_ENEMY; ++i)
        {
            if (!enemy[i].active)
            {
                enemy[i].active = true;
                enemy[i].collisionChecked = false;
                enemy[i].rec.x = screenWidth;
                int randHeight = GetRandomValue(1, 3);

                // enemy type for height 1 and below
                enemy[i].type = 2;

                int randDir = GetRandomValue(0, 1);
                if (randDir == 0)
                {
                    // Below
                    randDir = -1;
                    enemy[i].rec.y = screenHeight / 2 - 20 * randDir;
                }
                else
                {
                    // Above
                    enemy[i].rec.y = screenHeight / 2 - (randHeight) * 20 * randDir;
                    enemy[i].rec.height = UNIT; // To prevent collision from inverse run
                    // enemy type
                    if (randHeight != 1)
                    {
                        enemy[i].type = 1;
                        enemy[i].spriteIndex = 9;
                        enemy[i].rec.height = UNIT + 1; // To collide with 2nd lvl enemy
                    }
                }
                enemy[i].spriteIndex = 0;

                // Coin
                maxCoinDiff = GetRandomValue(2, 4); // Bringing randomness to coin occurence
                coinTimeCounter++;                  // coin after every 2 enemy
                if (coinTimeCounter >= maxCoinDiff)
                {
                    int fiveCounter = 0;
                    for (int j = 0; j < MAX_COIN; ++j)
                    {
                        if (!coin[j].active)
                        {
                            coin[j].active = true;

                            coin[j].origin.x = screenWidth + fiveCounter * 10 * (gameSpeed + 1) - 4 * UNIT;
                            if ((randHeight == 1 || randHeight == 2) && randDir != -1)
                            {
                                coin[j].origin.y = screenHeight / 2 - UNIT / 4 - (UNIT * fiveCounter * 2 - 0.5f * (0.5 * UNIT) * fiveCounter * fiveCounter);
                            }
                            else
                            {
                                coin[j].origin.y = screenHeight / 2 - UNIT / 2;
                            }

                            fiveCounter++;
                        }
                        if (fiveCounter == (int)ceil(9 * maxEnemyDiff))
                            break;
                    }
                    coinTimeCounter = 0;
                }
                break;
            }
        }
        enemyTimeCounter = 0;
    }

    for (int i = 0; i < 50; i++)
    {
        if (enemy[i].active)
        {
            enemy[i].rec.x -= (enemy[i].speed.x + gameSpeed);

            if (enemy[i].rec.x < 0)
            {
                enemy[i].active = false;
            }
        }
    }

    // Coin

    for (int i = 0; i < MAX_COIN; ++i)
    {
        if (coin[i].active)
        {

            coin[i].origin.x -= (5 + gameSpeed);

            if (coin[i].origin.x < 0)
            {
                coin[i].active = false;
                coin[i].spriteIndex = 0;
            }
        }
    }

    // Heart

    heartTimeCounter += deltaTime;

    if (heartTimeCounter >= maxHeartDiff && heartCount <= MAX_HEART)
    {
        for (int i = 0; i < MAX_HEART; ++i)
        {
            if (!heart[i].active)
            {

                heart[i].active = true;
                heart[i].rec.x = screenWidth;
                heart[i].rec.y = screenHeight / 2 - 5 * UNIT;
                heart[i].spriteIndex = 0;

                heartCount++;
                break;
            }
        }
        heartTimeCounter = 0;
    }

    if (heartCount <= MAX_HEART)
    {
        for (int i = 0; i < MAX_HEART; ++i)
        {
            if (heart[i].active)
            {
                heart[i].rec.x -= (5 + gameSpeed);

                if (heart[i].rec.x < 0)
                {
                    heart[i].active = false;
                }
            }
        }
    }

    // ULT or Obelisk
    if (obeliskTimeCounter < maxObeliskTime)
    {
        obeliskTimeCounter += deltaTime;
    }
    else if (!obelisk.activated)
    {
        obelisk.active = true;
        obelisk.activated = true;
    }

    if (obelisk.active)
    {
        obelisk.rec.x -= (5 + gameSpeed);

        if (obelisk.rec.x < 0)
        {
            obelisk.active = false;
        }
    }

    // Attack Orb
    attackOrbTimeCounter += deltaTime;
    if (attackOrbTimeCounter >= maxAttOrbTime)
    {
        for (int i = 0; i < MAX_ATTACK_ORB; ++i)
        {
            if (!attackOrb[i].active)
            {
                attackOrb[i].active = true;

                attackOrb[i].rec.x = screenWidth;
                attackOrb[i].rec.y = screenHeight / 2 - 6 * UNIT;
                attackOrb[i].spriteIndex = 0;
                break;
            }
        }
        attackOrbTimeCounter = 0;
    }

    for (int i = 0; i < MAX_ATTACK_ORB; ++i)
    {
        if (attackOrb[i].active)
        {
            attackOrb[i].rec.x -= (5 + gameSpeed);

            if (attackOrb[i].rec.x < 0)
            {
                attackOrb[i].active = false;
                attackOrb[i].spriteIndex = 0;
            }
        }
    }

    // // Collision

    // Enemy collision

    if(!player.attacking){
        for (int i = 0; i < MAX_ENEMY; ++i)
        {
            if (CheckCollisionRecs(player.rec, enemy[i].rec) && !enemy[i].collisionChecked)
            {
                player.health -= 0.5;
                enemy[i].collisionChecked = true;
            }
        }
    }

    // Coin collision

    for (int i = 0; i < MAX_COIN; ++i)
    {
        if (CheckCollisionCircleRec(coin[i].origin, coin[i].radius, player.rec) && coin[i].active)
        {
            coinCount++;
            coin[i].active = false;
            coin[i].spriteIndex = 0;
        }
    }

    // Heart Collision

    for (int i = 0; i < MAX_HEART; ++i)
    {
        if (CheckCollisionRecs(player.rec, heart[i].rec) && heart[i].active)
        {
            player.health += 1;
            heart[i].active = false;
        }
    }

    // Obelisk collision
    if (CheckCollisionRecs(player.rec, obelisk.rec) && obelisk.active)
    {
        player.ULT = true;
        obelisk.active = false;
    }

    // AttackOrb Collision
    if(!player.attacking){
        for (int i = 0; i < MAX_ATTACK_ORB && player.orbCount < MAX_REQ_ATT_ORB; ++i)
    {
        if (CheckCollisionRecs(player.rec, attackOrb[i].rec) && attackOrb[i].active)
        {
            attackOrb[i].active = false;
            player.orbCount++;
        }
        if(player.orbCount == MAX_REQ_ATT_ORB){
            player.attackReady = true;
        }
    }
    }
    // Shield time counter
    if(player.attacking){
        player.attackTimeCount += deltaTime;
        if(player.attackTimeCount >= maxAttackTime){
            player.attacking = false;
            spriteToggle = 1;
            player.framesCounter = 0;
            player.attackTimeCount = 0;
        }
    }
}
void DrawGame()
{

    // Some lines just for visuals
    DrawLineEx((Vector2){0, screenHeight / 2}, (Vector2){screenWidth, screenHeight / 2}, 2.0f, BLACK);
    // DrawLineEx((Vector2){0, screenHeight/2 - 20}, (Vector2){screenWidth, screenHeight/2 - 20}, 2.0f, BLACK);
    // DrawLineEx((Vector2){0, screenHeight/2 - 40}, (Vector2){screenWidth, screenHeight/2 - 40}, 2.0f, BLACK);
    // DrawLineEx((Vector2){0, screenHeight/2 - 60}, (Vector2){screenWidth, screenHeight/2 - 60}, 2.0f, BLACK);
    // DrawLineEx((Vector2){0, screenHeight/2 - 80}, (Vector2){screenWidth, screenHeight/2 - 80}, 2.0f, BLACK);
    // DrawLineEx((Vector2){0, screenHeight/2 - 100}, (Vector2){screenWidth, screenHeight/2 - 100}, 2.0f, BLACK);

    // Drawing Texts
    DrawText(TextFormat("PLAYER HEALTH: %.1f", player.health), UNIT, UNIT, UNIT, BLACK);
    DrawText(TextFormat("COIN COUNT : %d", coinCount), UNIT, UNIT * 3, UNIT, BLACK);
    DrawText(TextFormat("ULT : %s", (player.ULT) ? "Ready" : "NOT Ready"), screenWidth / 2, UNIT, UNIT, BLACK);
    DrawText(TextFormat("ORB COUNT : %d", player.orbCount), screenWidth / 2 - 5 * UNIT, UNIT * 3, UNIT, BLACK);
    DrawText(TextFormat("ATTACK : %s", (player.orbCount == MAX_REQ_ATT_ORB) ? "Ready" : "NOT Ready"), screenWidth * 0.65, UNIT * 3, UNIT, BLACK);

    // Drawing player
    // DrawRectangle(player.rec.x, player.rec.y, player.rec.width, player.rec.height, player.color);

    if (!player.attacking)
    {
        if (!player.isJumping)
        {
            if (player.rec.y < screenHeight / 2)
            {
                player.framesCounter++;
                if (player.framesCounter >= ANIMATION_SPEED)
                {
                    spriteIndex++;
                    if (spriteIndex >= 6)
                    {
                        spriteIndex = 0;
                    }
                    player.framesCounter = 0;
                }
                spriteToggle = 1;
                DrawTexturePro(playerSprite, (Rectangle){spriteIndex * _spritesWidth, spriteToggle * 80, _spritesWidth, _spritesHeight}, (Rectangle){player.rec.x - 32, player.rec.y - 44, _spritesWidth, _spritesHeight}, (Vector2){0, 0}, 0, WHITE);
            }
            else
            {
                player.framesCounter++;
                if (player.framesCounter >= ANIMATION_SPEED)
                {
                    spriteIndex++;
                    if (spriteIndex >= 12)
                    {
                        spriteIndex = 6;
                    }
                    player.framesCounter = 0;
                }
                spriteToggle = 1;
                DrawTexturePro(playerSprite, (Rectangle){spriteIndex * _spritesWidth, spriteToggle * 80, _spritesWidth, _spritesHeight}, (Rectangle){player.rec.x - 32, player.rec.y - 36, _spritesWidth, _spritesHeight}, (Vector2){0, 0}, 0, WHITE);
            }
        }
        else
        {
            spriteToggle = 1;
            DrawTexturePro(playerSprite, (Rectangle){4 * _spritesWidth, spriteToggle * 80, _spritesWidth, _spritesHeight}, (Rectangle){player.rec.x - 32, player.rec.y - 44, _spritesWidth, _spritesHeight}, (Vector2){0, 0}, 0, WHITE);
        }
    }
    else
    {
        if (!player.isJumping)
        {
            if (player.rec.y < screenHeight / 2)
            {
                player.framesCounter++;
                if (player.framesCounter >= ANIMATION_SPEED)
                {
                    spriteIndex++;
                    if (spriteIndex >= 15)
                    {
                        spriteIndex = 9;
                    }
                    player.framesCounter = 0;
                }
                spriteToggle = 0;
                DrawTexturePro(playerSprite, (Rectangle){spriteIndex * _spritesWidth, spriteToggle * 80, _spritesWidth, _spritesHeight}, (Rectangle){player.rec.x - 32, player.rec.y - 44, _spritesWidth, _spritesHeight}, (Vector2){0, 0}, 0, WHITE);
            }
            else
            {
                player.framesCounter++;
                if (player.framesCounter >= ANIMATION_SPEED)
                {
                    spriteIndex++;
                    if (spriteIndex >= 20)
                    {
                        spriteIndex = 15;
                    }
                    player.framesCounter = 0;
                }
                spriteToggle = 0;
                DrawTexturePro(playerSprite, (Rectangle){spriteIndex * _spritesWidth, spriteToggle * 80, _spritesWidth, _spritesHeight}, (Rectangle){player.rec.x - 32, player.rec.y - 36, _spritesWidth, _spritesHeight}, (Vector2){0, 0}, 0, WHITE);
            }
        }
        else
        {
            spriteToggle = 0;
            DrawTexturePro(playerSprite, (Rectangle){12 * _spritesWidth, spriteToggle * 80, _spritesWidth, _spritesHeight}, (Rectangle){player.rec.x - 32, player.rec.y - 44, _spritesWidth, _spritesHeight}, (Vector2){0, 0}, 0, WHITE);
        }
    }

    // Drawing enemy
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (enemy[i].active)
        {
            enemy[i].framesCounter++;

            // DrawRectangleRec(enemy[i].rec, enemy[i].color);
            if (enemy[i].type == 1)
            {
                if (enemy[i].framesCounter >= ANIMATION_SPEED)
                {
                    enemy[i].spriteIndex--;
                    if (enemy[i].spriteIndex < 0)
                    {
                        enemy[i].spriteIndex = 9;
                    }
                    enemy[i].framesCounter = 0;
                }
                DrawTexturePro(enemyType1, (Rectangle){enemy[i].spriteIndex * _eneT1SpritesWidth, 0, _eneT1SpritesWidth, _eneT1SpritesHeight}, (Rectangle){enemy[i].rec.x - 15, enemy[i].rec.y - 15, _eneT1SpritesWidth * 1.5, _eneT1SpritesHeight * 1.5}, (Vector2){0, 0}, 0, WHITE);
            }
            else
            {
                if (enemy[i].framesCounter >= ANIMATION_SPEED)
                {
                    enemy[i].spriteIndex++;
                    if (enemy[i].spriteIndex >= 10)
                    {
                        enemy[i].spriteIndex = 0;
                    }
                    enemy[i].framesCounter = 0;
                }
                if (enemy[i].rec.y >= screenHeight / 2)
                {
                    DrawTexturePro(enemyType2, (Rectangle){0, _eneT2SpritesHeight, _eneT2SpritesWidth, _eneT2SpritesHeight}, (Rectangle){enemy[i].rec.x - 15, enemy[i].rec.y - UNIT, _eneT2SpritesWidth, _eneT2SpritesHeight / 1.5}, (Vector2){0, 0}, 0, WHITE);
                    DrawTexturePro(enemyType2, (Rectangle){enemy[i].spriteIndex * _eneT2SpritesWidth, _eneT2SpritesHeight, _eneT2SpritesWidth, _eneT2SpritesHeight}, (Rectangle){enemy[i].rec.x - 15, enemy[i].rec.y - UNIT, _eneT2SpritesWidth, _eneT2SpritesHeight / 1.5}, (Vector2){0, 0}, 0, WHITE);
                }
                else
                {
                    DrawTexturePro(enemyType2, (Rectangle){0, 0, _eneT2SpritesWidth, _eneT2SpritesHeight}, (Rectangle){enemy[i].rec.x - 15, enemy[i].rec.y - UNIT - 3, _eneT2SpritesWidth, _eneT2SpritesHeight / 1.5}, (Vector2){0, 0}, 0, WHITE);
                    DrawTexturePro(enemyType2, (Rectangle){enemy[i].spriteIndex * _eneT2SpritesWidth, 0, _eneT2SpritesWidth, _eneT2SpritesHeight}, (Rectangle){enemy[i].rec.x - 15, enemy[i].rec.y - UNIT, _eneT2SpritesWidth, _eneT2SpritesHeight / 1.5}, (Vector2){0, 0}, 0, WHITE);
                }
            }
        }
    }

    // Drawing coins

    for (int i = 0; i < MAX_COIN; ++i)
    {

        if (coin[i].active)
        {

            DrawTexturePro(coins_Hearts, (Rectangle){63 + coinSpriteWidth * coin[i].spriteIndex, 32, coinSpriteWidth, coinSpriteHeight}, (Rectangle){coin[i].origin.x - coinSpriteWidth / 2, coin[i].origin.y - coinSpriteHeight / 2, coinSpriteWidth, coinSpriteHeight}, (Vector2){0, 0}, 0, WHITE);

            coin[i].framesCounter++;
            if (coin[i].framesCounter >= ANIMATION_SPEED)
            {
                coin[i].spriteIndex++;
                if (coin[i].spriteIndex >= 6)
                {
                    coin[i].spriteIndex = 0;
                }
                coin[i].framesCounter = 0;
            }
            // DrawCircle(coin[i].origin.x, coin[i].origin.y, coin[i].radius, coin[i].color);
        }
    }

    // Drawing Heart

    for (int i = 0; i < MAX_HEART; ++i)
    {
        if (heart[i].active)
        {
            // DrawRectangle(heart[i].rec.x, heart[i].rec.y, heart[i].rec.width, heart[i].rec.height, heart[i].color);

            DrawTexturePro(coins_Hearts, (Rectangle){528 + heart[i].spriteIndex * heartSpriteWidth, 240, heartSpriteWidth, heartSpriteHeight}, (Rectangle){heart[i].rec.x - heartSpriteWidth / 1.75, heart[i].rec.y - 20, heartSpriteWidth * 2.5, heartSpriteHeight * 2.5}, (Vector2){0, 0}, 0, WHITE);
            heart[i].framesCounter++;

            if (heart[i].framesCounter >= ANIMATION_SPEED)
            {
                heart[i].spriteIndex++;

                if (heart[i].spriteIndex >= 4)
                {
                    heart[i].spriteIndex = 0;
                }
                heart[i].framesCounter = 0;
            }
        }
    }

    // Drawing Obelisk
    if (obelisk.active)
    {
        // DrawRectangle(obelisk.rec.x, obelisk.rec.y, obelisk.rec.width, obelisk.rec.height, obelisk.color);

        DrawTexturePro(obeliskTexture, (Rectangle){obelisk.spriteIndex * obeliskWidth, 0, obeliskWidth, obelikHeight}, (Rectangle){obelisk.rec.x, obelisk.rec.y, obelisk.rec.width, obelisk.rec.height}, (Vector2){0, 0}, 0, WHITE);
        obelisk.framesCounter++;

        if (obelisk.framesCounter >= ANIMATION_SPEED)
        {
            obelisk.spriteIndex++;

            if (obelisk.spriteIndex >= 14)
            {
                obelisk.spriteIndex = 0;
            }
        }
    }

    // Drawing the attackOrb

    for (int i = 0; i < MAX_ATTACK_ORB; ++i)
    {
        if (attackOrb[i].active)
        {
            // DrawRectangle(attackOrb[i].rec.x, attackOrb[i].rec.y, attackOrb[i].rec.width, attackOrb[i].rec.height, attackOrb[i].color);

            DrawTexturePro(coins_Hearts, (Rectangle){528 + attackOrb[i].spriteIndex * heartSpriteWidth, 208, heartSpriteWidth, heartSpriteHeight}, (Rectangle){attackOrb[i].rec.x - heartSpriteWidth / 1.75, attackOrb[i].rec.y - 20, heartSpriteWidth * 2.5, heartSpriteHeight * 2.5}, (Vector2){0, 0}, 0, WHITE);
            attackOrb[i].framesCounter++;

            if (attackOrb[i].framesCounter >= ANIMATION_SPEED)
            {
                attackOrb[i].spriteIndex++;

                if (attackOrb[i].spriteIndex >= 4)
                {
                    attackOrb[i].spriteIndex = 0;
                }
                attackOrb[i].framesCounter = 0;
            }
        }
    }
}
