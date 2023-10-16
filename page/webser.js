const method = document.getElementById("method");
const url = document.getElementById("url");
const submit = document.getElementById("submit");
const output = document.getElementById('output');

submit.addEventListener("click", (e) => {
	e.preventDefault(); // not refresh page

	const v_method = method.value;
	const v_url = url.value;

	axios.request({
		method: v_method,
		url: v_url
	}).then(function (response) {
		// console.log(response); // show all response
		// let txt = JSON.stringify(response.data); // convert JSON to string
		// output.innerHTML = txt; // open when response is JSON
		output.innerHTML = response.data; // open when response is text
	}).catch(function (error) {
		console.log(error);
	});
});
