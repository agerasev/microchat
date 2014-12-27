/* TODO: Add synchronous request */

var conversationsPage;

function newConv()
{
	showPrompt(
		"Enter conversation name:",
		function (name)
		{
			conversationsPage.newPageName = name;
			if(name.length == 0)
			{
				showAlert("Conversation name field is empty");
				return false;
			}
			sendRequest(
				document.URL + "/request",
				"select exists(select 1 from conversations where name = '" + name + "');",
				function(req)
				{
					if((/1']]$/).test(req.responseText))
					{
						showAlert("Conversation name '" + conversationsPage.newPageName + "' already exists");
					}
					else
					if((/0']]$/).test(req.responseText))
					{
						sendRequest(
							document.URL + "/request",
							"insert into conversations(name,last_update) values('" + conversationsPage.newPageName + "',now());",
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
		sendRequest(
			document.URL + "/request",
			"select concat('conv',id) as a, name from conversations;",
			function(req)
			{
				document.getElementById("conversations-page").innerHTML = 
					"<div class='unit-element' onclick='newConv()'>" +
					"<div class='newconv-info'></div>" +
					"<div class='unit-header'>New conversation</div>" +
					"</div>";
				document.getElementById("conversations-page").innerHTML += formConversations(req.responseText);
			}
		);
	};
}
