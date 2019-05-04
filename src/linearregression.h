
/******************************************************************************
 * Necessary includes
 ******************************************************************************/
#include <emscripten/emscripten.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/******************************************************************************
 * Expose C interface
 ******************************************************************************/
extern "C" {

    /**
     * @struct LinearModel
     * @description Contains a linear model in the format y = ax + b
     * */
    typedef struct LinearModel
    {
        float a;
        float b;

    } LinearModel;

    /*
    * @function linearRegression
    * @declaration
    * @EMSCRIPTEN_KEEPALIVE
    * @description Fits a linear model for two given series
    */
    EMSCRIPTEN_KEEPALIVE
    LinearModel* linearRegression(float* independent, float* dependent, int size);

    /*
    * @function arithmeticMean
    * @declaration
    * @description Takes the arithmetic mean of a given array
    */
    static float arithmeticMean(float* data, int size);

    /*
    * @function meanOfProducts
    * @declaration
    * @description Takes the mean of the product of two given arrays
    */
    static float meanOfProducts(float* data1, float* data2, int size);

    /*
    * @function variance
    * @declaration
    * @description Determines the variance of a given array
    */
    static float variance(float* data, int size);

    /*
    * @function linearRegression
    * @description Fits a linear model for two given series
    */
    LinearModel* linearRegression(float* independent, float* dependent, int size)
    {
        float independentMean = arithmeticMean(independent, size);
        float dependentMean = arithmeticMean(dependent, size);
        float productsMean = meanOfProducts(independent, dependent, size);
        float independentVariance = variance(independent, size);

        LinearModel* lr;

        lr->a = (productsMean - (independentMean * dependentMean) ) / independentVariance;

        lr->b = dependentMean - (lr->a * independentMean);

        return lr;
    }

    /*
    * @function arithmeticMean
    * @declaration
    * @description Takes the arithmetic mean of a given array
    */
    static float arithmeticMean(float* data, int size)
    {
        float total = 0;

        for(int i = 0; i < size; total += data[i], i++);

        return total / size;
    }

    /*
    * @function meanOfProducts
    * @description Takes the mean of the product of two given arrays
    */
    static float meanOfProducts(float* data1, float* data2, int size)
    {
        float total = 0;

        for(int i = 0; i < size; total += (data1[i] * data2[i]), i++);

        return total / size;
    }

    /*
    * @function variance
    * @declaration
    * @description Determines the variance of a given array
    */
    static float variance(float* data, int size)
    {
        float squares[size];

        for(int i = 0; i < size; i++)
        {
            squares[i] = pow(data[i], 2);
        }

        float meanOfSquares = arithmeticMean(squares, size);
        float mean = arithmeticMean(data, size);
        float squareOfMean = pow(mean, 2);
        float varianceResult = meanOfSquares - squareOfMean;

        return varianceResult;
    }    

    /*
    * @function residuals
    * @Declaration
    * @EMSCRIPTEN_KEEPALIVE
    * @description Calculates the residuals of a given linear model and buffer
    */
    EMSCRIPTEN_KEEPALIVE
    float* residuals(LinearModel* model, float* buffer, int size) 
    {
        float modelLine[size];
        float* result = (float*) malloc(size * sizeof(float));

        for (int index = 0; index < size; index++) {
            modelLine[index] = (model->a * index) + model->b;
        }

        for (int index = 0; index < size; index++) {
            float resid = buffer[index] - modelLine[index];
            result[index] = resid;
        }

        return result;
    }
}
