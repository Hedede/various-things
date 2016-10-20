	void getInverse (Matrix4<T>& M) const
	{
		T det = determinant();
		Matrix4<T> temp;
	
		for(int i = 0; i < 4; ++i)
		{
			for(int j = 0; j < 4; ++j)
			{
				temp[i][j] = submatrix(i, j).determinant();
				if((i + j)%2 == 1) temp[i][j] = -temp[i][j];
			}
		}
	
		temp.transpose();
		M = temp / det;
	}

	Matrix3<T> submatrix(int i, int j) const
	{
		T madrex[9];
		int index = 0;
		for(int a = 0; a < 4; ++a)
		{
			if(a == i) continue;
			for(int b = 0; b < 4; ++b)
			{
				if(b == j) continue;
				madrex[index] = col_[a][b];
				++index;
			}
		}
		return Matrix3<T>(
			madrex[0], madrex[3], madrex[6], 
			madrex[1], madrex[4], madrex[7], 
			madrex[2], madrex[5], madrex[8]);
	}