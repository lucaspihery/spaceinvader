/*
 ******************************************************************************

 SPACE INVADERS LUCAS PIHERY

 ******************************************************************************
 */

/* Includes */
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "vt100.h"
#include "serial.h"

int i = 0;
int k = 0;
uint8_t sens = 1;
char vie = '3';
uint8_t inp_clav;
uint8_t xv = 40;
uint8_t yv = 20;
uint8_t xa = 0;
uint8_t ya = 3;
char ship[] = "<=^=>";
char bad_ship1[] = "<VV>";
char bad_ship2 = 'o';
char bad_ship3 = 'o';

int main(void) {

	serial_init(115200);
	vt100_clear_screen();
	vt100_move(35, 3);
	serial_puts("SPACE INVADERS");
	vt100_move(36, 5);
	serial_puts("LUCAS PIHERY");
	vt100_move(35, 8);
	serial_puts("<TT> : 500 Pts");
	vt100_move(35, 10);
	serial_puts("<==> : 250 Pts");
	vt100_move(35, 12);
	serial_puts("<VV> : 100 Pts");
	vt100_move(35, 14);
	serial_puts("<mm> : 50 Pts");
	vt100_move(31, 20);
	serial_puts("Press <space> to start !");

	while (inp_clav != 32) /*32 ASCII de l'espace*/
	{
		inp_clav = serial_get_last_char();
	}
	vt100_clear_screen();
	vt100_move(xv, 20);
	serial_puts(ship);

	while (1) {
		i++;

		vt100_move(37, 1);
		serial_puts("SPACE INVADERS");
		vt100_move(1, 1);
		serial_puts("SCORE: ");
		vt100_move(65, 1);
		serial_puts("VIES: ");
		vt100_move(75, 1);
		serial_putchar(vie);
		inp_clav = serial_get_last_char();

		if (inp_clav == 'd') {
			vt100_move(xv, yv);
			serial_puts("    ");
			if (xv != 76) {
				xv += 1;
			}
			vt100_move(xv, yv);
			serial_puts(ship);

		}

		if (inp_clav == 'q') {
			vt100_move(xv, yv);
			serial_puts("     ");
			if (xv != 1) {
				xv -= 1;
			}
			vt100_move(xv, 20);
			serial_puts(ship);
		}

		/*alien*/

		if (i == 15) {
			if (sens == 1) {
				vt100_move(xa, ya);
				serial_puts("    ");
				xa += 1;
				vt100_move(xa, ya);
				serial_puts(bad_ship1);
				if (xa == 78) {
					vt100_move(xa, ya);
					serial_puts("    ");
					ya += 1;
					sens = 0;
				}

			}
			if (sens == 0) {
				vt100_move(xa, ya);
				serial_puts("    ");
				xa -= 1;
				vt100_move(xa, ya);
				serial_puts(bad_ship1);
				if (xa == 1) {
					vt100_move(xa, ya);
					serial_puts("    ");
					ya += 1;
					sens = 1;
				}

				if (xa == 80) {
					vt100_move(xa, ya);
					serial_puts("    ");
					xa = 0;
					ya += 1;
				}

			}
			i = 0;
		}
	}
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size) {
	return;
}

uint16_t EVAL_AUDIO_GetSampleCallBack(void) {
	return -1;
}

