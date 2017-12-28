#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#define INF 100000
using namespace std;

ifstream fin("DSJC500.5.txt");
ofstream fout("output.txt");

int Final_Colornum;//������ɫ��
int Colornum; //��ɫ����
int Iter1, Iter2, Iter3;
int Conflict, Conflict_best;//��ѳ�ͻ����
int N, E; // �ڵ����ͱ���
int **Graph; //��ά����洢ͼ
int **Adjac; //�ڽӱ�
int *Adjaclen; //�ڽӱ���
int **Opt; //�ڵ���ɫ�仯���Ż���
int **Tabu; //���ɱ�
int *NewColor;
int *BestColor;

//��ʼ�� ����ͼ����ά����洢 �����������ж�󣬱߼�Ϊ1��
//��ʼ�� �ڽӱ���ά����洢���ڶ���� һά����洢���ڽڵ���� ͨ������Ϣ������
void initialization()
{
	char ch;
	int i, j;
	fin >> N >> E;
	Graph = new int *[N + 2];
	Adjac = new int *[N + 2];
	Adjaclen = new int[N + 2];
	Opt = new int *[N + 2];
	Tabu = new int *[N + 2];
	NewColor = new int[N + 2];
	BestColor = new int[N + 2];

	for (i = 0; i < N; i++)
	{
		Graph[i] = new int[N + 2];
		Adjac[i] = new int[N + 2];
		Opt[i] = new int[Colornum + 2];
		Tabu[i] = new int[Colornum + 2];
		Adjaclen[i] = 0;
	}

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

void Prepare()
{
	//�ڵ������ɫ
	for (int j = 0; j < N; ++j)
		NewColor[j] = rand() % Colornum;
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
		// find one move
		for (int i=0;i<N;++i)
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
		//���
		if (SP_len>0 && SP_delta<delta && SP_delta+Conflict<Conflict_best)
		{
			k=rand()%SP_len;
			p=SP_Vertex[k];q=SP_Col[k];
			Conflict=SP_delta+Conflict;
		}
		else if (len>0)
		{
			k=rand()%len;
			p=Vertex[k];q=Col[k]; 
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
		if (Conflict_best==0)  //�ҵ�һ�����Ž�
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

int main()
{
	int i;
	time_t start, stop;
	srand((unsigned)time(NULL));
	Final_Colornum = Colornum = 50;
	initialization();
	Iter1 = 0;
	while (Iter1 < 100000)
	{
		start = time(NULL);
		Iter1++;
		Prepare(); 
		Iter2 = 0;
		//one-move
		while (Iter2 < 1000000)
		{
			Iter2++;
			Clear_And_Prepare();
			Tabu_Search();
			if (Colornum < Final_Colornum)
			{
				Final_Colornum = Colornum;
				break;
			}
			cout << "��" << Iter2 << "�γ�����ɫ��Ϊ��" << Colornum << endl;
			fout << "��" << Iter2 << "�γ�����ɫ��Ϊ��" << Colornum << endl;
			cout << Conflict_best << " ";
			cout << endl;
			fout << Conflict_best << " ";
			fout << endl;
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