import java.util.Arrays;
import java.util.Comparator;
import java.lang.StringBuilder;

public class Main {
	private static Integer[] values = { 5, 2, 1, 9, 50, 56 };

	public static void main(String[] args) {
		Arrays.sort(values, new Comparator<Integer>() {
			@Override
			public int compare(Integer lhs, Integer rhs) {
				String v1 = lhs.toString();
				String v2 = rhs.toString();

				return (v1 + v2).compareTo(v2 + v1) * -1;
			}
		});

		StringBuilder result = new StringBuilder();
		for (Integer integer : values) {
			result.append(integer.toString());
			result.append(' ');
		}

		System.out.println(result);
	}   
}    
