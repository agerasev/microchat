var currentPage = null;
var containerElement = null;

function createPage()
{
	var page = new Object();
	page.element = null;
	page.update = null;
	return page;
}

var accountPage = createPage();
var peoplePage = createPage();
var conversationsPage = createPage();
var aboutPage = createPage();

function changeMode(mode)
{
	
	while (containerElement.firstChild)
	{
		containerElement.removeChild(containerElement.firstChild);
	}
	switch(mode)
	{
	case 'account':
		containerElement.appendChild(accountPage.element);
		currentPage = accountPage;
		break;
	case 'people':
		containerElement.appendChild(peoplePage.element);
		currentPage = peoplePage;
		break;
	case 'conversations':
		containerElement.appendChild(conversationsPage.element);
		currentPage = conversationsPage;
		break;
	case 'about':
		containerElement.appendChild(aboutPage.element);
		currentPage = aboutPage;
		break;
	default:
		currentPage = null;
		break;
	}
}

window.onload = function()
{
	containerElement = document.getElementById("content-container");
	accountPage.element = document.getElementById("account-page");
	peoplePage.element = document.getElementById("people-page");
	conversationsPage.element = document.getElementById("conversations-page");
	aboutPage.element = document.getElementById("about-page");
	// changeMode('about');
	
	sendRequest(document.URL + "/index.html","request",function(req){alert(req.responseText)});
};
