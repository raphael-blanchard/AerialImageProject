////////////////////////////////////////////////////////////////////////////////
/// This file is part of the Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// The sources of AIP are distributed WITHOUT ANY WARRANTY.
////////////////////////////////////////////////////////////////////////////////

#ifndef COLOR_H
#define COLOR_H

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
/// This is a color.
///
/// The availale colors are defined as static const members: Color::Black,
/// Color::White, Color::Red, Color::Blue, Color::Green.
///
/// An example follows:
///
/// Color c = Color::Blue;
/// int id  = c.toInt();
/// Color d = Color::makeColor(id);
/// if (c == d) { ... }
////////////////////////////////////////////////////////////////////////////////
class Color {
public:
  /// Creates a color equal to the black one
  Color();

  /// Default copy constructor
  Color(const Color&) = default;

  /// Default assignment operator
  Color& operator=(const Color&) = default;

  /// Default destructor
  ~Color() = default;

  ///@{
  /// The available colors.
  static const Color Black;
  static const Color White;
  static const Color Red;
  static const Color Blue;
  static const Color Green;
  ///@}

  /// Casts a color to a unique identifier between 0 and nbColors()-1
  int toInt() const;

  /// Returns the number of colors
  static int nbColors();

  /// Creates a color given its identifier 0 <= n < nbColors()
  static Color makeColor(int n);

  /// Equality test
  bool operator==(const Color& c) const;

  /// Difference test
  bool operator!=(const Color& c) const;

private:
  enum Value : uint8_t { _black = 0, _white, _red, _blue, _green, _size };
  Value val_;

  Color(Value val);

  friend std::ostream& operator<<(std::ostream& os, const Color& c);
};

/// Output of a color on a stream
std::ostream& operator<<(std::ostream& os, const Color& c);

#endif
