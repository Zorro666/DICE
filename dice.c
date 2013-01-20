#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int rollDice(void)
{
	const int r = rand();
	const int dice = (r%6) + 1;
	return dice;
}

#define MIN_DICE_ROLL (2)
#define MAX_DICE_ROLL (12)
#define MAX_NUM_COMBINATIONS (1024)

typedef struct Combination
{
	int m_roll[3];
	int m_count;
	int m_delta;
} Combination;

static int s_rolls[MAX_DICE_ROLL+1];
static int s_nRolls;
static int s_numCombinations;
static Combination s_combinations[MAX_NUM_COMBINATIONS];

static void combinationInit(Combination* const pThis)
{
	pThis->m_roll[0] = 0;
	pThis->m_roll[1] = 0;
	pThis->m_roll[2] = 0;
	pThis->m_count = 0;
	pThis->m_delta = 0;
}

static void printRolls(void)
{
	int i;
	printf("numRolls = %d\n", s_nRolls);
	for (i = MIN_DICE_ROLL; i <= MAX_DICE_ROLL; i++)
	{
		const double num = (double)(s_rolls[i]);
		const double total = (double)s_nRolls;
		const double ratio = num/total;
		const float pc = (float)(100.0*ratio);
		printf("%d = %3.1f\n", i, pc);
	}
}

static void printCombinations(void)
{
	int i;
	const int numCombinations = s_numCombinations;
	for (i = 0; i < numCombinations; i++)
	{
		const Combination* const pCombination = &s_combinations[i];
		if (pCombination->m_count != 0)
		{
			const double num = (double)(pCombination->m_count);
			const double total = (double)s_nRolls;
			const double ratio = num/total;
			const float pc = (float)(100.0*ratio);
			printf("%d, %d, %d = %3.1f\n", pCombination->m_roll[0], pCombination->m_roll[1], pCombination->m_roll[2], pc);
		}
	}
}

static int addCombination(const int a, const int b, const int c)
{
	int i;
	const int numCombinations = s_numCombinations;
	Combination* const pNewCombination = &s_combinations[numCombinations];
	int rolls[3];

	if ((a == b) || (a == c) || (b == c))
	{
		return 0;
	}
	rolls[0] = a;
	rolls[1] = b;
	rolls[2] = c;

	for (i = 0; i < 2; i++)
	{
		int j;
		for (j = i+1; j < 3; j++)
		{
			const int rollI = rolls[i];
			const int rollJ = rolls[j];
			if (rollJ < rollI)
			{
				const int temp = rollI;
				rolls[i] = rollJ;
				rolls[j] = temp;
			}
		}
	}

	for (i = 0; i < numCombinations; i++)
	{
		const Combination* const pCombination = &s_combinations[i];
		int roll;
		int match = 1;
		for (roll = 0; roll < 3; roll++)
		{
			if (pCombination->m_roll[roll] != rolls[roll])
			{
				match = 0;
				break;
			}
		}
		if (match == 1)
		{
			return 0;
		}
	}
	pNewCombination->m_roll[0] = rolls[0];
	pNewCombination->m_roll[1] = rolls[1];
	pNewCombination->m_roll[2] = rolls[2];
	pNewCombination->m_count = 0;
	pNewCombination->m_delta = 0;
	s_numCombinations++;

	return 1;
}

static void addRollToCombinations(const int roll)
{
	const int numCombinations = s_numCombinations;
	int i;
	for (i = 0; i < numCombinations; i++)
	{
		const Combination* const pCombination = &s_combinations[i];
		const int* const pRolls = pCombination->m_roll;
		if ((pRolls[0] == roll) || (pRolls[1] == roll) || (pRolls[2] == roll))
		{
			s_combinations[i].m_delta = 1;
		}
	}
}

static void updateCombinations(void)
{
	const int numCombinations = s_numCombinations;
	int i;
	for (i = 0; i < numCombinations; i++)
	{
		Combination* const pCombination = &s_combinations[i];
		pCombination->m_count += pCombination->m_delta;
		pCombination->m_delta = 0;
	}
}

static int makeCombinations(void)
{
	int numComb = 0;
	int i;
	for (i = MIN_DICE_ROLL; i <= MAX_DICE_ROLL-2; i++)
	{
		int j;
		for (j = i+1; j <= MAX_DICE_ROLL-1; j++)
		{
			int k;
			for (k = j+1; k <= MAX_DICE_ROLL; k++)
			{
				numComb += addCombination(i, j, k);
			}
		}
	}
	return numComb;
}

int main(int argc, char* argv[])
{
	int i;
	int count;

	printf("argc %d argv[0] '%s'\n", argc, argv[0]);

	for (i = 0; i < MAX_DICE_ROLL+1; i++)
	{
		s_rolls[i] = 0;
	}
	s_nRolls = 0;

	for (i = 0; i < MAX_NUM_COMBINATIONS; i++)
	{
		combinationInit(&s_combinations[i]);
	}
	s_numCombinations = 0;
	s_numCombinations = makeCombinations();
	printf("numCombinations %d\n", s_numCombinations);
	printCombinations();

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
			addRollToCombinations(roll);
			s_rolls[roll]++;
		}

		updateCombinations();
		s_nRolls++;

		if ((count % (1024*1024)) == 0)
		{
			printRolls();
			printCombinations();
			printf("numRolls %d\n", s_nRolls);
		}
		count++;
	} while (count < 1*1024*1024*1024);

	return 0;
}
