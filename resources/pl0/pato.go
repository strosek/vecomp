package main
import "fmt"
var ( n int
      i int
      j int
      a int
      s string )
//Cometario de Fibo
func fibo( n int) int {
   if n == 0 || n == 1 {
      return n 
   } else { 
      return fibo( n - 1 ) + fibo( n - 2)
   }
   
}

func main() {
   i:= 10;
   fmt.Println("Bienvenidos CUCEI 28/SEP/2013 ", i + 10);
   fmt.Println("Fibonacci(", i, ")= ", fibo( i ) )
} 