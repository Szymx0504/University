include <iostream>
#include <vector>
#include <algorithm>

void printASCIISheep()
{
   std::cout <<
      "                        __  _\n" <<
      "                    .-.'  `; `-._  __  _\n" <<
      "                   (_,         .-:'  `; `-._\n" <<
      "                 ,'o\"(        (_,           )\n" <<
      "                (__,-'      ,'o\"(            )> \n" <<
      "       __          (       (__,-'            )\n" <<
      "  (___()'`;         `-'._.--._(             )\n" <<
      "  /,    /`             |||  |||`-'._.--._.-'\n" <<
      "  \\\\\"--\\\\                         |||  |||\n\n";
}

enum class Color
{
   White = 0xFFFFFF,
   Black = 0x000000,
   Gray = 0x808080,
};

struct Sheep
{
   std::string name;
   int age;
   Color color;
};

std::ostream& operator<<(std::ostream& os, Color color)
{
   switch (color)
   {
   case Color::White:
      os << "White";
      break;
   case Color::Black:
      os << "Black";
      break;
   case Color::Gray:
      os << "Gray";
      break;
   default:
      os << "Undefined";
      break;
   }
   return os;
}

std::ostream& operator<<(std::ostream& os, const Sheep& sheep)
{
   os << sheep.name << " " << sheep.age << " " << sheep.color;
   return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Sheep>& sheep)
{
   for (const Sheep& s : sheep)
   {
      os << s << std::endl;
   }
   return os;
}

std::vector<Sheep> getSortedSheep(const std::vector<Sheep>& sheep)
{
   //todo
   return {};
}

std::vector<Sheep> getBlackSheep(const std::vector<Sheep>& sheep)
{
   //todo
   return {};
}

void removeSheepNameStartingWithE(std::vector<Sheep>& sheep)
{
   //todo
}

void fixKarenTypo(std::vector<Sheep>& sheep)
{
   //todo
}

bool areAllSheepWhite(const std::vector<Sheep>& sheep)
{
   //todo
   return true;
}

bool isThereSheepOlderThan9Years(const std::vector<Sheep>& sheep)
{
   //todo
   return false;
}

std::vector<Sheep> getSortedSheepSTL(const std::vector<Sheep>& sheep)
{
   //sort
   return {};
}

std::vector<Sheep> getBlackSheepSTL(const std::vector<Sheep>& sheep)
{
   //copy_if
   return {};
}

void removeSheepNameStartingWithESTL(std::vector<Sheep>& sheep)
{
   //remove_if or erase_if
}

void fixKarenTypoSTL(std::vector<Sheep>& sheep)
{
   //replace_if
}

bool areAllSheepWhiteSTL(const std::vector<Sheep>& sheep)
{
   //all_of
   return true;
}

bool isThereSheepOlderThan9YearsSTL(const std::vector<Sheep>& sheep)
{
   //any_of
   return false;
}

int main()
{
   printASCIISheep();
   std::vector<Sheep> sheep = {
      { "Daisy", 2, Color::White },
      { "Caroline", 3, Color::Gray },
      { "Elena", 4, Color::Black },
      { "Karne", 3, Color::White },
      { "Marta", 2, Color::Black },
      { "Lily", 1, Color::White },
      { "Olivia", 1, Color::Gray },
      { "Evelyn", 5, Color::White },
      { "Ava", 10, Color::Gray },
      { "Eleanor", 8, Color::Black }
   };

   std::cout << "Sorted:" << std::endl;
   std::cout << getSortedSheep(sheep) << std::endl;
   std::cout << "Black:" << std::endl;
   std::cout << getBlackSheep(sheep) << std::endl;
   std::cout << "Without sheep with name starting with 'E':" << std::endl;
   removeSheepNameStartingWithE(sheep);
   fixKarenTypo(sheep);
   std::cout << sheep << std::endl;
   std::cout << "All sheep are white: " << (areAllSheepWhite(sheep) ? "True" : "False") << std::endl;
   std::cout << "Sheep older than 9 years old: " << (isThereSheepOlderThan9Years(sheep) ? "True" : "False") << std::endl;
   return 0;
}
