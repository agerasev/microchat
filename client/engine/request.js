
function createRequest()
{
	var request = false;
	if (window.XMLHttpRequest){
		// Gecko-* browsers, Safari, Konqueror
		request = new XMLHttpRequest();
	}
	else if (window.ActiveXObject){
		// Internet explorer
		try{
			request = new ActiveXObject("Microsoft.XMLHTTP");
		}
		catch(exception){
			request = new ActiveXObject("Msxml2.XMLHTTP");
		}
	}
	if (!request){
		alert("Unable to create XMLHttpRequest");
	}
	return request;
}

function sendRequest(path, args, handler){
	// Create request
	var request = createRequest();
	// Check existance
	if (!request){
		return;
	}
	// Set handler
	request.onreadystatechange = function()
	{
		// If data transmission ended
		if (request.readyState == 4){
			if (request.status == 200){
				// Give control to handler
				handler(request);
			}
			else{
				alert('Error data loading. Check your internet connection.');
			}
		}
		else{
			// Loading
		}
	}
	// Initialize connection
	request.open('post', path, true);
	// Set header
	request.setRequestHeader("Content-Type","application/x-www-form-urlencoded; charset=utf-8");
	// Send request
	request.send(args);
}
