sealed class AlliumApp {
    private readonly AlliumCli Cli = new();
    private readonly TerminalUtils Utils = new();
    private readonly JsonDb Db = new();

    public bool Run(string[] args) {
        if (args.Length is 0) {
            Utils.Println(Constants.AlliumInfo);
            return true;
        }

        if (Db.Get<WaterSettings>() is null) {
          return Setup();
        }

        return Cli.Execute(args);
    }

    private bool Setup() {
        var intakeUnit = Utils.MenuList(["ml", "L", "gal"], "what do you prefer to measure with? (use arrow keys)");
        
        double intake;
        for (;;) {
            var intakeStr = Utils.Read("\nwhat is your daily intake goal?: ");

            if (!double.TryParse(intakeStr, out intake)) {
                return Utils.Error("invalid numeric input.");
            }

            break;
        }

        var name = Utils.Read("what should you be called?: ");

        var settings = new WaterSettings() {
            DailyIntake = intake,
            Unit = UnitMapper.Map(intakeUnit!),
            Username = name
        };

        Utils.Println("saving water settings.");
        Db.Write(settings);
        Utils.Println("saved settings.");

        return Utils.Info("finished .");
    }
}