
function initAccountPage()
{
	currentAccountPage = createAccountPage("other-account-page");
	
	currentAccountPage.enable = function ()
	{
		this.element.style.display = "inline-block";
		this.update();
	};
	
	currentAccountPage.disable = function ()
	{
		this.element.style.display = "none";
	};
	
	currentAccountPage.update = function ()
	{
		sendRequest(
			document.URL + "/request",
			"select username,first_name, last_name from accounts where id = " +
			this.id + ";",
			function(req)
			{
				if(req.responseText[0] == '[')
				{
					var array;
					eval("array = " + req.responseText + ";");
					
					if(array.length < 2)
					{
						document.getElementById("other-account-username").innerHTML = "No such user";
						document.getElementById("other-account-fl-names").innerHTML = "";
						return;
					}
					
					document.getElementById("other-account-username").innerHTML = array[1][0];
					document.getElementById("other-account-fl-names").innerHTML = array[1][1] + " " + array[1][2];
				}
				else
				{
					alert(req.responseText);
				}
			}
		);
	};
	
	currentAccountPage.select = function (id)
	{
		this.id = id;
	};
}
