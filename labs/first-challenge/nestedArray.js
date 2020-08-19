function getLength(arr) {
	var len = 0;
	if (arr.length > 0) {
		for(var i = 0; i < arr.length; i++) {
			if (Array.isArray(arr[i])) {
				len += getLength(arr[i]);
			} else {
				len += 1;
			}
		}
	}
	return len;
}