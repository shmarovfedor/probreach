//
// Created by fedor on 29/09/2026.
//

#ifndef PROBREACH_SYMBOL_H
#define PROBREACH_SYMBOL_H

#include <string>
#include <ostream>

class symbolt
{
private:
  std::string value;

public:
  symbolt(std::string value) : value(value)
  {
  }

  bool operator==(const symbolt &other) const
  {
    return value == other.value;
  }

  std::string get_type() const
  {
    return "symbolt";
  }

  std::string get_value()
  {
    return value;
  }

  void print(std::ostream &out) const
  {
    out << value;
  }
  
  friend std::ostream &operator<<(std::ostream &os, const symbolt &e)
  {
    e.print(os);
    return os;
  }
};

#endif // PROBREACH_SYMBOL_H
