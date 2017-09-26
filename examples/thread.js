self.importScripts('pow.js');

var ready = false;

Module['onRuntimeInitialized'] = function() {
    postMessage('ready');
}

onmessage = function(ev)
{
    //var PoW = Module.ccall("launchPoW", 'string', ['string']);
	let generate = Module.ccall("launchPoW", 'string', ['string'], ev.data);
	if (generate != "0000000000000000") {
	    console.log(generate +" found");
		postMessage(generate); // Worker return
	}
	else
	{
	    postMessage(false);
	}
}
