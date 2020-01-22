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

char my_ship[] = "<=^=>";
char bad_ship1[] = "<VV>";
char bad_ship2 = 'o';
char bad_ship3 = 'o';
int a;
int i = 0;
int x=38;
int xm=38;
int sens=1;
/*
 **===========================================================================
 **
 **  Abstract: main program
 **
 **===========================================================================
 */
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
	vt100_move(35,14);
	serial_puts("<mm> : 50 Pts");
	vt100_move(31,20);
	serial_puts("Press <space> to start !");

	while (a != 32) {
		a = serial_get_last_char();
	}

	vt100_clear_screen();
	vt100_move(x,20);
	serial_puts(my_ship);
	vt100_move(20,24);
	serial_puts("Vies : 3");
	vt100_move(50,24);
	serial_puts("Points : 0");

	while (1)
	{
		i++;

		if(i == 100)
		{
			if(sens == 1)
					{
						vt100_move(xm,10);
						serial_puts("    ");
						xm += 1;
						vt100_move(xm,10);
						serial_puts(bad_ship1);
						if(xm == 79)
						{
							sens=0;
						}
					}
					if(sens == 0)
					{
						vt100_move(xm,10);
						serial_puts("    ");
						xm -= 1;
						vt100_move(xm,10);
						serial_puts(bad_ship1);
						if(xm == 1)
						{
							sens=1;
						}
					}
					i=0;
		}




	a = serial_get_last_char();


	if (a == 'd')
	{
		vt100_move(x,20);
		serial_puts("     ");
		if(x != 76)
		{
			x += 1;
		}
		vt100_move(x,20);
		serial_puts(my_ship);
	}
	if (a == 'q')
		{
			vt100_move(x,20);
			serial_puts("     ");
			if(x != 1)
			{
				x -= 1;
			}
			vt100_move(x,20);
			serial_puts(my_ship);
		}

}
}
/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size) {
	/* TODO, implement your code here */
	return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void) {
	/* TODO, implement your code here */
	return -1;
}

