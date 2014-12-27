var currentUser = null;

function createUser(id,uname,passwd)
{
	var user = new Object();
	user.id = id;
	user.name = uname;
	user.password = passwd;
	return user;
}

function updateNames(username,first_name,last_name)
{
	document.getElementById("account-name").innerHTML = first_name + " " + last_name;
	document.getElementById("account-username").innerHTML = username;
	document.getElementById("account-fl-names").innerHTML = first_name + " " + last_name;
}

function updateCookies(username,password)
{
	var date = new Date(new Date().getTime() + 100*365*24*60*60*1000);
	document.cookie = "username=" + username + "; path=/; expires=" + date.toUTCString();
	document.cookie = "password=" + password + "; path=/; expires=" + date.toUTCString();
}

function deleteCookies()
{
	var date = new Date(new Date().getTime() - 100*365*24*60*60*1000);
	document.cookie = "username=; path=/; expires=" + date.toUTCString();
	document.cookie = "password=; path=/; expires=" + date.toUTCString();
}

function setUser(username,password)
{
	sendRequest(
		document.URL + "/request",
		"select id,first_name, last_name from accounts where username = '" +
		username + "' and password = '" +
		password + "';",
		function(req)
		{
			if(req.responseText[0] == '[')
			{
				var array;
				eval("array = " + req.responseText + ";");
				
				if(array.length < 2)
				{
					alert("No such user");
					return;
				}
				
				updateNames(username,array[1][1],array[1][2]);
				
				currentUser = createUser(array[1][0],username,password);
				hideCover();
				
				updateCookies(username,password);
			}
			else
			{
				alert(req.responseText);
			}
		}
	);
}

function showCover()
{
	document.getElementById("content-cover").style.display = 'inline-block';
	document.getElementById("sign-prompt").style.display = 'inline-block';
}

function hideCover()
{
	document.getElementById("content-cover").style.display = 'none';
	document.getElementById("sign-prompt").style.display = 'none';
}

function signIn()
{
	if(document.getElementById("sign-username").value.length == 0){showAlert("Username field is empty.");return;}
	if(document.getElementById("sign-password").value.length == 0){showAlert("Password field is empty.");return;}
	
	sendRequest(
		document.URL + "/request",
		"select exists(select 1 from accounts where username = '" +
		document.getElementById("sign-username").value + "' and password = '" +
		document.getElementById("sign-password").value + "');",
		function(req)
		{
			if((/1']]$/).test(req.responseText))
			{
				setUser(document.getElementById("sign-username").value,document.getElementById("sign-password").value);
			}
			else
			if((/0']]$/).test(req.responseText))
			{
				showAlert("There is no such combination of username and password.");
			}
			else
			{
				alert(req.responseText);
			}
		}
	);
}

function signOut()
{
	deleteCookies();
	showCover();
}

function register()
{
	if(document.getElementById("reg-username").value.length == 0){alert("Username field is empty.");return;}
	if(document.getElementById("reg-password_1").value.length == 0){alert("Password field is empty.");return;}
	if(document.getElementById("reg-first_name").value.length == 0){alert("First name field is empty.");return;}
	if(document.getElementById("reg-last_name").value.length == 0){alert("Second name field is empty.");return;}
	if(document.getElementById("reg-password_1").value != document.getElementById("reg-password_2").value)
	{
		showAlert("Passwords don't match.");
		return;
	}
	
	sendRequest(
		document.URL + "/request",
		"insert into accounts(username,password,first_name,last_name) values('" +
		document.getElementById("reg-username").value + "','" +
		document.getElementById("reg-password_1").value + "','" +
		document.getElementById("reg-first_name").value + "','" +
		document.getElementById("reg-last_name").value + "');",
		function(req)
		{
			if(req.responseText == "Done")
			{
				setUser(document.getElementById("reg-username").value,document.getElementById("reg-password_1").value);
				
				currentPage.update();
				
				document.getElementById("reg-username").value = "";
				document.getElementById("reg-first_name").value = "";
				document.getElementById("reg-last_name").value = "";
			}
			else
			{
				alert(req.responseText);
			}
		}
	);
}

function changeName()
{
	sendRequest(
		document.URL + "/request",
		"update accounts set first_name = '" + document.getElementById("change-first_name").value + 
		"', last_name = '" + document.getElementById("change-last_name").value + "' where id = " + 
		currentUser.id + ";",
		function(req)
		{
			if(req.responseText == "Done")
			{
				updateNames(
					currentUser.name,
					document.getElementById("change-first_name").value,
					document.getElementById("change-last_name").value
				);
				document.getElementById("change-first_name").value = "";
				document.getElementById("change-last_name").value = "";
			}
			else
			{
				alert(req.responseText);
			}
		}
	);
}

function changePassword()
{
	if(document.getElementById("change-password_old").value.length == 0){showAlert("Old password field is empty.");return;}
	if(document.getElementById("change-password_new_1").value.length == 0){showAlert("New password field is empty.");return;}
	if(
		document.getElementById("change-password_new_1").value != 
		document.getElementById("change-password_new_2").value
	)
	{
		showAlert("Passwords don't match.");
		return;
	}
	
	sendRequest(
		document.URL + "/request",
		"update accounts set password = '" + document.getElementById("change-password_new_1").value + 
		"' where id = " + currentUser.id + ";",
		function(req)
		{
			currentUser.password = document.getElementById("change-password_new_1").value;
			updateCookies(currentUser.name,currentUser.password);
			if(req.responseText == "Done")
			{
				document.getElementById("change-password_old").value = "";
				document.getElementById("change-password_new_1").value = "";
				document.getElementById("change-password_new_2").value = "";
				showAlert("Done");
			}
			else
			{
				alert(req.responseText);
			}
		}
	);
}
