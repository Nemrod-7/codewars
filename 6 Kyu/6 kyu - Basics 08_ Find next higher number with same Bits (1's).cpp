#include <bitset>

int nextHigher(int value) {
  size_t ref = std::bitset<32>(value).count();

  while (value++)
      if (std::bitset<32>(value).count() == ref)
          return value;

  return 0;
}
