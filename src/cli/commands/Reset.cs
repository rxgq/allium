sealed partial class AlliumCLI {
  private bool ResetHandler() {
    if (Args.Count < 1) return ResetHelp();

        var action = Args[0];
        return action switch {
            "--confirm" => Reset(),
            "-h" or "--help" or _ => ResetHelp(),
        };
  } 

  private bool ResetHelp() {
    Utils.Info("\nthe reset command requires the '--confirm' flag");
    Utils.Info("warning: this will delete *everything*");
    Console.ReadKey();
    return true;
  }

  private bool Reset() {
    if (File.Exists(AlliumConstants.UserPath)) {
      File.Delete(AlliumConstants.UserPath);
    }

    if (Directory.Exists(AlliumConstants.AppDataPath)) {
      Directory.Delete(AlliumConstants.AppDataPath);
    }

    Utils.Info("data has been deleted");
    Utils.Info("the program will now exit");
    Console.ReadKey();

    ShouldExit = true;

    return true;
  }
}

  