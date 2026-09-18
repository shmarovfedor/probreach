#include <memory>
#include <string>
#include <cstdio>

#include "frontend.h"
#include "pdrhparser.hpp"
#include "model.h"
#include "new_to_old.h"

extern FILE *yyin;
old::modelt old::global_model;

int frontendt::parse(const std::string &filename)
{
    FILE *input = fopen(filename.c_str(), "r");
    if(!input)
        return 1;

    yyin = input;
    yy::parser parser(*this);

    int result = parser.parse();

    if(result == 0 && root)
    {
      old::global_model = new_to_old(*root);
      old::global_model.finalise();
    }

    fclose(input);

    return result;
}
