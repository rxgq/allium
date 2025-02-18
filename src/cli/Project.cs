sealed partial class AlliumCLI {
      private bool ProjectHandler() {
        if (Args.Count < 1) return ProjectHelp();

        var action = Args[0];
        return action switch {
            // "new"  => AddNewTask(args),
            // "list" => ListTasks(),
            "-h" or "--help" or _ => ProjectHelp(),
        };
    }

    private bool ProjectHelp() {
        Utils.PrintLn(AlliumConstants.ProjectHelp);
        return true;
    }
}