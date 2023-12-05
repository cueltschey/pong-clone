inline int

#define global_var static
#define internal static


clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}