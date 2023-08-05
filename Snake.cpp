#include <raylib.h>
#include <iostream>
#include <deque>
#include <raymath.h>

using namespace std;

/* Define colors */
static bool allowMove = false;
Color green = { 149, 215, 160, 255 };
Color darkGreen = { 39, 59, 41, 255 };
Color darkPink = { 138, 67, 70, 255 };

/* Set pixel size and window */
int cellSize = 50;
int cellCount = 20;
int offset = 100;
int screenWidth = 2 * offset + cellSize * cellCount;
int screenHeight = 2 * offset + cellSize * cellCount;

/* Calculate center of screen*/
int centerX = screenWidth / 2;
int centerY = screenHeight / 2;


/* Tracks last time that snake has moved */
double lastUpdateTime = 0;

/* check if the element is in body deque*/
bool ElementInDeque(Vector2 element, deque<Vector2> deque) {
	for (unsigned int i = 0; i < deque.size(); i++) {
		if (Vector2Equals(deque[i], element)) {
			return true;
		}
	}
	return false;
}

/* Sets interval of snake movement in seconds */
bool eventTriggered(double interval) {
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval) {
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}

class Food {
public:
	Vector2 position;

	/* Initializing new function */
	Food(deque<Vector2> snakeBody) {
		position = GenerateRandomPos(snakeBody);
	}

	/* Draw the food */
	void Draw() {
		Rectangle rect = Rectangle{ offset + position.x * cellSize, offset + position.y * cellSize, (float)cellSize, (float)cellSize };
		DrawRectangleRounded(rect, 0.5f, 6, darkPink);
	}

	Vector2 GenerateRandomCell() {
		float x = (GetRandomValue(0, cellCount - 1));
		float y = (GetRandomValue(0, cellCount - 1));
		return Vector2{ x, y };
	}

	/* Get random position */
	Vector2 GenerateRandomPos(deque<Vector2> snakeBody) {
		Vector2 position = GenerateRandomCell();
		while (ElementInDeque(position, snakeBody)) {
			position = GenerateRandomCell();
		}
		return position;
	}
};

class Snake {
public:
	/* Snake position deque */
	deque<Vector2> body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
	Vector2 direction = { 1, 0 };
	bool addSegment = false;

	/* Draw the snake */
	void Draw() {
		for (unsigned int i = 0; i < body.size(); i++) {
			float x = body[i].x;
			float y = body[i].y;
			Rectangle segment = Rectangle{ offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize };
			DrawRectangleRounded(segment, 0.5, 6, darkGreen);
		}
	}

	/* Updating snake position */
	void Update() {
		body.push_front(Vector2Add(body[0], direction));
		if (addSegment == true) {
			addSegment = false;
		}
		else {
			body.pop_back();
		}
	}

	void Reset() {
		body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
		direction = { 1, 0 };
	}
};

class Game {
public:
	Snake snake = Snake();
	Food food = Food(snake.body);
	bool running = true;
	bool gameOver = false;
	int score = 0;

	Game() : snake(), food(snake.body) {}

	void Draw() {
		food.Draw();
		snake.Draw();
	}

	void Update() {
		if (running) {
			snake.Update();
			CheckCollisionWithFood();
			CheckCollisionWithEdge();
			CheckCollisionWithTail();
		}
	}

	void CheckCollisionWithFood() {
		if (Vector2Equals(snake.body[0], food.position)) {
			/* Regenerate food position outside of body deque*/
			food.position = food.GenerateRandomPos(snake.body);
			/* If snake touches food, triggers addSegment*/
			snake.addSegment = true;
			score++;
		}
	}


	void CheckCollisionWithEdge() {
		/* Checks if snake goes past window*/
		if (snake.body[0].x == cellCount || snake.body[0].x == -1) {
			GameOver();
		}
		if (snake.body[0].y == cellCount || snake.body[0].y == -1) {
			GameOver();
		}
	}

	void CheckCollisionWithTail() {
		deque<Vector2> headlessBody = snake.body;
		headlessBody.pop_front();
		if (ElementInDeque(snake.body[0], headlessBody)) {
			GameOver();
		}
	}

	void GameOver() {
		snake.Reset();
		food.position = food.GenerateRandomPos(snake.body); 
		running = false;
		gameOver = false;
		score = 0;
	}

	void DrawGameOverText() {
		const char* text = "Game Over!";
		Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 80, 1);
		int textPosX = centerX - textSize.x / 2;
		int textPosY = centerY - textSize.y / 2;

		DrawText(text, textPosX, textPosY, 80, darkGreen);
	}

}; 

int main(void) {
	/* Initialization */
	InitWindow(screenWidth, screenHeight, "Retro Snake");
	SetTargetFPS(60);


	Game game = Game();

	Vector2 snakePosition = { 50, 50 };

	/*Main game loop*/
	while (WindowShouldClose() == false) { /* Detect window close button or ESC key */
		BeginDrawing();

		if (eventTriggered(0.2)) {
			allowMove = true;
			game.Update();
		}
	
		/* Update keyboard input */
		if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
			game.snake.direction = { 0, -1 };
			game.running = true;
			allowMove = false;
		}
		if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
			game.snake.direction = { 0, +1 };
			game.running = true;
			allowMove = false;
		}
		if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
			game.snake.direction = { -1, 0 };
			game.running = true;
			allowMove = false;
		}
		if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
			game.snake.direction = { +1, 0 };
			game.running = true;
			allowMove = false;
		}

		/* Draw the game */
		ClearBackground(green); // Set the background to green
		DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, (float)cellSize*cellCount+10, (float)cellSize*cellCount+10}, 5, darkGreen);

		/* Draw Score*/
		DrawText(TextFormat("Score: %i", game.score), offset - 5, offset + cellSize * cellCount + 10, 40, darkGreen);

		/* Draw Title*/
		DrawText("Retro Snake", offset - 5, offset - cellSize, 50, darkGreen);
		game.Draw();

		/* Draw the "Game Over!" text if the game is over */
		if (!game.running) {
			game.DrawGameOverText();
		}

		EndDrawing();
	}

	/*De-Initialization*/
	CloseWindow();

	return 0;
}



