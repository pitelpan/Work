#include "raylib.h"

// Διαστάσεις παραθύρου
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

// Πλάτος και ύψος τούβλων
#define BRICK_WIDTH 50
#define BRICK_HEIGHT 20

// Γραμμές και στήλες τούβλων
#define ROWS 5
#define COLS 10

// Δομή για τα τούβλα
typedef struct Brick {
    Rectangle rect;
    Color color;
    bool active;
} Brick;

int main(void) {
    // Εκκίνηση παραθύρου και OpenGL
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout σε C");

    // Αρχικοποίηση μπάλας
    Vector2 ballPosition = { (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 };
    Vector2 ballSpeed = { 5.0f, 5.0f };
    float ballRadius = 7.0f;

    // Αρχικοποίηση ρακέτας
    Rectangle paddle = { (float)SCREEN_WIDTH / 2 - 50, (float)SCREEN_HEIGHT - 30, 100, 10 };

    // Αρχικοποίηση τούβλων
    Brick bricks[ROWS * COLS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            bricks[i * COLS + j].rect = (Rectangle){
                (float)j * (BRICK_WIDTH + 5) + 50,
                (float)i * (BRICK_HEIGHT + 5) + 50,
                BRICK_WIDTH,
                BRICK_HEIGHT
            };
            bricks[i * COLS + j].color = (Color){ GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255 };
            bricks[i * COLS + j].active = true;
        }
    }

    SetTargetFPS(60);

    // Κύριος βρόχος παιχνιδιού
    while (!WindowShouldClose()) {
        // --- ΕΝΗΜΕΡΩΣΗ ---

        // Κίνηση ρακέτας
        if (IsKeyDown(KEY_RIGHT) && paddle.x < SCREEN_WIDTH - paddle.width) {
            paddle.x += 7.0f;
        }
        if (IsKeyDown(KEY_LEFT) && paddle.x > 0) {
            paddle.x -= 7.0f;
        }

        // Κίνηση μπάλας
        ballPosition.x += ballSpeed.x;
        ballPosition.y += ballSpeed.y;

        // Έλεγχος σύγκρουσης μπάλας με τοίχους
        if (ballPosition.x >= (SCREEN_WIDTH - ballRadius) || ballPosition.x <= ballRadius) {
            ballSpeed.x *= -1;
        }
        if (ballPosition.y <= ballRadius) {
            ballSpeed.y *= -1;
        }

        // Έλεγχος σύγκρουσης μπάλας με τη ρακέτα
        if (CheckCollisionCircleRec(ballPosition, ballRadius, paddle)) {
            if (ballSpeed.y > 0) {
                ballSpeed.y *= -1;
            }
        }

        // Έλεγχος σύγκρουσης μπάλας με τούβλα
        for (int i = 0; i < ROWS * COLS; i++) {
            if (bricks[i].active && CheckCollisionCircleRec(ballPosition, ballRadius, bricks[i].rect)) {
                ballSpeed.y *= -1;
                bricks[i].active = false;
                break;
            }
        }
        
        // Τέλος παιχνιδιού αν η μπάλα περάσει τη ρακέτα
        if (ballPosition.y > SCREEN_HEIGHT) {
            // Εδώ μπορείτε να προσθέσετε λογική για "Game Over"
            ballPosition = (Vector2){ (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 };
        }

        // --- ΣΧΕΔΙΑΣΗ ---

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Σχεδίαση τούβλων
        for (int i = 0; i < ROWS * COLS; i++) {
            if (bricks[i].active) {
                DrawRectangleRec(bricks[i].rect, bricks[i].color);
            }
        }

        // Σχεδίαση ρακέτας
        DrawRectangleRec(paddle, BLUE);

        // Σχεδίαση μπάλας
        DrawCircleV(ballPosition, ballRadius, RED);

        EndDrawing();
    }

    // Κλείσιμο παραθύρου
    CloseWindow();

    return 0;
}