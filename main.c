#include "raylib.h"
#include "nob.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG true
#define CARD_HEIGHT 150
#define CARD_WIDTH 107
#define SPACING 25.0
#define CARD_WIDTH_SPACING (CARD_WIDTH + SPACING)
#define CARD_HEIGHT_SPACING (CARD_HEIGHT + SPACING)
#define OUTLINE_THICKNESS 0.5
#define KING 13
#define HEADER_HEIGHT 0
#define WINDOW_HEIGHT 1000
#define WINDOW_WIDTH 1200

enum Suit { 
    SPADE,
    HEART,
    CLUB,
    DIAMOND
};

enum WasteType {
    ONE,
    THREE
};

typedef struct {
    int id;
    enum Suit suit;
    int number;
    Vector2 pos;
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

typedef struct {
    Card *items;
    int count;
    int capacity;
} CardSplosion;


#define new_card(id, suit, number, image) { id, suit, number, {0}, {0}, "./SVG-cards/png/2x/" image, {0} }

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
    new_card(12, HEART, KING, "heart_king.png"),
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
    new_card(25, DIAMOND, KING, "diamond_king.png"),
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
    new_card(38, SPADE, KING, "spade_king.png"),
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
    new_card(51, CLUB, KING, "club_king.png"),
};

// globals
Pile stock = {0};
Pile waste = {0};
Pile foundation[4] = {0};
Pile play[7] = {0};
Pile grabbed = {0};
Pile *grabbedSource = NULL;
CardSplosion cardSplosion = {0};
bool mouseDown = false;
bool won = false;
enum WasteType wasteType = ONE;
int cardSplosionIndex = 51;
int bounceIter = 0;
int wasteShown = 0;
Vector2 cardMoveDefault = { 5, 0 };
int direction = -1;
int reduction = 0;
int currentY = 0;

Vector2 stockPos = { SPACING, HEADER_HEIGHT + SPACING };
Vector2 wastePos = { CARD_WIDTH + HEADER_HEIGHT + SPACING * 2, SPACING };
Vector2 playPos[7] = { 
    { CARD_WIDTH_SPACING * 0 + SPACING, HEADER_HEIGHT + CARD_HEIGHT_SPACING + SPACING },
    { CARD_WIDTH_SPACING * 1 + SPACING, HEADER_HEIGHT + CARD_HEIGHT_SPACING + SPACING },
    { CARD_WIDTH_SPACING * 2 + SPACING, HEADER_HEIGHT + CARD_HEIGHT_SPACING + SPACING },
    { CARD_WIDTH_SPACING * 3 + SPACING, HEADER_HEIGHT + CARD_HEIGHT_SPACING + SPACING },
    { CARD_WIDTH_SPACING * 4 + SPACING, HEADER_HEIGHT + CARD_HEIGHT_SPACING + SPACING },
    { CARD_WIDTH_SPACING * 5 + SPACING, HEADER_HEIGHT + CARD_HEIGHT_SPACING + SPACING },
    { CARD_WIDTH_SPACING * 6 + SPACING, HEADER_HEIGHT + CARD_HEIGHT_SPACING + SPACING },
};

Vector2 foundationPos[4] = { 
    { CARD_WIDTH_SPACING * 3 + SPACING, HEADER_HEIGHT + SPACING },
    { CARD_WIDTH_SPACING * 4 + SPACING, HEADER_HEIGHT + SPACING },
    { CARD_WIDTH_SPACING * 5 + SPACING, HEADER_HEIGHT + SPACING },
    { CARD_WIDTH_SPACING * 6 + SPACING, HEADER_HEIGHT + SPACING },
};

void loadCards() {
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

// this is not the most optimal way to do this
// but it is all in one place and happens every time 
// and removes having to do too much in other places.
void setCardPositions() {
    // stock
    da_foreach(Card*, c, &stock) {
        Card *card = *c;
        card->pos.x = stockPos.x;
        card->pos.y = stockPos.y;
    }

    // waste
    da_foreach(Card*, c, &waste) {
        Card *card = *c;

        card->pos.x = wastePos.x;
        card->pos.y = wastePos.y;
    }

    // foundation
    for (int i = 0; i < 4; i++) {
        Vector2 pos = foundationPos[i];

        Pile *pile = &foundation[i];
        da_foreach(Card*, c, pile) {
            Card *card = *c;

            card->pos.x = pos.x;
            card->pos.y = pos.y;
        }
    }
    
    // play
    for (int i = 0; i < 7; i++) {
        Vector2 pos = playPos[i];

        Pile *pile = &play[i];
        da_foreach(Card*, c, pile) {
            Card *card = *c;

            card->pos.x = pos.x;
            card->pos.y = pos.y;

            pos.y += SPACING;
        }

        pos.x += SPACING + CARD_WIDTH;
    }

    // grabbed
    if (grabbed.count > 0) {
        Vector2 pos = grabbed.items[0]->pos;

        da_foreach(Card*, c, &grabbed) {
            Card *card = *c;

            card->pos.x = pos.x;
            card->pos.y = pos.y;

            pos.y += SPACING;
        }

        pos.x += SPACING + CARD_WIDTH;
    }
}

void gameBoardInit() {
    cardSplosion.count = 0;
    Pile shuffle = {0};
    da_reserve(&shuffle, 52);

    for (int i = 0; i < 52; i++) {
        Card *card = &cards[i];
        card->visible = false;
        da_append(&shuffle, card);
    }

    int limit = 1;
    for (int i = 0; i < 7; i++) {
        Pile *pile = &play[i];
        pile->count = 0;

        for (int j = 0; j < limit; j++) {
            srand(time(NULL));
            int index = (rand() % shuffle.count);
            Card *choice = shuffle.items[index];
            da_append(pile, choice);
            da_remove_unordered(&shuffle, index);

            if (j + 1 == limit) {
                choice->visible = true;
            }
        }

        limit++;
    }

    for (int i = 0; i < 4; i++) {
        Pile *pile = &foundation[i];
        pile->count = 0;
    }

    stock.count = 0;
    waste.count = 0;

    while (shuffle.count > 0) {
        srand(time(NULL));
        int index = (rand() % shuffle.count);
        Card *card = shuffle.items[index];
        da_append(&stock, card);
        da_remove_unordered(&shuffle, index);
    }

    setCardPositions();
}

Rectangle getCardRectangle(Vector2 pos) {
    Rectangle rec = { pos.x, pos.y, CARD_WIDTH, CARD_HEIGHT };
    return rec;
}

bool grabCard(Card *card, Vector2 mousePos) {
    Rectangle rect = getCardRectangle(card->pos);
    if (CheckCollisionPointRec(mousePos, rect)) {
        mouseDown = true;

        card->offset.x = mousePos.x - card->pos.x;
        card->offset.y = mousePos.y - card->pos.y;

        return true;
    }

    return false;
}

void handleRightClick(Vector2 mousePos) {
    if (!IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) return;

    for (int i = 0; i < 7; i++) {
        Pile *pile = &play[i];
        if (pile->count == 0) continue;

        Card *card = da_last(pile);
        Rectangle rect = getCardRectangle(card->pos);

        if (!CheckCollisionPointRec(mousePos, rect)) continue;

        for (int f = 0; f < 4; f++) {
            Pile *found = &foundation[f];

            if (card->number == 1) {
                if (found->count > 0) continue;
                da_append(found, card);
                da_remove_unordered(pile, pile->count - 1);
                if (pile-> count > 0) {
                    da_last(pile)->visible = true;
                }
                return;
            } else if (found->count > 0) {
                Card *last = da_last(found);
                if (last->suit == card->suit && last->number == card->number - 1) {
                    da_append(found, card);
                    da_remove_unordered(pile, pile->count - 1);
                    if (pile-> count > 0) {
                        da_last(pile)->visible = true;
                    }
                    return;
                }
            }
        }
    }

    if (waste.count == 0) return;

    Rectangle rect = getCardRectangle(wastePos);

    if (!CheckCollisionPointRec(mousePos, rect)) return;

    Card *card = da_last(&waste);

    for (int f = 0; f < 4; f++) {
        Pile *found = &foundation[f];

        if (card->number == 1 && found->count == 0) {
            da_append(found, card);
            da_remove_unordered(&waste, waste.count - 1);
            return;

        } else if (found->count > 0) {
            Card *last = da_last(found);
            if (last->suit == card->suit && last->number == card->number - 1) {
                da_append(found, card);
                da_remove_unordered(&waste, waste.count - 1);
                return;
            }
        }
    }
}

void handleGrabbingCards(Vector2 mousePos) {
    if (mouseDown) return;

    if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON)) return;

    // play
    for (int i = 0; i < 7; i++) {
        Pile *pile = &play[i];
        int count = 0;
        for (int j = pile->count - 1; j >= 0; j--) {
            Card *card = pile->items[j];
            if (!card->visible) break;
            if (grabCard(card, mousePos)) {
                count = pile->count - j;
                grabbedSource = pile;
                break;
            }
        }

        while (count > 0) {
            int index = pile->count - count;
            Card *card = pile->items[index];
            da_append(&grabbed, card);
            da_remove_ordered(pile, index);
            count--;
        }
    }

    // waste
    if (waste.count > 0) {
        Card *card = da_last(&waste);

        if (grabCard(card, mousePos)) {
            da_append(&grabbed, card);
            da_remove_unordered(&waste, waste.count - 1);

            grabbedSource = &waste;
        }
    }

    // foundation
    for (int i = 0; i < 4; i++) {
        Pile *pile = &foundation[i];

        if (pile->count > 0) {
            Card *card = da_last(pile);

            if (grabCard(card, mousePos)) {
                da_append(&grabbed, card);
                da_remove_unordered(pile, pile->count - 1);

                grabbedSource = pile;
            }
        }
    }
}

void setGrabbedCardsPositions(Vector2 mousePos) {
    // Set the position of all the grabbed cards
    if (mouseDown) {
        da_foreach(Card*, c, &grabbed) {
            Card *card = *c;
            card->pos.x = mousePos.x - card->offset.x;
            card->pos.y = mousePos.y - card->offset.y;
        }
    }
}

void handlePlacingCard(Vector2 mousePos) {
    if (grabbed.count == 0) return;
    if (!mouseDown) return;
    if (!IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) return;

    bool placed = false;
    Card *place = grabbed.items[0];

    for (int i = 0; i < 7; i++) {
        Pile *pile = &play[i];

        // check for king
        Rectangle empty = getCardRectangle(playPos[i]);
        if (pile->count == 0 && place->number == KING && CheckCollisionPointRec(mousePos, empty)) {
            placed = true;
            da_foreach(Card*, c, &grabbed) {
                Card *card = *c;
                da_append(&play[i], card);
            }

            break;
        }

        if (pile->count == 0) continue;

        Card *last = da_last(pile);
        Rectangle rect = getCardRectangle(last->pos);
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
                    da_append(&play[i], card);
                }
            } else {
                placed = false;
            }

            break;
        }
    }

    // foundation
    for (int i = 0; i < 4; i++) {
        if (grabbed.count > 1) break;

        Vector2 pos = foundationPos[i];
        Pile *pile = &foundation[i];
        Rectangle rect = getCardRectangle(pos);
        Card *card = da_last(&grabbed);

        if (!CheckCollisionPointRec(mousePos, rect)) continue;

        if (pile->count == 0 && card->number == 1) {
            da_append(pile, card);
            placed = true;
        } else {
            Card *last = da_last(pile);
            if (card->suit == last->suit && (card->number - 1) == last->number) {
                da_append(pile, card);
                placed = true;
            }
        }
    }

    // put the cards back where they go
    if (!placed) {
        da_foreach(Card*, c, &grabbed) {
            Card *card = *c;

            da_append(grabbedSource, card);
        }
    } else if (grabbedSource->count > 0) {
        da_last(grabbedSource)->visible = true;
    }

    grabbed.count = 0;
    grabbedSource = NULL;
    mouseDown = false;
}

void handleDrawFromStock(Vector2 mousePos) {
    if (stock.count == 0 && waste.count == 0) return;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;

    Rectangle drawRect = getCardRectangle(stockPos);

    if (!CheckCollisionPointRec(mousePos, drawRect)) return;

    switch (wasteType) {
        case ONE: 
            wasteShown = 1;
            break;
        case THREE: 
            wasteShown = 3;
            break;
        default:
            assert("invalid wastetype");
    }

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

    Card *card = stock.items[0];
    card->pos.x = wastePos.x;
    card->pos.y = wastePos.y;
    card->visible = true;
    da_remove_ordered(&stock, 0);
    da_append(&waste, card);
}

void drawGame() {
    BeginDrawing();

    ClearBackground(GREEN);

    Font font = GetFontDefault();
    int fontSize = 20;

    // DrawRectangle(0, 0, WINDOW_WIDTH, HEADER_HEIGHT, GREEN);

    char* text = "Esc = Quit    R = Reset"; 
    Vector2 size = MeasureTextEx(font, text, fontSize, 1.0);
    DrawText(text, SPACING, SPACING / 2 - size.y / 2, 20, BLACK);



    if (stock.count > 0) {
        DrawTextureV(cardBack, stockPos, RAYWHITE);
    } else {
        Rectangle rect = getCardRectangle(stockPos);
        DrawRectangleLinesEx(rect, 5.0, BLACK);
    }

    if (waste.count > 0) {
        Card *card = da_last(&waste);

        DrawTextureV(card->texture, card->pos, RAYWHITE);
    }

    for (int i = 0; i < 7; i++) {
        Pile *pile = &play[i];
        if (pile->count == 0) {
            Vector2 pos = playPos[i];
            Rectangle rec = { 
                pos.x - OUTLINE_THICKNESS, 
                pos.y - OUTLINE_THICKNESS, 
                CARD_WIDTH + OUTLINE_THICKNESS * 2, 
                CARD_HEIGHT + OUTLINE_THICKNESS * 2 
            };
            DrawRectangleLinesEx(rec, 5.0, BLACK);
        }
        da_foreach(Card*, c, &play[i]) {
            Card* card = *c;
           if (card->visible) {
                DrawTextureV(card->texture, card->pos, RAYWHITE);
            } else {
                DrawTextureV(cardBack, card->pos, RAYWHITE);
            }
        }
    }


    for (int i = 0; i < 4; i++) {
        Vector2 pos = foundationPos[i];
        Pile *pile = &foundation[i];
        Rectangle rec = { 
            pos.x - OUTLINE_THICKNESS, 
            pos.y - OUTLINE_THICKNESS, 
            CARD_WIDTH + OUTLINE_THICKNESS * 2, 
            CARD_HEIGHT + OUTLINE_THICKNESS * 2 
        };

        if (foundation[i].count == 0) {
            DrawRectangleLinesEx(rec, 5.0, BLACK);
        } else {
            da_foreach(Card*, c, pile) {
                Card *card = *c;
                DrawTextureV(card->texture, card->pos, RAYWHITE);
            }
        }
    }

    da_foreach(Card*, c, &grabbed) {
        Card *card = *c;
        DrawTextureV(card->texture, card->pos, RAYWHITE);
    }

    if (won) {
        da_foreach(Card, card, &cardSplosion) {
            DrawTextureV(card->texture, card->pos, RAYWHITE);
        }
    }

    EndDrawing();
}

bool checkIfGameWon() {
    int count = 0;
    for (int i = 0; i < 4; i++) {
        count += foundation[i].count;
    }

    return count == 52;
}

void setGameToWonState() {
    won = true;
    while (stock.count > 0) {
        da_remove_unordered(&stock, 0); 
    }

    while (waste.count > 0) {
        da_remove_unordered(&waste, 0); 
    }

    for (int i = 0; i < 7; i++) {
        Pile *pile = &play[i];
        while (pile->count > 0) {
            da_remove_unordered(pile, 0); 
        }
    }

    int index = 0;
    for (int i = 0; i < 52; i++) {
        if (i != 0 && i % 13 == 0) {
            index++;
        }

        da_append(&foundation[index], &cards[i]);
    }
}

void handleGameWon() {
    if (cardSplosionIndex < 0) {
        drawGame();
        
        return;
    }

    bool firstTime = cardSplosion.count == 0;

    Card card = {0};
    if (cardSplosion.count == 0) {
        card = cards[cardSplosionIndex];
    } else {
        card = da_last(&cardSplosion);
    }

    if (card.pos.y + CARD_HEIGHT < 0 || card.pos.x + CARD_WIDTH <= 0 || card.pos.x >= WINDOW_WIDTH || card.pos.y + CARD_HEIGHT >= WINDOW_HEIGHT || firstTime) {
        if (!firstTime) cardSplosionIndex--;

        srand(time(NULL));
        cardMoveDefault.x = (rand() % 20) - 10;

        srand(time(NULL));
        cardMoveDefault.y = (rand() % 20) + 20;
        currentY = cardMoveDefault.y;

        if (cardSplosionIndex > 0) {
            card = cards[cardSplosionIndex];
            bounceIter = 0;
            direction = -1;
        } 
    }

    if (bounceIter == 0) {
        cardMoveDefault.y = currentY;
        if (direction == 1) {
            reduction = cardMoveDefault.y / 5;
            bounceIter = 5;
            direction = -1;
        } else {
            reduction = cardMoveDefault.y / 20;
            cardMoveDefault.y = 0;
            bounceIter = 20;
            direction = 1;
        }
    }


    bounceIter--;
    if (direction == 1) {
        cardMoveDefault.y += reduction;
    } else {
        cardMoveDefault.y -= reduction;
    }


    card.pos.x += cardMoveDefault.x;
    card.pos.y += (cardMoveDefault.y * direction);

    da_append(&cardSplosion, card);

    drawGame();
}

void changeWasteType() {

}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Solitaire");
    SetTraceLogLevel(LOG_ERROR); 
    SetTargetFPS(60);

    loadCards();
    gameBoardInit();

    int frameCount = 0;

    while(!WindowShouldClose()) {
        frameCount++;
        Vector2 mousePos = GetMousePosition();

        if (IsKeyPressed(KEY_R)) {
            gameBoardInit();
            cardSplosionIndex = 51;
            cardSplosionIndex = 51;
            bounceIter = 0;
            wasteShown = 0;
            cardMoveDefault.x = 5;
            cardMoveDefault.y = 5;
            direction = -1;
            reduction = 0;
            currentY = 0;
            won = false;
        }


        if (IsKeyPressed(KEY_T)) {
            changeWasteType();
            cardSplosionIndex = 51;
            won = false;
        }

        if (won) {
            handleGameWon();
            continue;
        }

        won = checkIfGameWon();

        if (!won && IsKeyPressed(KEY_W) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))) {
            printf("direction %d, reduction %d", direction, reduction);

            setGameToWonState();
        }

        handleRightClick(mousePos);
        handleGrabbingCards(mousePos);
        setGrabbedCardsPositions(mousePos);
        handlePlacingCard(mousePos);
        handleDrawFromStock(mousePos);
        setCardPositions();

        drawGame();

    }

    CloseWindow();

    return 0;
}
