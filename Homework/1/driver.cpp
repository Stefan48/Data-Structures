#include <iostream>
#include "driver.h"

// Driver class constructors
Driver::Driver()
{
    id = rank = points = rank_at_last_print = 0;
}

Driver::Driver(int id)
{
    this->id = id;
    rank = points = rank_at_last_print = 0;
}

Driver::Driver(int id, int rank, int points)
{
    this->id = id;
    this->rank = rank;
    this->points = points;
    rank_at_last_print = 0;
}

// Driver class operators overloading
bool Driver::operator==(const Driver& other)
{
    return id == other.id;
}
bool Driver::operator!=(const Driver& other)
{
    return id != other.id;
}
bool Driver::operator<(const Driver& other)
{
    return id < other.id;
}
bool Driver::operator>(const Driver& other)
{
    return id > other.id;
}
bool Driver::operator<=(const Driver& other)
{
    return id <= other.id;
}
bool Driver::operator>=(const Driver& other)
{
    return id >= other.id;
}
std::ostream& operator<< (std::ostream &out, const Driver &driver)
{
    std::cout<<driver.id<<" "<<driver.rank<<" "<<driver.points<<" ";
    return out;
}

// Driver class getters
int Driver::GetId()
{
    return id;
}

int& Driver::GetRank()
{
    return rank;
}

int& Driver::GetPoints()
{
    return points;
}

int& Driver::GetRankAtLastPrint()
{
    return rank_at_last_print;
}

// ScoreId class constructor
ScoreId::ScoreId()
{
    score = id = last_rank = 0;
}

// ScoreId class operators overloading
bool ScoreId::operator==(const ScoreId& other)
{
    return score == other.score;
}

bool ScoreId::operator!=(const ScoreId& other)
{
    return score != other.score;
}

bool ScoreId::operator<(const ScoreId& other)
{
    return score < other.score;
}

bool ScoreId::operator>(const ScoreId& other)
{
    return score > other.score;
}

bool ScoreId::operator<=(const ScoreId& other)
{
    return score <= other.score;
}

bool ScoreId::operator>=(const ScoreId& other)
{
    return score >= other.score;
}

std::ostream& operator<< (std::ostream &out, const ScoreId &score_id)
{
    std::cout<<score_id.score<<" "<<score_id.id<<" "<<score_id.last_rank;
    return out;
}

// ScoreId class getters
int& ScoreId::GetScore()
{
    return score;
}

int& ScoreId::GetId()
{
    return id;
}

int& ScoreId::GetLastRank()
{
    return last_rank;
}

