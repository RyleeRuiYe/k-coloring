#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

#define POP 10
#define INF 100000

using namespace std;
ifstream fin("DSJC500.5.txt");
ofstream fout("output.txt");

int Final_Colornum;//������ɫ��
int Colornum; //��ɫ����
int Iter1, Iter2, Iter3;
int *Conflictnum;
int Unsuccess1, Unsuccess2;
int Conflict, Conflict_best;//��ѳ�ͻ����
int N, E; // �ڵ����ͱ���
int **Graph; //��ά����洢ͼ
int **Adjac; //�ڽӱ�
int *Adjaclen; //�ڽӱ���
int **Color; //�ڵ�ĵ�ǰ��ɫ
int **Opt; //�ڵ���ɫ�仯���Ż���
int **Tabu; //���ɱ�
int *NewColor;//�ӽ����������Ӵ�
int *BestColor;

//��ʼ�� ����ͼ����ά����洢 �����������ж�󣬱߼�Ϊ1��
//��ʼ�� �ڽӱ���ά����洢���ڶ���� һά����洢���ڽڵ���� ͨ������Ϣ������
void initialization()
{
	char ch;
	int i, j;
//	cout << "�����붥���� ������" << endl;
	//cin >> N >> E;
	fin >> N >> E;
	Graph = new int *[N + 2];
	Adjac = new int *[N + 2];
	Adjaclen = new int[N + 2];
	Color = new int *[POP + 2];
	Conflictnum = new int[POP + 2];//����
	Opt = new int *[N + 2];
	Tabu = new int *[N + 2];
	NewColor = new int[N + 2];
	BestColor = new int[N + 2];
	for (i = 0; i < POP; ++i)
		Color[i] = new int[N + 2];
	for (i = 0; i < N; i++)
	{
		Graph[i] = new int[N + 2];
		Adjac[i] = new int[N + 2];
		Opt[i] = new int[Colornum + 2];
		Tabu[i] = new int[Colornum + 2];
		Adjaclen[i] = 0;
	}

	//��ά����洢 �ڽӱ� ��ʼ��Ϊ0
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
		{
			Graph[i][j] = 0;
			Adjac[i][j] = 0;
		}
	//�����ڽӱ� �� ͼ�α�
	for (i = 1; i <= E; ++i)
	{
		int a, b; //�й�ϵ��������
//		cout << "�����룺�ַ� �г�ͻ����Ķ����" << endl;
		//cin >> ch >> a >> b;
		fin >> ch >> a >> b;
		a--;
		b--;
		Graph[a][b] = Graph[b][a] = 1;
		Adjac[a][Adjaclen[a]] = b;
		Adjaclen[a]++;
		Adjac[b][Adjaclen[b]] = a;
		Adjaclen[b]++;
	}
			
}

//��ʼ����ɫ���� ���ż����ʼ�����ĳ�ͻ���
void Prepare()
{
	//�ڵ������ɫ
	//POP�Ƕ��ַ���ͬʱ����
	for (int i = 0; i < POP; ++i)
		for (int j = 0; j < N; ++j)
			Color[i][j] = rand() % Colornum;
	//����Ŀ�꺯������ʼ��ɫ�����ĳ�ͻ�� ��������ͬɫ���1��
	for (int l = 0; l < POP; ++l)
	{
		Conflictnum[l] = 0;
		for (int i = 0; i < N - 1; ++i)
			for (int j = i + 1; j < N; ++j)
				if (Graph[i][j] == 1 &&Color[l][i] == Color[l][j])
					Conflictnum[l]++;
	}
}

//����10�����������·���
void Crossover()
{
	int ColorsumA[200], ColorsumB[200];
	int Sign, SN = 0;
	int Coloring = 0;//��ǰ��ɫ
	int A, B;
	int MaxA, MaxB;
	int SignA[60], SignB[60];
	int SignAlen, SignBlen;
	int i;

	for (i = 0; i < Colornum; ++i)
	{
		ColorsumA[i] = 0;
		ColorsumB[i] = 0;
	}
	for (i = 0; i < N; ++i)
		NewColor[i] = -1;
	//���������ѡ������һ����
	while (1)
	{
		A = rand() % POP;
		B = rand() % POP;
		if (A != B)
			break;
	}
	for (i = 0; i < N; ++i)
	{
		ColorsumA[Color[A][i]]++;
		ColorsumB[Color[B][i]]++;
	}
	while (Coloring < Colornum && SN < N)
	{
		MaxA = MaxB = -INF;
		for (int i = 0; i<Colornum; ++i)
		{
			if (ColorsumA[i]>MaxA)
			{
				MaxA = ColorsumA[i];
				SignAlen = 0;
				SignA[0] = i;
				SignAlen++;
			}
			else if (ColorsumA[i] == MaxA && SignAlen <= 50)
			{
				SignA[SignAlen] = i;
				SignAlen++;
			}
			if (ColorsumB[i]>MaxB)
			{
				MaxB = ColorsumB[i];
				SignBlen = 0;
				SignB[0] = i;
				SignBlen++;
			}
			else if (ColorsumB[i] == MaxB && SignBlen <= 50)
			{
				SignB[SignBlen] = i;
				SignBlen++;
			}
		}
		if (Coloring % 2 == 0)
		{
			SN += MaxA;
			int k = rand() % SignAlen;
			Sign = SignA[k];
			ColorsumA[Sign] = -1;
			for (int i = 0; i<N; ++i)
				if (NewColor[i] == -1 && Color[A][i] == Sign)
				{
					NewColor[i] = Coloring;
					ColorsumB[Color[B][i]]--;
				}
		}
		else
		{
			SN += MaxB;
			int k = rand() % SignBlen;
			Sign = SignB[k];
			ColorsumB[Sign] = -1;
			for (int i = 0; i<N; ++i)
				if (NewColor[i] == -1 && Color[B][i] == Sign)
				{
					NewColor[i] = Coloring;
					ColorsumA[Color[A][i]]--;
				}
		}
		Coloring++;

	}
	Unsuccess1 = N - SN;
	Unsuccess2 = 0;
	for (i = 0; i<N; ++i)
		if (NewColor[i] == -1)
		{
			NewColor[i] = Color[A][i];
			Unsuccess2++;
		}
}

//��ʼ��OPT�� �� Tabu��
void Clear_And_Prepare()
{
	Conflict = 0;
	int i, j;
	for (i = 0; i<N; ++i)
		for (j = 0; j<Colornum; ++j)
			Opt[i][j] = 0, Tabu[i][j] = 0;
	for (i = 0; i<N - 1; ++i)
		for (j = i + 1; j<N; ++j)
			if (Graph[i][j] == 1)
			{
				Opt[i][NewColor[j]]++;
				Opt[j][NewColor[i]]++;
				if (NewColor[i] == NewColor[j]) Conflict++;
			}
	for (i = 0; i<N; ++i)
		BestColor[i] = NewColor[i];
	Conflict_best = Conflict;
}

/*local search*/
void Tabu_Search()
{
	int temp,k;
	int p,q;
	int delta;
	int SP_delta;
	int Vertex[60],Col[60],len;
	int SP_Vertex[60],SP_Col[60],SP_len;
	int Iter;
	int i;
	len=0;
	SP_len=0;
	Iter3=0;
	Iter=0;
	while (Iter<500000)
	{
		Iter3++;
		delta=SP_delta=INF;
		for (int i=0;i<N;++i)
			// find one move
			if (Opt[i][NewColor[i]]!=0) //�ҵ����г�ͻ�Ľڵ� ��ɫ
			{   
				//��ͬһ���ڵ� �ҵ�������С�ĵ�
				for (int j=0;j<Colornum;++j)
					if (NewColor[i]!=j) //������һ����ɫ
					{
						temp=Opt[i][j]-Opt[i][NewColor[i]];  //����
						if (Tabu[i][j]<=Iter3) //������
						{
							if (temp<delta)      
							{
								delta=temp;len=0;
								Vertex[len]=i; //��¼�Ķ��ĵ�
								Col[len]=j;    //��¼�ĺ����ɫ
								len++;
							}
							else if (temp==delta && len<=50) //�����ͬ��ֵ ֻ��¼50��
							{
								Vertex[len]=i;
								Col[len]=j;
								len++;
							}
						}
						else              //����
						{
							if (temp<SP_delta)
							{
								SP_delta=temp;SP_len=0;
								SP_Vertex[SP_len]=i;
								SP_Col[SP_len]=j;
								SP_len++;
							}
							else if (temp==SP_delta && SP_len<=50)
							{
								SP_Vertex[SP_len]=i;
								SP_Col[SP_len]=j;
								SP_len++;
							}
						}
					}
			}
		//���ܲ����Ͻ��ɱ�����
		if (SP_len>0 && SP_delta<delta && SP_delta+Conflict<Conflict_best)
		{
			k=rand()%SP_len;
			p=SP_Vertex[k];q=SP_Col[k];
			Conflict=SP_delta+Conflict;
		}
		else if (len>0)
		{
			k=rand()%len;
			p=Vertex[k];q=Col[k]; //one move
			Conflict=delta+Conflict;
		}
		else return;
		//���¸Ľ���Ľ��
		if (Conflict<Conflict_best) 
		{
			Conflict_best=Conflict;
			Iter=0;
			for (i=0;i<N;++i) 
				BestColor[i]=NewColor[i];
		}
		else Iter++;
		if (Conflict_best==0)  //����Ҫ���� �Ѿ��ҵ�һ�����Ž�
		{
			Colornum--;
			return;
		}
		//�޸�opt����һ�ֳ���
		for (i=0;i<Adjaclen[p];i++)
		{
			int t=Adjac[p][i];
			Opt[t][NewColor[p]]--;//�޸��Ż��� ��ǰͬɫ��ͻ ����1
			Opt[t][q]++; //���ͬɫ��ͻ����1
		}
		Tabu[p][NewColor[p]]=Iter3+rand()%10+1+Conflict; //�޸Ľ��ɱ�
		NewColor[p]=q;//�޸���ɫ
	}
}

void Updating_POP()
{
	int Maxm = -INF;
	int *Pos, Poslen, P;
	Pos = new int[20];
	for (int i = 0; i<POP; ++i)
		if (Conflictnum[i]>Maxm)
		{
			Maxm = Conflictnum[i];
			Poslen = 0;
			Pos[0] = i;
			Poslen++;
		}
		else if (Conflictnum[i] == Maxm)
		{
			Pos[Poslen] = i;
			Poslen++;
		}
	int k = rand() % Poslen;
	P = Pos[k];
	//���ѡһ������ ʹ���·�������
	if (Conflict_best <= Maxm)
	{
		for (int i = 0; i<N; ++i)
			Color[P][i] = BestColor[i];
		Conflictnum[P] = Conflict_best;
	}
}

int main()
{
	int i;
	time_t start, stop;
	srand((unsigned)time(NULL));
	Final_Colornum = Colornum = 49;
	initialization();
	Iter1 = 0;
	while (Iter1 < 100000)
	{
		start = time(NULL);
		Iter1++;
		Prepare(); //һ�γ�ʼ����
		Iter2 = 0;
		while (Iter2 < 1000000)
		{
			Crossover();
			Iter2++;
			Clear_And_Prepare();
			Tabu_Search();
			Updating_POP();
			if (Colornum < Final_Colornum)
			{
				Final_Colornum = Colornum;
				break;
			}
			cout << "��" << Iter2 << "�γ�����ɫ��Ϊ��" << Colornum << endl;
			fout << "��" << Iter2 << "�γ�����ɫ��Ϊ��" << Colornum << endl;
			for (int i = 0; i < POP; ++i)
				cout << Conflictnum[i] << " ";
			cout << endl;
			for (i = 0; i < POP; ++i)
				fout << Conflictnum[i] << " ";
			fout << endl;
		//	cout << "���า��ʧ��" << Unsuccess1 << " " << Unsuccess2 << endl;
		//	fout << "���า��ʧ��" << Unsuccess1 << " " << Unsuccess2 << endl;
		}
		stop = time(NULL);
		cout << "ʹ��ʱ��Ϊ" << (stop - start) << "s" << endl;
		cout << "��ǰ��ɫΪ" << Final_Colornum + 1 << endl;
		cout << "��ѳ�ͻ����Ϊ" << Conflict_best << endl;
		cout << "***********************************************" << endl;
		fout << "ʹ��ʱ��Ϊ" << (stop - start) << "s" << endl;
		fout << "��ǰ��ɫΪ" << Final_Colornum + 1 << endl;
		fout << "��ѳ�ͻ����Ϊ" << Conflict_best << endl;
		fout << "***********************************************" << endl;
		if (Final_Colornum == 1)
			break;
	}
	cout << "������ɫ��Ϊ" << Final_Colornum + 1 << endl;
	return 0;
}