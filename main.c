#include "raylib.h"
#include "nob.h"
#include <stdio.h>
#include <stdlib.h>


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
    char *imagePath;
    Texture2D texture;
    bool visible;
} Card;

typedef struct {
    int *items;
    int count;
    int capacity;
} Pile;

Texture2D cardBack = {0};
Card cards[52] = {
    { 0, HEART, 1, {0}, "./SVG-cards/png/2x/heart_1.png", {0} },
    { 1, HEART, 2, {0}, "./SVG-cards/png/2x/heart_2.png", {0} },
    { 2, HEART, 3, {0}, "./SVG-cards/png/2x/heart_3.png", {0} },
    { 3, HEART, 4, {0}, "./SVG-cards/png/2x/heart_4.png", {0} },
    { 4, HEART, 5, {0}, "./SVG-cards/png/2x/heart_5.png", {0} },
    { 5, HEART, 6, {0}, "./SVG-cards/png/2x/heart_6.png", {0} },
    { 6, HEART, 7, {0}, "./SVG-cards/png/2x/heart_7.png", {0} },
    { 7, HEART, 8, {0}, "./SVG-cards/png/2x/heart_8.png", {0} },
    { 8, HEART, 9, {0}, "./SVG-cards/png/2x/heart_9.png", {0} },
    { 9, HEART, 10, {0}, "./SVG-cards/png/2x/heart_10.png", {0} },
    { 10, HEART, 11, {0}, "./SVG-cards/png/2x/heart_jack.png", {0} },
    { 11, HEART, 12, {0}, "./SVG-cards/png/2x/heart_queen.png", {0} },
    { 12, HEART, 13, {0}, "./SVG-cards/png/2x/heart_king.png", {0} },
    { 13, DIAMOND, 1, {0}, "./SVG-cards/png/2x/diamond_1.png", {0} },
    { 14, DIAMOND, 2, {0}, "./SVG-cards/png/2x/diamond_2.png", {0} },
    { 15, DIAMOND, 3, {0}, "./SVG-cards/png/2x/diamond_3.png", {0} },
    { 16, DIAMOND, 4, {0}, "./SVG-cards/png/2x/diamond_4.png", {0} },
    { 17, DIAMOND, 5, {0}, "./SVG-cards/png/2x/diamond_5.png", {0} },
    { 18, DIAMOND, 6, {0}, "./SVG-cards/png/2x/diamond_6.png", {0} },
    { 19, DIAMOND, 7, {0}, "./SVG-cards/png/2x/diamond_7.png", {0} },
    { 20, DIAMOND, 8, {0}, "./SVG-cards/png/2x/diamond_8.png", {0} },
    { 21, DIAMOND, 9, {0}, "./SVG-cards/png/2x/diamond_9.png", {0} },
    { 22, DIAMOND, 10, {0}, "./SVG-cards/png/2x/diamond_10.png", {0} },
    { 23, DIAMOND, 11, {0}, "./SVG-cards/png/2x/diamond_jack.png", {0} },
    { 24, DIAMOND, 12, {0}, "./SVG-cards/png/2x/diamond_queen.png", {0} },
    { 25, DIAMOND, 13, {0}, "./SVG-cards/png/2x/diamond_king.png", {0} },
    { 26, SPADE, 1, {0}, "./SVG-cards/png/2x/spade_1.png", {0} },
    { 27, SPADE, 2, {0}, "./SVG-cards/png/2x/spade_2.png", {0} },
    { 28, SPADE, 3, {0}, "./SVG-cards/png/2x/spade_3.png", {0} },
    { 29, SPADE, 4, {0}, "./SVG-cards/png/2x/spade_4.png", {0} },
    { 30, SPADE, 5, {0}, "./SVG-cards/png/2x/spade_5.png", {0} },
    { 31, SPADE, 6, {0}, "./SVG-cards/png/2x/spade_6.png", {0} },
    { 32, SPADE, 7, {0}, "./SVG-cards/png/2x/spade_7.png", {0} },
    { 33, SPADE, 8, {0}, "./SVG-cards/png/2x/spade_8.png", {0} },
    { 34, SPADE, 9, {0}, "./SVG-cards/png/2x/spade_9.png", {0} },
    { 35, SPADE, 10, {0}, "./SVG-cards/png/2x/spade_10.png", {0} },
    { 36, SPADE, 11, {0}, "./SVG-cards/png/2x/spade_jack.png", {0} },
    { 37, SPADE, 12, {0}, "./SVG-cards/png/2x/spade_queen.png", {0} },
    { 38, SPADE, 13, {0}, "./SVG-cards/png/2x/spade_king.png", {0} },
    { 39, CLUB, 1, {0}, "./SVG-cards/png/2x/club_1.png", {0} },
    { 40, CLUB, 2, {0}, "./SVG-cards/png/2x/club_2.png", {0} },
    { 41, CLUB, 3, {0}, "./SVG-cards/png/2x/club_3.png", {0} },
    { 42, CLUB, 4, {0}, "./SVG-cards/png/2x/club_4.png", {0} },
    { 43, CLUB, 5, {0}, "./SVG-cards/png/2x/club_5.png", {0} },
    { 44, CLUB, 6, {0}, "./SVG-cards/png/2x/club_6.png", {0} },
    { 45, CLUB, 7, {0}, "./SVG-cards/png/2x/club_7.png", {0} },
    { 46, CLUB, 8, {0}, "./SVG-cards/png/2x/club_8.png", {0} },
    { 47, CLUB, 9, {0}, "./SVG-cards/png/2x/club_9.png", {0} },
    { 48, CLUB, 10, {0}, "./SVG-cards/png/2x/club_10.png", {0} },
    { 49, CLUB, 11, {0}, "./SVG-cards/png/2x/club_jack.png", {0} },
    { 50, CLUB, 12, {0}, "./SVG-cards/png/2x/club_queen.png", {0} },
    { 51, CLUB, 13, {0}, "./SVG-cards/png/2x/club_king.png", {0} },
};

Pile draw = {0};
Pile show = {0};
Pile completed[4] = {0};
Pile play[7] = {0};
int grabbedCard = -1;

void LoadCards() {
    for (int i = 0; i < 52; i++) {
        Image img = LoadImage(cards[i].imagePath);
        ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
        cards[i].texture = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    Image img = LoadImage("./SVG-cards/png/2x/back-red.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    cardBack = LoadTextureFromImage(img);
    UnloadImage(img);
} 

void GameBoardInit() {
    Pile shuffle = {0};
    da_reserve(&shuffle, 52);

    for (int i = 0; i < 52; i++) {
        da_append(&shuffle, cards[i].id);
    }

    int limit = 1;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < limit; j++) {
            int index = (rand() % shuffle.count);
            int choice = shuffle.items[index];
            da_append(&play[i], choice);
            nob_da_remove_unordered(&shuffle, index);

            if (j + 1 == limit) {
                cards[index].visible = true;
            }
        }
        limit++;
    }

    printf("Did we get here?\n");

    for (int i = shuffle.count - 1; i >= 0; i--) {
        int index = (rand() % shuffle.count);
        da_append(&draw, index);
        nob_da_remove_unordered(&shuffle, index);
    }

    printf("Draw Pile:\n");
    da_foreach(int, x, &draw) {
        printf("%d\n", *x);
    }

    for (int i = 0; i < 7; i++) {
        Pile p = play[i];
        printf("p.count = %d\n", p.count);
        for (int j = 0; j < p.count; j++) {
            printf("play %d, card %d\n", i, p.items[j]);
        }
        printf("\n");
    }

    printf("Did we get here?\n");
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, "Solitaire");
    SetTraceLogLevel(LOG_ERROR); 
    SetTargetFPS(60);

    LoadCards();
    GameBoardInit();

    int frameCount = 0;

    bool mouseDown = false;
    Vector2 offset = {0};

    while(!WindowShouldClose()) {
        frameCount++;

        if (MouseButtonIsPressed(KEY_0)) {


        }

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

        if (draw.count > 0) {
            DrawTextureV(cardBack, thing, RAYWHITE);
        }

        int x = SPACING * 2 + CARD_WIDTH;
        int y = SPACING * 2 + CARD_HEIGHT;

        Vector2 playStart = { x, y };

        for (int i = 0; i < 7; i++) {
            Vector2 pos = { playStart.x * (i + 1), playStart.y };

            da_foreach(int, cardIndex, &play[i]) {
                Card card = cards[*cardIndex];
                card.pos = pos;
                if (card.visible) {
                    DrawTextureV(card.texture, card.pos, RAYWHITE);
                } else {
                    DrawTextureV(cardBack, card.pos, RAYWHITE);
                }

                pos.y += SPACING;
            }

            pos.x += SPACING + CARD_WIDTH;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
