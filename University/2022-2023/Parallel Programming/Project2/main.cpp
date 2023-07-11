#include <iostream>
#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Constants
const float RYEGRASS_GROWS_PER_MONTH = 20.0;
const float ONE_RABBITS_EATS_PER_MONTH = 1.0;
const int BEAR_EATS_RABBITS_PER_MONTH = 3;

const float AVG_PRECIP_PER_MONTH = 12.0;
const float AMP_PRECIP_PER_MONTH = 4.0;
const float RANDOM_PRECIP = 2.0;

const float AVG_TEMP = 60.0;
const float AMP_TEMP = 20.0;
const float RANDOM_TEMP = 10.0;

const float MIDTEMP = 60.0;
const float MIDPRECIP = 14.0;

// Global variables
int NowYear = 2023;
int NowMonth = 0;
int NowMonthCumulative = 0;

float NowPrecip = 0.0;
float NowTemp = 0.0;
float NowHeight = 5.0;
int NowNumRabbits = 5;
int NowNumBears = 5; // number of bears in the current population

unsigned int seed = 0;

// Function prototypes
float Ranf(unsigned int* seedp, float low, float high);
float Sqr(float x);
void InitBarrier(int);
void WaitBarrier();
void Rabbits();
void RyeGrass();
void Watcher();
void Bears();

int main()
{
    // Set number of threads and initialize barrier
    omp_set_num_threads(4);
    InitBarrier(4);

    // Parallel sections
#pragma omp parallel sections
    {
#pragma omp section
        {
            Rabbits();
        }

#pragma omp section
        {
            RyeGrass();
        }

#pragma omp section
        {
            Watcher();
        }

#pragma omp section
        {
            Bears();
        }
    }

    return 0;
}

// Functions

float Ranf(unsigned int* seedp, float low, float high)
{
    float r = (float)rand_r(seedp);
    return (low + r * (high - low) / (float)RAND_MAX);
}

float Sqr(float x)
{
    return x * x;
}

void Rabbits()
{
    while (NowYear < 2029)
    {
        int nextNumRabbits = NowNumRabbits;
        int carryingCapacity = (int)(NowHeight);
        if (nextNumRabbits < carryingCapacity)
            nextNumRabbits++;
        else if (nextNumRabbits > carryingCapacity)
            nextNumRabbits--;

        if (nextNumRabbits < 0)
            nextNumRabbits = 0;

        WaitBarrier();

        NowNumRabbits = nextNumRabbits;

        WaitBarrier();

        WaitBarrier();
    }
}

void RyeGrass()
{
    while (NowYear < 2029)
    {
        float tempFactor = exp(-Sqr((NowTemp - MIDTEMP) / 10.0));
        float precipFactor = exp(-Sqr((NowPrecip - MIDPRECIP) / 10.0));

        float nextHeight = NowHeight;
        nextHeight += tempFactor * precipFactor * RYEGRASS_GROWS_PER_MONTH;
        nextHeight -= (float)NowNumRabbits * ONE_RABBITS_EATS_PER_MONTH;
        if (nextHeight < 0.0)
            nextHeight = 0.0;

        WaitBarrier();

        NowHeight = nextHeight;

        WaitBarrier();

        WaitBarrier();
    }
}

void Watcher()
{
    while (NowYear < 2029)
    {
        WaitBarrier();

        WaitBarrier();
        // Print current state
        fprintf(stderr, "%02d, %6.1f, %6.1f, %6.1f, %3d, %3d\n",
            NowMonthCumulative, NowTemp, NowPrecip, NowHeight, NowNumRabbits, NowNumBears);

        // Update month and year
        NowMonth++;
        NowMonthCumulative++;
        if (NowMonth > 11)
        {
            NowMonth = 0;
            NowYear++;
        }

        // Update temperature and precipitation
        float ang = (30.0 * (float)NowMonth + 15.0) * (M_PI / 180.0);
        float temp = AVG_TEMP - AMP_TEMP * cos(ang);
        NowTemp = temp + Ranf(&seed, -RANDOM_TEMP, RANDOM_TEMP);
        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
        NowPrecip = precip + Ranf(&seed, -RANDOM_PRECIP, RANDOM_PRECIP);
        if (NowPrecip < 0.0)
            NowPrecip = 0.0;

        WaitBarrier();
    }
}

// Barrier variables
int NumInThreadTeam;
int NumAtBarrier;
omp_lock_t Lock;
omp_lock_t WaitLock;

// Barrier functions
void InitBarrier(int n)
{
    NumInThreadTeam = n;
    NumAtBarrier = 0;
    omp_init_lock(&Lock);
    omp_init_lock(&WaitLock);
    omp_set_lock(&WaitLock);
}

void WaitBarrier()
{
    omp_set_lock(&Lock);
    {
        NumAtBarrier++;
        if (NumAtBarrier == NumInThreadTeam)
        {
            NumAtBarrier = 0;
            omp_unset_lock(&WaitLock);
        }
    }
    omp_unset_lock(&Lock);

    omp_set_lock(&WaitLock);
    omp_unset_lock(&WaitLock);
}

// Your own agent function
void Bears()
{
    int nextNumBears;
    while (NowYear < 2029)
    {
        nextNumBears = NowNumBears;
        int eatenRabbits = BEAR_EATS_RABBITS_PER_MONTH * NowNumBears;
        if (eatenRabbits > NowNumRabbits)
        {
            nextNumBears--;
            if (nextNumBears < 0)
                nextNumBears = 0;
        }
        else
        {
            nextNumBears++;
        }

        WaitBarrier();

        NowNumBears = nextNumBears;

        WaitBarrier();

        WaitBarrier();
    }
}