#include <raylib.h>

using namespace std;

/*Define colors*/
Color green = {149, 215, 160, 255};
Color darkGreen = {39, 59, 41, 255};
Color darkPink = {138, 67, 70, 255};

/*Set pixel size and window*/
int cellSize = 50;
int cellCount = 25;

class Food {
public:
	Vector2 position;
	Texture2D texture;

	Food() {
		Image image = LoadImage("Graphics/food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
	}

	~Food() {
		UnloadTexture(texture);
	}

	/*Draw the food*/
	void Draw() {
		DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE) ;
	}

	///*Get random position*/
	//Vector2 GenerateRandomPos() {
	//	float x = GetRandomValue(0, cellCount - 1);
	//	float y = GetRandomValue(0, cellCount - 1);
	//	return Vector2{x, y};
	//}
};

class Snake {
public:
	void Draw() {

	}
};

int main(void) {
	/*Initialization*/

	InitWindow(cellSize*cellCount, cellSize*cellCount, "Retro Snake");
	SetTargetFPS(60);

	Food food = Food();

	Vector2 snakePosition = {50, 50};

	/*Main game loop*/
	while (!WindowShouldClose()) { /*Detect window close button or ESC key*/
		/*Update keyboard input*/
		if (IsKeyPressed(KEY_RIGHT)) snakePosition.x += 2.0f; /*when key is pressed snake needs to keep moving*/
		if (IsKeyPressed(KEY_LEFT)) snakePosition.x -= 2.0f;
		if (IsKeyPressed(KEY_UP)) snakePosition.y -= 2.0f;
		if (IsKeyPressed(KEY_DOWN)) snakePosition.y += 2.0f;

		/*Draw*/
		BeginDrawing();
			ClearBackground(green);
			food.Draw();
		EndDrawing();
	}

	/*De-Initialization*/
	CloseWindow();

	return 0;
}