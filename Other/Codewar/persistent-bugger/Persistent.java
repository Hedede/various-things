import java.util.ArrayList;

class Persist {
  private static ArrayList<Long> getDigits(long n)
  {
    ArrayList<Long> digits = new ArrayList<Long>();
    while (n > 0) {
      digits.add(n % 10);
      n /= 10;
    }
    return digits;
  }
  private static int calculate(long prod, int tries)
  {
    ArrayList<Long> digits = getDigits(prod);
    if (digits.size() < 2)
      return tries;
    prod = digits.stream().reduce(1L, (Long a, Long b) -> a * b );
    return calculate(prod, tries + 1);
  }
  public static int persistence(long n) {
    return calculate(n, 0);
  }
}

class Persist {
  public static int persistence(long n) {
    int tries = -1;
    int count = -1;

    while (count != 1) {
      ++tries;
      count = 1;

      long prod = n % 10;
      n /= 10;

      while (n > 0) {
        prod *= n % 10;
        n /= 10;
        ++count;
      }

      n = prod;
    }
    return tries;
  }
}
