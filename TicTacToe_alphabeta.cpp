#include <SDL/SDL.h>
#include <algorithm>
using namespace std;
#define W 600
#define H 600
SDL_Surface *s;
int done = 0;
SDL_Event event;

int board[3][3];
int step = 0, player = 0;
int iswin()
{
	for (int i = 0; i < 3; i++)
		if (board[i][0] && board[i][0] == board[i][1]
			&& board[i][1] == board[i][2])
			return board[i][0];
	for (int i = 0; i < 3; i++)
		if (board[0][i] && board[0][i] == board[1][i]
			&& board[1][i] == board[2][i])
			return board[0][i];
	if (board[0][0] && board[0][0] == board[1][1]
		&& board[1][1] == board[2][2])
		return board[0][0];
	if (board[0][2] && board[0][2] == board[1][1]
		&& board[1][1] == board[2][0])
		return board[0][2];
	return 0;
}

int think_max(int &x, int &y);
int think_max(int &x, int &y);

int think_min(int &x, int &y)
{
	if (iswin())
		return -1000;
	if (step >= 9)
		return 0;
	int best = -100000, v;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (board[i][j] == 0)
			{
				board[i][j] = -1;
				step++;
				if (iswin() == -1)
				{
					x = i, y = j;
					board[i][j] = 0;
					step--;
					return -100;
				}
				else if (step >= 9)
				{
					x = i, y = j;
					board[i][j] = 0;
					step--;
					return 0;
				}
				else
				{
					v = -think_max(x, y);
					if (best < v)
					{
						x = i, y = j;
						best = v;
					}
					board[i][j] = 0;
					step--;
				}
			}
	return best;
}

int think_max(int &x, int &y)
{
	if (iswin())
		return 1000;
	if (step >= 9)
		return 0;
	int best = -999999, v;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (board[i][j] == 0)
			{
				board[i][j] = 1;
				step++;
				if (iswin() == 1)
				{
					x = i, y = j;
					board[i][j] = 0;
					step--;
					return 100;
				}
				else if (step >= 9)
				{
					x = i, y = j;
					board[i][j] = 0;
					step--;
					return 0;
				}
				else
				{
					v = think_min(x, y);
					if (best < v)
					{
						x = i, y = j;
						best = v;
					}
					board[i][j] = 0;
					step--;
				}
			}
	return best;
}

int alphabeta(int player, int alpha, int beta, int &x, int &y)
{
	if (iswin())
		return iswin() * 1000;
	if (step >= 9)
		return 0;
	int best = -999999, v;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (board[i][j] == 0)
			{
				board[i][j] = player;
				step++;
				if (iswin() == player)
				{
					x = i, y = j;
					board[i][j] = 0;
					step--;
					return 100;
				}
				else if (step >= 9)
				{
					x = i, y = j;
					board[i][j] = 0;
					step--;
					return 0;
				}
				else
				{
					v = alphabeta(-player, alpha, beta, x, y);
					board[i][j] = 0;
					step--;
					if (player < 0)
						alpha = max(alpha, v);
					else
						beta = min(beta, v);
					if (beta <= alpha)
						break;
				}
			}
	return player > 0 ? alpha : beta;
}

void Draw_block(int x, int y, int w, int h, SDL_Color color)
{
	SDL_Rect rect;
	rect.w = w;
	rect.h = h;
	rect.x = x;
	rect.y = y;
	SDL_FillRect(s, &rect, SDL_MapRGB(s->format, color.r, color.g, color.b));
}


void Draw_mouse(int x, int y)
{
	SDL_Color col = { 0, 0, 0xff };
	Draw_block(x, y, W, 1, col);
	Draw_block(x, 0, 1, H, col);
	Draw_block(x, y, 1, H, col);
	Draw_block(0, y, W, 1, col);
}

void Draw_board()
{
	SDL_Color color[3] = {
		{0xff, 0xff, 0x00},
		{0xff, 0xff, 0xff},
		{0x00, 0xff, 0xff}
	};
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			Draw_block(i * 200, j * 200, 190, 190, color[board[i][j] + 1]);
		}
}

int isgameover()
{
	return iswin() != 0 || step >= 9;
}

void Game_reset()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			board[i][j] = 0;
	step = 0;
	player = 1;
}

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	s = SDL_SetVideoMode(W, H, 32, SDL_DOUBLEBUF | SDL_HWSURFACE);
	if (s == NULL)
		return 0;
	Game_reset();
	while (!done)
	{
		SDL_FillRect(s, 0, 0);
		int x, y;
		if (player == 1)
		{
			//think_max(x, y);
			alphabeta(player, -10000, 10000, x, y);
			if (board[x][y] == 0)
			{
				board[x][y] = 1;
				step++;
				player = -1;
				Draw_board();
				SDL_Flip(s);
			}
		}
		Draw_board();
		SDL_PollEvent(&event);
		Draw_mouse(event.motion.x, event.motion.y);
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				done = 1;
				break;
			case SDLK_r:
				Game_reset();
				break;
			}
		}
		if (event.type == SDL_QUIT)
		{
			done = 1;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			//alphabeta(player, -10000, 10000, x, y);
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					if (event.button.x > i * 200
						&& event.button.x < i * 200 + 190
						&& event.button.y > j * 200
						&& event.button.y < j * 200 + 190)
					{
						Draw_block(i * 200, j * 200, 190, 190, (SDL_Color)
								   {
								   255, 0, 0}
						);
						if (board[i][j] == 0 && player == -1 && !isgameover())
						{
							board[i][j] = -1;
							step++;
							player = 1;
						}
					}
		}
		SDL_Flip(s);
	}

	SDL_Quit();
	return 0;
}