sealed partial class AlliumCLI {
  private readonly TerminalUtils Utils = new();
  private readonly JsonReader Reader = new();
  private readonly Dictionary<string, Func<bool>> Commands;

  private string? Command;
  private List<string> Args = [];

  public AlliumCLI() {
    Commands = new() {
      ["task"] = TaskHandler,
      ["project"] = ProjectHandler,
    };
  }

  public void Execute(string command) {
    if (command is "") return;

    var parts = command.Split(" ").ToList();

    Command = parts.First();
    Args = [.. parts.Skip(1)];

    if (Commands.TryGetValue(Command, out var action)) {
      action();
    } else {
      UtilsHandler();
    }

    Utils.PrintLn("");
  }
}