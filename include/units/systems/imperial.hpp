// SPDX-License-Identifier: MIT

#pragma once

#include <units/core/quantity_spec.hpp>
#include <units/core/unit.hpp>
#include <units/systems/isq/base.hpp>
#include <units/systems/isq/mechanics.hpp>

namespace units::imperial {

  // =============================================================================
  // Imperial/US Customary Units
  // =============================================================================
  // This file defines units from the Imperial (British) and US customary systems.
  // Where the two systems differ (primarily in volume), both variants are provided.
  //
  // All conversion factors are to SI base units.

  // =============================================================================
  // Length Units
  // =============================================================================
  // The international yard and pound agreement of 1959 defined:
  // - 1 yard = 0.9144 metres (exactly)
  // - 1 pound = 0.45359237 kilograms (exactly)

  // -----------------------------------------------------------------------------
  // Inch (in)
  // -----------------------------------------------------------------------------

  /// inch - Imperial/US unit of length
  /// Symbol: in
  /// Definition: 1 in = 2.54 cm = 0.0254 m (exactly)
  struct inch_t final : named_unit<"in", kind_of<isq::length_t>, mag_ratio<254, 10000>> {};
  inline constexpr inch_t inch {};

  // -----------------------------------------------------------------------------
  // Foot (ft)
  // -----------------------------------------------------------------------------

  /// foot - Imperial/US unit of length
  /// Symbol: ft
  /// Definition: 1 ft = 12 in = 0.3048 m (exactly)
  struct foot_t final : named_unit<"ft", kind_of<isq::length_t>, mag_ratio<3048, 10000>> {};
  inline constexpr foot_t foot {};

  // Plural alias
  inline constexpr foot_t feet = foot;

  // -----------------------------------------------------------------------------
  // Yard (yd)
  // -----------------------------------------------------------------------------

  /// yard - Imperial/US unit of length
  /// Symbol: yd
  /// Definition: 1 yd = 3 ft = 0.9144 m (exactly)
  struct yard_t final : named_unit<"yd", kind_of<isq::length_t>, mag_ratio<9144, 10000>> {};
  inline constexpr yard_t yard {};

  // -----------------------------------------------------------------------------
  // Mile (mi)
  // -----------------------------------------------------------------------------

  /// mile - Imperial/US unit of length (statute mile)
  /// Symbol: mi
  /// Definition: 1 mi = 5280 ft = 1760 yd = 1609.344 m (exactly)
  struct mile_t final : named_unit<"mi", kind_of<isq::length_t>, mag_ratio<160'9344, 1000>> {};
  inline constexpr mile_t mile {};

  // -----------------------------------------------------------------------------
  // Nautical Mile (nmi)
  // -----------------------------------------------------------------------------

  /// nautical_mile - International nautical mile
  /// Symbol: nmi
  /// Definition: 1 nmi = 1852 m (exactly)
  struct nautical_mile_t final : named_unit<"nmi", kind_of<isq::length_t>, mag<1852>> {};
  inline constexpr nautical_mile_t nautical_mile {};

  // -----------------------------------------------------------------------------
  // Fathom (ftm)
  // -----------------------------------------------------------------------------

  /// fathom - Nautical unit of depth
  /// Symbol: ftm
  /// Definition: 1 ftm = 6 ft = 1.8288 m
  struct fathom_t final : named_unit<"ftm", kind_of<isq::length_t>, mag_ratio<18288, 10000>> {};
  inline constexpr fathom_t fathom {};

  // -----------------------------------------------------------------------------
  // Chain (ch)
  // -----------------------------------------------------------------------------

  /// chain - Surveyor's chain (Gunter's chain)
  /// Symbol: ch
  /// Definition: 1 ch = 66 ft = 22 yd = 20.1168 m
  struct chain_t final : named_unit<"ch", kind_of<isq::length_t>, mag_ratio<20'1168, 10000>> {};
  inline constexpr chain_t chain {};

  // -----------------------------------------------------------------------------
  // Furlong (fur)
  // -----------------------------------------------------------------------------

  /// furlong - Unit of distance
  /// Symbol: fur
  /// Definition: 1 fur = 10 ch = 660 ft = 201.168 m
  struct furlong_t final : named_unit<"fur", kind_of<isq::length_t>, mag_ratio<20'1168, 1000>> {};
  inline constexpr furlong_t furlong {};

  // -----------------------------------------------------------------------------
  // Thou/Mil (th)
  // -----------------------------------------------------------------------------

  /// thou - Also known as mil, one thousandth of an inch
  /// Symbol: th
  /// Definition: 1 th = 0.001 in = 0.0000254 m = 25.4 μm
  struct thou_t final : named_unit<"th", kind_of<isq::length_t>, mag_ratio<254, 1000'0000>> {};
  inline constexpr thou_t thou {};
  inline constexpr thou_t mil = thou;

  // =============================================================================
  // Mass Units
  // =============================================================================

  // -----------------------------------------------------------------------------
  // Grain (gr)
  // -----------------------------------------------------------------------------

  /// grain - Smallest traditional English unit of mass
  /// Symbol: gr
  /// Definition: 1 gr = 1/7000 lb = 64.79891 mg (exactly)
  struct grain_t final
      : named_unit<"gr", kind_of<isq::mass_t>, mag_ratio<647'9891, 1000'0000'0000>> {};
  inline constexpr grain_t grain {};

  // -----------------------------------------------------------------------------
  // Ounce (oz) - Avoirdupois
  // -----------------------------------------------------------------------------

  /// ounce - Avoirdupois ounce
  /// Symbol: oz
  /// Definition: 1 oz = 1/16 lb = 28.349523125 g (exactly)
  /// Note: This is the avoirdupois ounce, not troy ounce
  struct ounce_t final
      : named_unit<"oz", kind_of<isq::mass_t>, mag_ratio<283'4952'3125, 1'0000'0000'0000>> {};
  inline constexpr ounce_t ounce {};

  // -----------------------------------------------------------------------------
  // Pound (lb) - Avoirdupois
  // -----------------------------------------------------------------------------

  /// pound - Avoirdupois pound
  /// Symbol: lb
  /// Definition: 1 lb = 0.45359237 kg (exactly, by international agreement)
  struct pound_t final
      : named_unit<"lb", kind_of<isq::mass_t>, mag_ratio<4535'9237, 1'0000'0000>> {};
  inline constexpr pound_t pound {};

  // -----------------------------------------------------------------------------
  // Stone (st) - British
  // -----------------------------------------------------------------------------

  /// stone - British unit of mass, commonly used for body weight
  /// Symbol: st
  /// Definition: 1 st = 14 lb = 6.35029318 kg
  struct stone_t final
      : named_unit<"st", kind_of<isq::mass_t>, mag_ratio<6'3502'9318, 1'0000'0000>> {};
  inline constexpr stone_t stone {};

  // -----------------------------------------------------------------------------
  // Hundredweight - US (cwt) and British (cwt)
  // -----------------------------------------------------------------------------

  /// short_hundredweight - US hundredweight
  /// Symbol: cwt (US)
  /// Definition: 1 short cwt = 100 lb = 45.359237 kg
  struct short_hundredweight_t final
      : named_unit<"cwt", kind_of<isq::mass_t>, mag_ratio<4535'9237, 100'0000>> {};
  inline constexpr short_hundredweight_t short_hundredweight {};

  /// long_hundredweight - British hundredweight
  /// Symbol: cwt (UK)
  /// Definition: 1 long cwt = 112 lb = 50.80234544 kg
  struct long_hundredweight_t final
      : named_unit<"cwt", kind_of<isq::mass_t>, mag_ratio<50'8023'4544, 1'0000'0000>> {};
  inline constexpr long_hundredweight_t long_hundredweight {};

  // -----------------------------------------------------------------------------
  // Ton - US (short) and British (long)
  // -----------------------------------------------------------------------------

  /// short_ton - US ton
  /// Symbol: ton (US)
  /// Definition: 1 short ton = 2000 lb = 907.18474 kg
  struct short_ton_t final
      : named_unit<"ton", kind_of<isq::mass_t>, mag_ratio<9071'8474, 10'0000>> {};
  inline constexpr short_ton_t short_ton {};

  /// long_ton - British ton (also called imperial ton)
  /// Symbol: ton (UK)
  /// Definition: 1 long ton = 2240 lb = 1016.0469088 kg
  struct long_ton_t final
      : named_unit<"tn", kind_of<isq::mass_t>, mag_ratio<101'6046'9088, 1000'0000>> {};
  inline constexpr long_ton_t long_ton {};
  inline constexpr long_ton_t imperial_ton = long_ton;

  // =============================================================================
  // Volume Units
  // =============================================================================
  // US and Imperial gallon differ significantly:
  // - US gallon = 231 cubic inches = 3.785411784 L
  // - Imperial gallon = 4.54609 L (exactly)

  // -----------------------------------------------------------------------------
  // US Fluid Ounce
  // -----------------------------------------------------------------------------

  /// us_fluid_ounce - US customary fluid ounce
  /// Symbol: fl oz (US)
  /// Definition: 1 US fl oz = 1/128 US gallon = 29.5735295625 mL
  struct us_fluid_ounce_t final
      : named_unit<"fl oz",
                   kind_of<isq::length_t>,
                   mag_ratio<2957'3529'5625, 1'0000'0000'0000'0000>> {};
  inline constexpr us_fluid_ounce_t us_fluid_ounce {};

  // -----------------------------------------------------------------------------
  // Imperial Fluid Ounce
  // -----------------------------------------------------------------------------

  /// imperial_fluid_ounce - British Imperial fluid ounce
  /// Symbol: fl oz (UK)
  /// Definition: 1 Imp fl oz = 1/160 imperial gallon = 28.4130625 mL
  struct imperial_fluid_ounce_t final
      : named_unit<"fl oz", kind_of<isq::length_t>, mag_ratio<2'8413'0625, 10'0000'0000'0000>> {};
  inline constexpr imperial_fluid_ounce_t imperial_fluid_ounce {};

  // Default fluid ounce aliases to US
  inline constexpr us_fluid_ounce_t fluid_ounce = us_fluid_ounce;

  // -----------------------------------------------------------------------------
  // US Pint
  // -----------------------------------------------------------------------------

  /// us_pint - US customary pint
  /// Symbol: pt (US)
  /// Definition: 1 US pt = 16 US fl oz = 1/8 US gallon = 473.176473 mL
  struct us_pint_t final
      : named_unit<"pt", kind_of<isq::length_t>, mag_ratio<4'7317'6473, 1'0000'0000'0000>> {};
  inline constexpr us_pint_t us_pint {};

  // -----------------------------------------------------------------------------
  // Imperial Pint
  // -----------------------------------------------------------------------------

  /// imperial_pint - British Imperial pint
  /// Symbol: pt (UK)
  /// Definition: 1 Imp pt = 20 Imp fl oz = 1/8 imperial gallon = 568.26125 mL
  struct imperial_pint_t final
      : named_unit<"pt", kind_of<isq::length_t>, mag_ratio<5682'6125, 1000'0000'0000>> {};
  inline constexpr imperial_pint_t imperial_pint {};

  // Default pint aliases to US
  inline constexpr us_pint_t pint = us_pint;

  // -----------------------------------------------------------------------------
  // US Quart
  // -----------------------------------------------------------------------------

  /// us_quart - US customary quart
  /// Symbol: qt (US)
  /// Definition: 1 US qt = 2 US pt = 1/4 US gallon = 946.352946 mL
  struct us_quart_t final
      : named_unit<"qt", kind_of<isq::length_t>, mag_ratio<9'4635'2946, 1'0000'0000'0000>> {};
  inline constexpr us_quart_t us_quart {};

  // -----------------------------------------------------------------------------
  // Imperial Quart
  // -----------------------------------------------------------------------------

  /// imperial_quart - British Imperial quart
  /// Symbol: qt (UK)
  /// Definition: 1 Imp qt = 2 Imp pt = 1/4 imperial gallon = 1.1365225 L
  struct imperial_quart_t final
      : named_unit<"qt", kind_of<isq::length_t>, mag_ratio<1136'5225, 100'0000'0000>> {};
  inline constexpr imperial_quart_t imperial_quart {};

  // Default quart aliases to US
  inline constexpr us_quart_t quart = us_quart;

  // -----------------------------------------------------------------------------
  // US Gallon
  // -----------------------------------------------------------------------------

  /// us_gallon - US customary gallon
  /// Symbol: gal (US)
  /// Definition: 1 US gal = 231 cubic inches = 3.785411784 L (exactly)
  struct us_gallon_t final
      : named_unit<"gal", kind_of<isq::length_t>, mag_ratio<37'8541'1784, 1'0000'0000'0000>> {};
  inline constexpr us_gallon_t us_gallon {};

  // -----------------------------------------------------------------------------
  // Imperial Gallon
  // -----------------------------------------------------------------------------

  /// imperial_gallon - British Imperial gallon
  /// Symbol: gal (UK)
  /// Definition: 1 Imp gal = 4.54609 L (exactly)
  struct imperial_gallon_t final
      : named_unit<"gal", kind_of<isq::length_t>, mag_ratio<45'4609, 1'0000'0000>> {};
  inline constexpr imperial_gallon_t imperial_gallon {};

  // Default gallon aliases to US
  inline constexpr us_gallon_t gallon = us_gallon;

  // -----------------------------------------------------------------------------
  // Barrel (US petroleum)
  // -----------------------------------------------------------------------------

  /// us_barrel - US petroleum barrel
  /// Symbol: bbl
  /// Definition: 1 bbl = 42 US gallons = 158.987294928 L
  struct us_barrel_t final
      : named_unit<"bbl", kind_of<isq::length_t>, mag_ratio<1589'8729'4928, 1'0000'0000'0000>> {};
  inline constexpr us_barrel_t us_barrel {};
  inline constexpr us_barrel_t barrel = us_barrel;

  // =============================================================================
  // Area Units
  // =============================================================================

  // -----------------------------------------------------------------------------
  // Acre
  // -----------------------------------------------------------------------------

  /// acre - Unit of area
  /// Symbol: ac
  /// Definition: 1 ac = 43560 sq ft = 4046.8564224 m²
  struct acre_t final
      : named_unit<"ac", kind_of<isq::dimensionless_t>, mag_ratio<404'6856'4224, 1000'0000>> {};
  inline constexpr acre_t acre {};

  // -----------------------------------------------------------------------------
  // Square Foot
  // -----------------------------------------------------------------------------

  /// square_foot - Unit of area
  /// Symbol: sq ft
  /// Definition: 1 sq ft = 144 sq in = 0.09290304 m²
  struct square_foot_t final
      : named_unit<"sq ft", kind_of<isq::dimensionless_t>, mag_ratio<929'0304, 1'0000'0000>> {};
  inline constexpr square_foot_t square_foot {};

  // -----------------------------------------------------------------------------
  // Square Inch
  // -----------------------------------------------------------------------------

  /// square_inch - Unit of area
  /// Symbol: sq in
  /// Definition: 1 sq in = 6.4516 cm² = 0.00064516 m²
  struct square_inch_t final
      : named_unit<"sq in", kind_of<isq::dimensionless_t>, mag_ratio<64516, 1'0000'0000>> {};
  inline constexpr square_inch_t square_inch {};

  // -----------------------------------------------------------------------------
  // Square Mile
  // -----------------------------------------------------------------------------

  /// square_mile - Unit of area
  /// Symbol: sq mi
  /// Definition: 1 sq mi = 640 acres = 2589988.110336 m² ≈ 2.59 km²
  struct square_mile_t final
      : named_unit<"sq mi", kind_of<isq::dimensionless_t>, mag_ratio<2'5899'8811'0336, 100'0000>> {
  };
  inline constexpr square_mile_t square_mile {};

  // =============================================================================
  // Speed Units
  // =============================================================================

  // -----------------------------------------------------------------------------
  // Miles per Hour
  // -----------------------------------------------------------------------------

  /// mile_per_hour - Common unit of speed
  /// Symbol: mph
  /// Definition: 1 mph = 1 mi/h = 0.44704 m/s (exactly)
  struct mile_per_hour_t final
      : named_unit<"mph", kind_of<isq::speed_t>, mag_ratio<44704, 10'0000>> {};
  inline constexpr mile_per_hour_t mile_per_hour {};
  inline constexpr mile_per_hour_t mph = mile_per_hour;

  // -----------------------------------------------------------------------------
  // Knot
  // -----------------------------------------------------------------------------

  /// knot - Nautical unit of speed
  /// Symbol: kn
  /// Definition: 1 kn = 1 nmi/h = 1.852 km/h = 0.514444... m/s
  struct knot_t final : named_unit<"kn", kind_of<isq::speed_t>, mag_ratio<1852, 3600>> {};
  inline constexpr knot_t knot {};

  // -----------------------------------------------------------------------------
  // Feet per Second
  // -----------------------------------------------------------------------------

  /// foot_per_second - Unit of speed
  /// Symbol: ft/s
  /// Definition: 1 ft/s = 0.3048 m/s
  struct foot_per_second_t final
      : named_unit<"ft/s", kind_of<isq::speed_t>, mag_ratio<3048, 10000>> {};
  inline constexpr foot_per_second_t foot_per_second {};
  inline constexpr foot_per_second_t fps = foot_per_second;

  // =============================================================================
  // Force Units
  // =============================================================================

  // -----------------------------------------------------------------------------
  // Pound-Force
  // -----------------------------------------------------------------------------

  /// pound_force - Unit of force in the Imperial system
  /// Symbol: lbf
  /// Definition: 1 lbf = 1 lb × g = 4.4482216152605 N
  /// where g = 9.80665 m/s² (standard gravity)
  struct pound_force_t final
      : named_unit<"lbf", kind_of<isq::force_t>, mag_ratio<44'4822'1615, 10'0000'0000>> {};
  inline constexpr pound_force_t pound_force {};
  inline constexpr pound_force_t lbf = pound_force;

  // =============================================================================
  // Pressure Units
  // =============================================================================

  // -----------------------------------------------------------------------------
  // Pound per Square Inch (psi)
  // -----------------------------------------------------------------------------

  /// psi - Pounds per square inch
  /// Symbol: psi
  /// Definition: 1 psi = 1 lbf/in² = 6894.757293168 Pa
  struct psi_t final
      : named_unit<"psi", kind_of<isq::pressure_t>, mag_ratio<6'8947'5729'3168, 10'0000'0000>> {};
  inline constexpr psi_t psi {};

  // =============================================================================
  // Energy Units
  // =============================================================================

  // -----------------------------------------------------------------------------
  // Foot-Pound
  // -----------------------------------------------------------------------------

  /// foot_pound - Unit of energy
  /// Symbol: ft⋅lbf
  /// Definition: 1 ft⋅lbf = 1.3558179483314 J
  struct foot_pound_t final
      : named_unit<"ft.lbf",
                   kind_of<isq::energy_t>,
                   mag_ratio<13'5581'7948'3314, 10'0000'0000'0000>> {};
  inline constexpr foot_pound_t foot_pound {};

  // -----------------------------------------------------------------------------
  // British Thermal Unit (BTU)
  // -----------------------------------------------------------------------------

  /// btu - British thermal unit (ISO)
  /// Symbol: BTU
  /// Definition: 1 BTU = 1055.06 J (ISO definition)
  struct btu_t final : named_unit<"BTU", kind_of<isq::energy_t>, mag_ratio<10'5506, 100>> {};
  inline constexpr btu_t btu {};

  // -----------------------------------------------------------------------------
  // Therm
  // -----------------------------------------------------------------------------

  /// therm - Unit of heat energy
  /// Symbol: thm
  /// Definition: 1 thm = 100,000 BTU = 105.506 MJ
  struct therm_t final
      : named_unit<"thm", kind_of<isq::energy_t>, mag_ratio<105'5060'0000, 100>> {};
  inline constexpr therm_t therm {};

  // =============================================================================
  // Power Units
  // =============================================================================

  // -----------------------------------------------------------------------------
  // Horsepower (mechanical)
  // -----------------------------------------------------------------------------

  /// horsepower - Mechanical horsepower
  /// Symbol: hp
  /// Definition: 1 hp = 550 ft⋅lbf/s = 745.69987158227 W
  struct horsepower_t final
      : named_unit<"hp", kind_of<isq::power_t>, mag_ratio<7'4569'9872, 100'0000>> {};
  inline constexpr horsepower_t horsepower {};
  inline constexpr horsepower_t hp = horsepower;

  // =============================================================================
  // Static assertions to verify Imperial unit properties
  // =============================================================================

  // Length units satisfy Unit concept
  static_assert(Unit<inch_t>);
  static_assert(Unit<foot_t>);
  static_assert(Unit<yard_t>);
  static_assert(Unit<mile_t>);
  static_assert(Unit<nautical_mile_t>);

  // Mass units satisfy Unit concept
  static_assert(Unit<ounce_t>);
  static_assert(Unit<pound_t>);
  static_assert(Unit<stone_t>);
  static_assert(Unit<short_ton_t>);
  static_assert(Unit<long_ton_t>);

  // Volume units satisfy Unit concept
  static_assert(Unit<us_gallon_t>);
  static_assert(Unit<imperial_gallon_t>);
  static_assert(Unit<us_pint_t>);
  static_assert(Unit<imperial_pint_t>);

  // Speed units satisfy Unit concept
  static_assert(Unit<mile_per_hour_t>);
  static_assert(Unit<knot_t>);

  // Force and pressure units satisfy Unit concept
  static_assert(Unit<pound_force_t>);
  static_assert(Unit<psi_t>);

  // Verify key conversion factors (ratios are GCD-normalized)
  // inch: 1 in = 0.0254 m = 254/10000 m = 127/5000 m (after GCD)
  static_assert(inch_t::magnitude_type::num == 127);
  static_assert(inch_t::magnitude_type::den == 5000);

  // foot: 1 ft = 0.3048 m = 3048/10000 m = 381/1250 m (after GCD)
  static_assert(foot_t::magnitude_type::num == 381);
  static_assert(foot_t::magnitude_type::den == 1250);

  // yard: 1 yd = 0.9144 m = 9144/10000 m = 1143/1250 m (after GCD)
  static_assert(yard_t::magnitude_type::num == 1143);
  static_assert(yard_t::magnitude_type::den == 1250);

  // Verify length relationships (approximately, using magnitude ratios)
  // 12 inches = 1 foot: inch magnitude * 12 ≈ foot magnitude
  // 254/10000 * 12 = 3048/10000 ✓

  // Verify symbols
  static_assert(inch_t::symbol == fixed_string { "in" });
  static_assert(foot_t::symbol == fixed_string { "ft" });
  static_assert(yard_t::symbol == fixed_string { "yd" });
  static_assert(mile_t::symbol == fixed_string { "mi" });
  static_assert(pound_t::symbol == fixed_string { "lb" });
  static_assert(ounce_t::symbol == fixed_string { "oz" });

} // namespace units::imperial
