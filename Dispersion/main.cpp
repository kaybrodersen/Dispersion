// A simple numerical simulation of fluid dispersion.
// Author: khbrodersen@gmail.com.

#include <iostream>
#include <vector>

// [[Rcpp::plugins(cpp11)]]
#include <Rcpp.h>

namespace dispersion {

static const std::vector<int> ns {1, 2, 3, 10, 100};  // Containers.
static constexpr int rounds {1000};  // Number of iterations.

static constexpr double vw {100.0};  // Total volume of water.
static constexpr double cg {0.0};  // Initial color concentration in water.
static constexpr double vb {0.5};  // Volume of fluid in tool.
static constexpr double cb {1.0};  // Conc. of fluid in tool before mixing.

// Updates all `n` concentrations in `cg[]`.
void Update(const int& n, const double& vw, double cg[n],
            const double& vb, double cb) {
  double vg {vw / n};
  double vmix {vg + vb};
  for (int g {0}; g < n; ++g) {
    cg[g] = vg / vmix * cg[g] + vb / vmix * cb;
    cb = cg[g];
  }
}

// Runs `rounds` iterations with `n` containers.
// Returns a vector of `rounds` concentrations.
std::vector<double> Simulate(const int& n, const int& rounds) {
  double _cg[n];
  for (int g {0}; g < n; ++g) {
    _cg[g] = cg;
  }
  std::vector<double> cbs(rounds);
  for (int i {1}; i <= rounds; ++i) {
    Update(n, vw, _cg, vb, cb);
    cbs[i] = _cg[n - 1];
  }
  return cbs;
}

}  // namespace dispersion

// [[Rcpp::export]]
Rcpp::DataFrame Run() {
  // Prepare accumulators.
  std::vector<int> result_containers;
  std::vector<int> result_iteration;
  std::vector<double> result_concentration;

  // Simulate `n` containers with `rounds` iterations.
  for (int s {0}; s < dispersion::ns.size(); ++s) {
    for (int r {1}; r <= dispersion::rounds; ++r) {
      result_containers.push_back(dispersion::ns[s]);
      result_iteration.push_back(r);
    }
    std::vector<double> current = dispersion::Simulate(dispersion::ns[s],
                                                       dispersion::rounds);
    result_concentration.insert(result_concentration.end(),
                                current.begin(), current.end());
  }
  Rcpp::DataFrame result = Rcpp::DataFrame::create(
    Rcpp::Named("Containers") = result_containers,
    Rcpp::_["Iteration"] = result_iteration,
    Rcpp::_["Concentration"] = result_concentration
  );
  return result;
}

/*** R
library(dplyr)
library(ggplot2)

result = Run()

result %>%
  dplyr::mutate(Containers = as.factor(Containers)) %>%
  ggplot(aes(Iteration, Concentration, color = Containers)) +
  theme_bw(base_size = 20) +
  geom_line(size = 1) +
  ylab("Remaining concentration")
*/
