#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <ctime>
#include <string>

class Random {
public:
    /**
    * @param min : lower bound
    * @param max : upper bound
    * @return random int value
    */
   static int randomInt(int min, int max) {
      static std::random_device rd;
      static std::seed_seq seed{rd(), static_cast<unsigned int>(time(nullptr))};
      static std::mt19937_64 gen(seed);
      std::uniform_int_distribution<int> dist(min, max);

      return dist(gen);
   }

   /**
   * @param min : lower bound
   * @param max : upper bound
   * @return random double value
   */
   static double randomDouble(double min, double max) {
      static std::random_device rd;
      static std::seed_seq seed{rd(), static_cast<unsigned int>(time(nullptr))};
      static std::mt19937_64 gen(seed);
      std::uniform_real_distribution<double> dist(min, max);

      return dist(gen);
   }

   /**
    * @param : length A length of string
    * @param : alphabet A set of characters that can be used in string
    * @return A random string
    */
   static std::string randomString(int length, const std::string& alphabet = "abcdefghijklmnopqrstuvwxyz") {
      std::string  res = "";

      for (int i = 0; i < length; i++)
         res += alphabet[randomInt(0, alphabet.size() - 1)];

      return res;
   }
};

#endif // RANDOM_H
