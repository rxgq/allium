sealed class TerminalUtils {
  public void Print(String message) {
    Console.Write(message);
  }

  public void PrintLn(String? message = null) {
    if (message is not null) Console.WriteLine(message);
    else Console.WriteLine();
  }

  public string Read(string? message = null) {
    if (message is not null) {
      Print(message);
    }

    var input = Console.ReadLine();
    return input ?? "";
  }

  public bool Error(string error) {
    for (int i = 0; i < 7; i++)
    {
        Console.ForegroundColor = i % 2 == 0 ? ConsoleColor.Red : ConsoleColor.Yellow;
        Console.Write($"\r? error: {error}");
        Thread.Sleep(35);
    }
    Console.WriteLine("");
    Console.ResetColor();

    return false;
  }

  public bool Info(string message) {
    PrintLn(message);
    return true;
  }

  public T? MenuList<T>(List<T> options, string header) {
    int activeIndex = 0;

    for(;;) {
      Console.Clear();
      Console.ForegroundColor = ConsoleColor.White;

      PrintLn($"{header}\n");

      for (int i = 0; i < options.Count; i++) {
        Console.WriteLine(i == activeIndex ? $"> {options[i]}" : options[i]);
      }

      ConsoleKeyInfo key = Console.ReadKey();
      Console.Clear();

      if (key.Key == ConsoleKey.UpArrow) {
        if (activeIndex > 0) activeIndex--;
        else activeIndex = options.Count - 1;
      }
      else if (key.Key == ConsoleKey.DownArrow) {
        if (activeIndex < options.Count - 1) activeIndex++;
        else activeIndex = 0;
      }
      else if (key.Key == ConsoleKey.Enter) {
        return options[activeIndex];
      }
      else if (key.Key == ConsoleKey.Tab) {
        return default;
      }
    }
  }
}