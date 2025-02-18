sealed partial class AlliumCLI {
      private bool ProjectHandler() {
        if (Args.Count < 1) {
            return Expect("project action");
        }

        var action = Args[0];
        return action switch {
            // "new"  => AddNewTask(args),
            // "list" => ListTasks(),
            "-h" or "--help" => ProjectHelp(),
            _      => Utils.Error($"unknown task action '{action}'."),
        };
    }

    private bool ProjectHelp() {
        Utils.PrintLn("usage: project [-h | --help]");
        return true;
    }
}