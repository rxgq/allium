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

  public T? MenuList<T>(List<T> options, Func<T, string> selector, string header) where T : class {
    int activeIndex = 0;

    for(;;) {
      Console.Clear();
      PrintLn($"{header}\n");

      for (int i = 0; i < options.Count; i++) {
        var option = selector(options[i]);
        Console.WriteLine(i == activeIndex ? $"> {option}" : option);
      }

      var key = Console.ReadKey();

      if (key.Key == ConsoleKey.UpArrow) {
        activeIndex = activeIndex > 0 ? --activeIndex : options.Count - 1;
      }
      else if (key.Key == ConsoleKey.DownArrow) {
        activeIndex = activeIndex < options.Count - 1 ? ++activeIndex : 0;
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