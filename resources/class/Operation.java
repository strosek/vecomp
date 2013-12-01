public class Operation 
{
  public static void main(String[] args)
  {
    int a, b;
    double c, d;
    int resultInt;
    double resultDouble;
    
    a = 1;
    b = 2;
    resultInt = a + b;
    System.out.println("Result int: " + resultInt);
    resultInt = a * b;
    System.out.println("Result int: " + resultInt);

    c = 1.0;
    d = 2.0;
    resultDouble = c + d;
    System.out.println("Result double: " + resultDouble);
    resultDouble = c * d;
    System.out.println("Result double: " + resultDouble);

    resultDouble = a + b * c + d;
    System.out.println("Result mixed: " + resultDouble);
  }
}

