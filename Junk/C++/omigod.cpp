float outside = 0.0;
for (unsigned i = 0; i < 3; ++i) {
	if (lpos[i] < bbox.mins[i]) {
		float f = (lpos[i] - bbox.mins[0]);
		outside += f * f;
	} else if (lpos[i] > bbox.maxs[i]) {
		float f = (lpos[i] - bbox.maxs[0]);
		outside += f * f;
	}
}
