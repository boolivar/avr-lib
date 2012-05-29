#include "freq.h"

#include <stdlib.h>

unsigned int cs0_table[8] = {1, 1, 8, 64, 256, 1024, 1, 1};	//Предделитель
unsigned int cs1_table[8] = {1, 1, 8, 32, 64, 128, 256, 1024};	//Предделитель

unsigned char fit_prescaler(unsigned long freq, unsigned int* top);

/*
	Выбор оптимального предделителя
	и вершины таймера для заданной частоты.
*/
unsigned char fit_prescaler(unsigned long freq, unsigned int* top, timer_prescalers_enum tps)
{
	const unsigned int* cs_table;
	unsigned long prescaled_freq;		// Частота после предделителя
	unsigned long pwm_top;				// Рассчетный делитель
	unsigned int pwm_freq;				// Фактическая частота ШИМ

	unsigned int delta;					// Разность заданной и фактической частот
	unsigned int best_delta = 0xFFFF;	// Наименьшая разность заданной и фактической частот
	unsigned char best_prescaler = 0;	// Индекс предделителя
	unsigned int best_top;

	unsigned char i;

	if (freq == 0)
	{
		return 0;
	}

	if (tps == TIMER_0_1_PS) {
		cs_table = cs0_table;
	} else {
		cs_table = cs1_table;
	}

	for (i = 1; i < 8; ++i)
	{
		prescaled_freq = F_CPU/cs_table[i];

		if (freq <= prescaled_freq)			//Если freq > prescaled_freq то получится 0
		{
			pwm_top = prescaled_freq/freq;	//Вершина ШИМ

			if (pwm_top <= 0xFFFF)			//Если больше - то не влезет в регистры
			{
				pwm_freq = prescaled_freq/pwm_top;	//Фактическая частота (округление отсечением)
				
				delta = abs(freq - pwm_freq);
				if (delta < best_delta)
				{
					best_delta = delta;
					best_top = pwm_top;
					best_prescaler = i;
				}
			}
		}
	}
	
	*top = best_top - 1;
	return best_prescaler;		//Если ничего подходящего - нулевое значение.
}
