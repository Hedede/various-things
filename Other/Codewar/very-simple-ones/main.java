import java.util.function.Function;

public class FunctionalProgramming {
  private static final String name = "John Smith";
  private static final String number = "js123";
  public static Function<Student, Boolean> f = (Student s) -> {
    // haha return true;
    return s.getFullName().equals(name) && s.studentNumber.equals(number);
  };
}

public class Factorial {
  public int factorial(int n) {
    if (n < 0 || n > 12)
      throw new IllegalArgumentException("Argument is out of range");
    return n != 0 ? n * factorial(n-1) : 1;
  }
}
