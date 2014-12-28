/* TODO: Add synchronous request */

var conversationsPage;

function newConv()
{
	showPrompt(
		"Enter conversation name:",
		function (name)
		{
			if((name.match(/,/g) || []).length > 0)
			{
				showAlert("Name contains illegal characters.");
				return false;
			}
			
			conversationsPage.newPageName = name;
			if(name.length == 0)
			{
				showAlert("Conversation name field is empty");
				return false;
			}
			
			sendRequest(
				document.URL + "/request",
				"getConversationByName(" + name + ")",
				function(req)
				{
					if(req.responseText[0] == "[")
					{
						if((req.responseText.match(/\[/g) || []).length >= 3)
						{
							showAlert("Conversation name '" + conversationsPage.newPageName + "' already exists");
						}
						else
						if((req.responseText.match(/\[/g) || []).length < 3)
						{
							sendRequest(
								document.URL + "/request",
								"addConversation(" + conversationsPage.newPageName + ")",
								function(req)
								{
									if(req.responseText == "Done")
									{
										showAlert("Conversation '" + conversationsPage.newPageName + "' successfully created");
										currentPage.update();
									}
									else
									{
										alert(req.responseText);
									}
								}
							);
						}
					}
					else
					{
						alert(req.responseText);
					}
				}
			);
			return true;
		}
	);
}

function initConversationsPage()
{
	conversationsPage = createPage("conversations-page");
	conversationsPage.newPageName = null;
	
	/* Conversations page */
	conversationsPage.enable = function ()
	{
		this.element.style.display = "inline-block";
		this.update();
	};
	conversationsPage.disable = function ()
	{
		this.element.style.display = "none";
	};
	conversationsPage.update = function ()
	{
		if(currentUser == null)
		{
			return;
		}
		
		sendRequest(
			document.URL + "/request",
			"getAllConversations()",
			function(req)
			{
				document.getElementById("conversations-page").innerHTML = 
					"<div class='unit-element' onclick='newConv()'>" +
					"<div class='newconv-info'></div>" +
					"<div class='unit-header'>New conversation</div>" +
					"</div>";
				var array = modifyColumn(convertToArray(req.responseText),0,function(val){return "conv"+val;});
				document.getElementById("conversations-page").innerHTML += formConversations(array);
			}
		);
	};
}
