#include <cstdlib>
#include "functions.h"

bool CoinFlip()
{
    int t = rand() % 2 + 1;
    return t==1;
}

// comparison functions for sorting ScoreId array

bool cmpAsc(ScoreId x,ScoreId y)
{
    return x.GetScore()<=y.GetScore();
}

bool cmpDesc(ScoreId x,ScoreId y)
{
    return x.GetScore()>=y.GetScore();
}

// for sorting by times and last ranks
bool cmpWithLastRank(ScoreId x,ScoreId y)
{
    if(x.GetScore()==y.GetScore()) return x.GetLastRank()<y.GetLastRank();
    return x.GetScore()<y.GetScore();
}

// for sorting by total scores and IDs
bool cmpWithId(ScoreId x,ScoreId y)
{
    if(x.GetScore()==y.GetScore()) return x.GetId()<y.GetId();
    return x.GetScore()>y.GetScore();
}
