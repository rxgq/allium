sealed class AlliumApp {
  private readonly AlliumCLI Cli = new();
  private readonly TerminalUtils Utils = new();

  public void Run() {
    Console.Clear();
    Utils.PrintLn("welcome, sir.\n");

    for (;;) {
      var input = Utils.Read("> ");
      if (input.Trim().ToLower() is "q") {
        break;
      }

      Cli.Execute(input);
    }

    Utils.PrintLn("\n  goodbye, friend.\n");
  }
}