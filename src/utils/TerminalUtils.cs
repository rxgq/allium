sealed class TerminalUtils {
  public void Print(String message) {
    Console.Write(message);
  }

  public void Println(String? message = null) {
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

  public bool Choice(string message) {
    for (;;) {
      var input = Read($"{message} (y/n): ").ToLower();

      if (input is "y") return true;
      else if (input is "n") return false;
    }
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
    Println($"  {message}");
    return true;
  }

  public T? MenuList<T>(List<T> options, string header, Func<T, string>? selector = null) where T : class {
    int activeIndex = 0;

    for(;;) {
      Console.Clear();
      Println($"{header}\n");

      for (int i = 0; i < options.Count; i++) {
        var option = selector != null ? selector(options[i]) : options[i].ToString();
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