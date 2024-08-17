#include <bits/stdc++.h>

using namespace std;

#define mp make_pair
#define fr first
#define sc second

void ac()
{
	printf("AC\n");
	exit(0);
}

void wa()
{
	printf("WA\n");
	exit(0);
}

void ok(int pts)
{
	printf("OK\n");
	printf("%d\n", pts);
	exit(0);
}

const int PTS[4] = {12, 24, 40, 24};

int main(int argc, char* argv[])
{
	ifstream inp(argv[1]);
	ifstream out(argv[2]);
	ifstream con(argv[3]);
	
	int subtask;
	inp >> subtask;
	
	long long outAns;
	int outAns2;
	out >> outAns >> outAns2;
	
	long long conAns;
	int conAns2;
	if(!(con >> conAns >> conAns2))
	{
		wa();
	}
	
	string s;
	if(con >> s)
	{
		wa();
	}
	
	if(conAns != outAns)
	{
		wa();
	}
	if(conAns2 != outAns2)
	{
		ok(PTS[subtask - 1] / 4);
	}
	else
	{
		ac();
	}
}
