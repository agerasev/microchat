function convertToArray(array_string)
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
	return array;
}

function formTable(array)
{
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
				table += "<td>";
				table += subarray[i];
				table += "</td>";
			}
		}
		
		table += "</tr>";
	}
	table += "</table>";
	
	return table;
}

function formUnits(array, outer_div, inner_divs, prev_text, post_text)
{
	if(!Array.isArray(array))
	{
		return array;
	}
	var table = "";
	var jlen = array.length;
	for(var j = 1; j < jlen; ++j)
	{
		table += "<div " + outer_div;
		var subarray = array[j];
		if(!Array.isArray(subarray))
		{
			table += ">";
			table += subarray;
		}
		else
		{
			table += "id='" + subarray[0] + "'>";
			var ilen = subarray.length - 1;
			for(var i = 0; i < Math.min(ilen,inner_divs.length); ++i)
			{
				if(prev_text != undefined)
				{
					table += prev_text;
				}
				table += "<div " + inner_divs[i] + "'>";
				table += subarray[i + 1];
				table += "</div>";
				if(post_text != undefined)
				{
					table += post_text;
				}
			}
		}
		
		table += "</div>";
	}
	
	return table;
}

function modifyColumn(array,cnum,func)
{
	if(!Array.isArray(array))
	{
		return;
	}
	for(var j = 0; j < array.length; ++j)
	{
		if(!Array.isArray(array[j]) || array[j].length <= cnum)
		{
			return;
		}
		array[j][cnum] = func(array[j][cnum]);
	}
	return array;
}

function formUsers(array)
{
	return formUnits(
		array,
		"class='unit-element' onclick='pushMode(this.id)'",
		["class='unit-header'"],
		"<div class='user-info'></div>"
	);
}

function formConversations(array)
{
	return formUnits(
		array,
		"class='unit-element' onclick='pushMode(this.id)'",
		["class='unit-header'"],
		"<div class='conv-info'></div>"
	);
}

function formMessages(array)
{
	return formUnits(
		array,
		"class='message'",
		["class='message-author'","class='message-text'","class='message-time'"]
	);
}

function getLastTime(array)
{
	if(array.length > 1)
	{
		return array[array.length-1][3];
	}
	return null;
}
