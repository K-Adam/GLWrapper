
// some functions

vec3 grayscale(vec3 col) {
	return vec3(dot(col, vec3(0.299, 0.587, 0.114)));
}