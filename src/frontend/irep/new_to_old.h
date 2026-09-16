#include "node.h"
#include "model.h"
#include "irep.h"


node *new_to_old(real_exprt &e);
node *new_to_old(symbolt &e);
node *new_to_old(numbert &e);
node *new_to_old(minust &e);
node *new_to_old(plust &e);
node *new_to_old(addt &e);
node *new_to_old(subt &e);
node *new_to_old(mult &e);
node *new_to_old(divt &e);
node *new_to_old(powt &e);
node *new_to_old(abst &e);
node *new_to_old(sqrtt &e);
node *new_to_old(expt &e);
node *new_to_old(logt &e);
node *new_to_old(sint &e);
node *new_to_old(cost &e);
node *new_to_old(tant &e);
node *new_to_old(asint &e);
node *new_to_old(acost &e);
node *new_to_old(atant &e);
