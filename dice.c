#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int rollDice(void)
{
	const int r = rand();
	const int dice = (r%6) + 1;
	return dice;
}

static int s_rolls[13];
static int s_nRolls;

static void printRolls(void)
{
	int i;
	printf("%d\n", s_nRolls);
	for (i = 2; i <= 12; i++)
	{
		const double num = (double)(s_rolls[i]);
		const double total = (double)s_nRolls;
		const double ratio = num/total;
		const float pc = (float)(100.0*ratio);
		printf("%d = %3.1f\n", i, pc);
	}
}

int main(int argc, char* argv[])
{
	int i;
	int count;

	printf("argc %d argv[0] '%s'\n", argc, argv[0]);

	for (i = 0; i < 13; i++)
	{
		s_rolls[i] = 0;
	}
	s_nRolls = 0;

	count = 0;
	do
	{
		int rollsToAdd[6];
		int placeToAdd;
		const int d1 = rollDice();
		const int d2 = rollDice();
		const int d3 = rollDice();
		const int d4 = rollDice();

		const int opt1 = d1 + d2;
		const int opt2 = d1 + d3;
		const int opt3 = d1 + d4;
		const int opt4 = d2 + d3;
		const int opt5 = d2 + d4;
		const int opt6 = d3 + d4;

		placeToAdd = 0;
		rollsToAdd[placeToAdd] = opt1;
		placeToAdd++;
		if (opt2 != opt1)
		{
			rollsToAdd[placeToAdd] = opt2;
			placeToAdd++;
		}
		if ((opt3 != opt1) && (opt3 != opt2))
		{
			rollsToAdd[placeToAdd] = opt3;
			placeToAdd++;
		}
		if ((opt4 != opt1) && (opt4 != opt2) && (opt4 != opt3))
		{
			rollsToAdd[placeToAdd] = opt4;
			placeToAdd++;
		}
		if ((opt5 != opt1) && (opt5 != opt2) && (opt5 != opt3) && (opt5 != opt4))
		{
			rollsToAdd[placeToAdd] = opt5;
			placeToAdd++;
		}
		if ((opt6 != opt1) && (opt6 != opt2) && (opt6 != opt3) && (opt6 != opt4) && (opt6 != opt5))
		{
			rollsToAdd[placeToAdd] = opt6;
			placeToAdd++;
		}
		for (i = 0; i < placeToAdd; i++)
		{
			const int roll = rollsToAdd[i];
			s_rolls[roll]++;
		}

		s_nRolls++;

		if ((count % (1024*1024)) == 0)
		{
			printRolls();
		}
		count++;
	} while (count < 1*1024*1024*1024);

	return 0;
}
