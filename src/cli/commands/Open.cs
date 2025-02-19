sealed partial class AlliumCLI {
      private bool OpenHandler() {
        if (Args.Count < 1) return OpenHelp();

        var action = Args[0];
        return action switch {
            "-h" or "--help" => OpenHelp(),
            _ => OpenProject()
        };
    }

    private bool OpenProject() {
      

      return true;
    }

    private bool OpenHelp() {
      return true;
    }
}