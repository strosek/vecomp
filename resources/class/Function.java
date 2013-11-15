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

  public static void main(String[] argv)
  {
    System.out.println(giveAnswer());
    System.out.println(function());
    sayHello();
  }
}
