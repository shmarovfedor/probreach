#include "new_to_old.h"
#include <iostream>

node *new_to_old(real_exprt &e)
{
  if (auto ptr = dynamic_cast<symbolt*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<numbert*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<minust*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<plust*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<addt*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<subt*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<mult*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<divt*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<powt*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<abst*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<sqrtt*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<expt*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<logt*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<sint*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<cost*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<tant*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<asint*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<acost*>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<atant*>(&e))
  {
    return new_to_old(*ptr);
  }
}

node *new_to_old(symbolt &e)
{
  return new node(e.get_value());
}

node *new_to_old(numbert &e)
{
  return new node(e.get_value());
}

node *new_to_old(minust &e)
{
  return new node("-", {new_to_old(e.get_value())});
}

node *new_to_old(plust &e)
{
  return new node("+", {new_to_old(e.get_value())});
}

node *new_to_old(addt &e)
{
  return new node("+", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(subt &e)
{
  return new node("-", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(mult &e)
{
  return new node("*", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(divt &e)
{
  return new node("/", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(powt &e)
{
  return new node("^", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(abst &e)
{
  return new node("abs", {new_to_old(e.get_value())});
}

node *new_to_old(sqrtt &e)
{
  return new node("sqrt", {new_to_old(e.get_value())});
}

node *new_to_old(expt &e)
{
  return new node("exp", {new_to_old(e.get_value())});
}

node *new_to_old(logt &e)
{
  return new node("log", {new_to_old(e.get_value())});
}

node *new_to_old(sint &e)
{
  return new node("sin", {new_to_old(e.get_value())});
}

node *new_to_old(cost &e)
{
  return new node("cos", {new_to_old(e.get_value())});
}

node *new_to_old(tant &e)
{
  return new node("tan", {new_to_old(e.get_value())});
}

node *new_to_old(asint &e)
{
  return new node("asin", {new_to_old(e.get_value())});
}

node *new_to_old(acost &e)
{
  return new node("acos", {new_to_old(e.get_value())});
}

node *new_to_old(atant &e)
{
  return new node("atan", {new_to_old(e.get_value())});
}
