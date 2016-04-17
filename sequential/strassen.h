#pragma once

#include <iostream>
#include <cstdint>
#include "matrix.h"

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

#define STRASS_SIZE_LIMIT 2

CMatrix strassenMult(const CMatrix & left, const CMatrix & right);
CMatrix strassenRec(const CMatrix & left, const CMatrix & right, uint64_t depth, uint64_t id);
