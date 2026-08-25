//
// Created by fedor on 06/11/17.
//

#include "pdrh_config.h"
#include "smt2_generator.h"

using namespace std;
using namespace capd;
using namespace pdrh;

string smt2_generator::generate_flow_invt_check(
  mode *m,
  interval time,
  box init,
  vector<box> boxes)
{
  stringstream s;
  s.precision(16);

  // declaring the logic type
  s << "(set-logic QF_NRA_ODE)" << endl;

  // declaring functions and their bounds
  s << "; declaring functions and their bounds" << endl;
  for (auto it = m->flow_map.begin(); it != m->flow_map.end(); it++)
  {
    s << "(declare-fun " << it->first << " () Real)" << endl;
    s << "(declare-fun " << it->first << "_0_0 () Real)" << endl;
    s << "(declare-fun " << it->first << "_0_t () Real)" << endl;
    if (it->second.first->value != "-infty")
    {
      s << "(assert (>= " << it->first << "_0_0 "
        << it->second.first->to_prefix() << "))" << endl;
      s << "(assert (>= " << it->first << "_0_t "
        << it->second.first->to_prefix() << "))" << endl;
    }
    if (it->second.second->value != "infty")
    {
      s << "(assert (<= " << it->first << "_0_0 "
        << it->second.second->to_prefix() << "))" << endl;
      s << "(assert (<= " << it->first << "_0_t "
        << it->second.second->to_prefix() << "))" << endl;
    }
  }

  // assigning the values for the provided samples
  s << "; assigning the values of the sample" << endl;
  for (box b : boxes)
  {
    std::map<string, interval> b_map = b.get_map();
    for (auto it = b_map.begin(); it != b_map.end(); it++)
    {
      s << "(assert (>= " << it->first << "_0_0 " << it->second.leftBound()
        << "))" << endl;
      s << "(assert (>= " << it->first << "_0_t " << it->second.leftBound()
        << "))" << endl;
      s << "(assert (<= " << it->first << "_0_0 " << it->second.rightBound()
        << "))" << endl;
      s << "(assert (<= " << it->first << "_0_t " << it->second.rightBound()
        << "))" << endl;
    }
  }

  // declaring time
  s << "; declaring time variable and integration bounds" << endl;
  s << "(declare-fun time () Real)" << endl;
  s << "(assert (>= time 0.0))" << endl;
  s << "(assert (<= time " << time.rightBound() << "))" << endl;

  // declaring a variable mocking the time variable
  // not needed for the complement formula
  s << "; declaring a variable mocking the time variable" << endl;
  s << "(declare-fun time_mock () Real)" << endl;
  s << "(declare-fun time_mock_0_0 () Real)" << endl;
  s << "(declare-fun time_mock_0_t () Real)" << endl;

  // defining odes
  s << "; defining odes" << endl;
  s << "(define-ode flow_" << m->id << " (";
  for (auto it = m->odes.begin(); it != m->odes.end(); it++)
  {
    s << "(= d/dt[" << it->first << "] " << it->second->to_prefix()
      << ")" << endl;
  }

  // declaring an ode for time_mock variable
  // not needed for the complement formula
  s << "(= d/dt[time_mock] 1.0)" << endl;
  s << "))" << endl;

  // defining initial condition
  s << "; defining initial condition" << endl;
  std::map<string, interval> b_map = init.get_map();
  for (auto it = b_map.begin(); it != b_map.end(); it++)
  {
    s << "(assert (and (>= " << it->first << "_0_0 " << it->second.leftBound()
      << ") "
      << "(<= " << it->first << "_0_0 " << it->second.rightBound() << ")))"
      << endl;
  }

  // defining the integral
  s << "; defining the integral" << endl;
  s << "(assert (= [";
  for (auto it = m->odes.cbegin(); it != m->odes.cend(); it++)
  {
    s << it->first << "_0_t ";
  }
  s << "time_mock_0_t] (integral 0.0 time [";
  for (auto it = m->odes.cbegin(); it != m->odes.cend(); it++)
  {
    s << it->first << "_0_0 ";
  }
  s << "time_mock_0_0] flow_" << m->id << ")))" << endl;

  // defining invariants
  s << "; defining invariants" << endl;
  for (node *invt : m->invts)
  {
    s << "(assert (forall_t " << m->id << " [0.0 time] "
      << invt->to_prefix(0, "t") << "))" << endl;
  }

  // defining the goal for the time mock variable
  s << "; defining goal for the time mock variable" << endl;
  s << "(assert (= time_mock_0_0 0.0))" << endl;
  s << "(assert (= time_mock_0_t " << time.rightBound() << "))" << endl;

  s << "(check-sat)" << endl;
  s << "(exit)" << endl;

  return s.str();
}

string smt2_generator::generate_flow_invt_check_c(
  mode *m,
  interval time,
  box init,
  vector<box> boxes)
{
  stringstream s;
  s.precision(16);

  // declaring the logic type
  s << "(set-logic QF_NRA_ODE)" << endl;

  // declaring functions and their bounds
  s << "; declaring functions and their bounds" << endl;
  for (auto it = m->flow_map.begin(); it != m->flow_map.end(); it++)
  {
    s << "(declare-fun " << it->first << " () Real)" << endl;
    s << "(declare-fun " << it->first << "_0_0 () Real)" << endl;
    s << "(declare-fun " << it->first << "_0_t () Real)" << endl;
    if (it->second.first->value != "-infty")
    {
      s << "(assert (>= " << it->first << "_0_0 "
        << it->second.first->to_prefix() << "))" << endl;
      s << "(assert (>= " << it->first << "_0_t "
        << it->second.first->to_prefix() << "))" << endl;
    }
    if (it->second.second->value != "infty")
    {
      s << "(assert (<= " << it->first << "_0_0 "
        << it->second.second->to_prefix() << "))" << endl;
      s << "(assert (<= " << it->first << "_0_t "
        << it->second.second->to_prefix() << "))" << endl;
    }
  }

  // assigning the values for the provided samples
  s << "; assigning the values of the sample" << endl;
  for (box b : boxes)
  {
    std::map<string, interval> b_map = b.get_map();
    for (auto it = b_map.begin(); it != b_map.end(); it++)
    {
      s << "(assert (>= " << it->first << "_0_0 " << it->second.leftBound()
        << "))" << endl;
      s << "(assert (>= " << it->first << "_0_t " << it->second.leftBound()
        << "))" << endl;
      s << "(assert (<= " << it->first << "_0_0 " << it->second.rightBound()
        << "))" << endl;
      s << "(assert (<= " << it->first << "_0_t " << it->second.rightBound()
        << "))" << endl;
    }
  }

  // declaring time
  s << "; declaring time variable and integration bounds" << endl;
  s << "(declare-fun time () Real)" << endl;
  s << "(assert (>= time 0.0))" << endl;
  s << "(assert (<= time " << time.rightBound() << "))" << endl;

  // defining odes
  s << "; defining odes" << endl;
  s << "(define-ode flow_" << m->id << " (";
  for (auto it = m->odes.begin(); it != m->odes.end(); it++)
  {
    s << "(= d/dt[" << it->first << "] " << it->second->to_prefix()
      << ")" << endl;
  }
  s << "))" << endl;

  // defining initial condition
  s << "; defining initial condition" << endl;
  std::map<string, interval> b_map = init.get_map();
  for (auto it = b_map.begin(); it != b_map.end(); it++)
  {
    s << "(assert (and (>= " << it->first << "_0_0 " << it->second.leftBound()
      << ") "
      << "(<= " << it->first << "_0_0 " << it->second.rightBound() << ")))"
      << endl;
  }

  // defining the integral
  s << "; defining the integral" << endl;
  s << "(assert (= [";
  for (auto it = m->odes.cbegin(); it != m->odes.cend(); it++)
  {
    s << it->first << "_0_t ";
  }
  s << "] (integral 0.0 time [";
  for (auto it = m->odes.cbegin(); it != m->odes.cend(); it++)
  {
    s << it->first << "_0_0 ";
  }
  s << "] flow_" << m->id << ")))" << endl;

  // defining invariants negations
  s << "; defining invariants negations" << endl;
  s << "(assert (or" << endl;
  for (node *invt : m->invts)
  {
    s << "(not " << invt->to_prefix(0, "t") << ")" << endl;
  }
  s << "))" << endl;

  s << "(check-sat)" << endl;
  s << "(exit)" << endl;

  return s.str();
}

string smt2_generator::generate_jump_check(
  mode *m,
  vector<mode::jump> jumps,
  box init,
  vector<box> boxes)
{
  stringstream s;
  s.precision(16);

  // declaring the logic type
  s << "(set-logic QF_NRA_ODE)" << endl;

  // declaring functions and their bounds
  s << "; declaring functions and their bounds" << endl;
  for (auto it = m->flow_map.begin(); it != m->flow_map.end(); it++)
  {
    s << "(declare-fun " << it->first << " () Real)" << endl;
    s << "(declare-fun " << it->first << "_0_0 () Real)" << endl;
    s << "(declare-fun " << it->first << "_0_t () Real)" << endl;
    if (it->second.first->value != "-infty")
    {
      s << "(assert (>= " << it->first << "_0_0 "
        << it->second.first->to_prefix() << "))" << endl;
      s << "(assert (>= " << it->first << "_0_t "
        << it->second.first->to_prefix() << "))" << endl;
    }
    if (it->second.second->value != "infty")
    {
      s << "(assert (<= " << it->first << "_0_0 "
        << it->second.second->to_prefix() << "))" << endl;
      s << "(assert (<= " << it->first << "_0_t "
        << it->second.second->to_prefix() << "))" << endl;
    }
  }

  // assigning the values for the provided samples
  s << "; assigning the values of the sample" << endl;
  for (box b : boxes)
  {
    std::map<string, interval> b_map = b.get_map();
    for (auto it = b_map.begin(); it != b_map.end(); it++)
    {
      s << "(assert (>= " << it->first << "_0_0 " << it->second.leftBound()
        << "))" << endl;
      s << "(assert (>= " << it->first << "_0_t " << it->second.leftBound()
        << "))" << endl;
      s << "(assert (<= " << it->first << "_0_0 " << it->second.rightBound()
        << "))" << endl;
      s << "(assert (<= " << it->first << "_0_t " << it->second.rightBound()
        << "))" << endl;
    }
  }

  // declaring time
  s << "; declaring time variable and integration bounds" << endl;
  s << "(declare-fun time () Real)" << endl;
  s << "(assert (>= time " << m->time.first->to_prefix() << "))"
    << endl;
  s << "(assert (<= time " << m->time.second->to_prefix()
    << "))" << endl;

  // declaring a variable mocking the time variable
  // not needed for the complement formula
  s << "; declaring a variable mocking the time variable" << endl;
  s << "(declare-fun time_mock () Real)" << endl;
  s << "(declare-fun time_mock_0_0 () Real)" << endl;
  s << "(declare-fun time_mock_0_t () Real)" << endl;

  // defining odes
  s << "; defining odes" << endl;
  s << "(define-ode flow_" << m->id << " (";
  for (auto it = m->odes.begin(); it != m->odes.end(); it++)
  {
    s << "(= d/dt[" << it->first << "] " << it->second->to_prefix()
      << ")" << endl;
  }

  // declaring an ode for time_mock variable
  // not needed for the complement formula
  s << "(= d/dt[time_mock] 1.0)" << endl;
  s << "))" << endl;

  // defining initial condition
  s << "; defining initial condition" << endl;
  std::map<string, interval> b_map = init.get_map();
  for (auto it = b_map.begin(); it != b_map.end(); it++)
  {
    s << "(assert (and (>= " << it->first << "_0_0 " << it->second.leftBound()
      << ") "
      << "(<= " << it->first << "_0_0 " << it->second.rightBound() << ")))"
      << endl;
  }

  // defining the integral
  s << "; defining the integral" << endl;
  s << "(assert (= [";
  for (auto it = m->odes.cbegin(); it != m->odes.cend(); it++)
  {
    s << it->first << "_0_t ";
  }
  s << "time_mock_0_t] (integral 0.0 time [";
  for (auto it = m->odes.cbegin(); it != m->odes.cend(); it++)
  {
    s << it->first << "_0_0 ";
  }
  s << "time_mock_0_0] flow_" << m->id << ")))" << endl;

  // defining jumps
  s << "; defining jumps" << endl;
  s << "(assert (or " << endl;
  for (mode::jump jump : jumps)
  {
    //        if(jump.guard->operands.front()->value != "counter" && jump.guard->operands.back()->value != "counter")
    //        {
    s << jump.guard->to_prefix(0, "t") << endl;
    //        }
  }
  s << "))" << endl;

  // defining the goal for the time mock variable
  // mock time variable is not used in the check
  s << "; defining goal for the time mock variable" << endl;
  s << "(assert (= time_mock_0_0 0.0))" << endl;
  s << "(assert (>= time_mock_0_t 0.0))" << endl;

  s << "(check-sat)" << endl;
  s << "(exit)" << endl;

  return s.str();
}

// getting a string representation of reachability formula in smt2 format for all combinations of initial and goal modes
string
smt2_generator::reach_to_smt2(vector<pdrh::mode *> path, vector<box> boxes)
{
  stringstream s;
  // setting logic
  s << "(set-logic QF_NRA_ODE)" << endl;
  s << "\n; declaring variables and defining bounds\n";
  for (auto it = pdrh::var_map.cbegin(); it != pdrh::var_map.cend(); it++)
  {
    s << "(declare-fun " << it->first << " () Real)" << endl;
    for (int i = 0; i < path.size(); i++)
    {
      s << "(declare-fun " << it->first << "_" << i << "_0 () Real)" << endl;
      s << "(declare-fun " << it->first << "_" << i << "_t () Real)" << endl;
      if (it->second.first->value != "-infty")
      {
        s << "(assert (>= " << it->first << "_" << i << "_0 "
          << it->second.first->to_prefix() << "))" << endl;
        s << "(assert (>= " << it->first << "_" << i << "_t "
          << it->second.first->to_prefix() << "))" << endl;
      }
      if (it->second.second->value != "infty")
      {
        s << "(assert (<= " << it->first << "_" << i << "_0 "
          << it->second.second->to_prefix() << "))" << endl;
        s << "(assert (<= " << it->first << "_" << i << "_t "
          << it->second.second->to_prefix() << "))" << endl;
      }
    }
  }
  s << "\n; declaring TIME variables and bounds\n";
  for (int i = 0; i < path.size(); i++)
  {
    s << "(declare-fun time_" << i << " () Real)" << endl;
    s << "(assert (>= time_" << i << " "
      << path.at(i)->time.first->to_prefix(i, "0") << "))" << endl;
    s << "(assert (<= time_" << i << " "
      << path.at(i)->time.second->to_prefix(i, "0") << "))" << endl;
  }
  s << "\n; defining ODEs\n";
  for (auto path_it = path.cbegin(); path_it != path.cend(); path_it++)
  {
    if (find(path.cbegin(), path_it, *path_it) == path_it)
    {
      s << "(define-ode flow_" << (*path_it)->id << " (";
      for (auto ode_it = (*path_it)->odes.cbegin();
           ode_it != (*path_it)->odes.cend();
           ode_it++)
      {
        s << "(= d/dt[" << ode_it->first << "] "
          << ode_it->second->to_prefix() << ")";
      }
      s << "))" << endl;
    }
  }
  for (box b : boxes)
  {
    // skipping if there are any empty boxes
    if(b.empty())
      continue;
    s << "\n; defining parameter box: " << b << "\n";
    std::map<string, capd::interval> m = b.get_map();
    for (int i = 0; i < path.size(); i++)
    {
      s << "(assert (and \n";
      for (auto it = m.cbegin(); it != m.cend(); it++)
      {
        s << "\t(>= " << it->first << "_" << i << "_0 " << it->second.leftBound()
          << ")" << endl;
        s << "\t(<= " << it->first << "_" << i << "_0 " << it->second.rightBound()
          << ")" << endl;
        s << "\t(>= " << it->first << "_" << i << "_t " << it->second.leftBound()
          << ")" << endl;
        s << "\t(<= " << it->first << "_" << i << "_t " << it->second.rightBound()
          << ")" << endl;
      }
      s << "))\n";
    }
  }
  s << "\n; defining initial states\n";
  s << "(assert (or \n";
  for (pdrh::state st : pdrh::init)
  {
    if (st.id == path.front()->id)
    {
      s << "\t(" << st.prop->to_prefix(0, "0") << ")" << endl;
    }
  }
  s << "))" << endl;
  int step = 0;
  for (pdrh::mode *m : path)
  {
    s << "\n; step " << step << ", mode " << m->id << "\n";
    s << "; flow\n";
    s << "(assert (= [";
    for (auto ode_it = m->odes.cbegin(); ode_it != m->odes.cend(); ode_it++)
    {
      s << ode_it->first << "_" << step << "_t ";
    }
    s << "] (integral 0.0 time_" << step << " [";
    for (auto ode_it = m->odes.cbegin(); ode_it != m->odes.cend(); ode_it++)
    {
      s << ode_it->first << "_" << step << "_0 ";
    }
    s << "] flow_" << m->id << ")))\n";
    s << "; invariants\n";
    for (node *invt : m->invts)
    {
      s << "(assert (forall_t " << m->id << " [0.0 time_" << step << "] "
        << invt->to_prefix(step, "t") << "))" << endl;
    }
    // checking the current depth
    if (step < path.size() - 1)
    {
      // defining jumps
      for (pdrh::mode::jump j : m->jumps)
      {
        // only the jumps to the next mode in the path
        if (j.next_id == path.at(step + 1)->id)
        {
          s << "; jump from " << m->id << " to " << path.at(step + 1)->id << "\n";
          s << "; guard\n";
          s << "(assert (" << j.guard->to_prefix(step, "t") << "))\n";
          s << "; reset\n";
          s << "(assert (and ";
          for (auto reset_it = j.reset.cbegin(); reset_it != j.reset.cend();
               reset_it++)
          {
            s << "(= " << reset_it->first << "_" << step + 1 << "_0 "
              << reset_it->second->to_prefix(step, "t") << ")";
          }
          s << "))\n";
        }
      }
    }
    step++;
  }
  s << "\n; defining the GOAL\n";
  s << "(assert (or \n";
  for (pdrh::state st : pdrh::goal)
  {
    if (st.id == path.back()->id)
    {
      s << "\t(" << st.prop->to_prefix(path.size() - 1, "t") << ")\n";
    }
  }
  s << "))" << endl;
  // final statements
  s << "(check-sat)" << endl;
  s << "(exit)" << endl;
  return s.str();
}

string
smt2_generator::reach_c_to_smt2(vector<pdrh::mode *> path, vector<box> boxes)
{
  stringstream s;
  // setting logic
  s << "(set-logic QF_NRA_ODE)" << endl;
  s << "\n; declaring variables and defining bounds\n";
  for (auto it = pdrh::var_map.cbegin(); it != pdrh::var_map.cend(); it++)
  {
    s << "(declare-fun " << it->first << " () Real)" << endl;
    for (int i = 0; i < path.size(); i++)
    {
      s << "(declare-fun " << it->first << "_" << i << "_0 () Real)" << endl;
      s << "(declare-fun " << it->first << "_" << i << "_t () Real)" << endl;
      if (it->second.first->value != "-infty")
      {
        s << "(assert (>= " << it->first << "_" << i << "_0 "
          << it->second.first->to_prefix() << "))" << endl;
        s << "(assert (>= " << it->first << "_" << i << "_t "
          << it->second.first->to_prefix() << "))" << endl;
      }
      if (it->second.second->value != "infty")
      {
        s << "(assert (<= " << it->first << "_" << i << "_0 "
          << it->second.second->to_prefix() << "))" << endl;
        s << "(assert (<= " << it->first << "_" << i << "_t "
          << it->second.second->to_prefix() << "))" << endl;
      }
    }
  }
  s << "\n; declaring TIME variables\n";
  for (int i = 0; i < path.size(); i++)
  {
    s << "(declare-fun time_" << i << " () Real)" << endl;
    s << "(assert (>= time_" << i << " "
      << path.at(i)->time.first->to_prefix(i, "0") << "))" << endl;
    s << "(assert (<= time_" << i << " "
      << path.at(i)->time.second->to_prefix(i, "0") << "))" << endl;
  }
  s << "\n; declaring LOCAL TIME and bounds\n";
  s << "(declare-fun local_time () Real)" << endl;
  for (unsigned long i = 0; i < path.size() - 1; i++)
  {
    s << "(declare-fun local_time_" << i << "_0 () Real)" << endl;
    s << "(declare-fun local_time_" << i << "_t () Real)" << endl;
    s << "(assert (= local_time_" << i << "_0 "
      << path.at(i)->time.first->to_prefix(i, "0") << "))" << endl;
    s << "(assert (>= local_time_" << i << "_t "
      << path.at(i)->time.first->to_prefix(i, "0") << "))" << endl;
    s << "(assert (<= local_time_" << i << "_t "
      << path.at(i)->time.second->to_prefix(i, "0") << "))" << endl;
  }
  // last mode
  s << "(declare-fun local_time () Real)" << endl;
  s << "(declare-fun local_time_" << path.size() - 1 << "_0 () Real)" << endl;
  s << "(declare-fun local_time_" << path.size() - 1 << "_t () Real)" << endl;
  s << "(assert (= local_time_" << path.size() - 1 << "_0 "
    << path.back()->time.first->to_prefix(path.size() - 1, "0")
    << "))" << endl;
  s << "(assert (>= local_time_" << path.size() - 1 << "_t "
    << path.back()->time.first->to_prefix(path.size() - 1, "0")
    << "))" << endl;
  s << "(assert (<= local_time_" << path.size() - 1 << "_t "
    << path.back()->time.second->to_prefix(path.size() - 1, "0")
    << "))" << endl;
  s << "\n; defining ODEs\n";
  for (auto path_it = path.cbegin(); path_it != path.cend(); path_it++)
  {
    if (std::find(path.cbegin(), path_it, *path_it) == path_it)
    {
      s << "(define-ode flow_" << (*path_it)->id << " (";
      for (auto ode_it = (*path_it)->odes.cbegin();
           ode_it != (*path_it)->odes.cend();
           ode_it++)
      {
        s << "(= d/dt[" << ode_it->first << "] "
          << ode_it->second->to_prefix() << ")";
      }
      s << "(= d/dt[local_time] 1.0)";
      s << "))" << endl;
    }
  }
  s << "\n; defining the negated reachability formula\n";
  s << "(assert (and (and " << endl;
  // defining initial states
  s << "(or ";
  for (pdrh::state st : pdrh::init)
  {
    if (path.front()->id == st.id)
    {
      s << "(" << st.prop->to_prefix(0, "0") << ")";
    }
  }
  s << ")" << endl;
  // defining boxes bounds
  for (box b : boxes)
  {
    std::map<string, capd::interval> m = b.get_map();
    for (int i = 0; i < path.size(); i++)
    {
      for (auto it = m.cbegin(); it != m.cend(); it++)
      {
        s << "(>= " << it->first << "_" << i << "_0 " << it->second.leftBound()
          << ")" << endl;
        s << "(<= " << it->first << "_" << i << "_0 " << it->second.rightBound()
          << ")" << endl;
        s << "(>= " << it->first << "_" << i << "_t " << it->second.leftBound()
          << ")" << endl;
        s << "(<= " << it->first << "_" << i << "_t " << it->second.rightBound()
          << ")" << endl;
      }
    }
  }
  // defining trajectory
  int step = 0;
  for (pdrh::mode *m : path)
  {
    // defining integrals
    s << "(= [";
    for (auto ode_it = m->odes.cbegin(); ode_it != m->odes.cend(); ode_it++)
    {
      s << ode_it->first << "_" << step << "_t ";
    }
    // defining local time if enabled
    //if((step == path.size() - 1) && (!timed_node_neg))
    //if(!timed_node_neg)
    //{
    s << "local_time_" << step << "_t";
    //}
    s << "] (integral 0.0 time_" << step << " [";
    for (auto ode_it = m->odes.cbegin(); ode_it != m->odes.cend(); ode_it++)
    {
      s << ode_it->first << "_" << step << "_0 ";
    }
    // defining local time if enabled
    //if((step == path.size() - 1) && (!timed_node_neg))
    //if(!timed_node_neg)
    //{
    s << "local_time_" << step << "_0";
    //}
    s << "] flow_" << m->id << "))" << endl;
    // defining invariants
    for (node *invt : m->invts)
    {
      s << "(forall_t " << m->id << " [0.0 time_" << step << "] "
        << invt->to_prefix(step, "t") << ")" << endl;
    }
    // checking the current depth
    if (step < path.size() - 1)
    {
      // defining jumps
      for (pdrh::mode::jump j : m->jumps)
      {
        //cout << "Jumping from " << m->id << " to " << j.next_id << "(control " << path.at(step+1)->id << ")" << endl;
        if (j.next_id == path.at(step + 1)->id)
        {
          //cout << "Recording the jumping from " << m->id << " to " << j.next_id << "(control " << path.at(step+1)->id << ")" << endl;
          s << j.guard->to_prefix(step, "t") << endl;
          if (step < path.size() - 1)
          {
            for (auto reset_it = j.reset.cbegin(); reset_it != j.reset.cend();
                 reset_it++)
            {
              s << "(= " << reset_it->first << "_" << step + 1 << "_0 "
                << reset_it->second->to_prefix(step, "t") << ")";
            }
          }
        }
      }
    }
    step++;
  }
  s << ")";
  // defining goal
  s << "(and ";
  for (pdrh::state st : pdrh::goal)
  {
    if (path.back()->id == st.id)
    {
      //node* timed_node_neg = ap::get_time_node_neg(st.prop);
      // defining time vars
      vector<string> time_vars = global_config.time_var_name;
      time_vars.push_back(global_config.sample_time);
      node *timed_node_neg =
        pdrh::get_node_neg_by_value(st.prop, time_vars);
      if (!timed_node_neg)
      {
        // checking if there is a not in front of the guard predicate because dReal does not work nicely
        // with double negation
        s << "(= local_time_" << path.size() - 1 << "_t "
          << path.back()->time.second->to_prefix(path.size() - 1, "0")
          << ")" << endl;
        if (st.prop->value == "not")
        {
          s << "(forall_t " << st.id << " [0 time_" << path.size() - 1 << "] ("
            << st.prop->operands.front()->to_prefix(path.size() - 1, "t")
            << "))";
        }
        // transforming the negation of disjunction into the conjunction of negations
        else if (st.prop->value == "or")
        {
          for (node *n : st.prop->operands)
          {
            s << "(forall_t " << st.id << " [0 time_" << path.size() - 1
              << "] (not " << n->to_prefix(path.size() - 1, "t")
              << "))";
          }
        }
        else
        {
          s << "(forall_t " << st.id << " [0 time_" << path.size() - 1
            << "] (not " << st.prop->to_prefix(path.size() - 1, "t")
            << "))";
        }
      }
      else
      {
        s << timed_node_neg->to_prefix(path.size() - 1, "t");
        delete timed_node_neg;
      }
    }
  }
  s << ")))" << endl;
  // final statements
  s << "(check-sat)" << endl;
  s << "(exit)" << endl;
  return s.str();
}

string smt2_generator::reach_c_to_smt2(
  int depth,
  vector<pdrh::mode *> path,
  vector<box> boxes)
{
  if (depth == path.size() - 1)
  {
    return smt2_generator::reach_c_to_smt2(path, boxes);
  }
  else
  {
    stringstream s;
    // setting logic
    s << "(set-logic QF_NRA_ODE)" << endl;
    // declaring variables and defining bounds
    for (auto it = pdrh::var_map.cbegin(); it != pdrh::var_map.cend(); it++)
    {
      s << "(declare-fun " << it->first << " () Real)" << endl;
      for (int i = 0; i <= depth; i++)
      {
        s << "(declare-fun " << it->first << "_" << i << "_0 () Real)" << endl;
        s << "(declare-fun " << it->first << "_" << i << "_t () Real)" << endl;
        if (it->second.first->value != "-infty")
        {
          s << "(assert (>= " << it->first << "_" << i << "_0 "
            << it->second.first->to_prefix() << "))" << endl;
          s << "(assert (>= " << it->first << "_" << i << "_t "
            << it->second.first->to_prefix() << "))" << endl;
        }
        if (it->second.second->value != "infty")
        {
          s << "(assert (<= " << it->first << "_" << i << "_0 "
            << it->second.second->to_prefix() << "))" << endl;
          s << "(assert (<= " << it->first << "_" << i << "_t "
            << it->second.second->to_prefix() << "))" << endl;
        }
      }
    }
    // declaring time
    for (int i = 0; i <= depth; i++)
    {
      s << "(declare-fun time_" << i << " () Real)" << endl;
      s << "(assert (>= time_" << i << " "
        << path.at(i)->time.first->to_prefix(i, "0") << "))" << endl;
      s << "(assert (<= time_" << i << " "
        << path.at(i)->time.second->to_prefix(i, "0") << "))"
        << endl;
    }
    // declaring local time and bounds
    //        if (!timed_node_neg)
    //        {
    s << "(declare-fun local_time () Real)" << endl;
    //cout << "HERE" << endl;
    for (unsigned long i = 0; i < depth; i++)
    {
      s << "(declare-fun local_time_" << i << "_0 () Real)" << endl;
      s << "(declare-fun local_time_" << i << "_t () Real)" << endl;
      s << "(assert (= local_time_" << i << "_0 "
        << path.at(i)->time.first->to_prefix(i, "0") << "))" << endl;
      s << "(assert (>= local_time_" << i << "_t "
        << path.at(i)->time.first->to_prefix(i, "0") << "))" << endl;
      s << "(assert (<= local_time_" << i << "_t "
        << path.at(i)->time.second->to_prefix(i, "0") << "))"
        << endl;
    }
    // last mode
    //s << "(declare-fun local_time () Real)" << endl;
    s << "(declare-fun local_time_" << depth << "_0 () Real)" << endl;
    s << "(declare-fun local_time_" << depth << "_t () Real)" << endl;
    s << "(assert (= local_time_" << depth << "_0 "
      << path.at(depth)->time.first->to_prefix(depth, "0") << "))"
      << endl;
    s << "(assert (>= local_time_" << depth << "_t "
      << path.at(depth)->time.first->to_prefix(depth, "0") << "))"
      << endl;
    s << "(assert (<= local_time_" << depth << "_t "
      << path.at(depth)->time.second->to_prefix(depth, "0") << "))"
      << endl;
    //        }
    // defining odes
    int step = 0;
    for (auto path_it = path.cbegin(); path_it != path.cend(); path_it++)
    {
      if (std::find(path.cbegin(), path_it, *path_it) == path_it)
      {
        s << "(define-ode flow_" << (*path_it)->id << " (";
        for (auto ode_it = (*path_it)->odes.cbegin();
             ode_it != (*path_it)->odes.cend();
             ode_it++)
        {
          s << "(= d/dt[" << ode_it->first << "] "
            << ode_it->second->to_prefix() << ")";
        }
        // introducing local time if defined
        //if((!timed_node_neg))
        //{
        s << "(= d/dt[local_time] 1.0)";
        //}
        s << "))" << endl;
      }
      if (step >= depth)
      {
        break;
      }
      step++;
    }
    // defining the negated reachability formula
    s << "(assert (and (and " << endl;
    // defining initial states
    s << "(or ";
    for (pdrh::state st : pdrh::init)
    {
      if (path.front()->id == st.id)
      {
        s << "(" << st.prop->to_prefix(0, "0") << ")";
      }
    }
    s << ")" << endl;
    // defining boxes bounds
    for (box b : boxes)
    {
      std::map<string, capd::interval> m = b.get_map();
      for (int i = 0; i <= depth; i++)
      {
        for (auto it = m.cbegin(); it != m.cend(); it++)
        {
          s << "(>= " << it->first << "_" << i << "_0 "
            << it->second.leftBound() << ")" << endl;
          s << "(<= " << it->first << "_" << i << "_0 "
            << it->second.rightBound() << ")" << endl;
          s << "(>= " << it->first << "_" << i << "_t "
            << it->second.leftBound() << ")" << endl;
          s << "(<= " << it->first << "_" << i << "_t "
            << it->second.rightBound() << ")" << endl;
        }
      }
    }
    // defining trajectory
    for (int i = 0; i <= depth; i++)
    {
      pdrh::mode *m = path.at(i);
      // defining integrals
      s << "(= [";
      for (auto ode_it = m->odes.cbegin(); ode_it != m->odes.cend(); ode_it++)
      {
        s << ode_it->first << "_" << i << "_t ";
      }
      // defining local time if enabled
      //if(!timed_node_neg)
      //{
      s << "local_time_" << i << "_t";
      //}
      s << "] (integral 0.0 time_" << i << " [";
      for (auto ode_it = m->odes.cbegin(); ode_it != m->odes.cend(); ode_it++)
      {
        s << ode_it->first << "_" << i << "_0 ";
      }
      // defining local time if enabled
      //if(!timed_node_neg)
      //{
      s << "local_time_" << i << "_0";
      //}
      s << "] flow_" << m->id << "))" << endl;
      // defining invariants
      for (node *invt : m->invts)
      {
        s << "(forall_t " << m->id << " [0.0 time_" << i << "] "
          << invt->to_prefix(i, "t") << ")" << endl;
      }
      // checking the current depth
      if (i < depth)
      {
        // defining jumps
        for (pdrh::mode::jump j : m->jumps)
        {
          // getting only the jumps leading to the next mode in the path
          if (j.next_id == path.at(i + 1)->id)
          {
            s << j.guard->to_prefix(i, "t") << endl;
            if (i < path.size() - 1)
            {
              for (auto reset_it = j.reset.cbegin(); reset_it != j.reset.cend();
                   reset_it++)
              {
                s << "(= " << reset_it->first << "_" << i + 1 << "_0 "
                  << reset_it->second->to_prefix(i, "t") << ")";
              }
            }
          }
        }
      }
    }
    s << ")" << endl;
    // defining the last jump
    s << "(and ";
    for (pdrh::mode::jump j : path.at(depth)->jumps)
    {
      if (j.next_id == path.at(depth + 1)->id)
      {
        //node* timed_node_neg = ap::get_time_node_neg(j.guard);
        // extracting time variables
        vector<string> time_vars = global_config.time_var_name;
        time_vars.push_back(global_config.sample_time);
        node *timed_node_neg =
          pdrh::get_node_neg_by_value(j.guard, time_vars);
        if (!timed_node_neg)
        {
          // checking if there is a not in front of the guard predicate because dReal does not work nicely
          // with double negation
          s << "(= local_time_" << depth << "_t "
            << path.at(depth)->time.second->to_prefix(depth, "0")
            << ")" << endl;
          if (j.guard->value == "not")
          {
            /*
                        node *guard_without_negation = new node();
                        pdrh::copy_tree(guard_without_negation, j.guard->operands.front());
                        s << "(forall_t " << path.at(depth)->id << " [0 time_" << depth << "] (" <<
                        pdrh::node_fix_index(guard_without_negation, depth, "t") << "))";
                        delete guard_without_negation;
                        */
            s << "(forall_t " << path.at(depth)->id << " [0 time_" << depth
              << "] ("
              << j.guard->operands.front()->to_prefix(depth, "t")
              << "))";
          }
          // transforming the negation of disjunction into the conjunction of negations
          else if (j.guard->value == "or")
          {
            for (node *n : j.guard->operands)
            {
              s << "(forall_t " << path.at(depth)->id << " [0 time_" << depth
                << "] (not " << n->to_prefix(depth, "t") << "))";
            }
          }
          else
          {
            s << "(forall_t " << path.at(depth)->id << " [0 time_" << depth
              << "] (not " << j.guard->to_prefix(depth, "t") << "))";
          }
        }
        else
        {
          s << timed_node_neg->to_prefix(depth, "t");
          delete timed_node_neg;
        }
      }
    }
    s << ")))" << endl;
    // asserting the time point for the last mode
    // final statements
    s << "(check-sat)" << endl;
    s << "(exit)" << endl;
    return s.str();
  }
}
