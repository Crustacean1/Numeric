#include "ExpEngine.h"

using namespace KCrypt;

ExpEngine::ExpEngine(CompEngine &cmp, AddEngine &add, MulEngine &mul,
                     DivEngine &div)
    : _cmp(cmp), _add(add), _mul(mul), _div(div) {}
