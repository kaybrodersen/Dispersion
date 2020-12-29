// A simple numerical simulation of fluid dispersion.
// Author: khbrodersen@gmail.com.

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

namespace dispersion {

static constexpr float vw = 100.0f;  // Total volume of water.
static constexpr float cg = 0.0f;  // Initial color concentration in water.
static constexpr float vb = 0.5f;  // Volume of fluid in tool.
static constexpr float cb = 1.0f;  // Conc. of fluid in tool before mixing.

static const char *kFileResults = "/concentrations.csv";  // Output file in ~.

// Updates all `n` concentrations in `cg[]`.
void Update(const int& n, const float& vw, float cg[n],
            const float& vb, float cb) {
  float vg = vw / n;
  float vmix = vg + vb;
  for (int g {0}; g < n; ++g) {
    cg[g] = vg / vmix * cg[g] + vb / vmix * cb;
    cb = cg[g];
  }
}

// Runs `rounds` iterations with `n` containers.
// Returns a string of the results of all rounds.
std::string Simulate(const int& n, const int& rounds) {
  float _cg[n];
  for (int g {0}; g < n; ++g) {
    _cg[g] = cg;
  }
  std::string result = "";
  for (int i {1}; i <= rounds; ++i) {
    Update(n, vw, _cg, vb, cb);
    result = result + std::to_string(n) + ", " + std::to_string(i) + ", " +
    std::to_string(_cg[n - 1]) + "\n";
  }
  return result;
}

// Saves a `results` string to `filename`.
void WriteResults(const std::string& results, const std::string& filename) {
  std::ofstream file;
  file.open(filename);
  file << "Containers, Iteration, Concentration" << std::endl;
  file << results;
  file.close();
}

}  // namespace dispersion

int main() {
  // Simulate `n` containers with `rounds` iterations.
  constexpr int rounds = 1000;
  constexpr int ns[] = {1, 2, 3, 10, 100};
  std::string results = "";
  for (auto i {0}; i < sizeof(ns) / sizeof(int); ++i) {
    results += dispersion::Simulate(ns[i], rounds);
  }
  
  // Store results on disk.
  const char* home_dir = getenv("HOME");
  std::string filename = std::string(home_dir) + dispersion::kFileResults;
  dispersion::WriteResults(results, filename);
  return 0;
}
