#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_map>
#include <experimental/filesystem>
#include <math.h>
#include <set>

#define EARTH_RADIUS 6371;

double deg2rad(double deg)
{
    return (deg * M_PI / 180);
}

double distance(double lat1, double lon1, double lat2, double lon2)
{
    double dlon = deg2rad(lon2 - lon1);
    double dlat = deg2rad(lat2 - lat1);
    double a = (sin(dlat / 2) * sin(dlat / 2)) + cos(deg2rad(lat1)) *
                cos(deg2rad(lat2)) * (sin(dlon / 2) *sin(dlon / 2));
    double angle = 2 * atan2(sqrt(a), sqrt(1 - a));
    return angle * EARTH_RADIUS;
}

int main()
{
    using namespace std::experimental;

    std::unordered_map<std::string,
                       std::pair<std::string, double>>  distance_map;
    std::set<std::string> files;

    for (auto & entry : filesystem::directory_iterator("../data"))
        files.insert(entry.path().string());

    for (auto const & file_name : files)
    {
        std::ifstream input { file_name };

        std::vector<std::string> flights;
        std::vector<double> latitudes;
        std::vector<double> longtitudes;

        while (!input.eof())
        {
            std::string flight, latitude, longtitude;

            if (!std::getline(input, flight, ','))   break;
            if (!std::getline(input, latitude, ',')) break;
            if (!std::getline(input, longtitude))    break;

            if (flight.compare("callsign") == 0)
                continue;

            flights.push_back(std::move(flight));
            latitudes.push_back(std::stod(latitude));
            longtitudes.push_back(std::stod(longtitude));
        }

        std::cout << "Processed CSV file '" << file_name << "':" << std::endl;

        for (size_t i = 0; i < flights.size(); ++i)
        {
            double min_distance = std::numeric_limits<double>::max();
            size_t min_distance_number;

            for (size_t j = 0; j < flights.size(); ++j)
            {
                if (i == j) continue;

                double cur_distance = distance(latitudes[j], longtitudes[j],
                                               latitudes[i], longtitudes[i]);

                if (min_distance > cur_distance)
                {
                    min_distance = cur_distance;
                    min_distance_number = j;
                }
            }

            auto it = distance_map.find(flights[i]);

            if (distance_map.end() == it)
            {
                std::pair<std::string, double> closest { flights[min_distance_number],
                                                         min_distance };
                it = distance_map.insert({ flights[i], std::move(closest) }).first;
            }
            else if (it->second.first != flights[min_distance_number])
            {
                it->second = { flights[min_distance_number], min_distance };
            }

            std::cout << it->first << ' '
                      << it->second.first << ' '
                      << it->second.second << std::endl;
        }
    }
    return 0;
}