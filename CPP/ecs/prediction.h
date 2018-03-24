#pragma once

#include <vector>
#include "cstdio"
#include <fstream>
#include "iostream"
#include "ARIMAModel.h"
#include <numeric>

void PredictAll(std::vector<std::vector<double> >& AlldataArray, int period, int numFla, int* predict);