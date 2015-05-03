export void simple_ispc(uniform int size,
						uniform int nums0[],
						uniform int nums1[],
						uniform int solu[])
{
	for (int j = 0; j < 1; j++) {
		foreach(i = 0 ... size) {
			solu[i] = nums0[i] * nums1[i];
		}
	}
}