#pragma once

#include <iostream>
#include <omp.h>
#include "matrix.h"

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

//Below this limit standard matrix multiplication is used.
#define STRASS_LIMIT 256
//Limit under which no more tasks are created.
#define STRASS_PAR_LIMIT 512

CMatrix strassenMult(const CMatrix & left, const CMatrix & right);
CMatrix strassenRec(const CMatrix & left, const CMatrix & right);
