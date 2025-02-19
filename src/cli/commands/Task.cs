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

        var tasks = JsonController.Read<List<TaskModel>>();
        var nextId = tasks.Count != 0 ? tasks.Max(x => x.Id) + 1 : 0;
        
        var name = string.Join(' ', Args.Skip(1).ToList());

        Console.Clear();
        Utils.PrintLn($"> {name} \n");

        var description = Utils.Read("description: ");

        var task = new TaskModel() {
            Name = name,
            Id = nextId,
            Description = description
        };

        JsonController.Create(task);
        return Utils.Info($"task '{name}' added.");
    }

  private bool ListTasks() {
    var tasks = JsonController.Read<List<TaskModel>>() ?? [];

    if (tasks.Count == 0) {
        return Utils.Info("no tasks available.");
    }
    
    for (;;) {
        var task = Utils.MenuList(tasks, $"tasks ({tasks.Count})", (x) => x.Name + x.IsComplete);
        if (task is null) return false;

        var option = Utils.MenuList(["finish", "delete", "alter", "return"], $"{task.Name}\n {task.Description}");
        if (option is "return") break;

        else if (option is "delete") {
            JsonController.Delete(task);
            Utils.Info($"task '{task.Name}' has been deleted");
            Console.ReadKey();
            break;
        }
        else if (option is "finish") {
            task.IsComplete = true;
            JsonController.Update(task);
        }
        else if (option is "alter") {
            task.Description = Utils.Read("description: ");;
            JsonController.Update(task);
        }
    }

    return true;
  }

    private bool TaskHelp() {
        Utils.PrintLn(AlliumConstants.TaskHelp);
        return true;
    }
}