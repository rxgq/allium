sealed partial class AlliumCLI {


      private bool ProjectHandler(List<string> args) {
        if (args.Count < 2) {
            return Expect("project action");
        }

        var action = args[1];
        return args[1] switch {
            "new"  => AddNewTask(args),
            "list" => ListTasks(),
            _      => Utils.Error($"unknown task action '{action}'. Use 'new' or 'list'."),
        };
    }
}