#include <stdint.h>
#include "serial.h"
#include "vt100.h"
#include "game_status.h"

extern uint8_t input;
extern char c_score[4];
uint8_t selection = 0;

void game_credit(void) {
	input = 0;
	vt100_clear_screen();
	vt100_move(37, 3);
	serial_puts("Credits :");
	vt100_move(24, 5);
	serial_puts("This game was created by Lucas Pihery");
	vt100_move(21, 7);
	serial_puts("A project proposed by Mr Vincent Poulailleau.");
	vt100_move(31, 11);
	serial_puts("github.com/lucaspihery");
	vt100_move(29, 22);
	serial_puts("Press <SPACE> to play !");

	while (input != 32) /*32 ASCII de l'espace*/
	{
		input = serial_get_last_char();
	}
}

void game_rules(void) {
	input = 0;
	vt100_clear_screen();
	vt100_move(38, 3);
	serial_puts("Controls :");
	vt100_move(30, 5);
	serial_puts("Q : Left   |   Right : D");
	vt100_move(35, 7);
	serial_puts("SPACE : Shoot");
	vt100_move(38, 11);
	serial_puts("Points :");
	vt100_move(35, 13);
	serial_puts("<VV> : 500 Pts");
	vt100_move(35, 15);
	serial_puts("<TT> : 250 Pts");
	vt100_move(35, 17);
	serial_puts("<==> : 100 Pts");
	vt100_move(30, 22);
	serial_puts("Press <SPACE> to play !");

	while (input != 32) /*32 ASCII de l'espace*/
	{
		input = serial_get_last_char();
	}
}

void game_waiting_screen(void) {
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

	vt100_move(23, 11);
	serial_puts("╔═╗╔═╗╔═╗╔═╗╔═╗  ╦╔╗╔╦  ╦╔═╗╔╦╗╔═╗╦═╗╔═╗");
	vt100_move(23, 12);
	serial_puts("╚═╗╠═╝╠═╣║  ║╣   ║║║║╚╗╔╝╠═╣ ║║║╣ ╠╦╝╚═╗");
	vt100_move(23, 13);
	serial_puts("╚═╝╩  ╩ ╩╚═╝╚═╝  ╩╝╚╝ ╚╝ ╩ ╩═╩╝╚═╝╩╚═╚═╝");
	vt100_move(37, 17);
	serial_puts("-> Play <-");
	vt100_move(40, 19);
	serial_puts("Rules");
	vt100_move(39, 21);
	serial_puts("Credits");

	while (input != 32) /*32 ASCII de l'espace*/
	{
		if (input == 'z') {
			if (selection > 0) {
				selection--;
				refresh_screen();
			}
		}
		if (input == 's') {
			if (selection < 2) {
				selection++;
				refresh_screen();
			}
		}

		input = serial_get_last_char();
	}
	switch (selection)
	{
	case 0:

		break;
	case 1:
		game_rules();
		break;
	case 2:
		game_credit();
		break;
	}
}

void game_loose(void) {

	vt100_clear_screen();
	vt100_move(29, 10);
	serial_puts("╔═╗╔═╗╔╦╗╔═╗  ╔═╗╦  ╦╔═╗╦═╗");
	vt100_move(29, 11);
	serial_puts("║ ╦╠═╣║║║║╣   ║ ║╚╗╔╝║╣ ╠╦╝");
	vt100_move(29, 12);
	serial_puts("╚═╝╩ ╩╩ ╩╚═╝  ╚═╝ ╚╝ ╚═╝╩╚═");
	vt100_move(34, 14);
	serial_puts("Your score : ");
	serial_puts(c_score);
	while (1) {

	}
}

void game_win(void) {

	vt100_clear_screen();
	vt100_move(31, 10);
	serial_puts("╦ ╦╔═╗╦ ╦  ╦ ╦╦╔╗╔");
	vt100_move(31, 11);
	serial_puts("╚╦╝║ ║║ ║  ║║║║║║║");
	vt100_move(31, 12);
	serial_puts(" ╩ ╚═╝╚═╝  ╚╩╝╩╝╚╝");
	vt100_move(31, 14);
	serial_puts("Your score : ");
	serial_puts(c_score);

	while (1) {

	}
}

int refresh_screen(void)
{
	switch (selection)
	{
	case 0:
		vt100_move(37, 17);
		serial_puts("-> Play <-");
		vt100_move(37, 19);
		serial_puts("  ");
		vt100_move(46, 19);
		serial_puts("  ");
		vt100_move(36, 21);
		serial_puts("  ");
		vt100_move(47, 21);
		serial_puts("  ");
		break;
	case 1:
		vt100_move(37, 19);
		serial_puts("-> Rules <-");
		vt100_move(37, 17);
		serial_puts("  ");
		vt100_move(45, 17);
		serial_puts("  ");
		vt100_move(36, 21);
		serial_puts("  ");
		vt100_move(47, 21);
		serial_puts("  ");
		break;
	case 2:
		vt100_move(36, 21);
		serial_puts("-> Credits <-");
		vt100_move(36, 17);
		serial_puts("  ");
		vt100_move(45, 17);
		serial_puts("  ");
		vt100_move(37, 19);
		serial_puts("  ");
		vt100_move(46, 19);
		serial_puts("  ");
		break;
	}
	return 0;
}


