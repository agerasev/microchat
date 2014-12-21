function createPage()
{
	var page = new Object();
	page.element = null;
	page.update = null;
	return page;
}

function formTable(array_string)
{
	var array;
	if(array_string[0] == '[')
	{
		eval("array = " + array_string + ";");
	}
	else
	{
		return array_string;
	}
	if(!Array.isArray(array))
	{
		return array;
	}
	var table = "<table border='1'>";
	var jlen = array.length;
	for(var j = 0; j < jlen; ++j)
	{
		table += "<tr>";
		var subarray = array[j];
		if(!Array.isArray(subarray))
		{
			table += subarray;
		}
		else
		{
			var ilen = subarray.length;
			for(var i = 0; i < ilen; ++i)
			{
				table += "<td>"
				table += subarray[i];
				table += "</td>"
			}
		}
		
		table += "</tr>";
	}
	table += "</table>";
	
	return table;
}

function createMessagePage(convId)
{
	var mpage = createPage();
	mpage.convId = convId;
	mpage.submitText = function()
	{
		var textarea = document.getElementById("input-text-area");
		if(textarea.value.length > 0)
		{
			sendRequest(
				document.URL + "/index.html",
				textarea.value,
				function(req)
				{
					document.getElementById("messages-history").innerHTML += "<div class='message'>" + formTable(req.responseText) + "</div>";
				}
			);
			textarea.value = "";
		}
	}
	return mpage;
}

var currentMessagePage = null;

function changeMode(mode)
{
	switch(mode)
	{
	case 'account':
		break;
	case 'people':
		break;
	case 'conversations':
		break;
	case 'about':
		break;
	default:
		break;
	}
}

function update_placeholders()
{
	document.getElementById("messages-input-placeholder").style.height = document.getElementById("input-text-pannel").offsetHeight + "px";
}

window.onload = function()
{
	currentMessagePage = createMessagePage(1);
	update_placeholders();
};

window.onresize = function()
{
	update_placeholders();
}
