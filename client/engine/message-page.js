
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
	};
	
	currentMessagePage.disable = function ()
	{
		this.element.style.display = "none";
	};
	
	currentMessagePage.update = function ()
	{
		if(currentMessagePage.scroll == null)
		{
			this.scroll = (Math.max(document.body.scrollTop,document.documentElement.scrollTop) + window.innerHeight) >= document.body.scrollHeight;
		}
		
		if(this.lastTime == null)
		{
			sendRequest(
				document.URL + "/request",
				"select b.id,concat(a.first_name,' ',a.last_name) as author, b.text, b.send_time " + 
				"from accounts as a, messages as b where a.id = b.author_id and " +
				"conversation_id = " + currentMessagePage.convId + 
				" order by send_time;",
				function(req)
				{
					document.getElementById("messages-history").innerHTML = formMessages(req.responseText);
					this.lastTime = getLastTime(req.responseText);
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
				"select b.id,concat(a.first_name,' ',a.last_name) as author, b.text, b.send_time " + 
				"from accounts as a, messages as b where a.id = b.author_id and " +
				"conversation_id = " + currentMessagePage.convId + 
				"and b.send_time > " + this.lastTime + " order by send_time;",
				function(req)
				{
					document.getElementById("messages-history").innerHTML += formMessages(req.responseText);
					this.lastTime = getLastTime(req.responseText);
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
				"insert into messages(author_id,conversation_id,text,send_time) values(" + 
				currentUser.id + "," +
				currentMessagePage.convId + ",'" +
				textarea.value + "',now());",
				function(req)
				{
					if(req.responseText == "Done")
					{
						currentMessagePage.update();
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
