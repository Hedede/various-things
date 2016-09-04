using System;
public static class Kata
{
  public static int Factorial(int n)
  {
    if (n < 0 || n > 12)
      throw new ArgumentOutOfRangeException();
    return n != 0 ? n * Factorial(n-1) : 1;
  }
}
