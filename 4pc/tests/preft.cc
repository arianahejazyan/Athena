#include <iostream>
#include <chrono>
#include <vector>
#include "generator.h"

/*
typedef std::chrono::high_resolution_clock::time_point Time;


#define currTime() (std::chrono::high_resolution_clock::now())

#define duration(tic, toc) (std::chrono::duration_cast<std::chrono::milliseconds>(toc - tic))

void preft(const std::vector<Position>& positions, std::vector<Record>& records, std::vector<Time>& durations, int max_depth, int curr_depth=0)
{
    if (curr_depth <= max_depth)
    {
        std::vector<Position> list;

        for (const Position& pos: positions)
        {
            std::vector<Position> temp;

            Time tic = currTime();

            GENERATOR::generate_all_positions(temp, pos);

            Time toc = currTime();

            list.insert(list.end(), temp.begin(), temp.end());

            durations[curr_depth] += duration(tic, toc);        
        }

        RECORD::updateRecord(list, records[curr_depth]);

        preft(list, records, durations, max_depth, curr_depth + 1);
    }
}

int main()
{
    Position pos = Position::Standard();

    int depth=10, nodes=1, time=0;

    preft(pos, depth, nodes, time);

    std::cout << "Depth: " << depth << std::endl;
    std::cout << "Nodes: " << nodes << std::endl;
    std::cout << "Time: " << time << std::endl;
    std::cout << "Nps: " << nodes/time << std::endl;

    return 0;
}
*/