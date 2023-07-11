#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define F_2_PI			(float)(2.*M_PI)

#define BOSS 0

#define BIGSIGNALFILEBIN	(char*)"bigsignal.bin"
#define BIGSIGNALFILEASCII	(char*)"bigsignal.txt"
#define CSVPLOTFILE		(char*)"plot.csv"

#define TAG_SCATTER		1
#define TAG_GATHER		2

#define NUMELEMENTS	(1*1024*1024)

#define MAXPERIODS	100

#define BINARY

#define DEBUG		true

float* BigSums;
float* BigSignal;
int	NumCpus;
float* PPSums;
float* PPSignal;
int	PPSize;

void	DoOneLocalFourier(int);

int
main(int argc, char* argv[])
{
	MPI_Status status;

	MPI_Init(&argc, &argv);

	int  me;

	MPI_Comm_size(MPI_COMM_WORLD, &NumCpus);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);

	PPSize = NUMELEMENTS / NumCpus;

	PPSignal = new float[PPSize];
	PPSums = new float[MAXPERIODS];

	if (me == BOSS)
	{
		BigSignal = new float[NUMELEMENTS];

#ifdef BINARY
		FILE* fp = fopen(BIGSIGNALFILEBIN, "rb");
		if (fp == NULL)
		{
			fprintf(stderr, "Cannot open data file '%s'\n", BIGSIGNALFILEBIN);
			return -1;
		}

		fread(BigSignal, sizeof(float), NUMELEMENTS, fp);
#endif
	}

	if (me == BOSS)
	{
		BigSums = new float[MAXPERIODS];
	}

	double time0 = MPI_Wtime();

	if (me == BOSS)
	{
		for (int i = 0; i < PPSize; i++)
		{
			PPSignal[i] = BigSignal[BOSS * PPSize + i];
		}
	}

	if (me == BOSS)
	{
		for (int dst = 1; dst < NumCpus; dst++)
		{
			MPI_Send(&BigSignal[dst * PPSize], PPSize, MPI_FLOAT, dst, TAG_SCATTER, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Recv(PPSignal, PPSize, MPI_FLOAT, BOSS, TAG_SCATTER, MPI_COMM_WORLD, &status);
	}

	DoOneLocalFourier(me);

	if (me == BOSS)
	{
		for (int s = 0; s < MAXPERIODS; s++)
		{
			BigSums[s] = PPSums[s];
		}
	}
	else
	{
		MPI_Send(PPSums, MAXPERIODS, MPI_FLOAT, BOSS, TAG_GATHER, MPI_COMM_WORLD);
	}

	if (me == BOSS)
	{
		float tmpSums[MAXPERIODS];
		for (int src = 1; src < NumCpus; src++)
		{
			MPI_Recv(tmpSums, MAXPERIODS, MPI_FLOAT, src, TAG_GATHER, MPI_COMM_WORLD, &status);
			for (int s = 0; s < MAXPERIODS; s++)
				BigSums[s] += tmpSums[s];
		}
	}

	double time1 = MPI_Wtime();

	if (me == BOSS)
	{
		double seconds = time1 - time0;
		double performance = (double)NumCpus * (double)MAXPERIODS * (double)PPSize / seconds / 1000000.;
		fprintf(stderr, "%3d processors, %10d elements, %9.2lf mega-multiplies computed per second\n",
			NumCpus, NUMELEMENTS, performance);
	}

	if (me == BOSS)
	{
		FILE* fp = fopen(CSVPLOTFILE, "w");
		if (fp == NULL)
		{
			fprintf(stderr, "Cannot write to plot file '%s'\n", CSVPLOTFILE);
		}
		else
		{
			for (int s = 0; s < MAXPERIODS; s++)
			{
				fprintf(fp, "%6d , %10.2f\n", s, BigSums[s]);
			}
			fclose(fp);
		}
	}

	MPI_Finalize();
	return 0;
}


void
DoOneLocalFourier(int me)
{
	MPI_Status status;

	if (DEBUG)	fprintf(stderr, "Node %3d entered DoOneLocalFourier( )\n", me);

	for (int p = 1; p < MAXPERIODS; p++)
	{
		PPSums[p] = 0.;
	}

	for (int p = 1; p < MAXPERIODS; p++)
	{
		float omega = F_2_PI / (float)p;;
		for (int t = 0; t < PPSize; t++)
		{
			float time = (float)(t + me * PPSize);
			PPSums[p] += PPSignal[t] * sinf(omega * time);
		}
	}

}