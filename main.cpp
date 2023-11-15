#include <Novice.h>

const char kWindowTitle[] = "テストブランチ";

typedef struct Vector2f {
	float x;
	float y;
} Vector2f;

struct Vector2 {
	int x = 0;
	int y = 0;
};

typedef struct Ball {
	Vector2f position = { 680.0f, 50.0f };
	Vector2f velocity = { 0.0f, 0.0f };
	Vector2f acceleration = { 0.0f, -0.8f };
	int radius = 30;
	unsigned int color = WHITE;
} Ball;

Ball ball;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// player座標
	Vector2 playerWorld = { 0, 0 };
	// playerのスクロール座標
	Vector2 playerScroll;
	// スピード
	int playerSpeed = 5;
	// 背景のスクロール
	Vector2 backgroundScroll;

	// 背景のスクロール座標
	Vector2 backScroll[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			backScroll[i][j] = { i * 1280, j * 720 };
		}
	}
	// mapのスクロール座標
	//   map
	//  タイルサイズ
	int tile = 70;
	const int mapX = 32;
	const int mapY = 9;
	int map1[mapY][mapX] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 0,
		 0, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	};
	// タイルの種類
	enum Tile {
		// 描画なし
		NOBLOCK,
		// 足し算
		ADD,
		// 引き算
		SUBTRACTION,
		// 掛け算
		MULTIPLICATION,
		// 割り算
		DIVISION,
		// ただのブロック
		NOMAL
	};

	// 絵
	int backgroundHandle[4][4] = {};
	backgroundHandle[0][0] = Novice::LoadTexture("./Resource/bg1.png");
	backgroundHandle[0][1] = Novice::LoadTexture("./Resource/bg2.png");
	backgroundHandle[0][2] = Novice::LoadTexture("./Resource/bg3.png");
	backgroundHandle[0][3] = Novice::LoadTexture("./Resource/bg4.png");
	backgroundHandle[1][0] = Novice::LoadTexture("./Resource/bg2.png");
	backgroundHandle[1][1] = Novice::LoadTexture("./Resource/bg3.png");
	backgroundHandle[1][2] = Novice::LoadTexture("./Resource/bg4.png");
	backgroundHandle[1][3] = Novice::LoadTexture("./Resource/bg1.png");
	backgroundHandle[2][0] = Novice::LoadTexture("./Resource/bg3.png");
	backgroundHandle[2][1] = Novice::LoadTexture("./Resource/bg4.png");
	backgroundHandle[2][2] = Novice::LoadTexture("./Resource/bg1.png");
	backgroundHandle[2][3] = Novice::LoadTexture("./Resource/bg2.png");
	backgroundHandle[3][0] = Novice::LoadTexture("./Resource/bg4.png");
	backgroundHandle[3][1] = Novice::LoadTexture("./Resource/bg1.png");
	backgroundHandle[3][2] = Novice::LoadTexture("./Resource/bg2.png");
	backgroundHandle[3][3] = Novice::LoadTexture("./Resource/bg3.png");

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		// マップの更新座標
		int nowMapX = mapX;
		// int nowMapY = mapY;

		// ジャンプ
		float reflectY = float((playerWorld.y - 700)) * -1;
		ball.acceleration.y = -0.8f;

		if ((keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) != 0 && playerWorld.y == ball.radius) {
			ball.velocity.y = 20.0f;
		}

		ball.velocity.x += ball.acceleration.x;
		ball.velocity.y += ball.acceleration.y;

		playerWorld.x += int(ball.velocity.x);
		playerWorld.y += int(ball.velocity.y);

		if (playerWorld.y <= int(ball.radius)) {
			playerWorld.y = int(ball.radius);
		}

		// 左
		if ((keys[DIK_A] || keys[DIK_LEFT]) && playerWorld.x > 0) {
			if (playerWorld.x >= 640 && playerWorld.x <= 4480) {
				playerScroll.x -= playerSpeed;
				backgroundScroll.x -= playerSpeed;
				nowMapX -= playerSpeed;
				if (backgroundScroll.x < 0 || playerScroll.x < 0) {
					backgroundScroll.x = 0;
					playerScroll.x = 0;
				}
			}
			playerWorld.x -= playerSpeed;
		}
		// 右
		if ((keys[DIK_D] || keys[DIK_RIGHT]) && playerWorld.x < 5120) {
			if (playerWorld.x >= 640 && playerWorld.x <= 4480) {
				playerScroll.x += playerSpeed;
				backgroundScroll.x += playerSpeed;
				nowMapX -= playerSpeed;
				if (backgroundScroll.x > 3840 || playerScroll.x > 3840) {
					backgroundScroll.x = 3840;
					playerScroll.x = 3840;
				}
			}
			playerWorld.x += playerSpeed;
		}

		// 当たり判定

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 絵

		// 背景
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Novice::DrawSprite(
					backScroll[i][j].x - backgroundScroll.x,
					backScroll[i][j].y - backgroundScroll.y, backgroundHandle[i][j], 1, 1, 0.0f,
					WHITE);
			}
		}
		// ブロック
		for (int y = 0; y < mapY; y++) {
			for (int x = 0; x < mapX; x++) {
				switch (map1[y][x]) {
					// 描画なし
				case NOBLOCK:
					break;
					// 足し算
				case ADD:
					Novice::DrawBox(
						(x * tile) - backgroundScroll.x, (y * tile) - backgroundScroll.y, tile,
						tile, 0.0f, RED, kFillModeSolid);
					break;
					// 引き算
				case SUBTRACTION:
					Novice::DrawBox(
						(x * tile) - backgroundScroll.x, (y * tile) - backgroundScroll.y, tile,
						tile, 0.0f, GREEN, kFillModeSolid);
					break;
					// 掛け算
				case MULTIPLICATION:
					Novice::DrawBox(
						(x * tile) - backgroundScroll.x, (y * tile) - backgroundScroll.y, tile,
						tile, 0.0f, BLUE, kFillModeSolid);
					break;
					// 割り算
				case DIVISION:
					Novice::DrawBox(
						(x * tile) - backgroundScroll.x, (y * tile) - backgroundScroll.y, tile,
						tile, 0.0f, BLACK, kFillModeSolid);
					break;
					// ただのブロック
				case NOMAL:
					Novice::DrawBox(
						(x * tile) - backgroundScroll.x, (y * tile) - backgroundScroll.y, tile,
						tile, 0.0f, WHITE, kFillModeSolid);
					break;
				}
			}
		}
		// 円
		Novice::DrawEllipse(
			playerWorld.x - playerScroll.x, int(reflectY) - playerScroll.y, ball.radius,
			ball.radius, 0.0f, WHITE, kFillModeSolid);

		Novice::ScreenPrintf(10, 10, "playerWorld(%d, %d)", playerWorld.x, playerWorld.y);
		Novice::ScreenPrintf(10, 25, "playerScroll(%d, %d)", playerScroll.x, playerScroll.y);

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Novice::ScreenPrintf(
					10 + (300 * i), 50 + (15 * j), "backScroll[%d][%d].x = %d", i, j,
					backScroll[i][j].x);
			}
		}

		Novice::ScreenPrintf(500, 500, "%d", nowMapX);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}