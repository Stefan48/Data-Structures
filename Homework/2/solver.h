// Copyright 2019 SD_Homework_Team

#ifndef SOLVER_H_
#define SOLVER_H_
#include <fstream>
#include "directed_graph.h"
#include "hash_table.h"
#include "driver.h"
#define MAX_STATIONS 555

class solver
{
	private:
		int cnt_stations, cnt_roads;
		HashTable<int> stations_ht = HashTable<int>(MAX_STATIONS);
		DirectedGraph<std::string> stations_graph = DirectedGraph<std::string>(MAX_STATIONS);
		std::string station1, station2, station3;
		int i, j, k, n;
		int cnt_queries, dist, dist2;
		char q_type1;
		int q_type2;
		std::string q_type3;
		int node1, node2, node3;
		int distance[MAX_STATIONS][MAX_STATIONS];
		bool mark[MAX_STATIONS];
		std::queue<int> Q;
		int cnt_drivers = 0;
		HashTable<int> drivers_ht = HashTable<int>(MAX_DRIVERS);
		Driver drivers[MAX_DRIVERS];
		std::string driver;
		int index;
		int rating;
		int dist_min, driver_dist_min;
		bool found;
		int cnt_top;
		std::pair<std::string, double> top1[MAX_DRIVERS], aux1;
		std::pair<std::string, int> top2[MAX_DRIVERS], aux2;
		int fuel;
		std::pair<int, std::string> stations[MAX_STATIONS], aux3;
		
	public:
		void task1_solver(std::ifstream& fin, std::ofstream& fout)
		{
			fin >> cnt_stations >> cnt_roads;
			for(i = 0; i < cnt_stations; ++i)
			{
				fin >> station1;
				stations_ht.insert(std::pair<std::string, int>(station1, i));
				stations_graph[i].info = station1;
			}
			for(i = 0; i < cnt_roads; ++i)
			{
				fin >> station1 >> station2;
				stations_graph.addEdge(stations_ht.get(station1), stations_ht.get(station2));
			}
			fin >> cnt_queries;
			for(i = 0; i < cnt_queries; ++i)
			{
				fin >> station1 >> station2;
				dist = stations_graph.distance(stations_ht.get(station1), stations_ht.get(station2));
				if(dist != -1) fout << "y\n";
				else fout << "n\n";
			}
		}

		void task2_solver(std::ifstream& fin, std::ofstream& fout)
		{
			fin >> cnt_queries;
			for(i = 0; i < cnt_queries; ++i)
			{
				fin >> station1 >> station2;
				dist = stations_graph.distance(stations_ht.get(station1), stations_ht.get(station2));
				fout << dist << "\n";
			}
		}

		void task3_solver(std::ifstream& fin, std::ofstream& fout)
		{
			fin >> cnt_queries;
			for(i = 0; i < cnt_queries; ++i)
			{
				fin >> q_type1 >> station1 >> station2 >> q_type2;
				node1 = stations_ht.get(station1);
				node2 = stations_ht.get(station2);
				if(q_type1 == 'c')
				{
					switch(q_type2)
					{
						case 0:
							stations_graph.addEdge(node1, node2);
							break;
						case 1:
							stations_graph.removeEdge(node1, node2);
							stations_graph.removeEdge(node2, node1);
							break;
						case 2:
							stations_graph.addEdge(node1, node2);
							stations_graph.addEdge(node2, node1);
							break;
						case 3:
							if(stations_graph.hasEdge(node1, node2))
							{
								if(!stations_graph.hasEdge(node2, node1))
								{
									stations_graph.removeEdge(node1, node2);
									stations_graph.addEdge(node2, node1);
								}
							}
							else if(stations_graph.hasEdge(node2, node1))
							{
								stations_graph.addEdge(node1, node2);
								stations_graph.removeEdge(node2, node1);
							}
							break;
						default: break;
					}
				}
				else
				{
					switch(q_type2)
					{
						case 0:
							dist = stations_graph.distance(node1, node2);
							if(dist != -1) fout << "y\n";
							else fout << "n\n";
							break;
						case 1:
							dist = stations_graph.distance(node1, node2);
							fout << dist << "\n";
							break;
						case 2:
							fin >> station3;
							node3 = stations_ht.get(station3);
							dist = stations_graph.distance(node1, node3);
							if(dist == -1) fout << "-1\n";
							else
							{
								dist2 = stations_graph.distance(node3, node2);
								if(dist2 == -1) fout << "-1\n";
								else fout << dist + dist2 << "\n";
							}
							break;
						default: break;
					}
				}
			}
			// precompute all distances
			for(j = 0; j < cnt_stations; ++j)
			{
				for(i = 0; i < cnt_stations; ++i) mark[i] = false;
				for(i = 0; i < cnt_stations; ++i) distance[j][i] = -1;
				distance[j][j] = 0;
				mark[j] = true;
				Q.push(j);
				while(!Q.empty())
				{
					n = Q.front();
					Q.pop();
					for(auto it = stations_graph[n].neighbors.begin(); it != stations_graph[n].neighbors.end(); ++it)
						if(!mark[*it])
						{
							mark[*it] = true;
							distance[j][*it] = distance[j][n] + 1;
							Q.push(*it);
						}
				}
			}
		}

		void task4_solver(std::ifstream& fin, std::ofstream& fout)
		{
			fin >> cnt_queries;
			for(i = 0; i < cnt_queries; ++i)
			{
				fin >> q_type3;
				if(q_type3 == "d")
				{
					fin >> driver >> station1;
					if(drivers_ht.hasKey(driver))
					{
						index = drivers_ht.get(driver);
						drivers[index].online = true;
						drivers[index].station = stations_ht.get(station1);
					}
					else
					{
						drivers_ht.insert(std::pair<std::string, int>(driver, cnt_drivers));
						drivers[cnt_drivers].name = driver;
						drivers[cnt_drivers].station = stations_ht.get(station1);
						//std::cout<<drivers[cnt_drivers].name<<"\n";
						cnt_drivers++;
						//std::cout<<cnt_drivers<<"\n";
					}
				}
				else if(q_type3 == "b")
				{
					fin >> driver;
					drivers[drivers_ht.get(driver)].online = false;
				}
				else if(q_type3 == "r")
				{
					fin >> station1 >> station2 >> rating;
					node1 = stations_ht.get(station1);
					node2 = stations_ht.get(station2);

					dist_min = cnt_stations;
					found = false;
					for(j = 0; j < cnt_drivers; ++j) if(drivers[j].online)
					{
						dist = distance[drivers[j].station][node1];
						if(dist != -1)
						{
							found = true;
							if(dist < dist_min)
							{
								dist_min = dist;
								driver_dist_min = j;
							}
							else if(dist == dist_min)
							{
								if(drivers[j].medium_rating > drivers[driver_dist_min].medium_rating)
								{
									driver_dist_min = j;
								}
								else if(drivers[j].medium_rating == drivers[driver_dist_min].medium_rating)
								{
									if(drivers[j].name < drivers[driver_dist_min].name)
									{
										driver_dist_min = j;
									}
								}
							}
						}
					}
					if(!found)
							fout << "Soferi indisponibili\n";
					else
					{
						found = true;
						dist2 = distance[node1][node2];
						if(dist2 == -1)
						{
							found = false;
							for(auto it = stations_graph[node2].neighbors.begin(); it != stations_graph[node2].neighbors.end(); ++it)
							{
								dist2 = distance[node1][*it];
								if(dist2 != -1)
								{
									found = true;
									//station2 = stations_graph[*it].info;
									node2 = *it;
									break;
								}
							}
						}
						if(!found)
							fout << "Destinatie inaccesibila\n";
						else
						{
							drivers[driver_dist_min].station = node2;
							drivers[driver_dist_min].cnt_rides++;
							drivers[driver_dist_min].total_rating += rating;
							drivers[driver_dist_min].medium_rating = (double)drivers[driver_dist_min].total_rating / drivers[driver_dist_min].cnt_rides;
							drivers[driver_dist_min].total_distance += dist_min + dist2;
						}
					}
				}
				else if(q_type3 == "top_rating")
				{
					fin >> cnt_top;
					if(cnt_top > cnt_drivers) cnt_top = cnt_drivers;
					for(j = 0; j < cnt_drivers; ++j)
					{
						top1[j].first = drivers[j].name;
						top1[j].second = drivers[j].medium_rating;
					}
					for(j = 0; j < cnt_drivers; ++j)
						for(k = j + 1; k < cnt_drivers; ++k)
							if(top1[j].second < top1[k].second || (top1[j].second == top1[k].second && top1[j].first > top1[k].first))
							{
								aux1 = top1[j];
								top1[j] = top1[k];
								top1[k] = aux1;
							}
					for(j = 0; j < cnt_top; ++j)
						fout << top1[j].first << ":" << std::fixed << std::setprecision(3) << top1[j].second << " ";
					fout << "\n";
				}
				else if(q_type3 == "top_dist")
				{
					fin >> cnt_top;
					if(cnt_top > cnt_drivers) cnt_top = cnt_drivers;
					for(j = 0; j < cnt_drivers; ++j)
					{
						top2[j].first = drivers[j].name;
						top2[j].second = drivers[j].total_distance;
					}
					for(j = 0; j < cnt_drivers; ++j)
						for(k = j + 1; k < cnt_drivers; ++k)
							if(top2[j].second < top2[k].second || (top2[j].second == top2[k].second && top2[j].first > top2[k].first))
							{
								aux2 = top2[j];
								top2[j] = top2[k];
								top2[k] = aux2;
							}
					for(j = 0; j < cnt_top; ++j)
						fout << top2[j].first << ":" << top2[j].second << " ";
					fout << "\n";
				}
				else if(q_type3 == "top_rides")
				{
					fin >> cnt_top;
					if(cnt_top > cnt_drivers) cnt_top = cnt_drivers;
					for(j = 0; j < cnt_drivers; ++j)
					{
						top2[j].first = drivers[j].name;
						top2[j].second = drivers[j].cnt_rides;
					}
					for(j = 0; j < cnt_drivers; ++j)
						for(k = j + 1; k < cnt_drivers; ++k)
							if(top2[j].second < top2[k].second || (top2[j].second == top2[k].second && top2[j].first > top2[k].first))
							{
								aux2 = top2[j];
								top2[j] = top2[k];
								top2[k] = aux2;
							}
					for(j = 0; j < cnt_top; ++j)
						fout << top2[j].first << ":" << top2[j].second << " ";
					fout << "\n";
				}
				else if(q_type3 == "info")
				{
					fin >> driver;
					index = drivers_ht.get(driver);
					fout << driver << ": " << stations_graph[drivers[index].station].info << " ";
					fout << std::fixed << std::setprecision(3) << drivers[index].medium_rating << " ";
					fout << drivers[index].cnt_rides << " " << drivers[index].total_distance <<" ";
					if(drivers[index].online) fout << "online\n";
					else fout << "offline\n";
				}
			}
		}

		void task5_solver(std::ifstream& fin, std::ofstream& fout)
		{
			fin >> fuel >> driver >> cnt_queries;
			index = drivers_ht.get(driver);
			cnt_top = 0;
			for(i = 0; i < cnt_stations; ++i) mark[i] = false;
			for(i = 0; i < cnt_queries; ++i)
			{
				fin >> station1;
				node1 = stations_ht.get(station1);
				if(!mark[node1])
				{
					mark[node1] = true;
					dist = distance[drivers[index].station][node1];
					if(dist != -1 && dist <= fuel)
					{
						stations[cnt_top].first = dist;
						stations[cnt_top].second = station1;
						cnt_top++;
					}
				}
			}
			for(i = 0; i < cnt_top; ++i)
				for(j = i + 1; j < cnt_top; ++j)
					if(stations[i].first > stations[j].first || (stations[i].first == stations[j].first && stations[i].second > stations[j].second))
					{
						aux3 = stations[i];
						stations[i] = stations[j];
						stations[j] = aux3;
					}
			for(i = 0; i < cnt_top; ++i)
				fout << stations[i].second << " ";
		}
};
#endif  // SOLVER_H_
