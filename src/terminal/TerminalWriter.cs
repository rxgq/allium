sealed class TerminalUtils {
  public void Print(String message) {
    Console.Write(message);
  }

    public void PrintLn(String message) {
    Console.WriteLine(message);
  }

  public string Read(string? message = null) {
    if (message is not null) {
      Print(message);
    }

    var input = Console.ReadLine();
    return input ?? "";
  }

  public void Error(string error) {
    for (int i = 0; i < 7; i++)
    {
        Console.ForegroundColor = i % 2 == 0 ? ConsoleColor.Red : ConsoleColor.Yellow;
        Console.Write($"\r? error: {error}");
        Thread.Sleep(35);
    }
    Console.WriteLine();
    Console.ResetColor();
  }
}