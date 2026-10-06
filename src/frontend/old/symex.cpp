//
// Created by fedor on 06/10/26.
//

#include <sstream>
#include <algorithm>
#include <set>
#include <iostream>

#include "model.h"
#include "symex.h"

using namespace std;

// getting successors of the mode m
vector<old::modet *> old::symext::get_successors(old::modet *m)
{
  vector<old::modet *> res;
  for (old::jumpt j : m->jumps)
  {
    old::modet *tmp = model.get_mode(j.next_id);
    if (tmp != NULL)
    {
      res.push_back(tmp);
    }
    else
    {
      stringstream s;
      s << "mode \"" << j.next_id
        << "\" is not defined but appears in the jump: " << j.guard << " ==>  @"
        << j.next_id << endl;
      throw invalid_argument(s.str());
    }
  }
  return res;
}

// getting all paths of length path_length between begin and end modes
vector<vector<old::modet *>>
old::symext::get_paths(old::modet *begin, old::modet *end, int path_length)
{
  // initializing the set of paths
  vector<std::vector<old::modet *>> paths;
  vector<old::modet *> path;
  path.push_back(begin);
  // initializing the stack
  vector<vector<old::modet *>> stack;
  stack.push_back(path);
  while (!stack.empty())
  {
    // getting the first paths from the set of paths
    path = stack.front();
    stack.erase(stack.cbegin());
    // checking if the correct path of the required length is found
    if ((path.back() == end) && (path.size() == path_length + 1))
    {
      paths.push_back(path);
    }
    // proceeding only if the length of the current path is ascending then the required length
    else if (path.size() < path_length + 1)
    {
      // getting the last mode in the path
      old::modet *cur_mode = path.back();
      // getting the successors of the mode
      vector<old::modet *> successors = get_successors(cur_mode);
      for (old::modet *suc_mode : successors)
      {
        // appending the successor the current paths
        vector<old::modet *> new_path = path;
        new_path.push_back(suc_mode);
        // pushing the new path to the set of the paths
        stack.push_back(new_path);
      }
    }
  }
  return paths;
}

// getting all paths of length path_length for
// all combinations of init and goal modes
vector<vector<old::modet *>> old::symext::get_all_paths(int path_length)
{
  vector<vector<old::modet *>> res;
  for (old::statet i : model.init)
  {
    for (old::statet g : model.goal)
    {
      vector<vector<old::modet *>> paths = get_paths(
        model.get_mode(i.id), model.get_mode(g.id), path_length);
      res.insert(res.end(), paths.begin(), paths.end());
    }
  }
  return res;
}

vector<vector<old::modet *>> old::symext::get_all_paths(int min_depth, int max_depth)
{
  vector<vector<old::modet *>> res;
  for (int i = min_depth; i <= max_depth; i++)
  {
    vector<vector<old::modet *>> paths = get_all_paths(i);
    res.insert(res.end(), paths.begin(), paths.end());
  }
  return res;
}


