const method = document.getElementById("method");
const port = document.getElementById("port");
const url = document.getElementById("url");
const submit = document.getElementById("submit");
const output = document.getElementById('output');

submit.addEventListener("click", (e) => {
	e.preventDefault(); // not refresh page

	const v_method = method.value ? method.value : "GET";
	const v_port = port.value ? port.value : "443";
	const v_url = url.value ? url.value : "https://127.0.0.1";

	axios.request({
		method: v_method,
		url: v_url + ":" + v_port,
	}).then(function (response) {
		// console.log(response); // show all response
		// let txt = JSON.stringify(response.data); // convert JSON to string
		// output.innerHTML = txt; // open when response is JSON
		output.innerHTML = response.data; // open when response is text
	}).catch(function (error) {
		console.log(error);
	});
});

const onlyNumberKey = (evt) => {            
	let ASCIICode = (evt.which) ? evt.which : evt.keyCode

	if (ASCIICode > 31 && (ASCIICode < 48 || ASCIICode > 57))
		return false;
	return true;
}
