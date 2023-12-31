#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

const char kWindowTitle[] = "subject";

// 構造体
// 座標とスピード
typedef struct Vector2 {
	int x;
	int y;
	int speed;
} Vector2;

typedef struct Vector2f {
	float x;
	float y;
} Vector2f;

typedef struct Ball {
	Vector2f velocity = {5.0f, 5.0f};
	Vector2f acceleration = {0.0f, 1.0f};
} Ball;

Ball ball;

// ステージ
enum STAGE { TITLE, YESJAMP, MENU };

STAGE stage = TITLE;

// player座標
Vector2 playerPos = {50, 50, 3};
// playerのスクロール座標
// 背景のスクロール
Vector2 scroll;

// 当たり判定
typedef struct horn {
	// 左上
	int leftTopX;
	int leftTopY;
	// 左下
	int leftBottomX;
	int leftBottomY;
	// 右上
	int rightTopX;
	int rightTopY;
	// 右下
	int rightBottomX;
	int rightBottomY;

} horn;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	const int screenWidth = 800;
	const int screenHeight = 800;
	Novice::Initialize(kWindowTitle, screenWidth, screenHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// 絵
	int titleTexture = Novice::LoadTexture("./Resources./Title.png");
	int falseStageYesJump = Novice::LoadTexture("./Resources./falseYesJamp.png");
	int trueStageYesJump = Novice::LoadTexture("./Resources./trueYesJamp.png");

	// マップサイズ
	const int mapX = 50;
	const int mapY = 25;
	// タイルサイズ
	int mapTileSize = 50;
	int playerTileSize = 50;
	// マップ
	int map[mapY][mapX] = {
	    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	     0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1,
	     1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
	     1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	     1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
	     1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	    {1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	     1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

	// スクロール最大・最小値
	// X
	int scrollMinY = 0;
	int scrollMaxY = 450;
	// Y
	int scrollMinX = 0;
	int scrollMaxX = 1700;

	// 移動する前にいる座標の変数
	int oldScrollX = scroll.x;
	int oldScrollY = scroll.y;

	int nowMapX = mapX;
	int nowMapY = mapY;

	horn box = {
	    // 左上
	    (playerPos.x / playerTileSize),
	    (playerPos.y / playerTileSize),
	    // 左下
	    (playerPos.x - playerTileSize) / mapTileSize,
	    (playerPos.y + playerTileSize - 1) / mapTileSize,
	    // 右上
	    (playerPos.x + playerTileSize - 1) / mapTileSize,
	    (playerPos.y - playerTileSize) / mapTileSize,
	    // 右下
	    (playerPos.x + playerTileSize - 1) / mapTileSize,
	    (playerPos.y + playerTileSize - 1) / mapTileSize,
	};
	// ステージ選択
	bool isYesJamp = false;
	// ジャンプするかしないか
	bool isJamp = false;

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

		switch (stage) {

#pragma region TITLE
		case TITLE:
			if (keys[DIK_UP] || keys[DIK_W]) {
				isYesJamp = true;
			}
			if (isYesJamp == true && keys[DIK_RETURN]) {
				stage = YESJAMP;
			}
			if (keys[DIK_DOWN] || keys[DIK_S]) {
				isYesJamp = false;
			}
			break;
#pragma endregion

#pragma region YESJAMP
			// ジャンプアリ
		case YESJAMP:
			// スクロール止める
			oldScrollX = scroll.x;
			oldScrollY = scroll.y;
			// 角の判定
			box.leftTopX = (playerPos.x) / mapTileSize;
			box.leftTopY = (playerPos.y) / mapTileSize;
			box.leftBottomX = (playerPos.x) / mapTileSize;
			box.leftBottomY = (playerPos.y + playerTileSize - 1) / mapTileSize;
			box.rightTopX = (playerPos.x + playerTileSize - 1) / mapTileSize;
			box.rightTopY = (playerPos.y) / mapTileSize;
			box.rightBottomX = (playerPos.x + playerTileSize - 1) / mapTileSize;
			box.rightBottomY = (playerPos.y + playerTileSize - 1) / mapTileSize;
			// ジャンプ
			// スペースでジャンプ
			if ((keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) && isJamp == false) {
				isJamp = true;
				ball.velocity.y = -21.0f;
			}
			// 速度に加速度加算
			ball.velocity.y += ball.acceleration.y;

			// 角の当たり判定
			box.leftTopX = (playerPos.x) / mapTileSize;
			box.leftTopY = (playerPos.y + int(ball.velocity.y)) / mapTileSize;
			box.leftBottomX = (playerPos.x) / mapTileSize;
			box.leftBottomY =
			    (playerPos.y + playerTileSize - 1 + int(ball.velocity.y)) / mapTileSize;
			box.rightTopX = (playerPos.x + playerTileSize - 1) / mapTileSize;
			box.rightTopY = (playerPos.y + int(ball.velocity.y)) / mapTileSize;
			box.rightBottomX = (playerPos.x + playerTileSize - 1) / mapTileSize;
			box.rightBottomY =
			    (playerPos.y + playerTileSize - 1 + int(ball.velocity.y)) / mapTileSize;
			// ジャンプしたときに地面にのめりこむかのめりこまないか
			if (map[box.leftTopY][box.leftTopX] == 1 || map[box.rightTopY][box.rightTopX] == 1) {
				ball.velocity.y = 0;
				playerPos.y = (box.leftTopY + 1) * mapTileSize;
			}
			// 自由落下したときに地面にのめりこむかのめりこまないか
			if (map[box.leftBottomY][box.leftBottomX] == 1 ||
			    map[box.rightBottomY][box.rightBottomX] == 1) {
				isJamp = false;
				ball.velocity.y = 0;
				playerPos.y = (box.leftBottomY - 1) * mapTileSize;
			}

			// 自由落下中のスクロール
			// スクロールする
			scroll.y = playerPos.y - screenHeight / 2;

			if (scroll.y < scrollMinY) {
				scroll.y = scrollMinY;
			}
			if (scroll.y > scrollMaxY) {
				scroll.y = scrollMaxY;
			}

			// Yに速度加算
			playerPos.y += int(ball.velocity.y);

			// マップ座標更新
			nowMapX = mapX;
			nowMapY = mapY;
			// スクロール止める
			oldScrollX = scroll.x;
			oldScrollY = scroll.y;

			// キー入力
			//  左
			if ((keys[DIK_LEFT] || keys[DIK_A])) {
				// 左角の判定
				box.leftTopX = (playerPos.x - int(ball.velocity.x)) / mapTileSize;
				box.leftTopY = (playerPos.y) / mapTileSize;
				box.leftBottomX = (playerPos.x - int(ball.velocity.x)) / mapTileSize;
				box.leftBottomY = (playerPos.y + playerTileSize - 1) / mapTileSize;
				// 進む先がブロックじゃなかったら左にスピード分進む
				if (map[box.leftTopY][box.leftTopX] == 0 &&
				    map[box.leftBottomY][box.leftBottomX] == 0) {
					playerPos.x -= int(ball.velocity.x);
					// 左上か左下がブロックだったらスクロールが止まる
					if (map[box.leftTopY][box.leftTopX] == 1 ||
					    map[box.leftBottomY][box.leftBottomX] == 1) {
						scroll.x = oldScrollX;
					}
				}
			}

			// 右
			if ((keys[DIK_RIGHT] || keys[DIK_D])) {
				// 右角の判定
				box.rightTopX =
				    (playerPos.x + playerTileSize - 1 + int(ball.velocity.x)) / mapTileSize;
				box.rightTopY = (playerPos.y) / mapTileSize;
				box.rightBottomX =
				    (playerPos.x + playerTileSize - 1 + int(ball.velocity.x)) / mapTileSize;
				box.rightBottomY = (playerPos.y + playerTileSize - 1) / mapTileSize;
				// 進む先がブロックじゃなかったら右にスピード分進む
				if (map[box.rightTopY][box.rightTopX] == 0 &&
				    map[box.rightBottomY][box.rightBottomX] == 0) {
					playerPos.x += int(ball.velocity.x);
					// スクロールする
					if (scrollMinX < playerPos.x || playerPos.x > scrollMaxX) {
						scroll.x = playerPos.x + screenWidth / 2;
					}
					// 右上か右下がブロックだったらスクロールが止まる
					if (map[box.rightTopY][box.rightTopX] == 1 ||
					    map[box.rightBottomY][box.rightBottomX] == 1) {
						scroll.x = oldScrollX;
					}
				}
			}

			// スクロールする
			if (scrollMinX < playerPos.x || playerPos.x > scrollMaxX) {
				scroll.x = playerPos.x - screenWidth / 2;
			}
			// スクロール止まる
			if (scroll.x < scrollMinX) {
				scroll.x = scrollMinX;
			}
			// スクロール止まる
			if (scroll.x > scrollMaxX) {
				scroll.x = scrollMaxX;
			}

			break;
#pragma endregion

			// シーン終了
			break;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		switch (stage) {
#pragma region Title
			// タイトル
		case TITLE:
			Novice::DrawSprite(0, 0, titleTexture, 1.0f, 1.0f, 0.0f, WHITE);

			Novice::DrawSprite(0, 0, titleTexture, 1.0f, 1.0f, 0.0f, WHITE);
			if (isYesJamp == false) {
				Novice::DrawSprite(0, 500, falseStageYesJump, 1.0f, 1.0f, 0.0f, WHITE);
			}
			if (isYesJamp == true) {
				Novice::DrawSprite(0, 500, trueStageYesJump, 1.0f, 1.0f, 0.0f, WHITE);
			}
			break;
#pragma endregion

#pragma region YESTITLE
			// ジャンプあり
		case YESJAMP:

			for (int y = 0; y < mapY; y++) {
				for (int x = 0; x < mapX; x++) {
					if (map[y][x] == 1) {
						Novice::DrawBox(
						    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y, mapTileSize,
						    mapTileSize, 0.0f, RED, kFillModeSolid);
					}
					if (map[y][x] == 0) {
						Novice::DrawBox(
						    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y, mapTileSize,
						    mapTileSize, 0.0f, BLACK, kFillModeSolid);
					}
				}
			}
			// player表記
			Novice::DrawBox(
			    (playerPos.x) - scroll.x, (int)(playerPos.y) - scroll.y, playerTileSize,
			    playerTileSize, 0.0f, BLUE, kFillModeSolid);
			break;
#pragma endregion

			// シーン終了
			break;
		}

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
