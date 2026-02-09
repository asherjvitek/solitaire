#include "raylib.h"
#include "nob.h"
#include <stdio.h>


#define CARD_HEIGHT 150
#define CARD_WIDTH 107

#define SPACING 20.0

enum Suit { 
    SPADE,
    HEART,
    CLUB,
    DIAMOND
};

typedef struct {
    int id;
    enum Suit suit;
    int number;
    Vector2 pos;
} Card;

char *images[52] = {
    "./SVG-cards/png/2x/heart_1.png",
    "./SVG-cards/png/2x/heart_2.png",
    "./SVG-cards/png/2x/heart_3.png",
    "./SVG-cards/png/2x/heart_4.png",
    "./SVG-cards/png/2x/heart_5.png",
    "./SVG-cards/png/2x/heart_6.png",
    "./SVG-cards/png/2x/heart_7.png",
    "./SVG-cards/png/2x/heart_8.png",
    "./SVG-cards/png/2x/heart_9.png",
    "./SVG-cards/png/2x/heart_10.png",
    "./SVG-cards/png/2x/heart_jack.png",
    "./SVG-cards/png/2x/heart_queen.png",
    "./SVG-cards/png/2x/heart_king.png",

    "./SVG-cards/png/2x/diamond_1.png",
    "./SVG-cards/png/2x/diamond_2.png",
    "./SVG-cards/png/2x/diamond_3.png",
    "./SVG-cards/png/2x/diamond_4.png",
    "./SVG-cards/png/2x/diamond_5.png",
    "./SVG-cards/png/2x/diamond_6.png",
    "./SVG-cards/png/2x/diamond_7.png",
    "./SVG-cards/png/2x/diamond_8.png",
    "./SVG-cards/png/2x/diamond_9.png",
    "./SVG-cards/png/2x/diamond_10.png",
    "./SVG-cards/png/2x/diamond_jack.png",
    "./SVG-cards/png/2x/diamond_queen.png",
    "./SVG-cards/png/2x/diamond_king.png",

    "./SVG-cards/png/2x/spade_1.png",
    "./SVG-cards/png/2x/spade_2.png",
    "./SVG-cards/png/2x/spade_3.png",
    "./SVG-cards/png/2x/spade_4.png",
    "./SVG-cards/png/2x/spade_5.png",
    "./SVG-cards/png/2x/spade_6.png",
    "./SVG-cards/png/2x/spade_7.png",
    "./SVG-cards/png/2x/spade_8.png",
    "./SVG-cards/png/2x/spade_9.png",
    "./SVG-cards/png/2x/spade_10.png",
    "./SVG-cards/png/2x/spade_jack.png",
    "./SVG-cards/png/2x/spade_queen.png",
    "./SVG-cards/png/2x/spade_king.png",

    "./SVG-cards/png/2x/club_1.png",
    "./SVG-cards/png/2x/club_2.png",
    "./SVG-cards/png/2x/club_3.png",
    "./SVG-cards/png/2x/club_4.png",
    "./SVG-cards/png/2x/club_5.png",
    "./SVG-cards/png/2x/club_6.png",
    "./SVG-cards/png/2x/club_7.png",
    "./SVG-cards/png/2x/club_8.png",
    "./SVG-cards/png/2x/club_9.png",
    "./SVG-cards/png/2x/club_10.png",
    "./SVG-cards/png/2x/club_jack.png",
    "./SVG-cards/png/2x/club_queen.png",
    "./SVG-cards/png/2x/club_king.png",
};

Texture2D cardBack = {0};
Texture2D cardFronts[52] = {0};
Card cards[52] = {
    { 0, HEART, 1, {0}, },
    { 1, HEART, 2, {0}, },
    { 2, HEART, 3, {0}, },
    { 3, HEART, 4, {0}, },
    { 4, HEART, 5, {0}, },
    { 5, HEART, 6, {0}, },
    { 6, HEART, 7, {0}, },
    { 7, HEART, 8, {0}, },
    { 8, HEART, 9, {0}, },
    { 9, HEART, 10, {0}, },
    { 10, HEART, 11, {0}, },
    { 11, HEART, 12, {0}, },
    { 12, HEART, 13, {0}, },
    { 13, DIAMOND, 1, {0}, },
    { 14, DIAMOND, 2, {0}, },
    { 15, DIAMOND, 3, {0}, },
    { 16, DIAMOND, 4, {0}, },
    { 17, DIAMOND, 5, {0}, },
    { 18, DIAMOND, 6, {0}, },
    { 19, DIAMOND, 7, {0}, },
    { 20, DIAMOND, 8, {0}, },
    { 21, DIAMOND, 9, {0}, },
    { 22, DIAMOND, 10, {0}, },
    { 23, DIAMOND, 11, {0}, },
    { 24, DIAMOND, 12, {0}, },
    { 25, DIAMOND, 13, {0}, },
    { 26, SPADE, 1, {0}, },
    { 27, SPADE, 2, {0}, },
    { 28, SPADE, 3, {0}, },
    { 29, SPADE, 4, {0}, },
    { 30, SPADE, 5, {0}, },
    { 31, SPADE, 6, {0}, },
    { 32, SPADE, 7, {0}, },
    { 33, SPADE, 8, {0}, },
    { 34, SPADE, 9, {0}, },
    { 35, SPADE, 10, {0}, },
    { 36, SPADE, 11, {0}, },
    { 37, SPADE, 12, {0}, },
    { 38, SPADE, 13, {0}, },
    { 39, CLUB, 1, {0}, },
    { 40, CLUB, 2, {0}, },
    { 41, CLUB, 3, {0}, },
    { 42, CLUB, 4, {0}, },
    { 43, CLUB, 5, {0}, },
    { 44, CLUB, 6, {0}, },
    { 45, CLUB, 7, {0}, },
    { 46, CLUB, 8, {0}, },
    { 47, CLUB, 9, {0}, },
    { 48, CLUB, 10, {0}, },
    { 49, CLUB, 11, {0}, },
    { 50, CLUB, 12, {0}, },
    { 51, CLUB, 13, {0}, },
};

typedef struct {
    int *items;
    int count;
    int capacity;
} Pile;

Pile draw = {0};
Pile show = {0};
Pile completed[4] = {0};
Pile play[7] = {0};

void LoadCards() {
    for (int i = 0; i < 52; i++) {
        Image img = LoadImage(images[i]);
        ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
        cardFronts[i] = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    Image img = LoadImage("./SVG-cards/png/2x/back-red.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    cardBack = LoadTextureFromImage(img);
    UnloadImage(img);
} 

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, "Solitaire");

    SetTraceLogLevel(LOG_ERROR); 

    LoadCards();

    SetTargetFPS(60);
    int frameCount = 0;

    bool mouseDown = false;
    Vector2 offset = {0};

    while(!WindowShouldClose()) {
        frameCount++;

        // Rectangle rect = { card.pos.x, card.pos.y, CARD_WIDTH, CARD_HEIGHT };
        // Vector2 mousePos = GetMousePosition();
        //
        // if (!mouseDown && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, rect)) {
        //     mouseDown = true;
        //     offset.x = mousePos.x - card.pos.x;
        //     offset.y = mousePos.y - card.pos.y;
        // }
        //
        // if (mouseDown) {
        //     card.pos.x = mousePos.x - offset.x;
        //     card.pos.y = mousePos.y - offset.y;
        // }
        //
        // if (mouseDown && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) 
        // {
        //     mouseDown = false;
        // }
        //
        BeginDrawing();

        ClearBackground(GREEN);
        Vector2 thing = { SPACING, SPACING };

        DrawTextureEx(cardBack, thing, 0, 1, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
