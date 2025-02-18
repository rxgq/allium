sealed partial class AlliumCLI {
  private readonly TerminalUtils Utils = new();
  private readonly JsonReader Reader = new();
  private readonly Dictionary<string, Func<List<string>, bool>> Commands;

  private string? currentCommand;

  public AlliumCLI() {
    Commands = new() {
      ["task"] = TaskHandler,
      ["project"] = ProjectHandler,
    };
  }

  public void Execute(string command) {
    if (command is "") return;

    var parts = command.Split(" ");
    if (Commands.TryGetValue(parts[0], out var action)) {
      currentCommand = parts[0];
      action([.. parts]);
      return;
    }

    UtilsHandler([.. parts]);
    Utils.PrintLn("");
  }
}