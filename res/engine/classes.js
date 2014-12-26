function createPage(elem_id)
{
	var page = new Object();
	
	page.element = document.getElementById(elem_id);
	page.enable = null;
	page.disable = null;
	page.update = null;
	
	return page;
}

function createMessagePage(elem_id)
{
	var mpage = createPage(elem_id);
	mpage.select = null;
	mpage.submitText = null;
	return mpage;
}
