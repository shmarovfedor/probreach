#include <memory>
#include <string>
#include <cstdio>
#include <iostream>

#include "frontend.h"
#include "pdrhparser.hpp"
#include "model.h"
#include "new_to_old.h"
#include "symbol_table.h"
#include "build_symbol_table.h"
#include "complete_model.h"

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
      symbol_tablet sym_table;
      build_symbol_tablet builder;
      builder.build(sym_table, root.get());
      complete_modelt completer;
      completer.run(sym_table, *root);

      old::global_model = new_to_old(*root, sym_table);
    }

    fclose(input);

    return result;
}
