#include "raylib.h"
#include "nob.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// TODOS:
// 1. move all the layout logic and positioning to somewhere upfont for setting the card positions
// 2. make some common code for setting card positions
// 3. We should have something for figuring out the card position that is not relative to another card
//  right now it is not possible to play cards on open spaces

#define DEBUG true
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
    Vector2 origPos;
    Vector2 offset;
    char *imagePath;
    Texture2D texture;
    bool visible;
} Card;

typedef struct {
    Card **items;
    int count;
    int capacity;
} Pile;

#define new_card(id, suit, number, image) { id, suit, number, {0}, {0}, {0}, "./SVG-cards/png/2x/" image, {0} }

bool containsCard(Pile *pile, Card *card) {
    da_foreach(Card*, c, pile) {
        if (card == *c) return true;
    }
    return false;
}

Texture2D cardBack = {0};
Card cards[52] = {
    new_card(0, HEART, 1, "heart_1.png"),
    new_card(1, HEART, 2, "heart_2.png"),
    new_card(2, HEART, 3, "heart_3.png"),
    new_card(3, HEART, 4, "heart_4.png"),
    new_card(4, HEART, 5, "heart_5.png"),
    new_card(5, HEART, 6, "heart_6.png"),
    new_card(6, HEART, 7, "heart_7.png"),
    new_card(7, HEART, 8, "heart_8.png"),
    new_card(8, HEART, 9, "heart_9.png"),
    new_card(9, HEART, 10, "heart_10.png"),
    new_card(10, HEART, 11, "heart_jack.png"),
    new_card(11, HEART, 12, "heart_queen.png"),
    new_card(12, HEART, 13, "heart_king.png"),
    new_card(13, DIAMOND, 1, "diamond_1.png"),
    new_card(14, DIAMOND, 2, "diamond_2.png"),
    new_card(15, DIAMOND, 3, "diamond_3.png"),
    new_card(16, DIAMOND, 4, "diamond_4.png"),
    new_card(17, DIAMOND, 5, "diamond_5.png"),
    new_card(18, DIAMOND, 6, "diamond_6.png"),
    new_card(19, DIAMOND, 7, "diamond_7.png"),
    new_card(20, DIAMOND, 8, "diamond_8.png"),
    new_card(21, DIAMOND, 9, "diamond_9.png"),
    new_card(22, DIAMOND, 10, "diamond_10.png"),
    new_card(23, DIAMOND, 11, "diamond_jack.png"),
    new_card(24, DIAMOND, 12, "diamond_queen.png"),
    new_card(25, DIAMOND, 13, "diamond_king.png"),
    new_card(26, SPADE, 1, "spade_1.png"),
    new_card(27, SPADE, 2, "spade_2.png"),
    new_card(28, SPADE, 3, "spade_3.png"),
    new_card(29, SPADE, 4, "spade_4.png"),
    new_card(30, SPADE, 5, "spade_5.png"),
    new_card(31, SPADE, 6, "spade_6.png"),
    new_card(32, SPADE, 7, "spade_7.png"),
    new_card(33, SPADE, 8, "spade_8.png"),
    new_card(34, SPADE, 9, "spade_9.png"),
    new_card(35, SPADE, 10, "spade_10.png"),
    new_card(36, SPADE, 11, "spade_jack.png"),
    new_card(37, SPADE, 12, "spade_queen.png"),
    new_card(38, SPADE, 13, "spade_king.png"),
    new_card(39, CLUB, 1, "club_1.png"),
    new_card(40, CLUB, 2, "club_2.png"),
    new_card(41, CLUB, 3, "club_3.png"),
    new_card(42, CLUB, 4, "club_4.png"),
    new_card(43, CLUB, 5, "club_5.png"),
    new_card(44, CLUB, 6, "club_6.png"),
    new_card(45, CLUB, 7, "club_7.png"),
    new_card(46, CLUB, 8, "club_8.png"),
    new_card(47, CLUB, 9, "club_9.png"),
    new_card(48, CLUB, 10, "club_10.png"),
    new_card(49, CLUB, 11, "club_jack.png"),
    new_card(50, CLUB, 12, "club_queen.png"),
    new_card(51, CLUB, 13, "club_king.png"),
};

Pile stock = {0};
Pile waste = {0};
Pile foundation[4] = {0};
Pile play[7] = {0};
Pile grabbed = {0};
Pile *grabbedSource = NULL;
bool mouseDown = false;

Vector2 stockPos = { SPACING, SPACING };
Vector2 wastePos = { CARD_WIDTH + SPACING * 2, SPACING };
Vector2 playPos[7] = { 
    { CARD_WIDTH + SPACING * 2, CARD_HEIGHT + SPACING },
    { (CARD_WIDTH + SPACING * 2) * 2, CARD_HEIGHT + SPACING },
    { (CARD_WIDTH + SPACING * 2) * 3, CARD_HEIGHT + SPACING },
    { (CARD_WIDTH + SPACING * 2) * 4, CARD_HEIGHT + SPACING },
    { (CARD_WIDTH + SPACING * 2) * 5, CARD_HEIGHT + SPACING },
    { (CARD_WIDTH + SPACING * 2) * 6, CARD_HEIGHT + SPACING },
};

Vector2 foundationPos[4] = { 
    { CARD_WIDTH + SPACING * 2, SPACING },
    { (CARD_WIDTH + SPACING * 2) * 2, SPACING },
    { (CARD_WIDTH + SPACING * 2) * 3, SPACING },
    { (CARD_WIDTH + SPACING * 2) * 4, SPACING },
};


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
        da_append(&shuffle, &cards[i]);
    }

    int x = SPACING * 2 + CARD_WIDTH;
    int y = SPACING * 2 + CARD_HEIGHT;

    int limit = 1;
    for (int i = 0; i < 7; i++) {
        Vector2 pos = { x * (i + 1), y };

        for (int j = 0; j < limit; j++) {
            srand(time(NULL));
            int index = (rand() % shuffle.count);
            Card *choice = shuffle.items[index];
            da_append(&play[i], choice);
            da_remove_unordered(&shuffle, index);

            choice->pos.x = pos.x;
            choice->pos.y = pos.y;

            pos.y += SPACING;

            if (j + 1 == limit) {
                choice->visible = true;
            }
        }

        pos.x += SPACING + CARD_WIDTH;

        limit++;
    }

    while (shuffle.count > 0) {
        srand(time(NULL));
        int index = (rand() % shuffle.count);
        Card *card = shuffle.items[index];
        da_append(&stock, card);
        da_remove_unordered(&shuffle, index);
    }
}

Rectangle getCardRectangle(Card card) {
    Rectangle rec = { card.pos.x, card.pos.y, CARD_WIDTH, CARD_HEIGHT };
    return rec;
}

void setGrabbedCardInfo(Card *card, Vector2 mousePos) {
    card->origPos.x = card->pos.x;
    card->origPos.y = card->pos.y;

    card->offset.x = mousePos.x - card->pos.x;
    card->offset.y = mousePos.y - card->pos.y;
}


bool grabCard(Card *card, Vector2 mousePos) {
    Rectangle rect = getCardRectangle(*card);
    if (!mouseDown && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, rect)) {
        mouseDown = true;

        setGrabbedCardInfo(card, mousePos);

        return true;
    }

    return false;
}

void printCard(Card card, char* message, int frameCount) {
    if (!DEBUG) return;

    if (frameCount % 120 == 0) {
        printf("%s suit: %d, number: %d, pos: %f, %f\n", message, card.suit, card.number, card.pos.x, card.pos.y);
    }
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, "Solitaire");
    SetTraceLogLevel(LOG_ERROR); 
    SetTargetFPS(60);

    LoadCards();
    GameBoardInit();

    int frameCount = 0;

    while(!WindowShouldClose()) {
        frameCount++;
        Vector2 mousePos = GetMousePosition();

        // play
        for (int i = 0; i < 7; i++) {
            if (mouseDown) break;

            Pile pile = play[i];
            int count = 0;
            for (int j = pile.count - 1; j >= 0; j--) {
                Card *card = pile.items[j];
                if (!card->visible) break;
                if (grabCard(card, mousePos)) {
                    count = pile.count - j;
                    break;
                }
            }

            if (mouseDown) {
                // printf("count: %d:");

                    
                break;
            }
        }


        // waist
        if (waste.count > 0 && !mouseDown) {
            Card *card = da_last(&waste);

            if (grabCard(card, mousePos)) {
                da_append(&grabbed, card);
                da_remove_ordered(&waste, waste.count - 1);
                grabbedSource = &waste;
            }
        }

        // for (int i = 0; i < 52; i++) {
        //     Card *card = &cards[i];
        //
        //     if (!card->visible) continue;
        //
        //     Rectangle rect = getCardRectangle(*card);
        //
        //     if (!mouseDown && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, rect)) {
        //         mouseDown = true;
        //         // we need to grab all the cards
        //         da_append(&grabbed, card);
        //         // grabbedSource = &grabbed;
        //
        //         card->origPos.x = card->pos.x;
        //         card->origPos.y = card->pos.y;
        //
        //         offset.x = mousePos.x - card->pos.x;
        //         offset.y = mousePos.y - card->pos.y;
        //         break;
        //     }
        //
        //
        // }

        if (mouseDown) {
            da_foreach(Card*, c, &grabbed) {
                Card *card = *c;
                card->pos.x = mousePos.x - card->offset.x;
                card->pos.y = mousePos.y - card->offset.y;
            }
        }

        if (grabbed.count > 0 && mouseDown && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {

            bool placed = false;
            Card *place = grabbed.items[0];

            for (int i = 0; i < 7; i++) {
                Pile pile = play[i];
                Card *last = da_last(&pile);
                Rectangle rect = getCardRectangle(*last);
                if (CheckCollisionPointRec(mousePos, rect)) {
                    if (
                        (
                            ((place->suit == HEART || place->suit == DIAMOND)
                            && (last->suit == CLUB || last->suit == SPADE))
                            || ((last->suit == HEART || last->suit == DIAMOND)
                            && (place->suit == CLUB || place->suit == SPADE))
                        )
                        && place->number + 1 == last->number
                    ) {
                        placed = true;
                        da_foreach(Card*, c, &grabbed) {
                            Card *card = *c;
                            card->pos.x = last->pos.x;
                            card->pos.y = last->pos.y + SPACING;
                            da_append(&play[i], card);
                            last = card;
                        }
                    } else {
                        placed = false;
                    }
                }
            }

            // foundation
            for (int i = 0; i < 4; i++) {
                if (grabbed.count > 1) break;

                Pile pile = foundation[i];
                // Card *card = da_last(&gr);

                // if (pile.count == 0 && card->number == 1) {
                    // TODO

                // }

                Card *last = da_last(&pile);
                Rectangle rect = getCardRectangle(*last);

            }

            // put the cards back where they go
            if (!placed) {
                da_foreach(Card*, c, &grabbed) {
                    Card *card = *c;

                    card->pos.x = card->origPos.x;
                    card->pos.y = card->origPos.y;

                    da_append(grabbedSource, card);
                }
            }

            grabbed.count = 0;
            grabbedSource = NULL;
            mouseDown = false;
        }

        Rectangle drawRect = { stockPos.x, stockPos.y, CARD_WIDTH, CARD_HEIGHT };
        if (CheckCollisionPointRec(mousePos, drawRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

            if (stock.count == 0) {
                while (waste.count > 0) {
                    Card *card = waste.items[0];                    
                    card->pos.x = stockPos.x;
                    card->pos.y = stockPos.y;
                    card->visible = false;
                    da_remove_ordered(&waste, 0);
                    da_append(&stock, card);
                }
            }

            da_remove_ordered(&stock, 0);
            Card *card = stock.items[0];
            card->pos.x = wastePos.x;
            card->pos.y = wastePos.y;
            card->visible = true;
            printCard(*card, "", 0);
            da_append(&waste, card);
        }

        BeginDrawing();

        ClearBackground(GREEN);

        if (stock.count > 0) {
            DrawTextureV(cardBack, stockPos, RAYWHITE);
        }

        if (waste.count > 0) {
            Card *card = da_last(&waste);

            // printCard(*card, "", frameCount);

            if (!containsCard(&grabbed, card)) {
                DrawTextureV(card->texture, card->pos, RAYWHITE);
            }
        }

        for (int i = 0; i < 7; i++) {
            da_foreach(Card*, c, &play[i]) {
                Card* card = *c;
                if (containsCard(&grabbed, card)) continue;

                if (card->visible) {
                    DrawTextureV(card->texture, card->pos, RAYWHITE);
                } else {
                    DrawTextureV(cardBack, card->pos, RAYWHITE);
                }
            }
        }

        Vector2 pos = { (SPACING * 2 + CARD_WIDTH) * 4, SPACING };
        float thickness = 5.0;

        for (int i = 0; i < 4; i++) {
            Rectangle rec = { pos.x - thickness, pos.y - thickness, CARD_WIDTH + thickness * 2, CARD_HEIGHT + thickness * 2 };

            if (foundation[i].count == 0) {
                DrawRectangleLinesEx(rec, 5.0, BLACK);
            }

            // DrawTextureV(card.texture, card.pos, RAYWHITE);

            pos.x += SPACING * 2 +  CARD_WIDTH;
        }

        if (grabbed.count != 0) {
            da_foreach(Card*, c, &grabbed) {
                Card *card = *c;
                DrawTextureV(card->texture, card->pos, RAYWHITE);
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
