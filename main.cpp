#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define fps_div 16
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define sprites_w 605
#define sprites_h 559
#define LETTER_MENU_SIZE 32
#define Margin 64
#define MarginWH 20
#define MarginXY 10
#define ILOSC_OPCJI 4
#define Mario_w 15
#define Mario_h 17
#define Mario_x 12
#define Mario_y 201
#define Monkey_w 38
#define Monkey_h 33
#define Monkey_x 9
#define Monkey_y 54
#define barrel_wh 12
#define barrel_x 331
#define barrel_y 43
#define barrel_lv1 11
#define Rmario_x 578
#define Platforms_lv1 7
#define Ladders_lv1 9
#define ladder_thc 23
#define platform_thc 15
#define ladder_margin 5
#define jump_h 22
#define oil_barrel_x 419
#define oil_barrel_y 45
#define oil_barrel_w 16
#define oil_barrel_h 33
#define oil_pos_x 230
#define Max_scores_on_page 3



struct rect_pos {
	int pos_x;
	int pos_y;
	int plat_nr;
	int spawned;
	double moveX;
	double moveY;
	int srednia1;
	int OnLadder;

};

struct Object_pos {
	int pos_x;
	int pos_y;
	int pos_w;
	int numer;
};

struct Player_pos {
	int pos_x;
	int pos_y;
	int plat_nr;
	int on_ladder;
};

struct HighScore {
	int score;
	char player[15];
};

// narysowanie napisu txt na powierzchni screen, zaczynajπc od punktu (x, y)
// charset to bitmapa 128x128 zawierajπca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
		};
	};

void DrawStringMenu(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset) {
	
	int px, py, c;

	
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = LETTER_MENU_SIZE;
	d.h = LETTER_MENU_SIZE;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitScaled(charset, &s, screen, &d);
		x += LETTER_MENU_SIZE;
		text++;
	};
};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt úrodka obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurfaceFULL(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) 
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
	};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y,w,h) kopiowanie (x,y,w,h)
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int paste_sprite_pos_x, int paste_sprite_pos_y,int paste_sprite_size_w, int paste_sprite_size_h, int copy_sprite_pos_x, int copy_sprite_pos_y, int copy_sprite_size_w, int copy_sprite_size_h )
{
	SDL_Rect dest;
	dest.x = paste_sprite_pos_x;
	dest.y = paste_sprite_pos_y;
	dest.w = paste_sprite_size_w;
	dest.h = paste_sprite_size_h;

	SDL_Rect src;
	src.x = copy_sprite_pos_x;
	src.y = copy_sprite_pos_y;
	src.w = copy_sprite_size_w;
	src.h = copy_sprite_size_h;
	SDL_BlitScaled(sprite, &src, screen, &dest);
};


// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
	};


// rysowanie linii o d≥ugoúci l w pionie (gdy dx = 0, dy = 1) 
// bπdü poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for(int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
 		x += dx;
		y += dy;
		};
	};


// rysowanie prostokπta o d≥ugoúci bokÛw l i k
// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
	};

void DrawRectangleNoFill(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor) {
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
};

int Initializationsdl()
{
	// okno konsoli nie jest widoczne, jeøeli chcemy zobaczyÊ
	// komunikaty wypisywane printf-em trzeba w opcjach:
	// project -> szablon2 properties -> Linker -> System -> Subsystem
	// zmieniÊ na "Console"
	// console window is not visible, to see the printf output
	// the option:
	// project -> szablon2 properties -> Linker -> System -> Subsystem
	// must be changed to "Console"
	//printf("wyjscie printfa trafia do tego okienka\n");
	//printf("printf output goes here\n");
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		exit(1);
		return 1;
	}
	return 0;
	
}

int rc_check(int rc)
{
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		exit(1);
		return 1;
	};
	return 0;
}

void set_renderer_and_window_title_and_cursor(SDL_Renderer *renderer, SDL_Window *window)
{
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Projekt 2 - Donkey Kong");

	// wy≥πczenie widocznoúci kursora myszy
	//SDL_ShowCursor(SDL_DISABLE);


}

SDL_Surface* set_screen_surface()
{
	SDL_Surface* screen;
	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	return screen;
}

SDL_Texture* set_scrtex_texture(SDL_Renderer* renderer)
{
	SDL_Texture* scrtex;
	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);
	return scrtex;

}

int set_color(SDL_Surface* screen,int R, int G, int B, int A)
{
	
	int kolor = SDL_MapRGBA(screen->format, R, G, B, A);
	return kolor;
}

void set_colors(SDL_Surface* screen, int *czarny, int *zielony, int *czerwony, int *niebieski, int *pomarancz, int* white)
{
	*czarny = set_color(screen, 0, 0, 0, 255);
	*zielony = set_color(screen, 0, 255, 0, 255);
	*czerwony = set_color(screen, 255, 0, 0, 255);
	*niebieski = set_color(screen, 17, 17, 204, 255);
	*pomarancz = set_color(screen,255, 165, 0, 255);
	*white = set_color(screen, 255, 255, 255, 255);
}

void Menu_Ui_initalization(SDL_Surface* screen, char text[128], SDL_Surface* litery, SDL_Rect* start_pos,SDL_Rect* scores_pos, SDL_Rect* stage_pos, SDL_Rect* exit_pos, SDL_Rect* cursor_pos, int czerwony)
{

	sprintf(text, "Start");
	start_pos->x = screen->w / 2 - (strlen(text) * LETTER_MENU_SIZE) / 2;
	start_pos->y = (screen->h / ILOSC_OPCJI) * 1 - LETTER_MENU_SIZE - Margin/2;
	start_pos->w = strlen(text) * LETTER_MENU_SIZE;
	DrawStringMenu(screen, start_pos->x, start_pos->y, text, litery);

	sprintf(text, "Scores");
	scores_pos->x = screen->w/2 - (strlen(text) * LETTER_MENU_SIZE)/2;
	scores_pos->y = (((screen->h / ILOSC_OPCJI) * 2) - LETTER_MENU_SIZE) - Margin/2;
	scores_pos->w = strlen(text) * LETTER_MENU_SIZE;
	DrawStringMenu(screen, scores_pos->x, scores_pos->y, text, litery);

	sprintf(text, "Stage");
	stage_pos->x = screen->w / 2 - (strlen(text) * LETTER_MENU_SIZE) / 2;
	stage_pos->y = (((screen->h / ILOSC_OPCJI) * 3) - LETTER_MENU_SIZE) - Margin/2;
	stage_pos->w = strlen(text) * LETTER_MENU_SIZE;
	DrawStringMenu(screen, stage_pos->x, stage_pos->y, text, litery);
	sprintf(text, "Not implemented yet");
	DrawString(screen, stage_pos->x , stage_pos->y+33, text, litery);

	sprintf(text, "Exit");
	exit_pos->x = screen->w / 2 - (strlen(text) * LETTER_MENU_SIZE) / 2;
	exit_pos->y = (((screen->h / ILOSC_OPCJI) * 4) - LETTER_MENU_SIZE) - Margin/2;
	exit_pos->w = strlen(text) * LETTER_MENU_SIZE;
	DrawStringMenu(screen, exit_pos->x, exit_pos->y, text, litery);

	if (cursor_pos->x == 0)
	{	
		cursor_pos->x = start_pos->x;
		cursor_pos->y = start_pos->y;
		cursor_pos->w = start_pos->w;
	}
	
	//rysowanie kursora
	
	DrawRectangleNoFill(screen, cursor_pos->x - MarginXY, cursor_pos->y - MarginXY, cursor_pos->w  +MarginWH, cursor_pos->h + MarginWH, czerwony);
	DrawRectangleNoFill(screen, cursor_pos->x - MarginXY-1, cursor_pos->y - MarginXY-1, cursor_pos->w + MarginWH+2, cursor_pos->h + MarginWH+2, czerwony);
	DrawRectangleNoFill(screen, cursor_pos->x - MarginXY-2, cursor_pos->y - MarginXY-2, cursor_pos->w + MarginWH+4, cursor_pos->h + MarginWH+4, czerwony);
}

int surface_check(SDL_Surface* surface, SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer)
{	
	if (surface == NULL) {
		printf("SDL_LoadBMP error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	return 0;
}

void cursor_check_UP(SDL_Rect* start_pos,SDL_Rect* scores_pos, SDL_Rect* stage_pos, SDL_Rect* exit_pos, SDL_Rect* cursor_pos)
 {

	if (cursor_pos->y == scores_pos->y && cursor_pos->x == scores_pos->x)
	{
		cursor_pos->x = start_pos->x;
		cursor_pos->y = start_pos->y;
		cursor_pos->w = start_pos->w;
		
	}
	else if (cursor_pos->y == stage_pos->y && cursor_pos->x == stage_pos->x)
	{
		cursor_pos->x = scores_pos->x;
		cursor_pos->y = scores_pos->y;
		cursor_pos->w = scores_pos->w;
		
	}
	else if (cursor_pos->y == exit_pos->y && cursor_pos->x == exit_pos->x)
	{
		cursor_pos->x = stage_pos->x;
		cursor_pos->y = stage_pos->y;
		cursor_pos->w = stage_pos->w;
		
	}
	
}

void cursor_check_DOWN(SDL_Rect* start_pos,SDL_Rect* scores_pos, SDL_Rect* stage_pos, SDL_Rect* exit_pos, SDL_Rect* cursor_pos) 
{
	if (cursor_pos->y == start_pos->y && cursor_pos->x == start_pos->x)
	{
		cursor_pos->x = scores_pos->x;
		cursor_pos->y = scores_pos->y;
		cursor_pos->w = scores_pos->w;
	}
	else if (cursor_pos->y == stage_pos->y && cursor_pos->x == stage_pos->x)
	{
		cursor_pos->x = exit_pos->x;
		cursor_pos->y = exit_pos->y;
		cursor_pos->w = exit_pos->w;
		
	}
	else if (cursor_pos->y == scores_pos->y && cursor_pos->x == scores_pos->x) 
	{
		cursor_pos->x = stage_pos->x;
		cursor_pos->y = stage_pos->y;
		cursor_pos->w = stage_pos->w;
		
	}
	

}

void cursor_color_switch(int *kolor,int pomarancz, int czerwony)
{	
	if (*kolor == pomarancz)
	{
		*kolor = czerwony;
	}
	else if (*kolor == czerwony)
	{
		*kolor = pomarancz;
	}

}

int option_choice_check(SDL_Rect* start_pos,SDL_Rect* scores_pos, SDL_Rect* stage_pos, SDL_Rect* exit_pos, SDL_Rect* cursor_pos)
{

	if (cursor_pos->y == start_pos->y && cursor_pos->x == start_pos->x)
	{
		return 2;
	}
	else if (cursor_pos->y == stage_pos->y && cursor_pos->x == stage_pos->x)
	{	
		return 3;
	}
	else if (cursor_pos->y == scores_pos->y && cursor_pos->x == scores_pos->x)
	{
		return 4;
	}
	else if (cursor_pos->y == exit_pos->y && cursor_pos->x == exit_pos->x)
	{
		return 1;
	}
	return 0;
}

int Menu_initalization_and_logic(SDL_Renderer* renderer, SDL_Surface* screen, SDL_Texture* scrtex,SDL_Window* window, int czarny, int pomarancz, int czerwony )
{

	int tick1, tick2, quit,kolor, menu_choice = 0;
	double delta;
	double color_change = 0.5;  
	double timeSinceColorChange = 0.0;
	char text[128];
	SDL_Event event;

	SDL_Rect start_pos;
	start_pos.x = 0;
	start_pos.y = 0;
	start_pos.w = 0;
	start_pos.h = LETTER_MENU_SIZE;

	SDL_Rect scores_pos;
	scores_pos.x = 0;
	scores_pos.y = 0;
	scores_pos.w = 0;
	scores_pos.h = LETTER_MENU_SIZE;

	SDL_Rect stage_pos;
	scores_pos.x = 0;
	scores_pos.y = 0;
	scores_pos.w = 0;
	scores_pos.h = LETTER_MENU_SIZE;

	SDL_Rect exit_pos;
	scores_pos.x = 0;
	scores_pos.y = 0;
	scores_pos.w = 0;
	scores_pos.h = LETTER_MENU_SIZE;

	SDL_Rect cursor_pos;
	cursor_pos.x = 0;
	cursor_pos.y = 0;
	cursor_pos.w = 0;
	cursor_pos.h = LETTER_MENU_SIZE;

	SDL_Surface* litery = SDL_LoadBMP("./cs8x8.bmp");
	surface_check(litery,screen,scrtex,window,renderer);
	SDL_SetColorKey(litery, true, 0x000000); // transparent color

	tick1 = SDL_GetTicks();
	quit = 0;
	kolor = czerwony;	

	while (!quit) {
		tick2 = SDL_GetTicks();
		delta = (tick2 - tick1) * 0.001;
		tick1 = tick2; 
		 
		SDL_FillRect(screen, NULL, czarny); // wyplenij ekran czarnym
		
		timeSinceColorChange += delta;
		
		if (timeSinceColorChange >= color_change) {
			cursor_color_switch(&kolor, pomarancz, czerwony);
			timeSinceColorChange = 0.0;
		}
		
		Menu_Ui_initalization(screen, text, litery, &start_pos,&scores_pos,&stage_pos,&exit_pos, &cursor_pos, kolor);


		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL); 
		SDL_RenderPresent(renderer);

		// obs≥uga zdarzeÒ (o ile jakieú zasz≥y) / handling of events (if there were any)
		while (SDL_PollEvent(&event)) {
			
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					quit = 1;
					break;	
				case SDLK_UP:	
					cursor_check_UP(&start_pos,&scores_pos, &stage_pos, &exit_pos, &cursor_pos);
					break;
				case SDLK_DOWN:
					cursor_check_DOWN(&start_pos,&scores_pos, &stage_pos, &exit_pos, &cursor_pos);
					break;
				case SDLK_RETURN:
					menu_choice = option_choice_check(&start_pos,&scores_pos, &stage_pos, &exit_pos, &cursor_pos);
					break;
				}
				break;
			case SDL_KEYUP:
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			}
		}
		if (menu_choice != 0)
			break;
	};

	SDL_FreeSurface(litery);
	return menu_choice;
};

void Game_UI_visualization(SDL_Surface* screen, SDL_Surface* sprites, int czerwony, char text[128], SDL_Surface* litery, double worldTime, double fps, SDL_Surface* spritesR, struct Player_pos* player, double delta, struct rect_pos* barrel, int Lives, int Hitdetection, int *LastMove, double*AnimationCheck, const Uint8* keystate, int Jump, int Fall, double SpawnTime, double *BarrelAC, int points, int *Purse )
{
	
	DrawSurface(screen, sprites, 1, 1, screen->w, screen->h, 10,300, 220, 250); //plansza ze spritu, 4 ostatnie numery to wspolrzedne i wymiary planszy ze spritu
	DrawRectangleNoFill(screen, 4, 3, SCREEN_WIDTH - 8, 36, czerwony);
	DrawRectangleNoFill(screen, 3, 2, SCREEN_WIDTH - 6, 38, czerwony);
	DrawRectangleNoFill(screen, 2, 1, SCREEN_WIDTH - 4, 40, czerwony); // ramka 3 warstwowa na gorze ekranu, wartosci ustawione "zeby bylo ladnie"
	DrawSurface(screen, sprites, 50, 145 - Monkey_h * 1.5, Monkey_w * 1.5, Monkey_h * 1.5, Monkey_x, Monkey_y, Monkey_w, Monkey_h); //malpa
	DrawSurface(screen, sprites, oil_pos_x, 396, oil_barrel_w * 2, oil_barrel_h * 2, oil_barrel_x, oil_barrel_y, oil_barrel_w, oil_barrel_h); // wyswietlanie tekstury, ktora niszczy beczki
	DrawSurface(screen, sprites, 284, 68, 16, 22, 7, 19, 16, 22); // princessa
	
	sprintf(text, "%d", points);
	DrawString(screen, 560 - strlen(text) * 8 / 2, 78, text, litery);
	
	if(*Purse == 0)
	DrawSurface(screen, sprites, 580, 263, 16, 16, 286, 128, 10, 10); // purse


	for (int i = 0; i < barrel_lv1; i++)
	{
		if (barrel[i].spawned == 1)
		{
			*BarrelAC += delta;
			if (barrel[i].OnLadder == 1)
			{
				DrawSurface(screen, sprites, barrel[i].pos_x - barrel_wh / 2, barrel[i].pos_y - barrel_wh, barrel_wh + 15, barrel_wh, 330, 24, 27, 9);
			}
			else if (*BarrelAC >= 0 && *BarrelAC <= 0.1)
			{
				DrawSurface(screen, sprites, barrel[i].pos_x - barrel_wh / 2, barrel[i].pos_y - barrel_wh, barrel_wh, barrel_wh, barrel_x, barrel_y, barrel_wh, barrel_wh); // wyswietlanie beczki
			}
			else if (*BarrelAC >= 0.1 && *BarrelAC <= 0.2)
			{
				DrawSurface(screen, sprites, barrel[i].pos_x - barrel_wh / 2, barrel[i].pos_y - barrel_wh, barrel_wh, barrel_wh, barrel_x + 16, barrel_y, barrel_wh, barrel_wh);
			}
			else if (*BarrelAC >= 0.2 && *BarrelAC <= 0.3)
			{
				DrawSurface(screen, sprites, barrel[i].pos_x - barrel_wh / 2, barrel[i].pos_y - barrel_wh, barrel_wh, barrel_wh, barrel_x + 32, barrel_y, barrel_wh, barrel_wh);
			}
			else if (*BarrelAC >= 0.3 && *BarrelAC <= 0.4)
			{
				DrawSurface(screen, sprites, barrel[i].pos_x - barrel_wh / 2, barrel[i].pos_y - barrel_wh, barrel_wh, barrel_wh, barrel_x + 49, barrel_y, barrel_wh, barrel_wh);
			}
			else if (*BarrelAC >= 0.4)
			{
				*BarrelAC = 0;
				DrawSurface(screen, sprites, barrel[i].pos_x - barrel_wh / 2, barrel[i].pos_y - barrel_wh, barrel_wh, barrel_wh, barrel_x, barrel_y, barrel_wh, barrel_wh);
			}
		}
	}
	

	if (player->on_ladder == 1)
	{
		if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_UP])
		{

			*AnimationCheck += delta;
			if (*AnimationCheck >= 0 && *AnimationCheck <= 0.1)
			{
				DrawSurface(screen, spritesR, player->pos_x - (Mario_w * 1.5) / 2, player->pos_y - (Mario_h * 1.5), Mario_w * 1.5, Mario_h * 1.5, 523, Mario_y + 1, Mario_w, Mario_h);
			}
			else if (*AnimationCheck >= 0.1 && *AnimationCheck <= 0.2)
			{
				DrawSurface(screen, sprites, player->pos_x - (Mario_w * 1.5) / 2, player->pos_y - (Mario_h * 1.5), Mario_w * 1.5, Mario_h * 1.5, 69, Mario_y + 1, Mario_w, Mario_h);
			}
			else if (*AnimationCheck >= 0.2)
			{
				DrawSurface(screen, spritesR, player->pos_x - (Mario_w * 1.5) / 2, player->pos_y - (Mario_h * 1.5), Mario_w * 1.5, Mario_h * 1.5, 523, Mario_y + 1, Mario_w, Mario_h);
				*AnimationCheck = 0;
			}
		}
		else if (!keystate[SDL_SCANCODE_DOWN] && !keystate[SDL_SCANCODE_UP])
		{
			DrawSurface(screen, sprites, player->pos_x - (Mario_w * 1.5) / 2, player->pos_y - (Mario_h * 1.5), Mario_w * 1.5, Mario_h * 1.5, 69, Mario_y + 1, Mario_w, Mario_h);
		}


	}

	if (Fall == 1 || Jump == 1)
	{
		if (*LastMove == 1)
		{
			DrawSurface(screen, sprites, player->pos_x - (18 * 1.5) / 2, player->pos_y - (Mario_h * 1.5), 18 * 1.5, Mario_h * 1.5, 28, Mario_y, 18, Mario_h);
		}
		else if (*LastMove == 0)
		{
			DrawSurface(screen, spritesR, player->pos_x - (18 * 1.5) / 2, player->pos_y - (Mario_h * 1.5), 18 * 1.5, Mario_h * 1.5, 562, Mario_y, 18, Mario_h);
		}
	} 

	else if ((!keystate[SDL_SCANCODE_RIGHT] && !keystate[SDL_SCANCODE_LEFT]) && player->on_ladder == 0 || (keystate[SDL_SCANCODE_RIGHT] && keystate[SDL_SCANCODE_LEFT]) && player->on_ladder == 0)
	{
		if (*LastMove == 1)
		{
			DrawSurface(screen, sprites, player->pos_x - (Mario_w * 1.5) / 2, player->pos_y - (Mario_h * 1.5), Mario_w * 1.5, Mario_h * 1.5, Mario_x, Mario_y, Mario_w, Mario_h);
		}
		else if (*LastMove == 0)
		{
			DrawSurface(screen, spritesR, player->pos_x - (Mario_w * 1.5) / 2, player->pos_y - (Mario_h * 1.5), Mario_w * 1.5, Mario_h * 1.5, sprites_w - 12 - 15, 201, 15, 17);
		}

	} 
	
	else if (keystate[SDL_SCANCODE_RIGHT] && !keystate[SDL_SCANCODE_LEFT] && player->on_ladder == 0 && (Fall == 0 || Jump == 0))
	{ 
		if (*LastMove != 0)
				*AnimationCheck = 0;
		*AnimationCheck += delta;
		*LastMove = 0;
		if (*AnimationCheck >= 0 && *AnimationCheck <= 0.1 || *AnimationCheck >= 0.2 && *AnimationCheck <= 0.3)
		{
			DrawSurface(screen, spritesR, player->pos_x - (Mario_w * 1.5) / 2, player->pos_y - (Mario_h * 1.5), Mario_w * 1.5, Mario_h * 1.5, sprites_w - 12 - 15, 201, 15, 17);
		}
		else if (*AnimationCheck >= 0.1 && *AnimationCheck <= 0.2)
		{
			DrawSurface(screen, spritesR, player->pos_x - (18 * 1.5) / 2, player->pos_y - (Mario_h * 1.5), 18 * 1.5, Mario_h * 1.5, 562, Mario_y, 18, Mario_h);
		}
		else if (*AnimationCheck >= 0.3 && *AnimationCheck < 0.4)
		{
			DrawSurface(screen, spritesR, player->pos_x - (18 * 1.5) / 2, player->pos_y - (Mario_h * 1.5), 18 * 1.5, Mario_h * 1.5, 542, Mario_y, 18, Mario_h);
		}
		else if (*AnimationCheck >= 0.4)
		{
			DrawSurface(screen, spritesR, player->pos_x - (Mario_w * 1.5) / 2, player->pos_y - (Mario_h * 1.5), Mario_w * 1.5, Mario_h * 1.5, sprites_w - 12 - 15, 201, 15, 17);
			*AnimationCheck = 0;
		}
	} 

	else if (keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_RIGHT] && player->on_ladder == 0 && (Fall == 0 || Jump == 0))
	{
		if (*LastMove != 1)
			*AnimationCheck = 0;

		*AnimationCheck += delta;
		*LastMove = 1;

			if (*AnimationCheck >= 0 && *AnimationCheck <= 0.1 || *AnimationCheck >= 0.20 && *AnimationCheck <= 0.3)
			{
				DrawSurface(screen, sprites, player->pos_x - (Mario_w * 1.5) / 2, player->pos_y - (Mario_h * 1.5), Mario_w * 1.5, Mario_h * 1.5, Mario_x, Mario_y, Mario_w, Mario_h);
			}
			else if (*AnimationCheck >= 0.1 && *AnimationCheck <= 0.20)
			{
				DrawSurface(screen, sprites, player->pos_x - (18 * 1.5) / 2, player->pos_y - (Mario_h * 1.5), 18 * 1.5, Mario_h * 1.5, 28, Mario_y, 18, Mario_h);
			}
			else if (*AnimationCheck >= 0.3 && *AnimationCheck < 0.4)
			{
				DrawSurface(screen, sprites, player->pos_x - (18 * 1.5) / 2, player->pos_y - (Mario_h * 1.5), 18 * 1.5, Mario_h * 1.5, 48, Mario_y, 18, Mario_h);
			}
			else if (*AnimationCheck >= 0.40)
			{
				*AnimationCheck = 0;
				DrawSurface(screen, sprites, player->pos_x - (18 * 1.5) / 2, player->pos_y - (Mario_h * 1.5), 18 * 1.5, Mario_h * 1.5, 48, Mario_y, 18, Mario_h);
			}
	}
	
	if (SpawnTime > 2.0)
	{
		DrawSurface(screen,sprites,50,90, 16, 13, 539, 123, 16,13 ); //warning
	}
	
	if (Hitdetection == 0)
	{
		sprintf(text, "TIME = %.1lf s  %.0lf FPS  Podpunkty wykonane:A,C,D,E,F,G,H", worldTime, fps);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, litery);
		sprintf(text, "ESC - QUIT    N - RESTART");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, litery);
		
	}
	if (Hitdetection == 1)
	{
		sprintf(text, "YOU GOT HIT, CONTINUE ?");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, litery);
		sprintf(text, "ENTER - CONTINUE");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, litery);
	}

	sprintf(text, "LIVES: ");
	DrawString(screen, 10, 46, text, litery);
	
	for (int k = 1; k <= Lives; k++)
	{
		DrawSurface(screen, sprites, 40 + k * 20, 43, 15, 12, 175, 163, 15, 12);
	}

}

void Game_platform_initialization(struct Object_pos* platform, struct Player_pos* player,  struct rect_pos* barrel)
{
	player->pos_x = 1 + (Mario_w*1.5) /2;
	player->pos_y = 460;
	player->plat_nr = 1;
	player->on_ladder = 0;

	platform[0].pos_x = 0;
	platform[0].pos_y = 459;
	platform[0].pos_w = 639;
	platform[0].numer = 1;

	platform[1].pos_x = 0;
	platform[1].pos_y = 405;
	platform[1].pos_w = 606;
	platform[1].numer = 2;

	platform[2].pos_x = 47;
	platform[2].pos_y = 341;
	platform[2].pos_w = 592;
	platform[2].numer = 3;

	platform[3].pos_x = 0;
	platform[3].pos_y = 278;
	platform[3].pos_w = 606;
	platform[3].numer = 4;

	platform[4].pos_x = 47;
	platform[4].pos_y = 214;
	platform[4].pos_w = 592;
	platform[4].numer = 5;

	platform[5].pos_x = 0;
	platform[5].pos_y = 151;
	platform[5].pos_w = 606;
	platform[5].numer = 6;

	platform[6].pos_x = 257;
	platform[6].pos_y = 89;
	platform[6].pos_w = 145;
	platform[6].numer = 7;

}

void Game_ladder_initialization(struct Object_pos* ladder) 
{
	
	ladder[0].pos_x = 536;
	ladder[0].pos_y = 419;
	ladder[0].pos_w = 32;
	ladder[0].numer = 1;

	ladder[1].pos_x = 280;
	ladder[1].pos_y = 348;
	ladder[1].pos_w = 47;
	ladder[1].numer = 2;

	ladder[2].pos_x = 94;
	ladder[2].pos_y = 355;
	ladder[2].pos_w = 32;
	ladder[2].numer = 2;

	ladder[3].pos_x = 327;
	ladder[3].pos_y = 284;
	ladder[3].pos_w = 47;
	ladder[3].numer = 3;

	ladder[4].pos_x = 535;
	ladder[4].pos_y = 292;
	ladder[4].pos_w = 32;
	ladder[4].numer = 3;

	ladder[5].pos_x = 211;
	ladder[5].pos_y = 225;
	ladder[5].pos_w = 39;
	ladder[5].numer = 4;

	ladder[6].pos_x = 94;
	ladder[6].pos_y = 229;
	ladder[6].pos_w = 31;
	ladder[6].numer = 4;

	ladder[7].pos_x = 536;
	ladder[7].pos_y = 165;
	ladder[7].pos_w = 32;
	ladder[7].numer = 5;

	ladder[8].pos_x = 373;
	ladder[8].pos_y = 105;
	ladder[8].pos_w = 40;
	ladder[8].numer = 6;


}

void Barrel_move_right(struct rect_pos* barrel, double etiSpeed, double delta, int i)
{
	barrel[i].moveX += etiSpeed * delta;
	barrel[i].pos_x = round(barrel[i].moveX);
}

void Barrel_move_left(struct rect_pos* barrel, double etiSpeed, double delta, int i)
{
	barrel[i].moveX -= etiSpeed * delta;
	barrel[i].pos_x = round(barrel[i].moveX);
}

void Barrel_move_down(struct rect_pos* barrel, double etiSpeed, double delta, int i)
{
	barrel[i].moveY += etiSpeed * delta;
	barrel[i].pos_y = round(barrel[i].moveY);
}

void Player_move_right(struct Player_pos* player, double etiSpeed, double delta, double* moveX)
{
	
	*moveX += etiSpeed * delta;
	player->pos_x = round(*moveX);
}

void Player_move_up(struct Player_pos* player, double etiSpeed, double delta, double* moveY)
{

	*moveY -= etiSpeed * delta;
	player->pos_y = round(*moveY);
}

void Player_move_down(struct Player_pos* player, double etiSpeed, double delta, double* moveY)
{

	*moveY += etiSpeed * delta;
	player->pos_y = round(*moveY);
}

void Player_move_left(struct Player_pos* player, double etiSpeed, double delta, double* moveX)
{
	*moveX -= etiSpeed * delta;
	player->pos_x = round(*moveX);

}

void FPS_60_LOCK(Uint32 t1) {
	Uint32 fps_get = SDL_GetTicks() - t1;
	if (fps_get < fps_div)
	SDL_Delay(fps_div - fps_get);
}

void Player_move_right_check_plat1(struct Player_pos* player, double etiSpeed, double delta, double* moveX, struct Object_pos* platform, int* srednia1, int* FirstMove, int* EndingJumpingPos, int* PlayerJumping)
{
	const int x_pos_platform_up = 280;
	int nr = player->plat_nr - 1;

	if (player->pos_x + delta * etiSpeed + (Mario_w * 1.5) / 2 < SCREEN_WIDTH)
	{
		Player_move_right(player, etiSpeed, delta, moveX);

		if (player->pos_x > x_pos_platform_up && player->pos_x + (Mario_w * 1.5) / 2 < SCREEN_WIDTH)
		{
			double one = (player->pos_x - x_pos_platform_up) / 46;
			double two = (player->pos_x - x_pos_platform_up) / 47;
			int srednia2 = *srednia1;
			*srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = *srednia1;
				*FirstMove = 0;
			}
			if (*srednia1 != srednia2 && *srednia1 == 3 && srednia2 == 2)
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos -= 1;
				}
				else
				{
					
					player->pos_y -= 1;
				}
				

			}
			else if (*srednia1 != srednia2)
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos -= 2;
				}
				else
				{
					
					player->pos_y -= 2;
				}
			}
		}
	}
}

void Player_move_left_check_plat1(struct Player_pos* player, double etiSpeed, double delta, double* moveX, struct Object_pos* platform, int* srednia1, int* FirstMove, int* EndingJumpingPos, int* PlayerJumping)
{
	const int x_pos_platform_up = 280;
	int nr = player->plat_nr - 1;

	if (player->pos_x - delta * etiSpeed - (Mario_w * 1.5) / 2 > 0)
	{
		Player_move_left(player, etiSpeed, delta, moveX);

		if (player->pos_x > x_pos_platform_up && player->pos_x - (Mario_w * 1.5) / 2 > 0)
		{
			double one = (player->pos_x - x_pos_platform_up) / 46;
			double two = (player->pos_x - x_pos_platform_up) / 47;
			int srednia2 = *srednia1;
			*srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = *srednia1;
				*FirstMove = 0;
			}
			if (*srednia1 != srednia2 && *srednia1 == 2 && srednia2 == 3)
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos += 1;
				} 
				else
				{
					player->pos_y += 1;
				}
					
				
			}
			else if(* srednia1 != srednia2)
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos += 2;
				}
				else
				{
					player->pos_y += 2;
				}
					
			}
		}
	}
}

void Player_move_left_check_plat2(struct Player_pos* player, double etiSpeed, double delta, double* moveX, struct Object_pos* platform, int* srednia1, int* FirstMove, int* EndingJumpingPos, int* PlayerJumping)
{
	const int x_pos_platform_up = 610;
	int nr = player->plat_nr - 1;

	if (player->pos_x - delta * etiSpeed - (Mario_w * 1.5) / 2 > 0)
	{
		Player_move_left(player, etiSpeed, delta, moveX);

		if (player->pos_x < x_pos_platform_up && player->pos_x - (Mario_w * 1.5) / 2 > 0)
		{
			double one = (x_pos_platform_up - player->pos_x) / 46;
			double two = (x_pos_platform_up - player->pos_x) / 47;
			int srednia2 = *srednia1;
			*srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = *srednia1;
				*FirstMove = 0;
			}
			if (*srednia1 != srednia2 && *srednia1 == 3 && srednia2 == 2)
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos -= 1;
				}
				else
				{
					player->pos_y -= 1;
				}

				
			}
			else if (*srednia1 != srednia2)
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos -= 2;
				}
				else
				{
					player->pos_y -= 2;
				}
			}
		}
	}
}

void Player_move_right_check_plat2(struct Player_pos* player, double etiSpeed, double delta, double* moveX, struct Object_pos* platform, int* srednia1, int* FirstMove, int* EndingJumpingPos, int* PlayerJumping, int* PlayerFalling)
{
	const int x_pos_platform_up = 610;
	int nr = player->plat_nr - 1;

	if (player->pos_x < x_pos_platform_up)
	{
		Player_move_right(player, etiSpeed, delta, moveX);

		if (player->pos_x < x_pos_platform_up && player->pos_x + (Mario_w * 1.5) / 2 < SCREEN_WIDTH)
		{
			double one = (x_pos_platform_up - player->pos_x) / 46;
			double two = (x_pos_platform_up - player->pos_x) / 47;
			int srednia2 = *srednia1;
			*srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = *srednia1;
				*FirstMove = 0;
			}
			if (*srednia1 != srednia2 && *srednia1 == 2 && srednia2 == 3)
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos += 1;
				}
				else
				{
					player->pos_y += 1;
				}
				

			}
			else if (*srednia1 != srednia2)
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos += 2;
				}
				else
				{
					player->pos_y += 2;
				}
			}
		}
	}
	else if (player->pos_x >= x_pos_platform_up)
	{
		Player_move_right(player, etiSpeed, delta, moveX);
		*PlayerJumping = 0;
		* PlayerFalling = 1;
		*EndingJumpingPos = 447;
		player->plat_nr -= 1;
		*FirstMove = 1;
		
	}
}

void Player_move_right_check_plat3(struct Player_pos* player, double etiSpeed, double delta, double* moveX, struct Object_pos* platform, int* srednia1, int* FirstMove, int* EndingJumpingPos, int* PlayerJumping)
{
	const int x_pos_platform_up = 43;
	int nr = player->plat_nr - 1;

	if (player->pos_x + delta * etiSpeed + (Mario_w * 1.5) / 2 < SCREEN_WIDTH)
	{
		Player_move_right(player, etiSpeed, delta, moveX);

		if (player->pos_x > x_pos_platform_up && player->pos_x + (Mario_w * 1.5) / 2 < SCREEN_WIDTH)
		{
			double one = (player->pos_x - x_pos_platform_up) / 46;
			double two = (player->pos_x - x_pos_platform_up) / 47;
			int srednia2 = *srednia1;
			*srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = *srednia1;
				*FirstMove = 0;
			}
			if (*srednia1 != srednia2 && (*srednia1 == 3 && srednia2 == 2)|| (*srednia1 == 8 && srednia2 == 7))
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos -= 1;
				}
				else
				{
					player->pos_y -= 1;
				}
				

			}
			else if (*srednia1 != srednia2)
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos -= 2;
				}
				else
				{
					player->pos_y -= 2;
				}

			}
		}
	}
}

void Player_move_left_check_plat3(struct Player_pos* player, double etiSpeed, double delta, double* moveX, struct Object_pos* platform, int* srednia1, int* FirstMove, int* EndingJumpingPos, int* PlayerJumping, int* PlayerFalling)
{
	const int x_pos_platform_up = 43; 
	int nr = player->plat_nr - 1;

	if (player->pos_x >= x_pos_platform_up)
	{
		Player_move_left(player, etiSpeed, delta, moveX);

		if (player->pos_x > x_pos_platform_up && player->pos_x - (Mario_w * 1.5) / 2 > 0)
		{
			double one = (player->pos_x - x_pos_platform_up) / 46;
			double two = (player->pos_x - x_pos_platform_up) / 47;
			int srednia2 = *srednia1;
			*srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = *srednia1;
				*FirstMove = 0;
			}
			if (*srednia1 != srednia2 &&  (*srednia1 == 2 && srednia2 == 3) || (*srednia1 == 7 && srednia2 == 8))
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos += 1;
				}
				else
				{
					player->pos_y += 1;
				}
				
			}
			else if (*srednia1 != srednia2)
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos += 2;
				}
				else
				{
					player->pos_y += 2;
				}
			}
		}
	}
	else if (player->pos_x <= x_pos_platform_up)
	{
		Player_move_left(player, etiSpeed, delta, moveX);
		*PlayerJumping = 0;
		*PlayerFalling = 1;
		*EndingJumpingPos = 383;
		player->plat_nr -= 1;
		*FirstMove = 1;
	}
}

void Player_move_left_check_plat4(struct Player_pos* player, double etiSpeed, double delta, double* moveX, struct Object_pos* platform, int* srednia1, int* FirstMove, int* EndingJumpingPos, int* PlayerJumping)
{
	const int x_pos_platform_up = 610;
	int nr = player->plat_nr - 1;

	if (player->pos_x - delta * etiSpeed - (Mario_w * 1.5) / 2 > 0)
	{
		Player_move_left(player, etiSpeed, delta, moveX);

		if (player->pos_x < x_pos_platform_up && player->pos_x - (Mario_w * 1.5) / 2 >= 0)
		{
			double one = (x_pos_platform_up - player->pos_x) / 46;
			double two = (x_pos_platform_up - player->pos_x) / 47;
			int srednia2 = *srednia1;
			*srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = *srednia1;
				*FirstMove = 0;
			}
			if (*srednia1 != srednia2 && (*srednia1 == 7 && srednia2 == 6))
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos -= 1;
				}
				else
				{
					player->pos_y -= 1;
				}
				
			}
			else if (*srednia1 != srednia2)
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos -= 2;
				}
				else
				{
					player->pos_y -= 2;
				}
			}
		}
	}
}

void Player_move_right_check_plat4(struct Player_pos* player, double etiSpeed, double delta, double* moveX, struct Object_pos* platform, int* srednia1, int* FirstMove, int* EndingJumpingPos, int* PlayerJumping, int* PlayerFalling)
{
	const int x_pos_platform_up = 610;
	int nr = player->plat_nr - 1;

	if (player->pos_x  < x_pos_platform_up)
	{
		Player_move_right(player, etiSpeed, delta, moveX);

		if (player->pos_x < x_pos_platform_up && player->pos_x + (Mario_w * 1.5) / 2 < SCREEN_WIDTH)
		{
			double one = (x_pos_platform_up - player->pos_x) / 46;
			double two = (x_pos_platform_up - player->pos_x) / 47;
			int srednia2 = *srednia1;
			*srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = *srednia1;
				*FirstMove = 0;
			}
			if (*srednia1 != srednia2 && (*srednia1 == 6 && srednia2 == 7))
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos += 1;
				}
				else
				{
					player->pos_y += 1;
				}
				

			}
			else if (*srednia1 != srednia2)
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos += 2;
				}
				else
				{
					player->pos_y += 2;
				}
			}
		}
	}
	else if (player->pos_x >= x_pos_platform_up)
	{
		Player_move_right(player, etiSpeed, delta, moveX);
		*PlayerJumping = 0;
		*PlayerFalling = 1;
		*EndingJumpingPos = 320;
		player->plat_nr -= 1;
		*FirstMove = 1;

	}
}

void Player_move_right_check_plat5(struct Player_pos* player, double etiSpeed, double delta, double* moveX, struct Object_pos* platform, int* srednia1, int* FirstMove, int* EndingJumpingPos, int* PlayerJumping)
{
	const int x_pos_platform_up = 43;
	int nr = player->plat_nr - 1;

	if (player->pos_x + delta * etiSpeed + (Mario_w * 1.5) / 2 < SCREEN_WIDTH)
	{
		Player_move_right(player, etiSpeed, delta, moveX);

		if (player->pos_x > x_pos_platform_up && player->pos_x + (Mario_w * 1.5) / 2 < SCREEN_WIDTH)
		{
			double one = (player->pos_x - x_pos_platform_up) / 46;
			double two = (player->pos_x - x_pos_platform_up) / 47;
			int srednia2 = *srednia1;
			*srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = *srednia1;
				*FirstMove = 0;
			}
			if (*srednia1 != srednia2 && (*srednia1 == 8 && srednia2 == 7))
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos -= 1;
				}
				else
				{
					player->pos_y -= 1;
				}
				

			}
			else if (*srednia1 != srednia2)
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos -= 2;
				}
				else
				{
					player->pos_y -= 2;
				}
			}
		}
	}
}

void Player_move_left_check_plat5(struct Player_pos* player, double etiSpeed, double delta, double* moveX, struct Object_pos* platform, int* srednia1, int* FirstMove, int* EndingJumpingPos, int* PlayerJumping, int* PlayerFalling)
{
	const int x_pos_platform_up = 43;
	int nr = player->plat_nr - 1;

	if (player->pos_x > x_pos_platform_up)
	{
		Player_move_left(player, etiSpeed, delta, moveX);

		if (player->pos_x > x_pos_platform_up && player->pos_x - (Mario_w * 1.5) / 2 > 0)
		{
			double one = (player->pos_x - x_pos_platform_up) / 46;
			double two = (player->pos_x - x_pos_platform_up) / 47;
			int srednia2 = *srednia1;
			*srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = *srednia1;
				*FirstMove = 0;
			}
			if (*srednia1 != srednia2 && (*srednia1 == 7 && srednia2 == 8))
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos += 1;
				}
				else
				{
					player->pos_y += 1;
				}
				
			}
			else if (*srednia1 != srednia2)
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos += 2;
				}
				else
				{
					player->pos_y += 2;
				}
			}
		}
	}
	else if (player->pos_x <= x_pos_platform_up)
	{
		Player_move_left(player, etiSpeed, delta, moveX);
		*PlayerJumping = 0;
		*PlayerFalling = 1;
		*EndingJumpingPos = 256;
		player->plat_nr -= 1;
		*FirstMove = 1;
	}
}

void Player_move_left_check_plat6(struct Player_pos* player, double etiSpeed, double delta, double* moveX, struct Object_pos* platform, int* srednia1, int* FirstMove, int* EndingJumpingPos, int* PlayerJumping)
{
	const int middle_platform = 365;
	int nr = player->plat_nr - 1;

	if (player->pos_x - delta * etiSpeed - (Mario_w * 1.5) / 2 > 0)
	{
		Player_move_left(player, etiSpeed, delta, moveX);

		if (player->pos_x > middle_platform && player->pos_x - (Mario_w * 1.5) / 2 > 0)
		{
			double one = (player->pos_x - middle_platform) / 46;
			double two = (player->pos_x - middle_platform) / 47;
			int srednia2 = *srednia1;
			*srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = *srednia1;
				*FirstMove = 0;
			}
			if (*srednia1 != srednia2 && (*srednia1 == 2 && srednia2 == 3))
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos -= 1;
				}
				else
				{
					player->pos_y -= 1;
				}
			}
			else if (*srednia1 != srednia2)
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos -= 2;
				}
				else
				{
					player->pos_y -= 2;
				}
			}
		}
	}
}

void Player_move_right_check_plat6(struct Player_pos* player, double etiSpeed, double delta, double* moveX, struct Object_pos* platform, int* srednia1, int* FirstMove, int* EndingJumpingPos, int* PlayerJumping, int* PlayerFalling)
{
	const int x_pos_platform_up = 610;
	const int middle_platform = 365;
	int nr = player->plat_nr - 1;

	if (player->pos_x < x_pos_platform_up)
	{
		Player_move_right(player, etiSpeed, delta, moveX);

		if (player->pos_x > middle_platform && player->pos_x + (Mario_w * 1.5) / 2 < x_pos_platform_up)

		{
			double one = (player->pos_x- middle_platform) / 46;
			double two = (player->pos_x- middle_platform) / 47;
			int srednia2 = *srednia1;
			*srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = *srednia1;
				*FirstMove = 0;
			}

			if (*srednia1 != srednia2 && (*srednia1 == 3 && srednia2 == 2))
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos += 1;
				}
				else
				{
					player->pos_y += 1;
				}

			}
			else if (*srednia1 != srednia2)
			{
				if (*PlayerJumping == 1)
				{
					*EndingJumpingPos += 2;
				}
				else
				{
					player->pos_y += 2;
				}
			}
		}
	}
	else if (player->pos_x >= x_pos_platform_up)
	{
		Player_move_right(player, etiSpeed, delta, moveX);
		*PlayerJumping = 0;
		*PlayerFalling = 1;
		*EndingJumpingPos = 193;
		player->plat_nr -= 1;
		*FirstMove = 1;

	}
}

void Player_move_left_check_plat7(struct Player_pos* player, double etiSpeed, double delta, double* moveX, struct Object_pos* platform, int* srednia1, int* FirstMove, int* EndingJumpingPos, int* PlayerJumping)
{
	int nr = player->plat_nr - 1;
	const int x_pos_platform_up = platform[nr].pos_x;
	
	if (player->pos_x - delta * etiSpeed - (Mario_w * 1.5) / 2 >= x_pos_platform_up)
	{
		Player_move_left(player, etiSpeed, delta, moveX);

	}
}

void Player_move_right_check_plat7(struct Player_pos* player, double etiSpeed, double delta, double* moveX, struct Object_pos* platform, int* srednia1, int* FirstMove, int* EndingJumpingPos, int* PlayerJumping, int* PlayerFalling)
{
	int nr = player->plat_nr - 1;
	const int x_pos_platform_up = platform[nr].pos_x+ platform[nr].pos_w;
	
	if (player->pos_x <= x_pos_platform_up)
	{
		Player_move_right(player, etiSpeed, delta, moveX);
	}
	else if (player->pos_x >= x_pos_platform_up)
	{
		Player_move_right(player, etiSpeed, delta, moveX);
		*PlayerJumping = 0;
		*PlayerFalling = 1;
		*EndingJumpingPos = 145;
		player->plat_nr -= 1;
		*FirstMove = 1;

	}

}

void player_jump_init(int *PlayerJumping, struct Player_pos* player, double delta, double* moveY, double etiSpeed, int* EndingJumpingPos)
{
	if (*PlayerJumping == 0) 
	{
		*EndingJumpingPos = player->pos_y;
		*PlayerJumping = 1;
	}
}

void player_jump(int* PlayerJumping, struct Player_pos* player, double delta, double* moveY, double etiSpeed, int* EndingJumpingPos, int* PlayerFalling)
{
	
	if (*PlayerFalling == 0)
	{

		if (player->pos_y >= *EndingJumpingPos - jump_h)
		{
			*moveY -= etiSpeed * delta;
			player->pos_y = round(*moveY);
		}

	}




}

void Jumpcheck(int* PlayerJumping, struct Player_pos* player, double delta, double* moveY, double etiSpeed, int* EndingJumpingPos,  int* PlayerFalling)
{
	
	if (player->pos_y >= *EndingJumpingPos)
	{
		*PlayerJumping = 0;
		*PlayerFalling = 0;
	}
	else if (*PlayerFalling == 1)
	{
		*moveY += etiSpeed * delta;
		player->pos_y = round(*moveY);
	} 
	else if (player->pos_y <= *EndingJumpingPos - jump_h)
	{
		*PlayerFalling = 1;
	}
	
}

void Barrel_move_plat6(struct rect_pos* barrel, double delta, double etiBarrelSpeed, struct Object_pos* platform, int i, int* FirstMove)
{
	const int x_pos_platform_up = 545;
	const int middle_platform = 365;
	int nr = barrel[i].plat_nr - 1;

	if (barrel[i].pos_x < x_pos_platform_up + ladder_margin && barrel[i].pos_x > x_pos_platform_up)
	{
		barrel[i].OnLadder = 1;
		barrel[i].plat_nr -= 1;
	}
	else if (barrel[i].OnLadder == 0)

	{
		Barrel_move_right(barrel, etiBarrelSpeed, delta, i);

		if (barrel[i].pos_x > middle_platform && barrel[i].pos_x + barrel_wh / 2 < x_pos_platform_up)

		{
			double one = (barrel[i].pos_x - middle_platform) / 46;
			double two = (barrel[i].pos_x - middle_platform) / 47;
			int srednia2 = barrel[i].srednia1;
			barrel[i].srednia1 = ((one + two) / 2) + 1;

			if (barrel[i].srednia1 != srednia2 && (barrel[i].srednia1 == 3 && srednia2 == 2))
			{
				barrel[i].pos_y += 1;
			}
			else if (barrel[i].srednia1 != srednia2)
			{
				barrel[i].pos_y += 2;
			}
		}
	}
}

void Barrel_move_plat5(struct rect_pos* barrel, double delta, double etiBarrelSpeed, struct Object_pos* platform, int i, int* FirstMove, struct Object_pos* ladder)
{
	const int x_pos_platform_up = 43;
	int nr = barrel[i].plat_nr - 1;


	if (barrel[i].pos_x < ladder[6].pos_x + (ladder[6].pos_w/2) && barrel[i].pos_x > ladder[6].pos_x + (ladder[6].pos_w / 2) - ladder_margin)
	{
		barrel[i].OnLadder = 1;
		barrel[i].plat_nr -= 1;


	}
	else if (barrel[i].OnLadder == 0)

	{
		Barrel_move_left(barrel, etiBarrelSpeed, delta, i);

		if (barrel[i].pos_x > x_pos_platform_up && barrel[i].pos_x - barrel_wh / 2 > 0)

		{
			double one = (barrel[i].pos_x - x_pos_platform_up) / 46;
			double two = (barrel[i].pos_x - x_pos_platform_up) / 47;
			int srednia2 = barrel[i].srednia1;
			barrel[i].srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = barrel[i].srednia1;
				*FirstMove = 0;
			}

			if (barrel[i].srednia1 != srednia2 && (barrel[i].srednia1 == 7 && srednia2 == 8))
			{
				barrel[i].pos_y += 1;
			}
			else if (barrel[i].srednia1 != srednia2)
			{
				barrel[i].pos_y += 2;
			}
		}
	}
}

void Barrel_move_plat4(struct rect_pos* barrel, double delta, double etiBarrelSpeed, struct Object_pos* platform, int i, int* FirstMove, struct Object_pos* ladder)
{
	const int x_pos_platform_up = 610;
	int nr = barrel[i].plat_nr - 1;


	if (barrel[i].pos_x < ladder[4].pos_x + (ladder[4].pos_w / 2) && barrel[i].pos_x > ladder[4].pos_x + (ladder[4].pos_w / 2) - ladder_margin)
	{
		barrel[i].OnLadder = 1;
		barrel[i].plat_nr -= 1;
	}
	else if (barrel[i].OnLadder == 0)

	{
		Barrel_move_right(barrel, etiBarrelSpeed, delta, i);

		if (barrel[i].pos_x < x_pos_platform_up && barrel[i].pos_x + barrel_wh / 2 < SCREEN_WIDTH)

		{
			double one = (barrel[i].pos_x - x_pos_platform_up) / 46;
			double two = (barrel[i].pos_x - x_pos_platform_up) / 47;
			int srednia2 = barrel[i].srednia1;
			barrel[i].srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = barrel[i].srednia1;
				*FirstMove = 0;
			}

			if (barrel[i].srednia1 != srednia2 && (barrel[i].srednia1 == 6 && srednia2 == 7))
			{
				barrel[i].pos_y += 1;
			}
			else if (barrel[i].srednia1 != srednia2)
			{
				barrel[i].pos_y += 2;
			}
		}
	}
}

void Barrel_move_plat3(struct rect_pos* barrel, double delta, double etiBarrelSpeed, struct Object_pos* platform, int i, int* FirstMove, struct Object_pos* ladder)
{
	const int x_pos_platform_up = 43;
	int nr = barrel[i].plat_nr - 1;


	if (barrel[i].pos_x < ladder[2].pos_x + (ladder[2].pos_w / 2) && barrel[i].pos_x > ladder[2].pos_x + (ladder[2].pos_w / 2) - ladder_margin)
	{
		barrel[i].OnLadder = 1;
		barrel[i].plat_nr -= 1;
		
	}
	else if (barrel[i].OnLadder == 0)

	{
		Barrel_move_left(barrel, etiBarrelSpeed, delta, i);

		if (barrel[i].pos_x > x_pos_platform_up && barrel[i].pos_x - barrel_wh / 2 > 0)

		{
			double one = (barrel[i].pos_x - x_pos_platform_up) / 46;
			double two = (barrel[i].pos_x - x_pos_platform_up) / 47;
			int srednia2 = barrel[i].srednia1;
			barrel[i].srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = barrel[i].srednia1;
				*FirstMove = 0;
			}

			if (barrel[i].srednia1 != srednia2 && (barrel[i].srednia1 == 6 && srednia2 == 7))
			{
				barrel[i].pos_y += 1;
			}
			else if (barrel[i].srednia1 != srednia2)
			{
				barrel[i].pos_y += 2;
			}
		}
	}
}

void Barrel_move_plat2(struct rect_pos* barrel, double delta, double etiBarrelSpeed, struct Object_pos* platform, int i, int* FirstMove, struct Object_pos* ladder)
{
	const int x_pos_platform_up = 610;
	int nr = barrel[i].plat_nr - 1;


	if (barrel[i].pos_x < ladder[0].pos_x + (ladder[0].pos_w / 2) && barrel[i].pos_x > ladder[0].pos_x + (ladder[0].pos_w / 2) - ladder_margin)
	{
		barrel[i].OnLadder = 1;
		barrel[i].plat_nr -= 1;
		
	}
	else if (barrel[i].OnLadder == 0)

	{
		Barrel_move_right(barrel, etiBarrelSpeed, delta, i);

		if (barrel[i].pos_x < x_pos_platform_up && barrel[i].pos_x + barrel_wh / 2 < SCREEN_WIDTH)

		{
			double one = (barrel[i].pos_x - x_pos_platform_up) / 46;
			double two = (barrel[i].pos_x - x_pos_platform_up) / 47;
			int srednia2 = barrel[i].srednia1;
			barrel[i].srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = barrel[i].srednia1;
				*FirstMove = 0;
			}

			if (barrel[i].srednia1 != srednia2 && (barrel[i].srednia1 == 2 && srednia2 == 3))
			{
				barrel[i].pos_y += 1;
			}
			else if (barrel[i].srednia1 != srednia2)
			{
				barrel[i].pos_y += 2;
			}
		}
	}
}

void Barrel_move_plat1(struct rect_pos* barrel, double delta, double etiBarrelSpeed, struct Object_pos* platform, int i, int* FirstMove, struct Object_pos* ladder)
{
	const int x_pos_platform_up = 280;
	int nr = barrel[i].plat_nr - 1;


	if (barrel[i].OnLadder == 0)

	{
		Barrel_move_left(barrel, etiBarrelSpeed, delta, i);

		if (barrel[i].pos_x > x_pos_platform_up && barrel[i].pos_x - barrel_wh / 2 > 0)

		{
			double one = (barrel[i].pos_x - x_pos_platform_up) / 46;
			double two = (barrel[i].pos_x - x_pos_platform_up) / 47;
			int srednia2 = barrel[i].srednia1;
			barrel[i].srednia1 = ((one + two) / 2) + 1;
			if (*FirstMove == 1)
			{
				srednia2 = barrel[i].srednia1;
				*FirstMove = 0;
			}

			if (barrel[i].srednia1 != srednia2 && (barrel[i].srednia1 == 2 && srednia2 == 3))
			{
				barrel[i].pos_y += 1;
			}
			else if (barrel[i].srednia1 != srednia2)
			{
				barrel[i].pos_y += 2;
			}
		}
	}
}

void Barrel_spawn(SDL_Surface* screen, SDL_Surface* sprites, struct Player_pos* player, struct rect_pos* barrel)
{
	int check = 0;

	for (int i = 0; i < barrel_lv1; i++)
	{
		if (barrel[i].spawned == 0)
		{
			barrel[i].spawned = 1;
			check = 1;
		}
		if (check == 1)
			break;
	}


}

void Barrel_movement(SDL_Surface* screen, SDL_Surface* sprites, struct Player_pos* player, struct rect_pos* barrel, double delta, double etiBarrelSpeed, struct Object_pos* platform, int* FirstMove, struct Object_pos* ladder, int nr)
{
	switch (barrel[nr].plat_nr)
	{
	case 6:
		Barrel_move_plat6(barrel, delta, etiBarrelSpeed, platform, nr, FirstMove);
		barrel[nr].moveY = barrel[nr].pos_y;
		break;
	case 5:
		Barrel_move_plat5(barrel, delta, etiBarrelSpeed, platform, nr, FirstMove, ladder);
		barrel[nr].moveY = barrel[nr].pos_y;
		break;
	case 4:
		Barrel_move_plat4(barrel, delta, etiBarrelSpeed, platform, nr, FirstMove, ladder);
		barrel[nr].moveY = barrel[nr].pos_y;
		break;
	case 3:
		Barrel_move_plat3(barrel, delta, etiBarrelSpeed, platform, nr, FirstMove, ladder);
		barrel[nr].moveY = barrel[nr].pos_y;
		break;
	case 2:
		Barrel_move_plat2(barrel, delta, etiBarrelSpeed, platform, nr, FirstMove, ladder);
		barrel[nr].moveY = barrel[nr].pos_y;
		break;
	case 1:
		Barrel_move_plat1(barrel, delta, etiBarrelSpeed, platform, nr, FirstMove, ladder);
		barrel[nr].moveY = barrel[nr].pos_y;
		break;
	}



}

void Barrel_move_ladd6(struct rect_pos* barrel, double delta, double etiBarrelSpeed, struct Object_pos* platform, int i, int* FirstMove, struct Object_pos* ladder)
{


	if ((barrel[i].pos_x > ladder[7].pos_x + ladder_margin) && (barrel[i].pos_x < (ladder[7].pos_x + ladder_thc - ladder_margin)) && (barrel[i].pos_y >= ladder[7].pos_y + ladder[7].pos_w && barrel[i].pos_y <= ladder[7].pos_y + ladder[7].pos_w+2))
	{

		barrel[i].OnLadder = 0;
		barrel[i].plat_nr = ladder[7].numer;
		*FirstMove = 1;

	}
	else if ((barrel[i].pos_x > ladder[7].pos_x + ladder_margin) && (barrel[i].pos_x < (ladder[7].pos_x + ladder_thc - ladder_margin)) && (barrel[i].pos_y <= ladder[7].pos_y + ladder[7].pos_w) && (barrel[i].pos_y >= ladder[7].pos_y - platform_thc))
	{
		Barrel_move_down(barrel, etiBarrelSpeed, delta, i);
	}


}

 void Barrel_move_ladd5(struct rect_pos* barrel, double delta, double etiBarrelSpeed, struct Object_pos* platform, int i, int* FirstMove, struct Object_pos* ladder)
{


	if ((barrel[i].pos_x > ladder[6].pos_x + ladder_margin) && (barrel[i].pos_x < (ladder[6].pos_x + ladder_thc - ladder_margin)) && (barrel[i].pos_y >= ladder[6].pos_y + ladder[6].pos_w && barrel[i].pos_y <= ladder[6].pos_y + ladder[6].pos_w +2))
	{
		
		barrel[i].OnLadder = 0;
		barrel[i].plat_nr = ladder[6].numer;
		*FirstMove = 1;

	}
	else if ((barrel[i].pos_x > ladder[6].pos_x + ladder_margin) && (barrel[i].pos_x < (ladder[6].pos_x + ladder_thc - ladder_margin)) && (barrel[i].pos_y <= ladder[6].pos_y + ladder[6].pos_w) && (barrel[i].pos_y >= ladder[6].pos_y - platform_thc - ladder_margin))
	{
		Barrel_move_down(barrel, etiBarrelSpeed, delta, i);
		
	}



}

void Barrel_move_ladd4(struct rect_pos* barrel, double delta, double etiBarrelSpeed, struct Object_pos* platform, int i, int* FirstMove, struct Object_pos* ladder)
{


	if ((barrel[i].pos_x > ladder[4].pos_x + ladder_margin) && (barrel[i].pos_x < (ladder[4].pos_x + ladder_thc - ladder_margin)) && (barrel[i].pos_y >= ladder[4].pos_y + ladder[4].pos_w && barrel[i].pos_y <= ladder[4].pos_y + ladder[4].pos_w +2))
	{
		
		barrel[i].OnLadder = 0;
		barrel[i].plat_nr = ladder[4].numer;
		*FirstMove = 1;

	}
	else if ((barrel[i].pos_x > ladder[4].pos_x + ladder_margin) && (barrel[i].pos_x < (ladder[4].pos_x + ladder_thc - ladder_margin)) && (barrel[i].pos_y < ladder[4].pos_y + ladder[4].pos_w) && (barrel[i].pos_y >= ladder[4].pos_y - platform_thc ))
	{
		Barrel_move_down(barrel, etiBarrelSpeed, delta, i);
		
	}



}

void Barrel_move_ladd3(struct rect_pos* barrel, double delta, double etiBarrelSpeed, struct Object_pos* platform, int i, int* FirstMove, struct Object_pos* ladder)
{


	if ((barrel[i].pos_x > ladder[2].pos_x + ladder_margin) && (barrel[i].pos_x < (ladder[2].pos_x + ladder_thc - ladder_margin)) && (barrel[i].pos_y >= ladder[2].pos_y + ladder[2].pos_w && barrel[i].pos_y <= ladder[2].pos_y + ladder[2].pos_w+2))
	{

		barrel[i].OnLadder = 0;
		barrel[i].plat_nr = ladder[2].numer;
		*FirstMove = 1;

	}
	else if ((barrel[i].pos_x > ladder[2].pos_x + ladder_margin) && (barrel[i].pos_x < (ladder[2].pos_x + ladder_thc - ladder_margin)) && (barrel[i].pos_y < ladder[2].pos_y + ladder[2].pos_w) && (barrel[i].pos_y >= ladder[2].pos_y - platform_thc))
	{
		Barrel_move_down(barrel, etiBarrelSpeed, delta, i);
	}



}

void Barrel_move_ladd2(struct rect_pos* barrel, double delta, double etiBarrelSpeed, struct Object_pos* platform, int i, int* FirstMove, struct Object_pos* ladder)
{


	if ((barrel[i].pos_x > ladder[0].pos_x + ladder_margin) && (barrel[i].pos_x < (ladder[0].pos_x + ladder_thc - ladder_margin)) && (barrel[i].pos_y >= ladder[0].pos_y + ladder[0].pos_w && barrel[i].pos_y <= ladder[0].pos_y + ladder[0].pos_w +2))
	{
		
		barrel[i].OnLadder = 0;
		barrel[i].plat_nr = ladder[0].numer;
		*FirstMove = 1;

	}
	else if ((barrel[i].pos_x > ladder[0].pos_x + ladder_margin) && (barrel[i].pos_x < (ladder[0].pos_x + ladder_thc - ladder_margin)) && (barrel[i].pos_y < ladder[0].pos_y + ladder[0].pos_w) && (barrel[i].pos_y >= ladder[0].pos_y - platform_thc))
	{
		Barrel_move_down(barrel, etiBarrelSpeed, delta, i);
		
	}



}

void Barrel_ladder_movement(SDL_Surface* screen, SDL_Surface* sprites, struct Player_pos* player, struct rect_pos* barrel, double delta, double etiBarrelSpeed, struct Object_pos* platform, int* FirstMove, struct Object_pos* ladder, int nr)
{
	switch (barrel[nr].plat_nr)
	{
	case 5:
		Barrel_move_ladd6(barrel, delta, etiBarrelSpeed, platform, nr, FirstMove, ladder);
		barrel[nr].moveY = barrel[nr].pos_y;
		break;
	case 4:
		Barrel_move_ladd5(barrel, delta, etiBarrelSpeed, platform, nr, FirstMove, ladder);
		barrel[nr].moveY = barrel[nr].pos_y;
		break;
	case 3:
		Barrel_move_ladd4(barrel, delta, etiBarrelSpeed, platform, nr, FirstMove, ladder);
		barrel[nr].moveY = barrel[nr].pos_y;
		break;
	case 2:
		Barrel_move_ladd3(barrel, delta, etiBarrelSpeed, platform, nr, FirstMove, ladder);
		barrel[nr].moveY = barrel[nr].pos_y;
		break;
	case 1:
		Barrel_move_ladd2(barrel, delta, etiBarrelSpeed, platform, nr, FirstMove, ladder);
		barrel[nr].moveY = barrel[nr].pos_y;
		break;
	}



}

void Barrel_Destroy_check(struct rect_pos* barrel, int i)
{
	if (barrel[i].pos_x <= oil_pos_x + oil_barrel_w && barrel[i].pos_x >= oil_pos_x && barrel[i].plat_nr == 1)
	{
		barrel[i] = { 150, 146, 6, 0, 150, 145, 1 ,0 };
	}
	
}

void CollisionPlayerAndBarrel(struct Player_pos* player, struct rect_pos* barrel, int i, int* Lives, int* Hit, double Iframes, int *points, double* BarrelJumplock,SDL_Surface* screen, char text[128], SDL_Surface* litery, int* PointsBlock)
{
	
	
		if (player->pos_x >= barrel[i].pos_x - barrel_wh / 2 && player->pos_x <= barrel[i].pos_x + barrel_wh / 2 && player->pos_y >= barrel[i].pos_y - barrel_wh + 2 && player->pos_y <= barrel[i].pos_y && Iframes >= 1.0)
		{
			*Lives -= 1;
			*Hit = 1;
		}
		else if (player->on_ladder == 1 && player->pos_x >= barrel[i].pos_x - barrel_wh / 2 && player->pos_x <= barrel[i].pos_x + barrel_wh / 2 && player->pos_y - Mario_h >= barrel[i].pos_y - barrel_wh && player->pos_y - Mario_h <= barrel[i].pos_y && Iframes >= 1.0)

		{
			*Lives -= 1;
			*Hit = 1;
		}
		else if (player->pos_x >= barrel[i].pos_x - barrel_wh / 2 && player->pos_x <= barrel[i].pos_x + barrel_wh / 2 && player->pos_y <= barrel[i].pos_y - barrel_wh && player->on_ladder == 0 && *BarrelJumplock > 0.8)
		{
			*points += 600;
			*BarrelJumplock = 0;
			barrel[barrel_lv1].pos_x = player->pos_x;
			barrel[barrel_lv1].pos_y = player->pos_y;
			*PointsBlock = 1;
			
			
			
		}

		if (*PointsBlock == 1)
		{
			if (*BarrelJumplock < 0.4)
			{
				sprintf(text, "600");
				DrawString(screen, barrel[barrel_lv1].pos_x - strlen(text) * 8 / 2, barrel[barrel_lv1].pos_y-barrel_wh, text, litery);
			}
			else if (*BarrelJumplock > 0.4)
			{
				*PointsBlock = 0;
			}
		}
	
	
	
}

void Barrel_movement_check(SDL_Surface* screen, SDL_Surface* sprites, struct Player_pos* player, struct rect_pos* barrel, double delta, double etiBarrelSpeed, struct Object_pos* platform, int* FirstBarrelMove, struct Object_pos* ladder, int *Lives, int* Hit, double IFrames, int *points, double*BarrelJumpLock, char text[128], SDL_Surface* litery, int *PointsBlock)
{
	
	for (int i = 0; i < barrel_lv1; i++)
	{
		if (barrel[i].spawned == 1)
		{
			if (barrel[i].plat_nr == 1)
			Barrel_Destroy_check(barrel, i);

			if (barrel[i].plat_nr == player->plat_nr)
			CollisionPlayerAndBarrel(player,barrel,i, Lives, Hit, IFrames, points, BarrelJumpLock, screen,text,litery,PointsBlock);
			
			if (barrel[i].OnLadder == 0 && barrel[i].spawned == 1)
			{
				Barrel_movement(screen, sprites, player, barrel, delta, etiBarrelSpeed, platform, FirstBarrelMove, ladder, i);
			}
			else if (barrel[i].OnLadder == 1 && barrel[i].spawned == 1)
			{
				Barrel_ladder_movement(screen, sprites, player, barrel, delta, etiBarrelSpeed, platform, FirstBarrelMove, ladder, i);
			}

		}

	}

}

void PurseCollisionCheck(struct Player_pos* player, int *points, int*PurseTaken, SDL_Surface* screen, char text[128], SDL_Surface* litery, double *PursePointslock)
{
	if (player->pos_x >= 580 && player->pos_x <= 596 && player->pos_y <= 283 && player->pos_y >= 263 && *PurseTaken == 0) // purse position
	{
		*points += 800;
		*PurseTaken = 1;
		*PursePointslock = 0;
	}

	if (*PurseTaken == 1 && *PursePointslock < 1)
	{
		sprintf(text, "800");
		DrawString(screen, 580 - strlen(text) * 8 / 2, 263 - Mario_h*1.5, text, litery);
	}


}

void Win_check(struct Player_pos* player, int *win)
{
	if (player->plat_nr == 7)
	{
		if (player->pos_x > 257 && player->pos_x < 300)
		{
			*win = 1;
		}
	}
}

void SaveHighScore(const char* scores, int *points, char name[]) {
	FILE* file = fopen(scores, "a");
	if (file != NULL) {
		fprintf(file, "%s %d\n", name, *points);
		fclose(file);
	}
}

void LoadHighScores(const char* scores, struct HighScore** highScores, int* numScores) {
	FILE* file = fopen(scores, "r");
	if (file != NULL) {
		*numScores = 0;

		*highScores = (struct HighScore*)malloc(sizeof(struct HighScore));

		while (fscanf(file, "%s %d", (*highScores)[*numScores].player, &(*highScores)[*numScores].score) == 2) {
			
			(*numScores)++;

			*highScores = (struct HighScore*)realloc(*highScores, (*numScores + 1) * sizeof(struct HighScore));
		}

		fclose(file);
	}
}

void Nickname_input(int * points,SDL_Surface* screen, SDL_Renderer* renderer, SDL_Texture* scrtex, SDL_Window* window, int czarny, char name[])
{
	SDL_Surface* charset;
	SDL_Event event;
	int quit = 0;
	char text[128];
	char nick[15] = {'\0'};
	int N = 0;

	charset = SDL_LoadBMP("./cs8x8.bmp");
	surface_check(charset, screen, scrtex, window, renderer);


	while (!quit)
	{
		SDL_FillRect(screen, NULL, czarny);
		
		sprintf(text, "SAVE YOUR SCORE,");
		DrawStringMenu(screen, screen->w / 2 - strlen(text) * LETTER_MENU_SIZE / 2, screen->h / 2 - 128, text, charset);

		sprintf(text, "WHAT IS YOUR NAME?");
		DrawStringMenu(screen, screen->w / 2 - strlen(text) * LETTER_MENU_SIZE / 2, screen->h/2 - 64, text, charset);

		sprintf(text, "%s", nick);
		DrawStringMenu(screen, screen->w / 2 - strlen(text) * LETTER_MENU_SIZE / 2, screen->h / 2, text, charset);


		

		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = 1;
			}
			else if (event.type == SDL_KEYDOWN) {
				const char* keyName = SDL_GetKeyName(event.key.keysym.sym);
					if (event.key.keysym.sym == SDLK_BACKSPACE) {
						if (N > 0) {
							N--;
							nick[N] = '\0';
						}
					}
					else if (event.key.keysym.sym == SDLK_RETURN)
					{
						if (nick[0] != '\0')
						{
							for(int c = 0; c<14; c++)
							name[c] = nick[c];
							
							quit = 1;

						}
					}
					else if (isalnum(keyName[0]) && nick[N] == '\0') {
						if (N < 14) 
						{
						nick[N] = keyName[0];
						N++;
						}
					}
			}

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
	}
	SDL_FreeSurface(charset);
}

void Game_Restart(int *WinCondition, int* points, int* LivesCount, double *WorldTime, struct Player_pos* player, int*srednia, int*FirstMove,int *PlayerJumping, int *PlayerFalling, int *PurseTaken, int* EndingJumpingPos, double *SpawnTime, struct rect_pos* barrel, double* MoveX, double*MoveY, int HitDetection, int *StageNr, struct HighScore* highScore, int* win)
{
	if (*WinCondition == 0 && HitDetection == 0)
	{
		*points = 6000;
		*WorldTime = 0;
		*LivesCount = 3;
		*StageNr = 1;
	}

	if (*WinCondition == 1 && *StageNr == 1)
	{
		*points += 10000;
		*StageNr += 1;
		*WinCondition = 0;
	}

	if (*WinCondition == 1 && *StageNr == 2)
	{
		*win = 1;	
	}

	player->pos_x = 1 + (Mario_w * 1.5) / 2;
	player->pos_y = 460;
	player->plat_nr = 1;
	player->on_ladder = 0;

	*srednia = 1;
	*FirstMove = 0;
	*PlayerJumping = 0;
	*PlayerFalling = 0;
	*PurseTaken = 0;
	*WinCondition = 0;
	*EndingJumpingPos = player->pos_y;
	*SpawnTime = 0;
	
	*MoveX = player->pos_x;
	*MoveY = player->pos_y;

	for (int i = 0; i < barrel_lv1; ++i) {
		barrel[i] = { 150, 146, 6, 0, 150, 145, 1 ,0 }; //barrel init
	}
}

int compareHighScores(const void* a, const void* b) {
	const struct HighScore* scoreA = (const struct HighScore*)a;
	const struct HighScore* scoreB = (const struct HighScore*)b;
	return scoreB->score - scoreA->score;
}

void Scores_load(struct HighScore* highScore, SDL_Surface* screen, SDL_Renderer* renderer, SDL_Texture* scrtex, SDL_Window* window, int czarny, struct HighScore* highScores, int numScores)
{
	SDL_Surface* charset;
	SDL_Event event;
	int quit = 0;
	char text[128];
	int currentPage = 0;  
	int maxPages = (numScores - 1) / Max_scores_on_page;

	charset = SDL_LoadBMP("./cs8x8.bmp");
	surface_check(charset, screen, scrtex, window, renderer);

	while (!quit) {
		SDL_FillRect(screen, NULL, czarny);

		sprintf(text, "HIGH SCORES - PAGE %d", currentPage + 1);
		DrawStringMenu(screen, screen->w / 2 - strlen(text) * LETTER_MENU_SIZE / 2, screen->h / 2 - 128, text, charset); //

		qsort(highScores, numScores, sizeof(struct HighScore), compareHighScores); // pocztek tablciy, liczba elementow, rozmiar, porownanie ze zwrotem

		int startIdx = currentPage * Max_scores_on_page;
		int endIdx = startIdx + Max_scores_on_page;

		if (endIdx > numScores) {
			endIdx = numScores;
		}


		for (int i = startIdx; i < endIdx; i++) {
			sprintf(text, "%d. %s %d", i + 1, highScores[i].player, highScores[i].score);
			DrawStringMenu(screen, screen->w / 2 - strlen(text) * LETTER_MENU_SIZE / 2, screen->h / 2 - 64 + (i - startIdx) * LETTER_MENU_SIZE, text, charset);
		}

		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = 1;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					quit = 1;
				}
				else if (event.key.keysym.sym == SDLK_UP && currentPage > 0) {
					currentPage = currentPage - 1;
				}
				else if (event.key.keysym.sym == SDLK_DOWN && currentPage < maxPages) {
					currentPage = currentPage +1;
				}
			}
		}

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	SDL_FreeSurface(charset);
}

void right_move_check(struct Player_pos* player, double delta, int etiSpeedX, struct Object_pos* platform, double *moveX, int *srednia, int *FirstMove, int* EndingJumpingPos, int *PlayerJumping, int* PlayerFalling, double* moveY)
{
	if (player->on_ladder == 0)
	{
		switch (player->plat_nr)
		{
		case 1:
			Player_move_right_check_plat1(player, etiSpeedX, delta, moveX, platform, srednia, FirstMove, EndingJumpingPos, PlayerJumping);
			*moveY = player->pos_y;

			break;
		case 2:
			Player_move_right_check_plat2(player, etiSpeedX, delta, moveX, platform, srednia, FirstMove, EndingJumpingPos, PlayerJumping, PlayerFalling);
			*moveY = player->pos_y;

			break;
		case 3:
			Player_move_right_check_plat3(player, etiSpeedX, delta, moveX, platform, srednia, FirstMove, EndingJumpingPos, PlayerJumping);
			*moveY = player->pos_y;

			break;
		case 4:
			Player_move_right_check_plat4(player, etiSpeedX, delta, moveX, platform, srednia, FirstMove, EndingJumpingPos, PlayerJumping, PlayerFalling);
			*moveY = player->pos_y;

			break;
		case 5:
			Player_move_right_check_plat5(player, etiSpeedX, delta, moveX, platform, srednia, FirstMove, EndingJumpingPos, PlayerJumping);
			*moveY = player->pos_y;

			break;
		case 6:
			Player_move_right_check_plat6(player, etiSpeedX, delta, moveX, platform, srednia, FirstMove, EndingJumpingPos, PlayerJumping, PlayerFalling);
			*moveY = player->pos_y;

			break;
		case 7:

			Player_move_right_check_plat7(player, etiSpeedX, delta, moveX, platform, srednia, FirstMove, EndingJumpingPos, PlayerJumping, PlayerFalling);
			*moveY = player->pos_y;
			break;
		}
	}
}

void left_move_check(struct Player_pos* player, double delta, int etiSpeedX, struct Object_pos* platform, double* moveX, int* srednia, int* FirstMove, int* EndingJumpingPos, int* PlayerJumping, int* PlayerFalling, double* moveY)
{
	if (player->on_ladder == 0)
	{
		switch (player->plat_nr)
		{
		case 1:
			Player_move_left_check_plat1(player, etiSpeedX, delta, moveX, platform, srednia, FirstMove, EndingJumpingPos, PlayerJumping);
			*moveY = player->pos_y;

			break;
		case 2:
			Player_move_left_check_plat2(player, etiSpeedX, delta, moveX, platform, srednia, FirstMove, EndingJumpingPos, PlayerJumping);
			*moveY = player->pos_y;

			break;
		case 3:
			Player_move_left_check_plat3(player, etiSpeedX, delta, moveX, platform, srednia, FirstMove, EndingJumpingPos, PlayerJumping, PlayerFalling);
			*moveY = player->pos_y;

			break;
		case 4:
			Player_move_left_check_plat4(player, etiSpeedX, delta, moveX, platform, srednia, FirstMove, EndingJumpingPos, PlayerJumping);
			*moveY = player->pos_y;

			break;
		case 5:
			Player_move_left_check_plat5(player, etiSpeedX, delta, moveX, platform, srednia, FirstMove, EndingJumpingPos, PlayerJumping, PlayerFalling);
			*moveY = player->pos_y;

			break;
		case 6:
			Player_move_left_check_plat6(player, etiSpeedX, delta, moveX, platform, srednia, FirstMove, EndingJumpingPos, PlayerJumping);
			*moveY = player->pos_y;

			break;
		case 7:
			Player_move_left_check_plat7(player, etiSpeedX, delta, moveX, platform, srednia, FirstMove, EndingJumpingPos, PlayerJumping);
			*moveY = player->pos_y;
			break;
		}

	}
}

void ladder_move_check_up(struct Player_pos* player, struct Object_pos* ladder, int etiSpeedY, double delta, double *moveY, int *FirstMove)
{
	for (int i = 0; i < Ladders_lv1; i++)
	{
		if (player->on_ladder == 0)
		{
			if ((player->pos_x > ladder[i].pos_x + ladder_margin) && (player->pos_x < (ladder[i].pos_x + ladder_thc - ladder_margin)) && (player->pos_y >= ladder[i].pos_y + ladder[i].pos_w && player->pos_y < ladder[i].pos_y + ladder[i].pos_w + 5))
			{
				player->on_ladder = 1;
				Player_move_up(player, etiSpeedY, delta, moveY);

			}

		}
		else if (player->on_ladder == 1)
		{
			if ((player->pos_y == ladder[i].pos_y - platform_thc) && (player->pos_x > ladder[i].pos_x + ladder_margin) && (player->pos_x < (ladder[i].pos_x + ladder_thc - ladder_margin)))
			{
				player->on_ladder = 0;
				player->plat_nr = ladder[i].numer + 1;
				*FirstMove = 1;

			}
			else if ((player->pos_x > ladder[i].pos_x + ladder_margin) && (player->pos_x < (ladder[i].pos_x + ladder_thc - ladder_margin)) && (player->pos_y > ladder[i].pos_y - platform_thc) && (player->pos_y <= ladder[i].pos_y + ladder[i].pos_w + 5))
			{
				Player_move_up(player, etiSpeedY, delta, moveY);
			}

		}


	}
}

void ladder_move_check_down(struct Player_pos* player, struct Object_pos* ladder, int etiSpeedY, double delta, double* moveY, int* FirstMove)
{
	for (int i = 0; i < Ladders_lv1; i++)
	{
		if (player->on_ladder == 0)
		{
			if ((player->pos_y >= ladder[i].pos_y - platform_thc && player->pos_y < ladder[i].pos_y - platform_thc + 5) && (player->pos_x > ladder[i].pos_x + ladder_margin) && (player->pos_x < (ladder[i].pos_x + ladder_thc - ladder_margin)))
			{
				player->on_ladder = 1;
				Player_move_down(player, etiSpeedY, delta, moveY);

			}
		}
		else if (player->on_ladder == 1)
		{
			if ((player->pos_x > ladder[i].pos_x + ladder_margin) && (player->pos_x < (ladder[i].pos_x + ladder_thc - ladder_margin)) && (player->pos_y == ladder[i].pos_y + ladder[i].pos_w))
			{
				player->on_ladder = 0;
				player->plat_nr = ladder[i].numer;
				*FirstMove = 1;

			}
			else if ((player->pos_x > ladder[i].pos_x + ladder_margin) && (player->pos_x < (ladder[i].pos_x + ladder_thc - ladder_margin)) && (player->pos_y <= ladder[i].pos_y + ladder[i].pos_w) && (player->pos_y > ladder[i].pos_y - platform_thc))
			{
				Player_move_down(player, etiSpeedY, delta, moveY);
			}
		}
	}
}

void Player_Jumping_check(int *PlayerJumping, struct Player_pos* player, double delta, double* moveY, int etiJumpSpeed, int*EndingJumpingPos, int* PlayerFalling)
{
	if (*PlayerJumping == 1)
	{
		player_jump(PlayerJumping, player, delta, moveY, etiJumpSpeed, EndingJumpingPos, PlayerFalling);
		Jumpcheck(PlayerJumping, player, delta, moveY, etiJumpSpeed, EndingJumpingPos, PlayerFalling);
	}
	else if (*PlayerFalling == 1)
	{
		Jumpcheck(PlayerJumping, player, delta, moveY, etiJumpSpeed, EndingJumpingPos, PlayerFalling);
	}
}

void Cheat(struct Player_pos* player, double *MoveY, double *MoveX)
{
	player->plat_nr = 7;
	player->pos_x = 390;
	player->pos_y = 90;
	*MoveY = player->pos_y;
	*MoveX = player->pos_x;
}

void Game_initialization(SDL_Renderer* renderer, SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, int czarny, int czerwony, int white, struct HighScore* highScore, int *win, int *points)
{
	int t1, t2, quit = 0, frames = 0, srednia = 1, FirstMove = 0, PlayerJumping = 0, EndingJumpingPos, PlayerFalling = 0, BarrelsOnScreen = 0, FirstBarrelMove = 0, LivesCount = 3, HitDetection = 0, LastMove = 0, PurseTaken = 0, PointsBlock = 0, WinCondition = 0, StageNr = 1;
	double delta, worldTime = 0, fpsTimer = 0, fps = 0, etiSpeedX, etiSpeedY, etiJumpSpeed, etiBarrelSpeed, SpawnTime = 0, IFrames = 0, AnimationCheck = 0, BarrelAnimationCheck = 0, pointsExp = 0, BarrelJumpLock = 0, PursePointsLock = 0;
	SDL_Event event;
	SDL_Surface* charset, *sprites, *spritesReversed;
	char text[128];
	double moveX;
	double moveY;
	struct Object_pos platform[Platforms_lv1];
	struct Object_pos ladder[Ladders_lv1];
	struct Player_pos player;
	struct rect_pos barrel[barrel_lv1+1];
	
	
	for (int i = 0; i < barrel_lv1; ++i) {
		barrel[i] = { 150, 146, 6, 0, 150, 145, 1 ,0}; //barrel init, startowe wartosci beczek
	}
	
	Game_platform_initialization(platform,&player,barrel);
	Game_ladder_initialization(ladder);

	charset = SDL_LoadBMP("./cs8x8.bmp");
	surface_check(charset, screen, scrtex, window, renderer);

	sprites = SDL_LoadBMP("./sprites.bmp");
	surface_check(sprites, screen, scrtex, window, renderer);
	SDL_SetColorKey(sprites, true, 0x000000);

	spritesReversed = SDL_LoadBMP("./spritesR.bmp");
	surface_check(spritesReversed, screen, scrtex, window, renderer);
	SDL_SetColorKey(spritesReversed, true, 0x000000);

	t1 = SDL_GetTicks();
	etiSpeedX = 100;
	etiSpeedY = 50;
	etiJumpSpeed = 60;
	etiBarrelSpeed = 120;
	moveX = player.pos_x;
	moveY = player.pos_y;
	EndingJumpingPos = player.pos_y;
	
	while (!quit && LivesCount != 0) {
		t2 = SDL_GetTicks();
		delta = (t2 - t1) * 0.001;
		t1 = t2;
		if (HitDetection == 0)
		{
			worldTime += delta;
			fpsTimer += delta;
			IFrames += delta;
			pointsExp += delta;
			BarrelJumpLock += delta;
			PursePointsLock += delta;
		}

		if (HitDetection == 1 || WinCondition == 1) // warunki restartu gry
		{	
			Game_Restart(&WinCondition, points, &LivesCount, &worldTime, &player, &srednia, &FirstMove, &PlayerJumping, &PlayerFalling, &PurseTaken, &EndingJumpingPos, &SpawnTime, barrel, &moveX, &moveY, HitDetection, &StageNr, highScore, win);
			if (*win == 1)
			{
				quit = 1;
			}
			frames = 0;
			IFrames = 0;
		}

		if (fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		};

		SDL_FillRect(screen, NULL, czarny);
		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		Game_UI_visualization(screen, sprites, czerwony,text,charset, worldTime, fps, spritesReversed, &player, delta, barrel, LivesCount, HitDetection, &LastMove, &AnimationCheck, keystate, PlayerJumping, PlayerFalling, SpawnTime, &BarrelAnimationCheck, *points, &PurseTaken); // ostatnia ma 34,
		
		if(HitDetection == 0)
		{ 
		SpawnTime += delta;

		if (SpawnTime > 3.0)
		{
			Barrel_spawn(screen, sprites, &player, barrel); //spawn beczek co 3 sekundy
			SpawnTime = 0;
		}

 		Barrel_movement_check(screen, sprites, &player, barrel, delta, etiBarrelSpeed, platform, &FirstBarrelMove, ladder, &LivesCount, &HitDetection, IFrames,points,&BarrelJumpLock,text,charset,&PointsBlock);
		
		PurseCollisionCheck(&player,points, &PurseTaken,screen,text,charset,&PursePointsLock );
		
		Win_check(&player, &WinCondition);

		Player_Jumping_check(&PlayerJumping, &player, delta, &moveY, etiJumpSpeed, &EndingJumpingPos, &PlayerFalling);
		
		if (pointsExp >= 1.5)
		{
			*points -= 100;
			pointsExp = 0;
		}

		}
		
		
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
		FPS_60_LOCK(t1);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					quit = 1;
					break;
				case SDLK_RETURN:
					HitDetection = 0;
					break;
				case SDLK_n:
					Game_Restart(&WinCondition, points, &LivesCount, &worldTime, &player, &srednia, &FirstMove, &PlayerJumping, &PlayerFalling, &PurseTaken, &EndingJumpingPos, &SpawnTime, barrel, &moveX, &moveY, HitDetection, &StageNr, highScore, win);
					break;
				case SDLK_g:
					Cheat(&player, &moveY, &moveX);
					break;
				}
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			};
		};
		
		
		if (HitDetection == 0)
		{

		if (!(PlayerFalling == 1 && PlayerJumping == 0))
		{
			if (keystate[SDL_SCANCODE_RIGHT])
			{
				right_move_check(&player, delta, etiSpeedX, platform, &moveX, &srednia, &FirstMove, &EndingJumpingPos, &PlayerJumping, &PlayerFalling, &moveY);
			}

			if (keystate[SDL_SCANCODE_LEFT])
			{
				left_move_check(&player, delta, etiSpeedX, platform, &moveX, &srednia, &FirstMove, &EndingJumpingPos, &PlayerJumping, &PlayerFalling, &moveY);

			}
		}
		
		
		if (keystate[SDL_SCANCODE_UP] && PlayerJumping == 0)
		{
			ladder_move_check_up(&player, ladder, etiSpeedY, delta, &moveY, &FirstMove);
		}
			
		if (keystate[SDL_SCANCODE_DOWN] && PlayerJumping == 0)
		{
			ladder_move_check_down(&player, ladder, etiSpeedY, delta, &moveY, &FirstMove);
		}

		if (keystate[SDL_SCANCODE_SPACE])
		{
			if (PlayerFalling == 0 && player.on_ladder == 0)
			{
				player_jump_init(&PlayerJumping, &player, delta, &moveY, etiJumpSpeed, &EndingJumpingPos);
			}
		}

		}
		frames++;
	};

	SDL_FreeSurface(charset);
	SDL_FreeSurface(sprites);
	SDL_FreeSurface(spritesReversed);
}


// main
#ifdef __cplusplus
	extern "C"
#endif
	int main(int argc, char** argv) {
	int quit = 0,  rc, czarny, czerwony, zielony, niebieski, pomarancz, menu_choice, white, win = 0, points = 6000, numScores = 0;
	char name[15] = { '\0' };
	SDL_Surface* screen;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
	struct HighScore* highScore = NULL;
	
	Initializationsdl();

	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);

	rc_check(rc);
	set_renderer_and_window_title_and_cursor(renderer, window);

	screen = set_screen_surface();

	scrtex = set_scrtex_texture(renderer);

	set_colors(screen, &czarny, &zielony, &czerwony, &niebieski, &pomarancz, &white);

	menu_choice = 0;
	
	while (menu_choice != 1)
	{

	menu_choice = Menu_initalization_and_logic(renderer, screen, scrtex, window, czarny, pomarancz, czerwony);
	
	if (menu_choice == 2)
	{
		Game_initialization(renderer, screen, scrtex, window, czarny, czerwony, zielony, highScore, &win, &points);
		if (win == 1)
		{
			Nickname_input(&points, screen,renderer, scrtex,window,czarny, name);
			SaveHighScore("scores.txt", &points, name);
			win = 0;
		}
	}
	else if (menu_choice == 4)
	{
		LoadHighScores("scores.txt", &highScore, &numScores);
		Scores_load(highScore, screen, renderer, scrtex, window, czarny,highScore, numScores);
		free(highScore);

	}
	}
	
	
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}

	
