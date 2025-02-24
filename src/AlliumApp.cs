sealed class AlliumApp {
  private readonly AlliumCli Cli = new();
  private readonly TerminalUtils Utils = new();
  private readonly JsonDb Db = new();

  public bool Run(string[] args) {
    if (Db.Get<WaterSettings>() is null) {
      return Setup();
    }

    if (args.Length is 0) {
        Utils.Println(Constants.AlliumInfo);
        return true;
    }

    return Cli.Execute(args);
  }

  private bool Setup() {
    Utils.Println("allium setup required..");
    
    var intakeUnit = Utils.MenuList(["ml", "L", "gal"], "select your preferred unit of measurement? (use arrow keys)");
    
    double intake;
    for (;;) {
        var intakeStr = Utils.Read("\nwhat is your daily intake goal?: ");

        if (!double.TryParse(intakeStr, out intake)) {
            return Utils.Error("invalid numeric input.");
        }

        break;
    }

    var name = Utils.Read("your name: ");

    var settings = new WaterSettings() {
        DailyIntake = intake,
        Unit = UnitMapper.Map(intakeUnit!),
        Username = name
    };

    Utils.Println("saving water settings.");
    Thread.Sleep(200);

    Db.Write(settings);
    Utils.Println("saved settings.");

    Utils.Println("\ntry using 'allium --log <amount>'");
    return Utils.Info("finished setup.");
  }
}