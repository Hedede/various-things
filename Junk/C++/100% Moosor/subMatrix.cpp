#include <utility>
#if 0
	template<size_t... Cols, size_t Row>
	Matrix<T,N-1,M-1> subMatrix(index_sequence<Cols...>c, index_sequence<Rows...>r)
	{
		/* Matrix<T,N-1,M-1> sub;
		size_t cols[] = {Cols...};
		size_t rows[] = {Rows...};
		eval([&] (size_t i) {
			eval([&] (size_t j) {
				sub[i][j] = columns[cols[i]][rows[j]];
			}, make_index_sequence<M-1>{});
		}, make_index_sequence<N-1>{});

		return sub;*/
		// return { get<Rows>(get<Cols>(*this)) ...};
		return { columns[cols].sub(Row) };
	};
#endif

