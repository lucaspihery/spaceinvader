/*
 ******************************************************************************

 SPACE INVADERS LUCAS PIHERY

 ******************************************************************************
 */

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "vt100.h"
#include "serial.h"
#include "game_status.h"

typedef struct {
	uint8_t x;
	uint8_t y;
	uint8_t sens;
	uint8_t status;
	uint8_t type;
} alien;

uint8_t alien_killed = 0;
uint8_t alien_speed = 15;
uint8_t live = 51;
uint16_t score = 0;
uint8_t input;
uint8_t xv = 40;
uint8_t yv = 20;
uint8_t missile_lance = 0;
uint8_t bombe_lance = 0;
uint8_t xm = 40;
uint8_t ym = 20;
uint8_t xb;
uint8_t yb;
char c_score[4];
char missile = '|';
char bombe = '*';
char ship[] = "<=^=>";
char alien1[] = "<VV>";
char alien2[] = "<TT>";
char alien3[] = "<==>";

uint8_t va = 0;
alien aliens[15] = { 0 };

int main(void) {

	int i = 0;
	int h = 0;
	int j = 0;
	int k = 0;
	uint8_t random = 0;
	int rand(void);
	for (va = 0; va < 5; va++) {
		aliens[va].y = 4;
		aliens[va].x = va * 8;
		aliens[va].status = 1;
		aliens[va].sens = 1;
		aliens[va].type = 1;
	}
	for (va = 5; va < 10; va++) {
		aliens[va].y = 6;
		aliens[va].x = (va - 5) * 8;
		aliens[va].status = 1;
		aliens[va].sens = 1;
		aliens[va].type = 2;
	}
	for (va = 10; va < 15; va++) {
		aliens[va].y = 8;
		aliens[va].x = (va - 10) * 8;
		aliens[va].status = 1;
		aliens[va].sens = 1;
		aliens[va].type = 3;
	}

	serial_init(115200);

	game_waiting_screen(); /* Fonction d'initialisation du jeu */

	vt100_clear_screen();
	vt100_move(xv, 20);
	serial_puts(ship);

	for (va = 0; va < 15; va++) {
		if (va < 5) {
			vt100_move(aliens[va].x, aliens[va].y);
			serial_puts(alien1);
		}
		if ((5 <= va) && (va < 10)) {
			vt100_move(aliens[va].x, aliens[va].y);
			serial_puts(alien2);
		}
		if (va >= 10) {
			vt100_move(aliens[va].x, aliens[va].y);
			serial_puts(alien3);
		}
	}

	while (1) {

		if (live == 48) {
			break;
		}

		i++;
		j++;
		sprintf(c_score, "%d", score);
		vt100_move(xv, yv);
		serial_puts(ship);
		vt100_move(2, 1);
		serial_puts("Score : ");
		serial_puts(c_score);
		vt100_move(72, 1);
		serial_puts("Lives : ");
		serial_putchar(live);

		input = serial_get_last_char();

		if (input == 'd') {
			vt100_move(xv, yv);
			serial_puts("    ");
			if (xv != 76) {
				xv += 1;
			}
			vt100_move(xv, yv);
			serial_puts(ship);

		}

		if (input == 'q') {
			vt100_move(xv, yv);
			serial_puts("     ");
			if (xv != 1) {
				xv -= 1;
			}
			vt100_move(xv, 20);
			serial_puts(ship);
		}

		/* Missile */
		if (input == 32 && ym == yv) {
			missile_lance = 1;
			xm = xv + 2;
		}
		if (missile_lance == 1) {
			h++;

			if (ym != 3 && h == 10) {
				vt100_move(xm, ym);
				serial_putchar(' ');
				ym -= 1;
				vt100_move(xm, ym);
				serial_putchar(missile);
				h = 0;

				if (ym == 3) {
					vt100_move(xm, ym);
					serial_putchar(' ');
					ym = 20;
					missile_lance = 0;
				}
			}
		}
		/* Bombe alien */
		if (j == 300) {
			random = rand() % 15;
			while (aliens[random].status == 0) {
				random = rand() % 15;
			}
			bombe_lance = 1;
			xb = aliens[random].x + 1;
			yb = aliens[random].y;

		}
		if (bombe_lance == 1) {
			k++;
			if (yb != 23 && k == 20) {
				vt100_move(xb, yb);
				serial_putchar(' ');
				yb += 1;
				vt100_move(xb, yb);
				serial_putchar(bombe);
				k = 0;
				if (((xb == xv) || (xb == xv + 1) || (xb == xv + 2)
						|| (xb == xv + 3) || (xb == xv + 4)) && (yb == yv)) {
					live -= 1;
					vt100_move(xb, yb);
					serial_putchar(' ');
					bombe_lance = 0;
					j = 0;

				}
				if ((xb == xm) && (yb == ym)) {
					vt100_move(xb, yb);
					serial_putchar(' ');
					bombe_lance = 0;
					ym = 20;
					missile_lance = 0;
					j = 0;
				}
				if (yb == 23) {
					vt100_move(xb, yb);
					serial_putchar(' ');
					bombe_lance = 0;
					j = 0;
				}
			}
		}

		/* Deplacement des aliens */
		if (i == alien_speed) {
			for (va = 0; va < 15; va++) {
				if (aliens[va].status == 1) {
					vt100_move(aliens[va].x, aliens[va].y);
					serial_puts("    ");

					if (aliens[va].sens == 1) {
						aliens[va].x += 1;
						vt100_move(aliens[va].x, aliens[va].y);
						switch (aliens[va].type) {
						case 1:
							serial_puts(alien1);
							break;
						case 2:
							serial_puts(alien2);
							break;
						case 3:
							serial_puts(alien3);
							break;
						}
						if (aliens[va].x == 78) {
							vt100_move(aliens[va].x, aliens[va].y);
							serial_puts("    ");
							aliens[va].y += 1;
							aliens[va].sens = 0;
						}
					}
					if (aliens[va].sens == 0) {
						aliens[va].x -= 1;
						vt100_move(aliens[va].x, aliens[va].y);
						switch (aliens[va].type) {
						case 1:
							serial_puts(alien1);
							break;
						case 2:
							serial_puts(alien2);
							break;
						case 3:
							serial_puts(alien3);
							break;
						}
						if (aliens[va].x == 0) {
							vt100_move(aliens[va].x, aliens[va].y);
							serial_puts("    ");
							aliens[va].y += 1;
							aliens[va].sens = 1;
						}
					}
					if (aliens[va].y == 20) {
						live = 48;
					}
					i = 0;
				}
			}
		}

		/* Collision missile alien */
		for (va = 0; va < 15; va++) {
			if (((xm == aliens[va].x) || (xm == aliens[va].x + 1)
					|| (xm == aliens[va].x + 2) || (xm == aliens[va].x + 3))
					&& (ym == aliens[va].y)) {
				aliens[va].status = 0;
				vt100_move(aliens[va].x, aliens[va].y);
				serial_puts("    ");
				alien_killed++;

				switch (aliens[va].type)
				{
				case 1:
					score += 500;
					break;
				case 2:
					score += 250;
					break;
				case 3:
					score += 100;
					break;
				}

				ym = 20;
				missile_lance = 0;
				aliens[va].x = 0;
				aliens[va].y = 24;
			}
		}

		/* Variation de vitesse */
		if (alien_killed == 5) {
			alien_speed = 10;
		}
		if (alien_killed == 10) {
			alien_speed = 5;
		}
	}
	if (live == 48) {
		game_loose();
	}

	game_win();
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size) {
	return;
}

uint16_t EVAL_AUDIO_GetSampleCallBack(void) {
	return -1;
}

