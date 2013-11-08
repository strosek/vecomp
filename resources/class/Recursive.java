public class Recursive
{
  public static int fibonacci(int number)
  {
    if (number <= 0)
      return number;
    else
      return (fibonacci(number - 1) + fibonacci(number - 2));
  }

  public static void main(String[] args)
  {
    System.out.println(fibonacci(10));
  }
}
