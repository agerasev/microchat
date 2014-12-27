var okAlert = null;
var alertCallback = null;
function showAlert(message,callback)
{
	alertCallback = callback;
	okAlert = function()
	{
		document.getElementById("alert").style.display = 'none';
		document.getElementById("alert-cover").style.display = 'none';
		if(alertCallback != undefined)
		{
			alertCallback();
		}
	};
	document.getElementById("alert-message").innerHTML = message;
	document.getElementById("alert-cover").style.display = 'inline-block';
	document.getElementById("alert").style.display = 'inline-block';
}

var submitPrompt = null;
var backPrompt = null;
var promptCallback = null;
function showPrompt(message,callback)
{
	promptCallback = callback;
	backPrompt = function()
	{
		document.getElementById("prompt").style.display = 'none';
		document.getElementById("prompt-cover").style.display = 'none';
	};
	submitPrompt = function()
	{
		if(promptCallback != undefined)
		{
			if(promptCallback(document.getElementById("prompt-input").value) == false)
			{
				return;
			}
		}
		document.getElementById("prompt").style.display = 'none';
		document.getElementById("prompt-cover").style.display = 'none';
	};
	document.getElementById("prompt-message").innerHTML = message;
	document.getElementById("prompt-cover").style.display = 'inline-block';
	document.getElementById("prompt").style.display = 'inline-block';
}
