sealed class AlliumCLI {
  private readonly TerminalUtils Utils = new();
  private readonly Dictionary<string, Func<List<string>, bool>> Commands;

  private string? currentCommand;
  
  public AlliumCLI() {
    Commands = new() {
      ["task"] = AddNewTask,
      ["cls"] = ClearTerminal,
      ["echo"] = Echo
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
    
    Utils.Error($"unknown command '{parts[0]}'");
  }

  private bool Echo(List<string> args) {
    if (args.Count < 2) {
      return ExpectArg("one argument");
    }

    var message = string.Join(' ', args.Skip(1).ToList());
    Utils.PrintLn(message);

    return true;
  }

  private bool ClearTerminal(List<string> args) {
    Console.Clear();
    return true;
  }

  private bool AddNewTask(List<string> args) {
    if (args.Count < 2) {
      return ExpectArg("task name");
    }

    return true;
  }

  private bool ExpectArg(string arg) {
    Utils.Error($"command '{currentCommand}' expects {arg}.");
    return false;
  }
}