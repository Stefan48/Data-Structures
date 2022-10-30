#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include "functions.h"
#include "driver.h"
#include "skip_list.h"

#ifndef MAX_DRIVERS
#define MAX_DRIVERS 1005
#endif // MAX_DRIVERS

int main()
{
    srand(time(NULL));
    std::ifstream fin("races.in");
    std::ofstream fout("races.out");

    int no_drivers;
    int no_races;
    int no_prints;
    fin>>no_drivers>>no_races>>no_prints;

    Skiplist<Driver> drivers(Driver(0,0,0),Driver(MAX_DRIVERS,0,0));
    for(int i=0;i<no_drivers;++i)
        drivers.Insert(Driver(i+1,i+1,0));
    ScoreId *id_for_current_placement = new ScoreId[no_drivers];
    for(int i=0;i<no_drivers;++i)
        id_for_current_placement[i].GetLastRank() = i + 1;

    int no_operations = no_races + no_prints;
    char *s = new char[10];
    Node<Driver> *this_driver;
    int points_given;
    for(int i=0;i<no_operations;++i)
    {
        fin>>s;
        if(strcmp(s,"print")==0)
        {
            // print
            int j;
            for(j=0;j<no_drivers;j++)
            {
				// search driver in skiplist
                this_driver = drivers.Search(id_for_current_placement[j].GetId());
                fout<<this_driver->info.GetId()<<" "<<this_driver->info.GetPoints()<<" ";
                if(this_driver->info.GetRankAtLastPrint())
                    fout<<this_driver->info.GetRankAtLastPrint() - this_driver->info.GetRank();
                else fout<<0;
                this_driver->info.GetRankAtLastPrint() = this_driver->info.GetRank();
                fout<<"\n";
            }
            fout<<"\n";
        }
        else
        {
            // race
            int j;
            int no_participants = 0; // how many drivers took part in race
            id_for_current_placement[0].GetScore() = atoi(s);
            if(id_for_current_placement[0].GetScore())
                no_participants++;
            id_for_current_placement[0].GetId() = 1;
            // last rank of each driver = (current) rank
            // going through the bottom level of the skiplist
            this_driver = drivers.GetHead();
            while(this_driver->level>1)
                this_driver = this_driver->below;
            this_driver = this_driver->next;
            id_for_current_placement[0].GetLastRank() = this_driver->info.GetRank();
            this_driver = this_driver->next;
            for(j=1;j<no_drivers;++j)
            {
                fin>>id_for_current_placement[j].GetScore();
                if(id_for_current_placement[j].GetScore())
                    no_participants++;
                id_for_current_placement[j].GetId() = j+1;
                id_for_current_placement[j].GetLastRank() = this_driver->info.GetRank();
                this_driver = this_driver->next;
            }
            // sort by times and last ranks
            std::sort(id_for_current_placement,id_for_current_placement + no_drivers,cmpWithLastRank);
            // update points for drivers who took part in the race
            points_given = no_participants/2;
            for(j=0;j<no_drivers;++j)
            {
                // search driver in skiplist
                this_driver = drivers.Search(id_for_current_placement[j].GetId());
                if(id_for_current_placement[j].GetScore()) // if driver took part in race
                {
                    this_driver->info.GetPoints() += points_given--;
                    if(points_given==0 && no_participants%2==0) points_given--;
                }
                // reuse id_for_current_placement array when sorting by new total points of each driver
                id_for_current_placement[j].GetScore() = this_driver->info.GetPoints();
            }
            // update ranks based on points
            // sort by total scores and IDs
            std::sort(id_for_current_placement,id_for_current_placement + no_drivers,cmpWithId);
            for(j=0;j<no_drivers;++j)
            {
                // search driver in skiplist
                this_driver = drivers.Search(id_for_current_placement[j].GetId());
                this_driver->info.GetRank() = j + 1;
            }
        }
    }

    delete[] id_for_current_placement;
    delete[] s;
    fin.close();
    fout.close();
    return 0;
}
