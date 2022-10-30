#include "driver.h"

#ifndef __FUNCTIONS__H__
#define __FUNCTIONS__H__


bool CoinFlip();

// 4 comparison functions for sorting ScoreId array

bool cmpAsc(ScoreId x,ScoreId y);

bool cmpDesc(ScoreId x,ScoreId y);

// for sorting by times and last ranks
bool cmpWithLastRank(ScoreId x,ScoreId y);

// for sorting by total scores and IDs
bool cmpWithId(ScoreId x,ScoreId y);


#endif // __FUNCTIONS__H__
