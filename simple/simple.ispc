export void simple_ispc(uniform unsigned int size,
						uniform unsigned int nums0[],
						uniform unsigned int nums1[],
						uniform unsigned int solu[]) {

	foreach(i = 0 ... size) {
		solu[i] = nums0[i] + nums1[i];
	}
}