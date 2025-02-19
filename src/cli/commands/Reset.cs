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

    return true;
  }

  private bool Reset() {
    if (File.Exists(AlliumConstants.UserPath)) {
      File.Delete(AlliumConstants.UserPath);
    }

    if (Directory.Exists(AlliumConstants.AppDataPath)) {
      Directory.Delete(AlliumConstants.AppDataPath);
    }

    return true;
  }
}

  