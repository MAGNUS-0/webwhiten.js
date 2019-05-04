/**
 * @function webwhiten
 * @async
 * @param {Float32Array} buffer0 : Buffer to be whitened (and the filter we be determined from)
 * @param {Float32Array} buffer1 : Secondary buffer to undergo same whitening process
 */
async function webwhiten (buffer0, buffer1) {
    
    // Declare whitening wasm wrapper
    let whitenWrapper = function prewhitenSignal (buffer) {
        let wBuffer = ccallArrays("prewhiten", "array", ["array"], [buffer], {returnArraySize: buffer.length})
        return new Float32Array(wBuffer);
    }

    // Obtain whitening filter from buffer0
    let whiteningCoefficients = whitenWrapper(buffer0);

    // Inverse ar series
    whiteningCoefficients = whiteningCoefficients.map(value => -value);

    // Declare convolution filtering function
    let FilterArSeries = async function (coefficients, buffer) {

        let ctx = new OfflineAudioContext(1, buffer.length, 44100);
        let convNode = ctx.createConvolver();
        let filtBuffer = ctx.createBuffer(1, coefficients.length, 44100);
        let bufferBuffer = ctx.createBuffer(1, buffer.length, 44100);
        let buffSource = ctx.createBufferSource();

        filtBuffer.copyToChannel(coefficients, 0);
        bufferBuffer.copyToChannel(buffer, 0);

        convNode.buffer = filtBuffer;
        buffSource.buffer = bufferBuffer; 

        buffSource.connect(convNode);
        convNode.connect(ctx.destination);

        buffSource.start();

        let result = await ctx.startRendering();
        return result;
    }

    // Convolve whitening filter with buffer0
    let buffer0White = await FilterArSeries(buffer0, whiteningCoefficients);

    // Convolve whitening filter with buffer1
    let buffer1White = await FilterArSeries(buffer1, whiteningCoefficients);

    // Return the buffers from each AudioBuffer
    return {
        buffer0: buffer0White.getChannelData(0),
        buffer1: buffer1White.getChannelData(0),
    }
}
