var peoplePage;

function initPeoplePage()
{
	peoplePage = createPage("people-page");
	
	/* People page */
	peoplePage.enable = function ()
	{
		this.element.style.display = "inline-block";
		this.update();
	};
	peoplePage.disable = function ()
	{
		this.element.style.display = "none";
	};
	peoplePage.update = function ()
	{
		sendRequest(
			document.URL + "/request",
			"select concat('user',id) as a, concat(first_name,' ',last_name) as b from accounts;",
			function(req)
			{
				document.getElementById("people-page").innerHTML = formUsers(req.responseText);
			}
		);
	};
}

function selectUser(userId)
{
	
}
