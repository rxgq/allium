sealed partial class AlliumCLI {
    private bool TaskHandler() {
        if (Args.Count < 1) return TaskHelp();

        var action = Args[0];
        return action switch {
            "new"  => NewTask(),
            "list" => ListTasks(),
            "-h" or "--help" => TaskHelp(),
            _      => Utils.Error($"unknown task action '{action}'. Use 'new' or 'list'."),
        };
    }

    private bool NewTask() {
        if (Args.Count < 2) return ExpectArg("task name");

        var name = string.Join(' ', Args.Skip(1).ToList());
        var task = new TaskModel() {
            Name = name
        };

        Reader.Write(AlliumConstants.TasksPath, task);
        return Utils.Info($"task '{name}' added.");
    }

  private bool ListTasks() {
    var tasks = Reader.Read<List<TaskModel>>(AlliumConstants.TasksPath) ?? [];

    if (tasks.Count == 0) {
        return Utils.Info("no tasks available.");
    }

    Utils.MenuList([.. tasks.Select(x => x.Name)], "tasks");

    Utils.Info("\nlisting tasks:");
    foreach (var task in tasks) {
        Utils.Info($"- {task.Name}");
    }

    return true;
  }

    private bool TaskHelp() {
        Utils.PrintLn(AlliumConstants.TaskHelp);
        return true;
    }
}