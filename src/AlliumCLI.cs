sealed class AlliumCLI {
  private readonly TerminalUtils Utils = new();
  private readonly JsonReader Reader = new();
  private readonly Dictionary<string, Func<List<string>, bool>> Commands;

  private string? currentCommand;

  public AlliumCLI() {
    Commands = new() {
      ["task"] = TaskHandler,
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

  private bool TaskHandler(List<string> args) {
    if (args.Count < 2) {
      return ExpectArg("task action");
    }

    var action = args[1];
    switch (action) {
      case "new" : return AddNewTask(args);
      case "list": return ListTasks();
    }
    
    Utils.Error($"unknown task action '{action}'. Use 'new' or 'list'.");
    return false;
  }

  private bool AddNewTask(List<string> args) {
    if (args.Count < 3) {
      return ExpectArg("task name");
    }

    var name = string.Join(' ', args.Skip(2).ToList());
    var task = new TaskModel() {
      Name = name
    };

    Reader.Write("data/tasks.json", task);
    Utils.Info($"task '{name}' added.");

    return true;
  }

  private bool ListTasks() {
    var tasks = Reader.Read<List<TaskModel>>("data/tasks.json") ?? [];

    if (tasks.Count == 0) {
      Utils.Info("no tasks available.");
      return true;
    }

    Utils.Info("\nlisting tasks:");
    foreach (var task in tasks) {
      Utils.Info($"- {task.Name}");
    }

    return true;
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

  private bool ExpectArg(string arg) {
    Utils.Error($"command '{currentCommand}' expects {arg}.");
    return false;
  }
}