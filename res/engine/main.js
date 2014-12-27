var currentPage = null;
var currentMessagePage = null;
var currentAccountPage = null;
var updater = null;

var accountPage;
var aboutPage;

function initPages()
{
	initPeoplePage();
	initConversationsPage();
	
	initMessagePage();
	initAccountPage();
	
	accountPage = createPage("account-page");
	aboutPage = createPage("about-page");
	
	/* Account page */
	accountPage.enable = function ()
	{
		this.element.style.display = "inline-block";
		this.update();
	};
	accountPage.disable = function ()
	{
		this.element.style.display = "none";
	};
	accountPage.update = function ()
	{
		
	};
	
	/* About page */
	aboutPage.enable = function ()
	{
		this.element.style.display = "inline-block";
		this.update();
	};
	aboutPage.disable = function ()
	{
		this.element.style.display = "none";
	};
	aboutPage.update = function ()
	{
		
	}; 
}

function pushMode(mode)
{
	history.pushState({page: mode}, mode, '?' + mode);
	changeMode(mode);
}

function changeMode(mode)
{
	if(currentPage != null)
	{
		currentPage.disable();
		currentPage = null;
	}
	switch(mode)
	{
	case 'account':
		(currentPage = accountPage).enable();
		break;
	case 'people':
		(currentPage = peoplePage).enable();
		break;
	case 'conversations':
		(currentPage = conversationsPage).enable();
		break;
	case 'about':
		(currentPage = aboutPage).enable();
		break;
	default:
		if((/^conv/).test(mode))
		{
			currentMessagePage.select(mode.substring(4));
			currentPage = currentMessagePage;
			currentPage.enable();
		}
		else if((/^user/).test(mode))
		{
			currentAccountPage.select(mode.substring(4));
			currentPage = currentAccountPage;
			currentPage.enable();
		}
		break;
	}
}

window.onpopstate = function(event) 
{
	changeMode(event.state.page);
};

function update_placeholders()
{
	document.getElementById("messages-input-placeholder").style.height = document.getElementById("input-text-pannel").offsetHeight + "px";
}

window.onload = function()
{
	initPages();
	
	if(window.location.search != "")
	{
		pushMode(window.location.search.slice(1));
	}
	else
	{
		pushMode("people");
	}
	
	if(document.cookie.match(/username=[^;]*/) != null)
	{
		var username = document.cookie.match(/username=[^;]*/)[0].replace("username=","");
		document.getElementById("sign-username").value = username;
		if(document.cookie.match(/password=[^;]*/) != null)
		{
			var password = document.cookie.match(/password=[^;]*/)[0].replace("password=","");
			setUser(username,password);
		}
	}
	
	var updater = setInterval(function()
	{
		if(currentPage != null)
		{
			currentPage.update();
		}
	}, 1000);
};

window.onresize = function()
{
	update_placeholders();
}
