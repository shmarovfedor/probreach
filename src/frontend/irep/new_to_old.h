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

node *new_to_old(bool_exprt &e);
node *new_to_old(truet &e);
node *new_to_old(falset &e);
node *new_to_old(greater_thant &e);
node *new_to_old(greater_equalt &e);
node *new_to_old(less_thant &e);
node *new_to_old(less_equalt &e);
node *new_to_old(equalt &e);
node *new_to_old(not_equalt &e);
node *new_to_old(nott &e);
node *new_to_old(implyt &e);
node *new_to_old(andt &e);
node *new_to_old(ort &e);
node *new_to_old(xort &e);

std::pair<node *, node *> new_to_old(intervalt &e);
std::vector<node *> new_to_old(std::vector<std::unique_ptr<invtt>> &e);

node *new_to_old(rvaluet &e);
std::pair<std::string, node *> new_to_old(assignt &e);
std::map<std::string, node *>
new_to_old(std::vector<std::unique_ptr<assignt>> &e);
std::pair<std::string, std::map<std::string, node *>>
new_to_old(reset_statet &e);
old::statet new_to_old(cond_statet &e);
std::vector<old::statet>
new_to_old(std::map<std::unique_ptr<symbolt>, std::unique_ptr<cond_statet>> &e);
std::map<std::string, node *> new_to_old(flowt &e);
old::jumpt new_to_old(jumpt &e);
std::vector<old::jumpt>
new_to_old(std::map<std::unique_ptr<symbolt>, std::unique_ptr<jumpt>> &e);
old::modet new_to_old(modet &e);
std::vector<old::modet>
new_to_old(std::map<std::unique_ptr<symbolt>, std::unique_ptr<modet>> &e);

node *new_to_old(distt &e);
node *new_to_old(discrete_distt &e);
node *new_to_old(normal_distt &e);
node *new_to_old(uniform_distt &e);
node *new_to_old(exp_distt &e);

old::declarationt *new_to_old(declt &e);
old::declarationt *new_to_old(const_declt &e);
old::declarationt *new_to_old(var_declt &e);
old::declarationt *new_to_old(dist_declt &e);
std::map<std::string, old::declarationt> new_to_old(std::map<std::unique_ptr<symbolt>, std::unique_ptr<declt>> &e);

old::modelt new_to_old(modelt &e);


