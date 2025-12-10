#include <bits/stdc++.h>

using namespace std;

class Sheep
{
public:
    int age;
    string colour;
    string name;

    Sheep(int age, const string &colour, const string &name)
    {
        this->age = age;
        this->colour = colour;
        this->name = name;
    }
};

vector<Sheep> getSortedSheepSTL(const vector<Sheep> &sheep)
{
    vector<Sheep> sortedSheep = sheep;
    sort(sortedSheep.begin(), sortedSheep.end(), [](const Sheep &a, const Sheep &b)
         { return a.age < b.age; });
    return sortedSheep;
}

vector<Sheep> getBlackSheepSTL(const vector<Sheep> &sheep)
{
    vector<Sheep> blackSheep;
    copy_if(sheep.begin(), sheep.end(), back_inserter(blackSheep), [](const Sheep &a)
            { return a.colour == "black"; });
    return blackSheep;
}

void removeSheepNameStartingWithESTL(vector<Sheep> &sheep)
{
    auto new_end = remove_if(sheep.begin(), sheep.end(), [](const Sheep &a)
                             {
        if(a.name.empty()) return false;
        return a.name.at(0) == 'E' || a.name.at(0) == 'e'; });
    sheep.erase(new_end, sheep.end());
}

void fixKarenTypoSTL(vector<Sheep> &sheep)
{
    auto karne_sheep = find_if(sheep.begin(), sheep.end(), [](const Sheep &a){
        return a.name == "Karne";
    });
    karne_sheep->name = "Karen";
    // const int original_age = 5;
    // const string original_colour = "white";
    // Sheep karen_replacement(original_age, original_colour, "Karen");
    // replace_if(sheep.begin(), sheep.end(), [](const Sheep &a)
    //            { return a.name == "Karne"; }, karen_replacement);
}

bool areAllSheepWhiteSTL(const vector<Sheep> &sheep)
{
    return all_of(sheep.begin(), sheep.end(), [](const Sheep &a){
        return a.colour == "white";
    });
}

bool isThereSheepOlderThan9YearsSTL(const vector<Sheep> &sheep){
    return any_of(sheep.begin(), sheep.end(), [](const Sheep &a){
        return a.age > 9;
    });
}

int main()
{

    return 0;
}