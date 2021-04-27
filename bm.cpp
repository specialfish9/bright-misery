#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

#define BASE "/sys/class/backlight/intel_backlight/"
#define MAX BASE "max_brightness"
#define ACTUAL BASE "actual_brightness"

using namespace std;

int main(int argc, char **argv) {
  ifstream max_fin(MAX);

  if (!max_fin) {
    cerr << "MAX BOUND NOT FOUND" << endl;
    cerr << "Error: " << MAX << " file not found." << endl;
    return 1;
  }

  ifstream actual_fin(ACTUAL);

  if (!actual_fin) {
    cerr << "ACTUAL NOT FOUND" << endl;
    cerr << "Error: " << ACTUAL << " file not found." << endl;
    return 1;
  }

  if (argc < 2) {
    cerr << "MISSING PARAMETER: " << endl;
    cerr << "bm [value]" << endl;
    return 1;
  }

  size_t max, actual;
  max_fin >> max;
  actual_fin >> actual;

  max_fin.close();
  actual_fin.close();

  int16_t delta = stoi(argv[1]);

  // delta : 100 = x : actual
  int16_t x = (delta * max) / 100;

  int16_t fin = actual + delta;

  if (fin > max) {
    fin = max;
  } else if (fin < 10) {
    fin = 10;
  }

  char out_str[100];
  snprintf(out_str, 100,
           "echo %d | sudo tee /sys/class/backlight/intel_backlight/brightness "
           ">> /dev/null",
           (int)(fin));

  cout << system(out_str);

  return 0;
}
