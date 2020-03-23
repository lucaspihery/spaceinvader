/*
 ******************************************************************************

 SPACE INVADERS LUCAS PIHERY

 ******************************************************************************
 */

#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "vt100.h"
#include "serial.h"
#include "game_status.h"

typedef enum {
	Gauche, Droite
} direction;

typedef struct {
	uint8_t x;
	uint8_t y;
	direction sens;
	uint8_t status;
	uint8_t type;
} t_alien;

uint8_t input;
char c_score[4];
uint8_t random = 0;
int rand(void);

int main(void) {

	/* Coordonées */
	uint8_t x_vaisseau = 40;
	uint8_t y_vaisseau = 20;
	uint8_t x_missile = 40;
	uint8_t y_missile = 20;
	uint8_t x_bombe;
	uint8_t y_bombe;

	uint8_t alien_speed = 15;
	uint8_t live = 51;
	uint16_t score = 0;
	uint8_t missile_lance = 0;
	uint8_t bombe_lance = 0;
	uint8_t va;
	t_alien aliens[15]= {0};

	/* Skins */
	char missile = '|';
	char bombe = '*';
	char ship[] = "<=^=>";
	char alien1[] = "<VV>";
	char alien2[] = "<TT>";
	char alien3[] = "<==>";

	/* Compteur */
	int i = 0;
	int h = 0;
	int j = 0;
	int k = 0;

	/* Initialisation du tableau des aliens */
	for (va = 0; va < 5; va++) {
		aliens[va].y = 4;
		aliens[va].x = va * 8;
		aliens[va].status = 1;
		aliens[va].sens = Droite;
		aliens[va].type = 1;
	}
	for (va = 5; va < 10; va++) {
		aliens[va].y = 6;
		aliens[va].x = (va - 5) * 8;
		aliens[va].status = 1;
		aliens[va].sens = Droite;
		aliens[va].type = 2;
	}
	for (va = 10; va < 15; va++) {
		aliens[va].y = 8;
		aliens[va].x = (va - 10) * 8;
		aliens[va].status = 1;
		aliens[va].sens = Droite;
		aliens[va].type = 3;
	}

	serial_init(115200); /* Initialisation de la communication serie */

	game_waiting_screen(); /* Fonction d'initialisation du jeu */

	vt100_clear_screen();

	while (1) {

		i++;
		j++;

		if ((live == 48) || (score >= 4250)) {
			break;
		}

		sprintf(c_score, "%d", score); /* Converti l'entier score en une chaine de caractere */
		vt100_move(x_vaisseau, y_vaisseau);
		serial_puts(ship);
		vt100_move(2, 1);
		serial_puts("Score : ");
		serial_puts(c_score);
		vt100_move(72, 1);
		serial_puts("Lives : ");
		serial_putchar(live);

		input = serial_get_last_char(); /* Prend le code ascii de la derniere touche appuyé */

		if (input == 'd') {   /* Condition de déplacement a droite */
			vt100_move(x_vaisseau, y_vaisseau);
			serial_puts("    ");
			if (x_vaisseau != 76) {
				x_vaisseau += 1;
			}
			vt100_move(x_vaisseau, y_vaisseau);
			serial_puts(ship);

		}

		if (input == 'q') {   /* Condition de déplacement a gauche */
			vt100_move(x_vaisseau, y_vaisseau);
			serial_puts("     ");
			if (x_vaisseau != 1) {
				x_vaisseau -= 1;
			}
			vt100_move(x_vaisseau, 20);
			serial_puts(ship);
		}

		/* Missile */
		if (input == 32 && y_missile == y_vaisseau) {
			missile_lance = 1;
			x_missile = x_vaisseau + 2;
		}
		if (missile_lance == 1) {
			h++;

			if (y_missile != 3 && h == 10) {
				vt100_move(x_missile, y_missile);
				serial_putchar(' ');
				y_missile -= 1;
				vt100_move(x_missile, y_missile);
				serial_putchar(missile);
				h = 0;

				if (y_missile == 3) {
					vt100_move(x_missile, y_missile);
					serial_putchar(' ');
					y_missile = 20;
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
			x_bombe = aliens[random].x + 1;
			y_bombe = aliens[random].y;

		}
		if (bombe_lance == 1) {
			k++;
			if (y_bombe != 23 && k == 20) {
				vt100_move(x_bombe, y_bombe);
				serial_putchar(' ');
				y_bombe += 1;
				vt100_move(x_bombe, y_bombe);
				serial_putchar(bombe);
				k = 0;
				if (((x_bombe == x_vaisseau) || (x_bombe == x_vaisseau + 1) || (x_bombe == x_vaisseau + 2)
						|| (x_bombe == x_vaisseau + 3) || (x_bombe == x_vaisseau + 4)) && (y_bombe == y_vaisseau)) {
					live -= 1;
					vt100_move(x_bombe, y_bombe);
					serial_putchar(' ');
					bombe_lance = 0;
					j = 0;

				}
				if ((x_bombe == x_missile) && (y_bombe == y_missile)) {
					vt100_move(x_bombe, y_bombe);
					serial_putchar(' ');
					bombe_lance = 0;
					y_missile = 20;
					missile_lance = 0;
					j = 0;
				}
				if (y_bombe == 23) {
					vt100_move(x_bombe, y_bombe);
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

					if (aliens[va].sens == Droite) {
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
							aliens[va].sens = Gauche;
						}
					}
					if (aliens[va].sens == Gauche) {
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
							aliens[va].sens = Droite;
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
			if (((x_missile == aliens[va].x) || (x_missile == aliens[va].x + 1)
					|| (x_missile == aliens[va].x + 2) || (x_missile == aliens[va].x + 3))
					&& (y_missile == aliens[va].y)) {
				aliens[va].status = 0;
				vt100_move(aliens[va].x, aliens[va].y);
				serial_puts("    ");

				switch (aliens[va].type) {
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

				y_missile = 20;
				missile_lance = 0;
				aliens[va].x = 0;
				aliens[va].y = 24;
			}
		}

		/* Variation de vitesse */
		if ((score > 1500) && (alien_speed != 15)) {
			alien_speed = 15;
		}
		if ((score > 3000) && (alien_speed != 10)) {
			alien_speed = 10;
		}
	}

	if (live == 48) {
		game_loose(); /* Affiche l'écran de défaite */
	}

	game_win(); /* Affiche l'écran de victoire */
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size) {
	return;
}

uint16_t EVAL_AUDIO_GetSampleCallBack(void) {
	return -1;
}

