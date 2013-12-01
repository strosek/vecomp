public class Function
{
  static private int function()
  {
    return 124;
  }

  static public void sayHello()
  {
    System.out.println("Hello");
  }

  static protected String giveAnswer()
  {
    return "AnAnswer";
  }

  static private void printParameters(int a, double b, byte c)
  {
    System.out.println("Value a: " + a);
    System.out.println("Value b: " + b);
    System.out.println("Value c: " + c);
  }

  public static void main(String[] argv)
  {
    System.out.println(giveAnswer());
    System.out.println(function());
    printParameters(1, 2.0, (byte)0x3);
    sayHello();
  }
}
