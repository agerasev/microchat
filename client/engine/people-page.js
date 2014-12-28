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
		if(currentUser == null)
		{
			return;
		}
		
		sendRequest(
			document.URL + "/request",
			"getAllUsers()",
			function(req)
			{
				var array = convertToArray(req.responseText);
				var newarray = new Array(array.length);
				for(var i = 0; i < array.length; ++i)
				{
					newarray[i] = ["user"+array[i][0],array[i][2]+" "+array[i][3]];
				}
				document.getElementById("people-page").innerHTML = formUsers(newarray);
			}
		);
	};
}
