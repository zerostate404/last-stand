#include <cmath>
#include <raylib.h>

int main() {
    InitWindow(1280, 720, "Arena");
    SetTargetFPS(60);

    Vector2 pos = {640, 360};
    const float speed = 300.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        Vector2 dir = {0 ,0};
        if (IsKeyDown(KEY_W)) dir.y -= 1;
        if (IsKeyDown(KEY_S)) dir.y += 1;
        if (IsKeyDown(KEY_A)) dir.x -= 1;
        if (IsKeyDown(KEY_D)) dir.x += 1;

        float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
        if (len > 0) {dir.x /= len; dir.y /= len;}

        pos.x += dir.x * speed * dt;
        pos.y += dir.y * speed * dt;

        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircleV(pos, 16, SKYBLUE);
        DrawFPS(10, 10);
        EndDrawing();
        
    }
    CloseWindow();
}
