#include "raylib.h"
#include "Figura.h"

int main() {
	
	// Resolution
	//int monitor = GetCurrentMonitor();
	
	const int height = 900, width = 1200;
	InitWindow(width, height, "Pong");
	SetTargetFPS(60);

	//ball
	Ball ball;
	ball.SetSpeed();
	ball.SetPosition();
	
	// Icon
	Image icon = LoadImage("resources/concentric-crescents.png");
	SetWindowIcon(icon);
	
	//ball texture
	Image* wskk = &icon;
	ImageResize(wskk, 2*ball.radious, 2*ball.radious);
	Texture2D tpilka = LoadTextureFromImage(icon);
	UnloadImage(icon);

	// Background texture
	Image krowa = LoadImage("resources/background.png");
	wskk = &krowa;
	ImageResize(wskk, width, height);
	Texture2D background = LoadTextureFromImage(krowa);
	UnloadImage(krowa);
	
	// Paddles
	float LPX = (float)GetScreenWidth() * 0.03f;
	float LPY = (float)GetScreenHeight() / 2 ;
	
	float RPX = (float)GetScreenWidth() * 0.97f;
	float RPY = (float)GetScreenHeight() / 2;
	
	Paddle LeftPaddle(LPX, LPY);
	Paddle RightPaddle(RPX, RPY);

	// Left Paddle texture
	Image paletka1 = LoadImage("resources/paletka1.png");
	wskk = &paletka1;
	ImageCrop(wskk, Rectangle{100, 100, LeftPaddle.width, LeftPaddle.height});
	Texture2D p1 = LoadTextureFromImage(paletka1);
	UnloadImage(paletka1);
	
	// Right Paddle texture
	Image paletka2 = LoadImage("resources/paletka2.png");
	wskk = &paletka2;
	ImageCrop(wskk, Rectangle{300, 300, RightPaddle.width, RightPaddle.height});
	Texture2D p2 = LoadTextureFromImage(paletka2);
	UnloadImage(paletka2);
	
	// Audio
	InitAudioDevice();
	Music music = LoadMusicStream("resources/muzyka.mp3");
	Sound s_wall = LoadSound("resources/sciana.wav");         // Load WAV audio file
	Sound s_paddle = LoadSound("resources/paletka.wav");
	Sound s_score = LoadSound("resources/punkt.wav");
	Sound s_cow = LoadSound("resources/muuu.wav");
	
	PlayMusicStream(music);
	SetMusicVolume(music, 0.35f);
	SetSoundPitch(s_paddle, ball.speedX * 0.0008);
	SetSoundPitch(s_wall, ball.speedX * 0.0008);

	// Text
	const char* WinnerText = nullptr;
	const char* ReplayText = nullptr;
	const char* PauseText = nullptr;
	const char* ScoreLText = nullptr;
	const char* ScoreRText = nullptr;
	
	// Game values
	int scoreLeft = 0;
	int scoreRight = 0;
	unsigned int frameCounter = 0;
	bool pause = 1, mpause = 0;
	int bounces = 0;
	float rotation = 0.0f;

	// Main loop
	while (!WindowShouldClose()) {
	
		UpdateMusicStream(music);

		// Pause Game
		if (IsKeyPressed(KEY_LEFT_CONTROL)) pause = !pause;
		
		// Pause/Resume music playing
		if (IsKeyPressed(KEY_M)) {

			mpause = !mpause;

			if (mpause) PauseMusicStream(music);
			else ResumeMusicStream(music);
		}
		
		// While game is not paused
		if (!pause) {
			// Moving the ball
			ball.posX += ball.speedX * GetFrameTime();
			ball.posY += ball.speedY * GetFrameTime();
			rotation += 10.f;
			
			// Moving LeftPaddle
			if (IsKeyDown(KEY_W))
				if (LeftPaddle.posY - LeftPaddle.height / 2 > 0)
					LeftPaddle.posY -= LeftPaddle.speedY * GetFrameTime();

			if (IsKeyDown(KEY_S))
				if (LeftPaddle.posY + LeftPaddle.height / 2 < GetScreenHeight())
					LeftPaddle.posY += LeftPaddle.speedY * GetFrameTime();

			// Moving RightPaddle
			if (IsKeyDown(KEY_UP))
				if (RightPaddle.posY - RightPaddle.height / 2 > 0)
					RightPaddle.posY -= RightPaddle.speedY * GetFrameTime();

			if (IsKeyDown(KEY_DOWN))
				if (RightPaddle.posY + RightPaddle.height / 2 < GetScreenHeight())
					RightPaddle.posY += RightPaddle.speedY * GetFrameTime();
		}

		


		// Bouncing off walls

		if (ball.posY < 0) {
			ball.posY = 0;
			ball.speedY *= -1;
			PlaySound(s_wall);
		}
			
		if (ball.posY >= GetScreenHeight()) {
			ball.posY = GetScreenHeight();
			ball.speedY *= -1;
			PlaySound(s_wall);
		}
		
		// Colissions
		if (CheckCollisionCircleRec(
			Vector2{ball.posX, ball.posY}, ball.radious,
			LeftPaddle.GetRect())) 
		{
			if (ball.speedX < 0) {
				
				// Sound
				PlaySound(s_paddle);
				
				// New ball speed
				ball.speedX *= -1.1f;
				ball.speedY = (ball.posY - LeftPaddle.posY) / (LeftPaddle.height / 2) * ball.speedX / 2;
				
				// New Paddle speed
				LeftPaddle.speedY *= 1.05;
				RightPaddle.speedY *= 1.05;

				// New Sound pitch
				if (ball.speedX * 0.001 < 1.2f) 
					SetSoundPitch(s_paddle, ball.speedX * 0.001);
				if (ball.speedX * 0.001 < 1.05f)
					SetSoundPitch(s_wall, ball.speedX * 0.001);
				
				// count++
				bounces++;
			}
			
		}
		if (CheckCollisionCircleRec(
			Vector2{ball.posX, ball.posY}, ball.radious,
			RightPaddle.GetRect()))
		{
			if (ball.speedX > 0) {
				
				// Sound
				PlaySound(s_paddle);
				
				// New ball speed
				ball.speedX *= -1.1f;
				ball.speedY = (ball.posY - RightPaddle.posY) / (RightPaddle.height / 2) * -ball.speedX / 2;
				
				// New Paddle speed
				LeftPaddle.speedY *= 1.1;
				RightPaddle.speedY *= 1.1;
				
				// New Sound pitch
				if (ball.speedX * 0.001 < 1.2f) 
					SetSoundPitch(s_paddle, ball.speedX * 0.001);
				if (ball.speedX * 0.001 < 1.05f)
					SetSoundPitch(s_wall, ball.speedX * 0.001);
				
				// count++
				bounces++;
			}

		}
		// Win condition
		if (ball.posX >= GetScreenWidth()) {
			WinnerText = "Left Player Wins";
			scoreLeft++;
			ball.SetPosition();
			ball.speedX = 0;
			ball.speedY = 0;
			PlaySound(s_score);
		}

		if (ball.posX <= 0) {
			WinnerText = "Right Player Wins";
			scoreRight++;
			ball.SetPosition();
			ball.speedX = 0;
			ball.speedY = 0;
			PlaySound(s_score);
		}
		// Restart
		if (WinnerText && IsKeyDown(KEY_SPACE)) {
			ball.SetSpeed();
			LeftPaddle.SetSpeed();
			RightPaddle.SetSpeed();
			WinnerText = nullptr;
			ReplayText = nullptr;
			bounces = 0;
			SetSoundPitch(s_paddle, ball.speedX * 0.0008);
			SetSoundPitch(s_wall, ball.speedX * 0.0008);
		}

		// Drawing
		BeginDrawing();
			ClearBackground(BLACK);

			// Draw background texture
			if (bounces >= 5); 
				DrawTexture(background, 0, 0, MAROON);
			
			if (bounces <= 5)
				DrawTexture(background, 0, 0, DARKGRAY);

			// Play cow sound
			if (bounces == 5)
				PlaySound(s_cow);
			
			// Draw Ball and It's textures
			ball.Draw();
			DrawTexturePro(tpilka, 
				Rectangle{0 ,0 , 2 * ball.radious, 2 * ball.radious},
				Rectangle{ ball.posX, ball.posY , 2*ball.radious, 2*ball.radious },
				Vector2{ball.radious, ball.radious },
				rotation,
				WHITE);
			
			// Draw LeftPaddle and It's textures
			LeftPaddle.Draw();
			DrawTexture(p1, 
				LeftPaddle.posX - LeftPaddle.width / 2, 
				LeftPaddle.posY - LeftPaddle.height / 2, 
				WHITE);
			
			// Draw RightPaddle and It's textures
			RightPaddle.Draw();
			DrawTexture(p2, 
				RightPaddle.posX - RightPaddle.width / 2, 
				RightPaddle.posY - RightPaddle.height / 2, 
				WHITE);

			//Draw Score
			const char * ScoreLText = TextFormat("%02i", scoreLeft);
			const char* ScoreRText = TextFormat("%02i", scoreRight);
			
			int fontsizeS = (GetScreenHeight() * 0.07 + GetScreenWidth() * 0.07) / 2;
			int textwidthS = MeasureText(ScoreLText, fontsizeS);
			
			DrawText(ScoreLText, 
				GetScreenWidth() * 0.25 - textwidthS / 2, 
				GetScreenHeight() * 0.1 - fontsizeS / 2, 
				fontsizeS, 
				WHITE);
			DrawText(ScoreRText, 
				GetScreenWidth() * 0.75 - textwidthS / 2, 
				GetScreenHeight() * 0.1 - fontsizeS / 2, 
				fontsizeS, 
				WHITE);
			
			if (WinnerText) {
				rotation = 0.0f;
				// Draw Winner text
				int fontsizeW = (GetScreenHeight() * 0.07 + GetScreenWidth() * 0.07) / 2;
				int textwidthW = MeasureText(WinnerText, fontsizeW);
				DrawText(WinnerText, 
					GetScreenWidth() / 2 - textwidthW / 2, 
					GetScreenHeight() * 0.25 - fontsizeW / 2, 
					fontsizeW, 
					YELLOW);
				
				// Draw Restart Text
				ReplayText = "Press Space To Restart";
				int fontsizeR = (GetScreenHeight() * 0.04 + GetScreenWidth() * 0.04) / 2;
				int textwidthR = MeasureText(ReplayText, fontsizeR);
				if (frameCounter / 30 % 2 == 0)
				DrawText(ReplayText, 
					GetScreenWidth() / 2 - textwidthR / 2, 
					GetScreenHeight() * 0.7 - fontsizeR / 2, 
					fontsizeR, 
					WHITE);
			}
				// Draw Pause Text
			if (pause && !WinnerText) {
				PauseText = "Press CTRL To Unpause";
				int fontsizeP = (GetScreenHeight() * 0.04 + GetScreenWidth() * 0.04) / 2;
				int textwidthP = MeasureText(PauseText, fontsizeP);
				if (frameCounter / 30 % 2 == 0)
					DrawText(PauseText, 
						GetScreenWidth() / 2 - textwidthP / 2, 
						GetScreenHeight() * 0.7 - fontsizeP / 2, 
						fontsizeP, 
						WHITE);
			}

			DrawFPS(10, 10);
		EndDrawing();
		
		frameCounter++;


	}

	// Unload
	UnloadSound(s_paddle);     // Unload sound data
	UnloadSound(s_wall);     // Unload sound data
	UnloadSound(s_score);
	UnloadSound(s_cow);
	
	UnloadTexture(background);
	UnloadTexture(p1);
	UnloadTexture(p2);
	UnloadTexture(tpilka);
	
	PauseText = nullptr;
	ScoreLText = nullptr;
	ScoreRText = nullptr;
	
	CloseAudioDevice();     // Close audio device
	
	CloseWindow();

	return 0;
}
