/*
 * game_status.c
 *
 *  Created on: 4 mars 2020
 *      Author: lucas
 */
#include <stdint.h>
#include "serial.h"
#include "vt100.h"
#include "game_status.h"

	extern uint8_t input;

	void game_waiting_screen(void)
	{
		serial_init(115200);
		vt100_clear_screen();
		vt100_move(31, 2);
		serial_puts("    ##          ##");
		vt100_move(31, 3);
		serial_puts("      ##      ##");
		vt100_move(31, 4);
		serial_puts("    ##############");
		vt100_move(31, 5);
		serial_puts("  ####  ######  ####");
		vt100_move(31, 6);
		serial_puts("######################");
		vt100_move(31, 7);
		serial_puts("##  ##############  ##");
		vt100_move(31, 8);
		serial_puts("##  ##          ##  ##");
		vt100_move(31, 9);
		serial_puts("      ####  ####");
		vt100_move(35, 11);
		serial_puts("<TT> : 500 Pts");
		vt100_move(35, 13);
		serial_puts("<==> : 250 Pts");
		vt100_move(35, 15);
		serial_puts("<VV> : 100 Pts");
		vt100_move(35, 17);
		serial_puts("<mm> : 50 Pts");
		vt100_move(31, 22);
		serial_puts("Press <space> to start !");

		while (input != 32) /*32 ASCII de l'espace*/
		{
			input = serial_get_last_char();
		}
	}

	void game_loose(void)
	{

		vt100_clear_screen();
		vt100_move(35,9);
		serial_puts("GAME OVER !");
		vt100_move(34,11);
		serial_puts("Your score : ");
		//serial_putchar(score);

		while(1)
		{

		}
	}


	void game_win(void)
	{

		vt100_clear_screen();
		vt100_move(35,9);
		serial_puts("YOU WIN !");
		vt100_move(34,11);
		serial_puts("Your score : ");
		//serial_putchar(score);

		while(1)
		{

		}
	}

