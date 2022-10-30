#include <iostream>

#ifndef __DRIVER__H__
#define __DRIVER__H__

class Driver
{
public:
    Driver();
    Driver(int id);
    Driver(int id, int rank, int points);

    bool operator==(const Driver& other);
    bool operator!=(const Driver& other);
    bool operator<(const Driver& other);
    bool operator>(const Driver& other);
    bool operator<=(const Driver& other);
    bool operator>=(const Driver& other);
    friend std::ostream& operator<< (std::ostream &out, const Driver &driver);
    
    int GetId();
    int& GetRank();
    int& GetPoints();
    int& GetRankAtLastPrint();

private:
    int id;
    int rank;
    int points;
    int rank_at_last_print;
};

class ScoreId
{
public:
    ScoreId();
    ScoreId(int score,int id);

    bool operator==(const ScoreId& other);
    bool operator!=(const ScoreId& other);
    bool operator<(const ScoreId& other);
    bool operator>(const ScoreId& other);
    bool operator<=(const ScoreId& other);
    bool operator>=(const ScoreId& other);
    friend std::ostream& operator<< (std::ostream &out, const ScoreId &score_id);

    int& GetScore();
    int& GetId();
    int& GetLastRank();

private:
    int score;
    int id;
    int last_rank;
};

#endif // __DRIVER__H__

