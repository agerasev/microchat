
function initMessagePage()
{
	currentMessagePage = createMessagePage("message-page");
	
	currentMessagePage.enable = function ()
	{
		this.element.style.display = "inline-block";
		update_placeholders();
		currentMessagePage.lastTime = null;
		currentMessagePage.scroll = true;
		this.update();
		stopUpdating();
		startUpdating(1000);
	};
	
	currentMessagePage.disable = function ()
	{
		this.element.style.display = "none";
	};
	
	currentMessagePage.update = function ()
	{
		if(currentUser == null)
		{
			return;
		}
		
		if(currentMessagePage.scroll == null)
		{
			this.scroll = (Math.max(document.body.scrollTop,document.documentElement.scrollTop) + window.innerHeight) >= document.body.scrollHeight;
		}
		
		if(this.lastTime == null)
		{
			sendRequest(
				document.URL + "/request",
				"getAllMessagesWithAuthor(" + currentMessagePage.convId + ")",
				function(req)
				{
					var array = convertToArray(req.responseText);
					document.getElementById("messages-history").innerHTML = formMessages(array);
					
					var last_time = getLastTime(array);
					if(last_time != null)
					{
						currentMessagePage.lastTime = last_time;
					}
					else
					{
						currentMessagePage.lastTime = '0000-00-00 00:00:00'
					}
					
					if(currentMessagePage.scroll == true)
					{
						window.scrollTo(0,document.body.scrollHeight);
					}
					currentMessagePage.scroll = null;
				}
			);
		}
		else
		{
			sendRequest(
				document.URL + "/request",
				"getNewMessagesWithAuthor(" + currentMessagePage.convId + "," + currentMessagePage.lastTime + ")",
				function(req)
				{
					var array = convertToArray(req.responseText);
					document.getElementById("messages-history").innerHTML += formMessages(array);
					
					var last_time = getLastTime(array);
					if(last_time != null)
					{
						currentMessagePage.lastTime = last_time;
					}
					
					if(currentMessagePage.scroll == true)
					{
						window.scrollTo(0,document.body.scrollHeight);
					}
					currentMessagePage.scroll = null;
				}
			);
		}
	};
	
	currentMessagePage.select = function(convId)
	{
		this.convId = convId;
	}
	
	currentMessagePage.submitText = function()
	{
		var textarea = document.getElementById("input-text-area");
		if(textarea.value.length > 0)
		{
			sendRequest(
				document.URL + "/request",
				"addMessage(" + 
				currentUser.id + "," +
				currentMessagePage.convId + "," +
				textarea.value.replace(/,/g,"#") + ")",
				function(req)
				{
					if(req.responseText == "Done")
					{
						// currentMessagePage.update();
					}
					else
					{
						alert(req.responseText);
					}
				}
			);
			textarea.value = "";
		}
	}
}
