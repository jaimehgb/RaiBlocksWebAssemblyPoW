# RaiBlocksWebAssemblyPoW

<h3>Overview</h3>
<p>
This repo contains a simple RaiBlocks PoW implementation compiled to WebAssembly to boost its performance on browsers. That's been done using
emscripten.
Compiling to WebAssembly, the result is around 10 times faster than a pure JS PoW implementation (e.g.: <a href="https://github.com/SergiySW/RaiBlocksJS/blob/master/rai.pow.js" target="_blank">RaiBlocksJS</a>).

<br/>
This basically makes possible to generate proofs of work on modern browsers in a reasonable time. So yep, lets see...


</p>

<hr/>

<h3>Installation and usage</h3>
<p>
All the PoW thing takes place at <code>functions.cpp</code>. There is the main loop which calculates stuff and a function which 
can be called from JS and runs the loop (the iterations function). <br/>

To compile it to webassembly you need to install emscripten:
```bash
git clone https://github.com/juj/emsdk.git
cd emsdk

# on Linux or Mac OS X
./emsdk install --build=Release sdk-incoming-64bit binaryen-master-64bit
./emsdk activate --global --build=Release sdk-incoming-64bit binaryen-master-64bit

# on Windows
emsdk install --build=Release sdk-incoming-64bit binaryen-master-64bit
emsdk activate --global --build=Release sdk-incoming-64bit binaryen-master-64bit
```

Then:

```bash
# on Linux or Mac OS X
source ./emsdk_env.sh

# on Windows
emsdk_env.bat
```

(From <a href="https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm" target="_blank">here</a>) <br/>

With that done, at the repo folder run this:<br/>
```bash
emcc functions.cpp blake2/blake2b-ref.cpp  -o pow.js -s WASM=1 -std=gnu++11 -O3 -s EXPORTED_FUNCTIONS="['_launchPoW']"
```

It will output 2 files: <code>pow.js</code> and <code>pow.wasm</code>, place those files together somewhere and include <code>pow.js</code>
in your html as usual.<br/>

To call the "launchPoW" function you can do 2 things:


```javascript
var hash = "SOME_RAIBLOCKS_HASH_HERE_AND_STUFF_HEX_ENCODED_32_BYTES";
var pow = Module.ccall("launchPoW", "string", ["string"], hash);
console.log(pow);
```


OR

```javascript
var hash = "SOME_RAIBLOCKS_HASH_HERE_AND_STUFF_HEX_ENCODED_32_BYTES";
var calculatePoW = Module.cwrap("launchPoW", "string", ["string"]);
var pow = calculatePoW(hash);
console.log(pow);
```

I prefer the second option :D <br/>

What that function does is to try to find a valid PoW in 10,000,000 iterations. If it finds it it will return the result as a hex string. 
If it does not find it, will return a 64bit hex 0 (<code>0000000000000000</code>). Keep that in mind :P
<br/><br/>
<strong>BUT</strong> ... but. You cannot call the function before WebAssembly is loaded and compiled. How do we know when that happens?
Well, we have:
```javascript
Module['onRuntimeInitialized'] = function() {
    // Its all fine here
    // do stuff
    // ...
}
```
If <code>Module['onRuntimeInitialized']</code> is a function it will be called once its all ready to run. So yeah, be aware of that :)
<br/>
There are more docs about the Module API and emscripten itself here <a href="http://kripken.github.io/emscripten-site/docs/porting/connecting_cpp_and_javascript/index.html" target="_blank">http://kripken.github.io/emscripten-site/docs/porting/connecting_cpp_and_javascript/index.html</a>.

</p>

<hr/>

<h3>Compatibility</h3>
<p>
This implementation has just been tested in Chrome (Windows 64bit), Firefox (Windows 64bit) and Chrome (Android) but should also work in 
all the <a href="https://developer.mozilla.org/en-US/docs/WebAssembly#Browser_compatibility" target="_blank">devices supporting WASM</a>.
</p>
<hr/>
<h3>Examples</h3>
<p>

I've made two examples for you to see/use/improve/whatever. They are at the /examples folder.
One of them uses webworkers to multithread this stuff. The other one is single threaded. 
<br/>
Yeah, take a look there :P

</p>
