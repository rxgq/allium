sealed partial class AlliumCLI {
  private readonly TerminalUtils Utils = new();
  private readonly JsonController JsonController = new();
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

    if (command.StartsWith('.')) {
      var macro = MacroHandler(command);
      
      if (macro is not null) {
        command = macro;
      }
    }

    var parts = command.Split(" ").ToList();

    Command = parts.First();
    Args = [.. parts.Skip(1)];

    if (Commands.TryGetValue(Command, out var action)) {
      action();
    } else {
      UtilsHandler();
    }
  }

  private string? MacroHandler(string command) {
    var macros = JsonController.Read<List<MacroModel>>();
    var macro = macros.FirstOrDefault(x => x.Macro == command);

    return macro?.Command;
  }
}