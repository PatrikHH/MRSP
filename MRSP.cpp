#include <stdio.h>
#include <stdlib.h>
#include <iostream>  
#include <fstream> 
#pragma warning(disable:4996)

/*
* seznam funkci:
* create_TSR_65 - slouzi pro vytvoreni vektoru dat z paketu 65 (pridani noveho TSR)
* create_SSP - slouzi pro vytvoreni vektoru dat z paketu 27 (pridani noveho SSP)
* create_MRSP - uvodni vytvoreni pole MRSP na zaklade SSP/TSR/narodnich hodnot
* sort_MRSP - slouzi pro prepocet pole MRSP za zaklade pridani noveho TSR
* modify_MRSP - slouzi k pridani/smazani zaznamu z pole MRSP vzhledem k TSR/SSP
* two_zeros_MRSP - pokud pri prvnostnim sestaveni MRSP jsou na prvnich dvou radcich zaznamy s pocatkem v 0, taot funkce je prepocita 
* delete_row_MRSP - vymazani radku z MRSP po prepoctu
* add_row_MRSP - pridano nove radky pro prepocet MRSP 
* create_field_p_p - vytvoreni dynamicky alokovaneho pole typu pointer na pointer
* realocate_field_p_p - realokace dynamicky alokovaneho pole typu pointer na pointer
* hardcode_test - data pro hardcode testu
* file_test - data pro test se nacitaji ze souboru
* default_test - uvodni zadani dat
*/

typedef struct tsr65 {
	unsigned int d_tsr;
	unsigned int l_tsr;
	unsigned int v_tsr;
	unsigned int q_tsr;
	unsigned int id_tsr;
	unsigned int flag_tsr;
} TSR65;
TSR65 tsr65;

typedef struct default_profile {
	unsigned int d_default = 0;
	unsigned int v_default = 160;
	unsigned int l_default = 327670;
	unsigned int q_default = 0;
	unsigned int id_default = 0;
	unsigned int flag_default = 1;
} DEFAULT_PROFILE;
DEFAULT_PROFILE def_pro;

typedef struct DATA {
	int** values;
} DATA;
DATA data;

typedef struct SSP {
	int** values;
} SSP;
SSP ssp;

typedef struct MRSP {
	int** values;
} MRSP;
MRSP mrsp;

constexpr unsigned int q_front = 0, infinity_number = 327670, row_flag_one = 1, row_flag_zero = 0, column = 6;
constexpr int id_ssp = -1;

void create_TSR_65(unsigned int* p_row, unsigned int i_test, unsigned int* p_init_TSR, int* p_mod_TSR, int* p_modify_rows);
void create_SSP(unsigned int* p_row, unsigned int i_test, unsigned int* p_init_SSP, unsigned int* p_mod_SSP, int* p_modify_rows, int* p_row_SSP, unsigned int* p_row_test_ssp, unsigned int* p_row_test_tsr);
void create_MRSP(unsigned int* p_row, unsigned int* p_init_MRSP, int* p_modify_rows, int* p_mod_TSR, unsigned int* p_mod_SSP);
void sort_MRSP(unsigned int* p_row, int* p_modify_rows);
void modify_MRSP(unsigned int* p_row, int* p_modify_rows, int* p_mod_TSR, unsigned int* p_mod_SSP);
void two_zeros_MRSP(unsigned int* p_row, int* p_modify_rows, unsigned int iter);
void delete_row_MRSP(unsigned int* p_row, int* p_modify_rows);
void add_row_MRSP(unsigned int* p_row, int* p_modify_rows, unsigned int iter, int* l_tsr_prev, int** p_p);
void hardcode_test(unsigned int* p_row_test_ssp, unsigned int* p_row_test_tsr);
void file_test(unsigned int* p_row_test_ssp, unsigned int* p_row_test_tsr);
void default_test(unsigned int* p_row_test_ssp, unsigned int* p_row_test_tsr);
int** create_field_p_p(unsigned int* p_row, unsigned int column);
int** realocate_field_p_p(unsigned int* p_row, int* p_modify_rows, int** p_p);

int main(unsigned int argc, char** argv)
{
	unsigned int i, i_test = 0, i_test_max = 3, p_row = 1, p_init_TSR = 1, p_init_SSP = 1, p_init_MRSP = 1, p_mod_SSP = 0, p_row_test_ssp = 8, p_row_test_tsr = 3;
	int p_row_SSP = 0, j, p_modify_rows, p_mod_TSR = 0;

	if ((argc == 2) && (strcmp(argv[1], "-t") == 0))
		hardcode_test(&p_row_test_ssp, &p_row_test_tsr);
	else if ((argc == 2) && (strcmp(argv[1], "-f") == 0))
		file_test(&p_row_test_ssp, &p_row_test_tsr);
	else
	{ 
		i_test_max = 2;
		p_row_test_tsr = 2;
		p_row_test_ssp = 5;
		default_test(&p_row_test_ssp, &p_row_test_tsr);
	}
	create_TSR_65(&p_row, i_test, &p_init_TSR, &p_mod_TSR, &p_modify_rows);
	create_SSP(&p_row, i_test_max, &p_init_SSP, &p_mod_SSP, &p_modify_rows, &p_row_SSP, &p_row_test_ssp, &p_row_test_tsr);
	create_MRSP(&p_row, &p_init_MRSP, &p_modify_rows, &p_mod_TSR, &p_mod_SSP);
	for (i_test = 0; i_test < i_test_max; i_test++)
	{ 
		create_TSR_65(&p_row, i_test , &p_init_TSR, &p_mod_TSR, &p_modify_rows);
		create_MRSP(&p_row, &p_init_MRSP, &p_modify_rows, &p_mod_TSR, &p_mod_SSP);
	}
	for (i = 0; i < p_row; i++)
	{
		for (j = 0; j < column - 3; j++)
			printf("%d\t", mrsp.values[i][j]);
		printf("\n");
	}
	return 0;
}

void create_TSR_65(unsigned int* p_row, unsigned int i_test, unsigned int* p_init_TSR, int* p_mod_TSR, int* p_modify_rows)
{
	tsr65.d_tsr = data.values[i_test][0];
	tsr65.l_tsr = data.values[i_test][1];
	tsr65.v_tsr = data.values[i_test][2];
	tsr65.q_tsr = q_front;
	tsr65.id_tsr = rand() % 20 + 1;
	tsr65.flag_tsr = row_flag_one;

	if (*p_init_TSR)
	{
		*p_row += 1;
		*p_init_TSR = 0;
	}
	else
	{
		*p_modify_rows = 1;
		*p_mod_TSR = 1;
	}
}

void create_SSP(unsigned int* p_row, unsigned int i_test_max, unsigned int* p_init_SSP, unsigned int* p_mod_SSP, int* p_modify_rows, int* p_row_SSP, unsigned int* p_row_test_ssp, unsigned int* p_row_test_tsr)
{
	unsigned int i, j, y = 0;
	int p_row_new_SSP;
	
	if (*p_init_SSP)
	{
		*p_row += *p_row_test_ssp;
		*p_row_SSP = *p_row_test_ssp;
		*p_init_SSP = 0;
		ssp.values = create_field_p_p(p_row_test_ssp, column);
	}
	else
	{
		p_row_new_SSP = *p_row_test_ssp;
		*p_modify_rows = -(*p_row_SSP - p_row_new_SSP);
		ssp.values = realocate_field_p_p(p_row, p_modify_rows, ssp.values);
		*p_mod_SSP = 2;
	}
	for (i = 0; i < *p_row_test_ssp; i++)
	{
		for (j = 0; j < column; j++) 
		{
			if (j == 2)
				if (i == *p_row_test_ssp - 1)
					ssp.values[i][j] = infinity_number;
				else
					ssp.values[i][j] = data.values[i + i_test_max + 1][0];
			else if (j == 3)
				ssp.values[i][j] = q_front;
			else if (j == 4)
				ssp.values[i][j] = id_ssp;
			else if (j == 5)
				ssp.values[i][j] = row_flag_one;
			else if (j < 2)
				ssp.values[i][j] = data.values[i + i_test_max][j];				
		}
		if (i != 0)
			ssp.values[i][0] += ssp.values[i - 1][0];
	}
}

void create_MRSP(unsigned int* p_row, unsigned int* p_init_MRSP, int* p_modify_rows, int* p_mod_TSR, unsigned int* p_mod_SSP)
{
	unsigned int i, j, TSR_written = 0;
	
	if (*p_init_MRSP)
	{
		mrsp.values = create_field_p_p(p_row, column);
		for (i = 0; i < *p_row + (TSR_written - 1); i++)
		{
			if (i == 0)
			{
				if ((ssp.values[0][0] <= tsr65.d_tsr))
				{
					for (j = 0; j < column; j++)
						mrsp.values[0][j] = ssp.values[0][j];
					mrsp.values[0][5] = row_flag_one;
				}
				else if ((ssp.values[0][0] > tsr65.d_tsr))
				{
					mrsp.values[0][0] = tsr65.d_tsr;
					mrsp.values[0][1] = tsr65.v_tsr;
					mrsp.values[0][2] = tsr65.l_tsr;
					mrsp.values[0][3] = tsr65.q_tsr;
					mrsp.values[0][4] = tsr65.id_tsr;
					mrsp.values[0][5] = tsr65.flag_tsr;
					TSR_written = 1;
					break;
				}		
			}
			else if (i > 0)
			{
				if ((TSR_written < 1) && (i < (*p_row - 1)) && (mrsp.values[i - 1][0] <= tsr65.d_tsr) && (ssp.values[i][0] > tsr65.d_tsr))
				{
					mrsp.values[i][0] = tsr65.d_tsr;
					mrsp.values[i][1] = tsr65.v_tsr;
					mrsp.values[i][2] = tsr65.l_tsr;
					mrsp.values[i][3] = tsr65.q_tsr;
					mrsp.values[i][4] = tsr65.id_tsr;
					mrsp.values[i][5] = tsr65.flag_tsr;
					TSR_written = 1;
				}
				else if ((TSR_written < 1) && (i == (*p_row - 1)))
				{
					mrsp.values[i][0] = tsr65.d_tsr;
					mrsp.values[i][1] = tsr65.v_tsr;
					mrsp.values[i][2] = tsr65.l_tsr;
					mrsp.values[i][3] = tsr65.q_tsr;
					mrsp.values[i][4] = tsr65.id_tsr;
					mrsp.values[i][5] = tsr65.flag_tsr;
					TSR_written = 1;
				}
				else if (i == (*p_row) - 1 - (TSR_written - 1))
				{
					mrsp.values[i][0] = def_pro.d_default;
					mrsp.values[i][1] = def_pro.v_default;
					mrsp.values[i][2] = def_pro.l_default;
					mrsp.values[i][3] = def_pro.q_default;
					mrsp.values[i][4] = def_pro.id_default;
					mrsp.values[i][5] = def_pro.flag_default;
				}
				else
				{
					for (j = 0; j < column; j++)
						mrsp.values[i][j] = ssp.values[i - TSR_written][j];
					mrsp.values[i][5] = row_flag_one;
				}
			}	
		}
		sort_MRSP(p_row, p_modify_rows);
		*p_init_MRSP = 0;
	}
	else
	{ 
		modify_MRSP(p_row, p_modify_rows, p_mod_TSR, p_mod_SSP);
		sort_MRSP(p_row, p_modify_rows);
	}
}

void sort_MRSP(unsigned int* p_row, int* p_modify_rows)
{
	int l_tsr = 0, line_delete = 0, l_reziduum = 0, l_tsr_prev = 0, *p_l_tsr_prev = &l_tsr_prev;
	unsigned int i, u, j, del = 0;

	for (i = 0; i < *p_row - 1; i++)
	{
		if ((mrsp.values[i][0] == 0) && (mrsp.values[i + 1][0] == 0))
			two_zeros_MRSP(p_row, p_modify_rows, i);
		if ((mrsp.values[i][4] > 0) && (mrsp.values[i][5] == row_flag_one))
		{
			mrsp.values[i][5] = row_flag_zero;
			if ((i > 0) && (mrsp.values[i][1] > mrsp.values[i - 1][1]))
			{
				if ((mrsp.values[i][0] + mrsp.values[i][2]) < (mrsp.values[i - 1][0] + mrsp.values[i - 1][2]))
					mrsp.values[i][2] = line_delete;
					
				else if ((mrsp.values[i][0] + mrsp.values[i][2]) > (mrsp.values[i - 1][0] + mrsp.values[i - 1][2]))
					{
					mrsp.values[i][2] -= (mrsp.values[i - 1][2] - (mrsp.values[i][0] - mrsp.values[i - 1][0]));
					mrsp.values[i][0] += (mrsp.values[i - 1][2] - (mrsp.values[i][0] - mrsp.values[i - 1][0]));
					}
				else
					{
					mrsp.values[i][2] -= (mrsp.values[i][0] - mrsp.values[i - 1][0]);
					mrsp.values[i][0] += (mrsp.values[i][0] - mrsp.values[i - 1][0]);
					}
			}
			else if ((i > 0) && (mrsp.values[i][1] <= mrsp.values[i - 1][1]))
				if ((mrsp.values[i - 1][0] + mrsp.values[i - 1][2]) > (mrsp.values[i][0]))
					if (mrsp.values[i][2] < (mrsp.values[i - 1][2] - (mrsp.values[i][0] - mrsp.values[i - 1][0])))
					{
						*p_l_tsr_prev = (mrsp.values[i - 1][2] - (mrsp.values[i][0] - mrsp.values[i - 1][0])) - mrsp.values[i][2];
						mrsp.values[i - 1][2] = mrsp.values[i][0] - mrsp.values[i - 1][0];
						add_row_MRSP(p_row, p_modify_rows, i, p_l_tsr_prev, mrsp.values);
					}
					else
					{
						mrsp.values[i - 1][2] = mrsp.values[i][0] - mrsp.values[i - 1][0];
						if (mrsp.values[i][0] == infinity_number)
							mrsp.values[i][2] = line_delete;
					}	
			if (mrsp.values[i][2] != line_delete)
			{
				l_tsr = mrsp.values[i][2] - (mrsp.values[i + 1][0] - mrsp.values[i][0]);
				for (del = i + 1; del < *p_row - 1; del++)
				if (l_tsr >= mrsp.values[del][2])
				{
					l_tsr -= mrsp.values[del][2];
					mrsp.values[del][2] = line_delete;
				}
				else
				{
					if (mrsp.values[del][2] < infinity_number)
						mrsp.values[del][2] -= l_tsr;
					mrsp.values[del][0] += l_tsr;
					break;
				}
			}
			if ((i > 0) && (mrsp.values[i - 1][2] == infinity_number))
				mrsp.values[i - 1][2] = (mrsp.values[i][0] - mrsp.values[i - 1][0]);
			delete_row_MRSP(p_row, p_modify_rows);
			if (mrsp.values[i][4] > 0)
				add_row_MRSP(p_row, p_modify_rows, i, p_l_tsr_prev, mrsp.values);
		}	
	}
}

void modify_MRSP(unsigned int* p_row, int* p_modify_rows, int* p_mod_TSR, unsigned int* p_mod_SSP)
{
	unsigned int i, j, TSR_written = 0, change_SSP_rows = *p_modify_rows;
	
	if (*p_mod_TSR == 1)
	{
		mrsp.values = realocate_field_p_p(p_row, p_modify_rows, mrsp.values);
		for (i = (*p_row) - 1; i > 0; i--)
		{
			if ((TSR_written < 1) && (i == 0) && (tsr65.d_tsr <= mrsp.values[0][0]))
			{
				mrsp.values[i][0] = tsr65.d_tsr;
				mrsp.values[i][1] = tsr65.v_tsr;
				mrsp.values[i][2] = tsr65.l_tsr;
				mrsp.values[i][3] = tsr65.q_tsr;
				mrsp.values[i][4] = tsr65.id_tsr;
				mrsp.values[i][5] = row_flag_one;
				TSR_written = 1;
			}
			else if ((TSR_written < 1) && (tsr65.d_tsr > mrsp.values[i - 1][0]) && (mrsp.values[i - 1][4] != 0))
			{
				mrsp.values[i][0] = tsr65.d_tsr;
				mrsp.values[i][1] = tsr65.v_tsr;
				mrsp.values[i][2] = tsr65.l_tsr;
				mrsp.values[i][3] = tsr65.q_tsr;
				mrsp.values[i][4] = tsr65.id_tsr;
				mrsp.values[i][5] = row_flag_one;
				TSR_written = 1;
			}
			else if (((i != (*p_row) - 1) && (TSR_written < 1) && (tsr65.d_tsr >= (mrsp.values[i - 1][0])) && (tsr65.d_tsr <= mrsp.values[i][0])))
			{
				mrsp.values[i][0] = tsr65.d_tsr;
				mrsp.values[i][1] = tsr65.v_tsr;
				mrsp.values[i][2] = tsr65.l_tsr;
				mrsp.values[i][3] = tsr65.q_tsr;
				mrsp.values[i][4] = tsr65.id_tsr;
				mrsp.values[i][5] = row_flag_one;
				TSR_written = 1;
			}
			else
				for (j = 0; j < column; j++)
					mrsp.values[i][j] = mrsp.values[i - 1 + TSR_written][j];
		}
	}
	else if (*p_mod_TSR == -1)
		; // odvolani paketu 65
	else if (*p_mod_TSR == 2)
		; // nove SSP
}

void two_zeros_MRSP(unsigned int* p_row, int* p_modify_rows, unsigned int i)
{
	int row_sort[6], j;

	if (mrsp.values[i][1] > mrsp.values[i + 1][1])
		for (j = 0; j < column; j++)
		{
			row_sort[j] = mrsp.values[i][j];
			mrsp.values[i][j] = mrsp.values[i + 1][j];
			mrsp.values[i + 1][j] = row_sort[j];
		}
	mrsp.values[i + 1][0] = mrsp.values[i][2];
	mrsp.values[i + 1][2] -= mrsp.values[i + 1][0];
	delete_row_MRSP(p_row, p_modify_rows);
}

void delete_row_MRSP(unsigned int* p_row, int* p_modify_rows)
{
	unsigned int i = 0, j, del;

	for (i = 0; i < *p_row - 1; i++)
		while (mrsp.values[i][2] <= 0)
		{
			*p_modify_rows = -1;
			for (del = i; del < *p_row - 1; del++)
				for (j = 0; j < column; j++)
					mrsp.values[del][j] = mrsp.values[del + 1][j];
			mrsp.values = realocate_field_p_p(p_row, p_modify_rows, mrsp.values);
		}
}

void add_row_MRSP(unsigned int* p_row, int* p_modify_rows, unsigned int i, int* p_l_tsr_prev, int** p_p)
{
	unsigned int ii = 0, j, del = 0, l_tsr = 0;

	if (*p_l_tsr_prev > 0)
	{
		*p_modify_rows = 1;
		mrsp.values = realocate_field_p_p(p_row, p_modify_rows, mrsp.values);
		for (ii = *p_row - 1; ii > i + 1; ii--)
			for (j = 0; j < column; j++)
				mrsp.values[ii][j] = mrsp.values[ii - 1][j];
		mrsp.values[ii][0] = mrsp.values[i][0] + mrsp.values[i][2];
		mrsp.values[ii][1] = mrsp.values[i - 1][1];
		mrsp.values[ii][2] = *p_l_tsr_prev;
		mrsp.values[ii][3] = q_front;
		mrsp.values[ii][4] = mrsp.values[i - 1][4];
		mrsp.values[ii][5] = row_flag_zero;
		*p_l_tsr_prev = 0;
	}
	else
	{
		l_tsr = mrsp.values[i][2];
		if (mrsp.values[i][1] >= mrsp.values[i + 1][1])
		{
			mrsp.values[i][2] = mrsp.values[i + 1][0] - mrsp.values[i][0];
			l_tsr -= mrsp.values[i][2];
		}
		else
		{ 
			mrsp.values[i + 1][0] = (mrsp.values[i][0] + mrsp.values[i][2]);
			l_tsr = 0;
		}
		for (del = i + 1; del < *p_row - 1; del++)
			if (l_tsr > 0)
			{
				if (mrsp.values[del][4] == 0)
					break;
				if ((mrsp.values[del][0] + mrsp.values[del][2]) == mrsp.values[del + 1][0])
					l_tsr -= mrsp.values[del][2];
				else if ((mrsp.values[del][0] + mrsp.values[del][2]) < mrsp.values[del + 1][0])
				{
					l_tsr -= mrsp.values[del][2];
					*p_modify_rows = 1;
					mrsp.values = realocate_field_p_p(p_row, p_modify_rows, mrsp.values);
					del++;
					for (ii = *p_row - 1; ii > i + del - 1; ii--)
						for (j = 0; j < column; j++)
							mrsp.values[ii][j] = mrsp.values[ii - 1][j];	
					mrsp.values[ii][0] = mrsp.values[del - 1][0] + mrsp.values[del - 1][2];
					mrsp.values[ii][1] = mrsp.values[i][1];
					mrsp.values[ii][2] = mrsp.values[del + 1][0] - mrsp.values[del][0];
					mrsp.values[ii][3] = q_front;
					mrsp.values[ii][4] = mrsp.values[i][4];
					mrsp.values[ii][5] = row_flag_zero;
					l_tsr -= mrsp.values[ii][2];
				}
			}
			else
				break;	
	}
}

int** create_field_p_p(unsigned int* p_row, unsigned int column)
{
	int** p_p_field;
	unsigned int i;

	p_p_field = new int* [*p_row * sizeof(SSP*)];
	for (i = 0; i < *p_row; i++)
		p_p_field[i] = new int[(column) * sizeof(SSP)];
	return p_p_field;
}

int** realocate_field_p_p(unsigned int* p_row, int* p_modify_rows, int** p_p)
{
	int** new_p_p = NULL;
	int i;
	if (*p_modify_rows >= 1)
	{
		new_p_p = (int**)realloc(p_p, sizeof * p_p * (*p_row + *p_modify_rows));
		for (i = 0; i < *p_modify_rows; i++)
			new_p_p[*p_row + i] = (int*)malloc(sizeof * new_p_p[*p_row + i] * (column));
	}
	else
	{
		for (i = 1; i <= abs(*p_modify_rows); i++)
			free(p_p[*p_row - i]);
		new_p_p = (int**)realloc(p_p, sizeof * p_p * (*p_row + *p_modify_rows));
	}
	*p_row += *p_modify_rows;
	*p_modify_rows = 0;
	return (int**)new_p_p;
}

void hardcode_test(unsigned int* p_row_test_ssp, unsigned int* p_row_test_tsr)
{
	unsigned int row_test_hardcode, * p_row_test_hardcode = &row_test_hardcode;
	row_test_hardcode = *p_row_test_ssp + *p_row_test_tsr;

	data.values = create_field_p_p(p_row_test_hardcode, column - 3);

	data.values[0][0] = 10;
	data.values[0][1] = 200;
	data.values[0][2] = 55;

	data.values[1][0] = 300;
	data.values[1][1] = 550;
	data.values[1][2] = 70;

	data.values[2][0] = 860;
	data.values[2][1] = 250;
	data.values[2][2] = 25;

	data.values[3][0] = 0;
	data.values[3][1] = 100;

	data.values[4][0] = 150;
	data.values[4][1] = 80;

	data.values[5][0] = 210;
	data.values[5][1] = 70;

	data.values[6][0] = 240;
	data.values[6][1] = 15;

	data.values[7][0] = 410;
	data.values[7][1] = 80;
	
	data.values[8][0] = 790;
	data.values[8][1] = 35;

	data.values[9][0] = 845;
	data.values[9][1] = 90;

	data.values[10][0] = 1200;
	data.values[10][1] = 120;
}

void file_test(unsigned int* p_row_test_ssp, unsigned int* p_row_test_tsr)
{
	unsigned int distance, length, speed, i = 0, row_test_file, * p_row_test_file = &row_test_file;

	row_test_file = *p_row_test_ssp + *p_row_test_tsr;
	data.values = create_field_p_p(p_row_test_file, column - 3);

	std::ifstream infile_tsr("tsr.txt");
	while (infile_tsr >> distance >> length >> speed)
	{
		data.values[i][0] = distance;
		data.values[i][1] = length;
		data.values[i][2] = speed;
		i++;
	}
	infile_tsr.close();
	std::ifstream infile_ssp("ssp.txt");
	while (infile_ssp >> distance >> length)
	{ 
		data.values[i][0] = distance;
		data.values[i][1] = length;
		i++;
	}
	infile_ssp.close();
}

void default_test(unsigned int* p_row_test_ssp, unsigned int* p_row_test_tsr)
{
	unsigned int row_test_default, * p_row_test_default = &row_test_default;
	row_test_default = *p_row_test_ssp + *p_row_test_tsr;

	data.values = create_field_p_p(p_row_test_default, column - 3);

	data.values[0][0] = 50;
	data.values[0][1] = 100;
	data.values[0][2] = 5;

	data.values[1][0] = 500;
	data.values[1][1] = 200;
	data.values[1][2] = 10;

	data.values[2][0] = 0;
	data.values[2][1] = 40;

	data.values[3][0] = 100;
	data.values[3][1] = 30;

	data.values[4][0] = 450;
	data.values[4][1] = 130;

	data.values[5][0] = 70;
	data.values[5][1] = 50;

	data.values[6][0] = 30;
	data.values[6][1] = 130;
}

