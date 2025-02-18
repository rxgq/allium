sealed partial class AlliumCLI {
    private bool TaskHandler(List<string> args) {
        if (args.Count < 2) return Expect("task action");

        var action = args[1];
        return action switch {
            "new"  => AddNewTask(args),
            "list" => ListTasks(),
            _      => Utils.Error($"unknown task action '{action}'. Use 'new' or 'list'."),
        };
    }

    private bool AddNewTask(List<string> args) {
        if (args.Count < 3) return Expect("task name");

        var name = string.Join(' ', args.Skip(2).ToList());
        var task = new TaskModel() {
            Name = name
        };

        Reader.Write("data/tasks.json", task);
        return Utils.Info($"task '{name}' added.");
    }

  private bool ListTasks() {
    var tasks = Reader.Read<List<TaskModel>>("data/tasks.json") ?? [];

    if (tasks.Count == 0) {
        return Utils.Info("no tasks available.");
    }

    Utils.Info("\nlisting tasks:");
    foreach (var task in tasks) {
        Utils.Info($"- {task.Name}");
    }

    return true;
  }
}