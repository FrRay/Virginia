#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define MAX 100

typedef struct point
{
	int point_x;
	int point_y;
} Point;
typedef struct ecc
{
	struct point p[MAX];
	int len;
} ECCPoint;
typedef struct generator
{
	Point p;
	int p_class;
} GENE_SET;

void get_all_points();
int int_sqrt(int s);
Point timesPiont(int k, Point p);
Point add_two_points(Point p1, Point p2);
int inverse(int n, int b);
void get_generetor_class();
void encrypt_ecc();
void decrypt_ecc();
int mod_p(int s);
void print();
int isPrime(int n);

char alphabet[27] = "abcdefghijklmnopqrstuvwxyz";
int a = -1, b = 0, p = 89;
ECCPoint eccPoint;
GENE_SET geneSet[MAX];
int geneLen;
char plain[] = "yes";
int m[MAX];
int cipher[MAX];
int nB;
Point P1, P2, Pt, G, PB;
Point Pm;
int C[MAX];

int main()
{
	get_generetor_class();
	encrypt_ecc();
	decrypt_ecc();
	return 0;
}
void encrypt_ecc()
{
	int num, i, j;
	int gene_class;
	int num_t;
	int k;
	srand(time(NULL));
	for (i = 0; i < strlen(plain); i++)
	{
		for (j = 0; j < 26; j++)
		{
			if (plain[i] == alphabet[j])
			{
				m[i] = j;
			}
		}
	}
	num = rand() % geneLen;
	gene_class = geneSet[num].p_class;
	while (isPrime(gene_class) == -1)
	{
		num = rand() % (geneLen - 3) + 3;
		gene_class = geneSet[num].p_class;
	}
	G = geneSet[num].p;
	nB = rand() % (gene_class - 1) + 1;
	PB = timesPiont(nB, G);
	printf("\nPublic key: \n");
	printf("{y^2=x^3%d*x+%d,%d,(%d,%d),(%d,%d)}\n", a, b, gene_class, G.point_x, G.point_y, PB.point_x, PB.point_y);
	printf("Private key: \n");
	printf("nB=%d\n", nB);
	k = rand() % (gene_class - 2) + 1;
	P1 = timesPiont(k, G);
	num_t = rand() % eccPoint.len;
	Pt = eccPoint.p[num_t];

	P2 = timesPiont(k, PB);
	Pm = add_two_points(Pt, P2);
	printf("Encrypted data: \n");
	printf("kG=(%d,%d),Pt+kPB=(%d,%d),C={", P1.point_x, P1.point_y, Pm.point_x, Pm.point_y);
	for (i = 0; i < strlen(plain); i++)
	{

		C[i] = m[i] * Pt.point_x + Pt.point_y;
		printf("{%d}", C[i]);
	}
	printf("}\n");
}
void decrypt_ecc()
{
	Point temp, temp1;
	int m, i;
	temp = timesPiont(nB, P1);
	temp.point_y = 0 - temp.point_y;
	temp1 = add_two_points(Pm, temp);
	printf("\nDecryption result: \n");
	for (i = 0; i < strlen(plain); i++)
	{
		m = (C[i] - temp1.point_y) / temp1.point_x;
		printf("%c", alphabet[m]);
	}
	printf("\n");
}
int isPrime(int n)
{
	int i, k;
	k = sqrt(n);
	for (i = 2; i <= k; i++)
	{
		if (n % i == 0)
			break;
	}
	if (i <= k)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}
void get_generetor_class()
{
	int i, j = 0;
	int count = 1;
	Point p1, p2;
	get_all_points();

	printf("\n**********************************Output the generator and its order: *************************************\n");
	for (i = 0; i < eccPoint.len; i++)
	{
		count = 1;
		p1.point_x = p2.point_x = eccPoint.p[i].point_x;
		p1.point_y = p2.point_y = eccPoint.p[i].point_y;
		while (1)
		{
			p2 = add_two_points(p1, p2);
			if (p2.point_x == -1 && p2.point_y == -1)
			{
				break;
			}
			count++;
			if (p2.point_x == p1.point_x)
			{
				break;
			}
		}
		count++;
		if (count <= eccPoint.len + 1)
		{
			geneSet[j].p.point_x = p1.point_x;
			geneSet[j].p.point_y = p1.point_y;
			geneSet[j].p_class = count;
			printf("(%d,%d)--->>%d\t", geneSet[j].p.point_x, geneSet[j].p.point_y, geneSet[j].p_class);
			j++;
			if (j % 6 == 0)
			{
				printf("\n");
			}
		}
		geneLen = j;
	}
}

Point timesPiont(int k, Point p0)
{
	if (k == 1)
	{
		return p0;
	}
	else if (k == 2)
	{
		return add_two_points(p0, p0);
	}
	else
	{
		return add_two_points(p0, timesPiont(k - 1, p0));
	}
}

Point add_two_points(Point p1, Point p2)
{
	long t;
	int x1 = p1.point_x;
	int y1 = p1.point_y;
	int x2 = p2.point_x;
	int y2 = p2.point_y;
	int tx, ty;
	int x3, y3;
	int flag = 0;
	if ((x2 == x1) && (y2 == y1))
	{
		if (y1 == 0)
		{
			flag = 1;
		}
		else
		{
			t = (3 * x1 * x1 + a) * inverse(p, 2 * y1) % p;
		}
	}
	else
	{
		ty = y2 - y1;
		tx = x2 - x1;
		while (ty < 0)
		{
			ty += p;
		}
		while (tx < 0)
		{
			tx += p;
		}
		if (tx == 0 && ty != 0)
		{
			flag = 1;
		}
		else
		{
			t = ty * inverse(p, tx) % p;
		}
	}
	if (flag == 1)
	{
		p2.point_x = -1;
		p2.point_y = -1;
	}
	else
	{
		x3 = (t * t - x1 - x2) % p;
		y3 = (t * (x1 - x3) - y1) % p;
		while (x3 < 0)
		{
			x3 += p;
		}
		while (y3 < 0)
		{
			y3 += p;
		}
		p2.point_x = x3;
		p2.point_y = y3;
	}
	return p2;
}
int inverse(int n, int b)
{
	int q, r, r1 = n, r2 = b, t, t1 = 0, t2 = 1, i = 1;
	while (r2 > 0)
	{
		q = r1 / r2;
		r = r1 % r2;
		r1 = r2;
		r2 = r;
		t = t1 - q * t2;
		t1 = t2;
		t2 = t;
	}
	if (t1 >= 0)
		return t1 % n;
	else
	{
		while ((t1 + i * n) < 0)
			i++;
		return t1 + i * n;
	}
}
void get_all_points()
{
	int i = 0;
	int j = 0;
	int s, y = 0;
	int n = 0, q = 0;
	int modsqrt = 0;
	int flag = 0;
	if (4 * a * a * a + 27 * b * b != 0)
	{
		for (i = 0; i <= p - 1; i++)
		{
			flag = 0;
			n = 1;
			y = 0;
			s = i * i * i + a * i + b;
			while (s < 0)
			{
				s += p;
			}
			s = mod_p(s);
			modsqrt = int_sqrt(s);
			if (modsqrt != -1)
			{
				flag = 1;
				y = modsqrt;
			}
			else
			{
				while (n <= p - 1)
				{
					q = s + n * p;
					modsqrt = int_sqrt(q);
					if (modsqrt != -1)
					{
						y = modsqrt;
						flag = 1;
						break;
					}
					flag = 0;
					n++;
				}
			}
			if (flag == 1)
			{
				eccPoint.p[j].point_x = i;
				eccPoint.p[j].point_y = y;
				j++;
				if (y != 0)
				{
					eccPoint.p[j].point_x = i;
					eccPoint.p[j].point_y = (p - y) % p;
					j++;
				}
			}
		}
		eccPoint.len = j;
		print();
	}
}

int mod_p(int s)
{
	int i;
	int result;
	i = s / p;
	result = s - i * p;
	if (result >= 0)
	{
		return result;
	}
	else
	{
		return result + p;
	}
}

int int_sqrt(int s)
{
	int temp;
	temp = (int)sqrt(s);
	if (temp * temp == s)
	{
		return temp;
	}
	else
	{
		return -1;
	}
}
void print()
{
	int i;
	int len = eccPoint.len;
	printf("\nThe elliptic curve has a total of **%d** points (including the point at infinity).\n", len + 1);
	for (i = 0; i < len; i++)
	{
		if (i % 8 == 0)
		{
			printf("\n");
		}
		printf("(%2d,%2d)\t", eccPoint.p[i].point_x, eccPoint.p[i].point_y);
	}
	printf("\n");
}