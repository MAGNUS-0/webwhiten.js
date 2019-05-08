# webwhiten.js
Time series prewhitening on web enabled devices.

# What is webwhiten.js?

`webwhiten.js` is a time series prewhitening based around a Burg estimation of the the autoregressive model. This can be used for time series prewhitening, and was designed in mind for use with cross-correlation. It uses a combination of WebAssembly & WebAudio to implement the procedure.

Credit goes to [ar](https://github.com/RhysU/ar) for the easily accessible autoregressive utility functions in C++ header files, as well as [wasm-arrays](https://github.com/DanRuta/wasm-arrays) for WebAssembly array manipulation. 

# Getting Started

Currently, the process of including `webwhiten.js` requires adding three files to your project. These can be incorporated into your build system, but for a simple example with an `index.html` file:

```
<script src="../dist/prewhiten.js"></script>
<script src="../dist/wasm-arrays.js"></script>
<script src="../dist/webwhiten.js"></script>
```

On including these files, the webwhiten function will then appear accessible on the window. This can then be simply called using the following syntax, in an `async` `function` as:

```javascript

// buffer0: Float32Array
// buffer1: Float32Array

let result = await webwhiten(buffer0, buffer1);

```
Or in a promised based callback:

```javascript

// buffer0: Float32Array
// buffer1: Float32Array

webwhiten(buffer0, buffer1).then(result => {
  // Do something with the result...
  console.log(result);
});

```

Both of which will return an object formatted as so:

```javascript

{
  buffer0: [] // A singular Float32Array (prewhitened buffer0)
  buffer1: [] // A singular Float32Array (prewhitened buffer1)
}

```

# Author

Magnus Woodgate
