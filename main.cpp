#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <algorithm>

struct Bullet {
    Vector2 pos;
    Vector2 vel;
    float life;      // seconds remaining
};

struct Player {
    Vector2 pos{640, 360};
    float speed = 300.0f;
    float radius = 16.0f;
    float cooldown = 0.0f;   // time until next shot allowed
    float fireRate = 0.15f;  // seconds between shots
};

struct Game {
    Player player;
    std::vector<Bullet> bullets;

    void Update(float dt) {
        // --- movement ---
        Vector2 dir{0, 0};
        if (IsKeyDown(KEY_W)) dir.y -= 1;
        if (IsKeyDown(KEY_S)) dir.y += 1;
        if (IsKeyDown(KEY_A)) dir.x -= 1;
        if (IsKeyDown(KEY_D)) dir.x += 1;
        dir = Vector2Normalize(dir);  // zero vector stays zero
        player.pos = Vector2Add(player.pos, Vector2Scale(dir, player.speed * dt));

        // keep inside window
        player.pos.x = Clamp(player.pos.x, player.radius, GetScreenWidth() - player.radius);
        player.pos.y = Clamp(player.pos.y, player.radius, GetScreenHeight() - player.radius);

        // --- aiming ---
        Vector2 aim = Vector2Normalize(Vector2Subtract(GetMousePosition(), player.pos));

        // --- shooting ---
        player.cooldown -= dt;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && player.cooldown <= 0.0f) {
            bullets.push_back({player.pos, Vector2Scale(aim, 600.0f), 1.5f});
            player.cooldown = player.fireRate;
        }

        // --- update bullets ---
        for (auto& b : bullets) {
            b.pos = Vector2Add(b.pos, Vector2Scale(b.vel, dt));
            b.life -= dt;
        }
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                           [](const Bullet& b) { return b.life <= 0.0f; }),
            bullets.end());
    }

    void Draw() const {
        Vector2 aim = Vector2Normalize(Vector2Subtract(GetMousePosition(), player.pos));
        Vector2 tip = Vector2Add(player.pos, Vector2Scale(aim, 30.0f));

        DrawCircleV(player.pos, player.radius, SKYBLUE);
        DrawLineEx(player.pos, tip, 4.0f, WHITE);   // aim indicator

        for (const auto& b : bullets) DrawCircleV(b.pos, 4.0f, YELLOW);

        DrawText(TextFormat("Bullets: %d", (int)bullets.size()), 10, 35, 20, GRAY);
        DrawFPS(10, 10);
    }
};

int main() {
    InitWindow(1280, 720, "Arena");
    SetTargetFPS(60);
    Game game;

    while (!WindowShouldClose()) {
        game.Update(GetFrameTime());
        BeginDrawing();
        ClearBackground(BLACK);
        game.Draw();
        EndDrawing();
    }
    CloseWindow();
}
