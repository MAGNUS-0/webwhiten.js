
/******************************************************************************
 * Necessary includes
 ******************************************************************************/
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include <emscripten/emscripten.h>
#include "linearregression.h"
#include "./ar/ar.hpp"

/******************************************************************************
 * Define for shorthand exporting via the extern "C" interface
 ******************************************************************************/
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

 /*
* @function burgAutoregression
* @declaration
* @EXTERNC
* @description Estimates the AR series using the Burg method
*/
EXTERNC float* burgAutoregression (float* buffer, int bufferSize);

/*
* @function burgAutoregression
* @EXTERNC
* @description Estimates the AR series using the Burg method
*/
float* burgAutoregression (float* buffer, int bufferSize) {

    size_t maxorder = 750;

    std::vector<float> data {buffer, buffer + bufferSize};

    float mean;
    std::vector<float> params, sigma2e, gain, autocor;
    ar::burg_method(data.begin(), data.end(), mean, maxorder,
                    back_inserter(params), back_inserter(sigma2e),
                    back_inserter(gain), back_inserter(autocor),
                    /* subtract mean?    */ true,
                    /* output hierarchy? */ true);

    auto result = &params[0];
    return result;
}

/******************************************************************************
 * Directly Expose C interface (allow multiple functions if needed)
 ******************************************************************************/
extern "C" {

    /*
    * @function prewhiten
    * @declaration
    * @EXTERNC
    * @description Returns the AR estimate of a time series for use in prewhitening filtering
    */
    EMSCRIPTEN_KEEPALIVE
    float* prewhiten (float* buffer, int bufferSize) {

        float removeTrend[bufferSize];

        for (int index = 0; index < bufferSize; index++){
            removeTrend[index] = (float) index;
        }
        
        // Remove linear trend
        LinearModel* result = linearRegression(buffer, removeTrend, bufferSize);
        float* resid = residuals(result, buffer, bufferSize);

        // Determine AR series
        float* output = burgAutoregression(resid, bufferSize);

        // Setup output return value
        auto arrayPtr = &output[0];
        return arrayPtr;
    }
}
