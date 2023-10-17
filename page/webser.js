const uploadfile = document.getElementById("uploadfile");
const delfile = document.getElementById("delfile");

uploadfile.addEventListener("click", (e) => {
	e.preventDefault(); // not refresh page

	axios.post('http://127.0.0.1', {
			photo: document.querySelector('#file').files
		}, {
			headers: {
				'Content-Type': 'multipart/form-data'
			}
		}); // change url
});

delfile.addEventListener("click", (e) => {
	e.preventDefault(); // not refresh page

	axios.delete('http://127.0.0.1'); // change url
});
