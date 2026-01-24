// SPDX-License-Identifier: MIT
// README example: Natural Units (Particle Physics)

#include <units/units.hpp>

auto main() -> int {
  using namespace units::natural::literals;

  // Energy literals
  auto energy_ev  = 511.0_keV;   // Electron rest mass energy
  auto energy_mev = 938.272_MeV; // Proton rest mass energy
  auto higgs_mass = 125.0_GeV;   // Higgs boson mass
  auto lhc_energy = 13.0_TeV;    // LHC collision energy

  // Mass literals (eV/c^2)
  auto electron  = 0.511_MeVc2;   // Electron mass
  auto proton    = 938.272_MeVc2; // Proton mass
  auto top_quark = 172.69_GeVc2;  // Top quark mass

  // Momentum literals (eV/c)
  auto momentum = 100.0_MeVc; // Particle momentum

  // Cross-section literals
  auto total_xs = 100.0_mb; // Total cross-section
  auto lhc_lumi = 140.0_fb; // Integrated luminosity (inverse femtobarns)

  // Length (fermi/femtometre)
  auto nuclear_radius = 1.2_fm; // Nuclear radius scale

  (void)energy_ev;
  (void)energy_mev;
  (void)higgs_mass;
  (void)lhc_energy;
  (void)electron;
  (void)proton;
  (void)top_quark;
  (void)momentum;
  (void)total_xs;
  (void)lhc_lumi;
  (void)nuclear_radius;
  return 0;
}
